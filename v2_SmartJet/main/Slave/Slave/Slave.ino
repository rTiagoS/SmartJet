#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>


// ============================================================================= //
// --- Protótipo das Funções utilizadas --- //

// Acionadores e Dispenser                                      
void sensorAproximidade();
void ligaBomba();
void desligaBomba();

// Configurações e setup do Broker
void startAPWiFiClient();
void startWiFiClient();
void setupBroker();
void reconnectBroker();

// Armazenamento de dados na EEPROM
void saveCredentials();
void saveEmail();
void loadCredentials();
void loadEmail();

// Rotas do client
void httpGETRequest(const char* serverName);

// Decodificação do Json proveniente do Broker
String retrieveAction(String brokerPayload);
String retrieveID(String  brokerPayload);

// Tópicos Broker
void  publish_genericResponse(const char* topico, unsigned long ID);
void  publishBuffer(int Acionamentos, int Level, unsigned long ID);
char * set_publishTopic (const char * user_email);
char * set_subscribeTopic (const char * user_email);

char * set_brokerClientID (unsigned long ID);



// ===============================================================================                  
// --- Mapeamento de Hardware --- //
#define pinoBomba       03
#define pinoProximidade 00


// ===============================================================================
// --- Variáveis Globais ---
int qtde_Acionamentos{1};

bool bEmailCadastrado{false};

bool modoRecarga{false};

bool connect; // Should I connect to WLAN asap? 

unsigned long esp_chipID = ESP.getChipId();

unsigned long lastConnectTry = 0; // Last time I tried to connect to WLAN 

unsigned int status = WL_IDLE_STATUS; // Current WLAN status 


// ===============================================================================
// --- Configurações do WiFi Client, softAP, Web Server e Broker ---

/* Set these to your desired softAP credentials. They are not configurable at runtime */
const char *softAP_ssid = "SmarJet_2020";
const char *softAP_password = "12345678";

const char* listenerTopic;
const char* publishTopic;
const char* broker_clientID = set_brokerClientID(esp_chipID);

const char* serverCredentials = "http://172.217.28.1/credentials";

/* Configuração de Rede do Broker MQTT -------------------- */
const char* mqttServer = "179.191.235.42";
const int mqttPort = 1883;

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[33] = "";
char password[65] = "";
char email[40] = "";

/* Instanciação de objetos para conexão com o broker ---*/
WiFiClient SmartJetClient;
PubSubClient client(SmartJetClient);

void setup()
{
  delay(1000);
  Serial.begin(115200);

  /* - - - Inicialização dos pinos como INPUT ou OUTPUT - - - */
  pinMode(pinoBomba, OUTPUT);
  pinMode(pinoProximidade , INPUT_PULLUP);

  /* - - - LEDS RGB e Microbomba iniciam desligados - - - */
  digitalWrite(pinoBomba, LOW); 

  startAPWiFiClient();

  // Faz uma requisição para o servidor web das credenciais
  httpGETRequest(serverCredentials);

  loadCredentials(); // Load WLAN credentials from network

  loadEmail(); // Load User Email from EEPROM.
  
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
  
  bEmailCadastrado = (strlen(email) > 0); // Change Status if there is a User Email.
  
  if (bEmailCadastrado)
  {
    listenerTopic = set_subscribeTopic(email); // Melhorar aqui... atribuir dentro da função.
    publishTopic = set_publishTopic(email);
    Serial.println("");
    Serial.println("E-mail do Usuario: ");
    Serial.print(listenerTopic);
    Serial.println("\n");
    
    setupBroker();
  } // fim do if
  else
  {
    Serial.println("\nEmail não cadastrado");
  }
}

void loop() 
{
   
    
  if (connect) // Este IF só é true caso exista uma SSID na EEPROM
  /*
   * O bloco a seguir é importante para a primeira
   * inicialização do dispenser, em que não haverá ainda
   * uma SSID e PASS. Ele permite que seja possível se conectar a uma
   * rede sem precisar reiniciar o dispositivo.
   */
  {
    Serial.println("Connect requested");
    connect = false;
    startWiFiClient();
    lastConnectTry = millis();
  }

  {
    unsigned int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 60000))
    {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      connect = true;
    }
    if (status != s)
    { // WLAN status change
      Serial.print("Status: ");
      Serial.println(s);
      status = s;
      if (s == WL_CONNECTED)
      {
        /* Just connected to WLAN */
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

      }
      else if (s == WL_NO_SSID_AVAIL)
      {
        WiFi.disconnect();
      }
    }
  }
  
  if (!client.connected())
  {
    reconnectBroker();
  }  
  sensorAproximidade();

  client.loop(); 
  
}

void startAPWiFiClient()
{
  Serial.println("\nConnecting as wifi client to the Access Point...");
  WiFi.disconnect();
  WiFi.begin(softAP_ssid, softAP_password);
  int connRes = WiFi.waitForConnectResult();
  Serial.print("\nconnRes: ");
  Serial.println(connRes);
}

void startWiFiClient()
{
  Serial.println("Connecting as wifi client to the WLAN...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult();
  Serial.print("connRes: ");
  Serial.println(connRes);
}

/* SetupBroker() set the configurations to connect the SmarJetClient to Broker via mqtt protocol */
void setupBroker()
{ 
  
  client.setServer(mqttServer, mqttPort); // connect to broker via mqtt protocol
  client.setCallback(callback);

} /* End of SetupBroker() function */

void reconnectBroker() 
{ 
  while (!client.connected()) // Loop until we're reconnected
  {
    Serial.print("Connecting to Broker...");
  
    if (client.connect(broker_clientID)) // mudar para incoporar o ID do smarjet.
    { 
      Serial.print("\nconnected\nClient ID: ");
      Serial.print(broker_clientID);
      publish_genericResponse(publishTopic, esp_chipID); // Once connected, publish an announcement...
      client.subscribe(listenerTopic);
      Serial.println("\n");
      Serial.println("Inscrição no tópico realizada!");
    } //End of if section
    else 
    { 
      Serial.print("failed with state");
      Serial.print(client.state());
      delay(2000); 
    } //End of else section
  } // End of while section.
} // End of reconnect function



void httpGETRequest(const char* serverName)
{
  WiFiClient client;

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, serverName))
  {  // HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
          // Parsing
        const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
        DynamicJsonDocument doc(bufferSize);
        deserializeJson(doc,(http.getString()));
        // Parameters
        const char* buffer_ssid= doc["ssid"]; // "Leanne Graham"
        const char* buffer_password= doc["pass"]; // "Bret"
        const char* buffer_email = doc["email"];
        
        strcpy(ssid,buffer_ssid);
        strcpy(password,buffer_password);
        strcpy(email, buffer_email);
        
        // Output to serial monitor
        Serial.print("\nSSID: ");
        Serial.print(ssid);
        Serial.print("\nPassword: "); 
        Serial.print(password);
        Serial.print("\nEmail: ");
        Serial.print(email);
         
        // salva as credenciais na EEPROM
        saveCredentials();
        saveEmail();
        //String payload = http.getString();
        //Serial.println(payload);
        
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

      http.end();
    }
  else
  {
    Serial.printf("[HTTP} Unable to connect\n");
  }
}

/* Envia dados para o servidor Broker ----------------------- */
void publishBuffer(int Acionamentos, int Level, unsigned long ID, const char* topico)
{
  
 /* Envia um buffer para o Broker contendo os dados dos sensores em formato JSON: 
  *  {"ID":122313, "Jets": 15, "Level":75}  
 /*
  static char Buffer[15]; // Por ser um buffer estático, a variável não é destruída ao término da função. Evita bugs.
  memset(Buffer, 0, sizeof(Buffer)); // Limpa o buffer para receber novos dados.
  sprintf(Buffer, "%d", Acionamentos); // Envia os dados x,y para o buffer estático.
  client.publish(topico_Buffer, Buffer); // Publica o buffer no Broker. 
  */

  // Declaração de objetos.
  DynamicJsonDocument doc(64);
  char sensorData[64];

  // Formatação JSON
  doc["ID"] = ID;
  doc["Jets"] = Acionamentos;
  doc["Level"] = Level;
  serializeJson(doc, sensorData);
  
  // Publica mensagem de feedback no tópico especificado
  client.publish(topico,sensorData);
  
} // Fim da função publishBuffer

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived: ");
  Serial.print(topic);
  String s = "";
  for (int i = 0; i < length; i++)
  {
    //Serial.println((char)payload[i]);
    s += ((char)payload[i]);
  }
  Serial.println(s);


  if (esp_chipID == (retrieveID(s).toInt())) // Se a mensagem recebida tiver o ID deste ESP:
  {
    if ((retrieveAction(s)).equals("recarregar")) // Se a ação requerida for recarregar
    {
      modoRecarga = true; // Aciona modo Recarga.
      Serial.println("");
      Serial.print("Ativando Modo Recarga...");
    }
  }
  else if (retrieveID(s) == "all") // Se a mensagem recebida tiver o ID all:
  {
    if (retrieveAction(s).equals("login")) // Se a ação requerida for login:
    {
      publish_genericResponse(publishTopic, esp_chipID); // Publica a quantidade de acionamentos atual.
    }
  }
} // Fim da função callback.

String retrieveID( String  brokerPayload)
{
  /*
   *  Decodifica a mensagem json da ação a ser executada.
   */
   
  // Parsing
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  DynamicJsonDocument doc(bufferSize);
  deserializeJson(doc, brokerPayload);
  
  // Parameters 
  String ID = doc["ID"];
  Serial.println("");
  Serial.println("ID Retrieved: ");
  Serial.print(ID);
  return ID;  
} // Fim da função retrieveID.

String retrieveAction(String brokerPayload)
{
  /*
   *  Decodifica a mensagem json da ação a ser executada.
   */
   
  // Parsing
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  DynamicJsonDocument doc(bufferSize);
  deserializeJson(doc, brokerPayload);
  
  // Parameters 
  String action = doc["Action"];
  Serial.println("");
  Serial.println("Action Retrieved: ");
  Serial.print(action);
  return action;  
  
} // Fim da função retrieAction.

void  publish_genericResponse(const char* topico, unsigned long ID)
{
   /* 
   * Tópico em que o Smarjet enviará feedback do status da conexão
   */

  // Declaração de objetos.
  DynamicJsonDocument doc(64);
  char feedback_firstLogin[64];

  // Formatação JSON
  doc["ID"] = ID;
  doc["Jets"] = "0";
  doc["Level"] = "100";
  serializeJson(doc, feedback_firstLogin);
  
  // Publica mensagem de feedback no tópico especificado
  client.publish(topico,feedback_firstLogin);
} // Fim da função publish_genericResponse


char * set_publishTopic (const char * user_email)
{
  char * topic_toPublish;
  
  // Aloca um espaço de memória na HEAP enquanto o programa estiver sendo executado.
  topic_toPublish = (char*)malloc(sizeof(char)*50); 
  memset(topic_toPublish, 0, sizeof(char)*50);
  sprintf(topic_toPublish, "/iacinovation/smarjet/esp/%s/", user_email);
  return topic_toPublish;
}

char * set_subscribeTopic (const char * user_email)
{
  char * topic_toSubscribe;
  
  // Aloca um espaço de memória na HEAP enquanto o programa estiver sendo executado.
  topic_toSubscribe = (char*)malloc(sizeof(char)*50); 
  memset(topic_toSubscribe, 0, sizeof(char)*50);
  sprintf(topic_toSubscribe, "/iacinovation/smarjet/app/%s/", user_email);
  return topic_toSubscribe;
}



char * set_brokerClientID (unsigned long ID)
{
  char * client_name;
  client_name = (char*)malloc(sizeof(char)*30);
  memset(client_name, 0, sizeof(char)*30);
  sprintf(client_name, "SmartJetClient_%lu", ID);
  return client_name;
}
