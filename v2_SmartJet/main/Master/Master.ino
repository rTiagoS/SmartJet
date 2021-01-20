#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string>



// ============================================================================= //
// --- Protótipo das Funções utilizadas --- //

// Acionadores e Dispenser                                      
void sensorAproximidade();
void ligaBomba();
void desligaBomba();

// Configurações e setup do Broker
void startWiFiAP();
void startWebServer();
void startWiFiClient();
void setupBroker();
void reconnectBroker();

// Armazenamento de dados na EEPROM
void saveCredentials();
void saveEmail();
void loadCredentials();
void loadEmail();

// Rotas do servidor Web
void handleRoot();
void handleWifi();
void handleWifiSave();
void handleCredentialsRequest();
void handleSaveEmail();
void handleMasterID();

// Decodificação do Json proveniente do Broker
String retrieveAction(String brokerPayload);
String retrieveID(String  brokerPayload);

// Tópicos Broker
void  publish_feedbackCadastro(const char* topico, unsigned long ID);
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

bool AindaNaoDesativou{true};

bool modoRecarga{false};

bool modoCadastro{false};

bool bEmailCadastrado{false};

bool bEmailCadastradoComSucesso{false};

bool connect; // Should I connect to WLAN asap? 

unsigned long tempoReferencia{0};

unsigned long lastConnectTry = 0; // Last time I tried to connect to WLAN 

unsigned long esp_chipID = ESP.getChipId();

unsigned int status = WL_IDLE_STATUS; // Current WLAN status 


// ===============================================================================
// --- Configurações do WiFi Client, softAP, Web Server e Broker ---


/* Set these to your desired softAP credentials. They are not configurable at runtime */
const char *softAP_ssid = "SmarJet_2020";
const char *softAP_password = "12345678";

/* Tópicos no broker em que serão publicados os dados ------*/
const char topico_Buffer[] = "/iainovation/espirrinho/buffer"; 

const char* listenerTopic;
const char* publishTopic;
//const char* topicoSmartjet = "/iacinovation/smarjet/tiago.ramos121@gmail.com/";
const char* broker_clientID = set_brokerClientID(esp_chipID);


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


// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(172, 217, 28, 1);
IPAddress netMsk(255, 255, 255, 0);


void setup()
{
  delay(1000);
  Serial.begin(115200);

  
  
  /* - - - Inicialização dos pinos como INPUT ou OUTPUT - - - */
  pinMode(pinoBomba, OUTPUT);
  pinMode(pinoProximidade , INPUT_PULLUP);


  /* - - - LEDS RGB e Microbomba iniciam desligados - - - */
  digitalWrite(pinoBomba, LOW); 

  
  startWiFiAP();

  startWebServer();
  
  loadCredentials(); // Load WLAN credentials from network

  loadEmail(); // Load User Email from EEPROM.
  
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID.  

  bEmailCadastrado = (strlen(email) > 0); // Change Status if there is a User Email.

  startWiFiClient();

  if (bEmailCadastrado)
  {
    listenerTopic = set_subscribeTopic(email); // Melhorar aqui... atribuir dentro da função.
    publishTopic = set_publishTopic(email);
    Serial.println("");
    Serial.println("E-mail do Usuario: ");
    Serial.print(listenerTopic);
    Serial.println("\n");
    
    setupBroker();
    bEmailCadastradoComSucesso = true;
  }  
}


void loop()
{
  if ((millis() - tempoReferencia) > 60000)
  {
    if (AindaNaoDesativou) // Esse IF só é TRUE uma vez após o intervalo de 2 minutos.
    {
      WiFi.softAPdisconnect(true); // Desativa o Access Point
      Serial.println("\nRede AP Desligada!");
      AindaNaoDesativou = false;
    }
 
    if (!client.connected())
    {
      reconnectBroker();
    }  
    sensorAproximidade();
    
    client.loop();

    if (modoCadastro) // Se o usuário solicitar novo cadastro:
    {
      
      startWiFiAP(); // Habilita a Rede Local
      
      Serial.println("");
      Serial.println("Modo Cadastro Ativado");
      
      publish_feedbackCadastro(publishTopic, esp_chipID); // Envia feedback que modo cadastro foi habilitado.
      
      AindaNaoDesativou = true;
      modoCadastro = false; // Habilita
      tempoReferencia = millis(); // Reinicia a contagem do tempo
      
      
    } // fim do if
    
  } // fim do if 
  else
  {
    server.handleClient();  // Client Handling
    
    if (!bEmailCadastradoComSucesso)
    {
      loadEmail();
      bEmailCadastrado = strlen(email) > 0; 
      if (bEmailCadastrado)
      {
        listenerTopic = set_subscribeTopic(email); // Melhorar aqui... atribuir dentro da função.
        publishTopic = set_publishTopic(email);
        Serial.println("");
        Serial.println("E-mail do Usuario: ");
        Serial.print(email);
        Serial.println("\n");
        
        setupBroker();
        bEmailCadastradoComSucesso = true;
      }
    }
  }

  
  if (connect) // Este IF só é true caso exista uma SSID na EEPROM
  /*
   * O bloco a seguir é importante para a primeira
   * inicialização do dispenser, em que não haverá ainda
   * uma SSID e PASS. Ele permite que seja possível se conectar a uma
   * rede sem precisar reiniciar o dispositivo.
   */
  {
    Serial.println("Connect requested");
    connect = false; // Para que o IF não seja ativado novamente
    startWiFiClient(); //
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
    } // fim do if
  } // fim do bloco de escopo
} // fim da função loop ()




void startWiFiAP()
{
  Serial.println();
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());  
}

void startWebServer()
{
  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/credentials", handleCredentialsRequest);
  server.on("/master/id", handleMasterID);
  server.on("/user_auth", handleSaveEmail);
  server.on("/user_credentials", handleUserCredentials);
  server.begin(); // Web server start
  Serial.println("HTTP server started");
}

void startWiFiClient()
{
  Serial.println("Connecting as wifi client...");
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
    if ((retrieveAction(s)).equals("cadastrar")) // Se a ação requerida for cadastrar:
    {
      modoCadastro = true; // Aciona modo cadastro.
      Serial.println("");
      Serial.print("Ativando Modo Cadastro...");
    }
    else if ((retrieveAction(s)).equals("recarregar")) // Se a ação requerida for recarregar
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

void  publish_feedbackCadastro(const char* topico, unsigned long ID)
{
   /* 
   * Indica ao Broker que o Modo Cadastro foi habilitado.
   */
   
  // Declaração de objetos.
  DynamicJsonDocument doc(64);
  char feedback_modoCadastro[64];

  // Formatação JSON
  doc["ID"] = ID;
  doc["Feedback"] = "Modo Cadastro Ativado";
  serializeJson(doc, feedback_modoCadastro);

  // Publica mensagem de feedback no tópico especificado
  client.publish(topico,feedback_modoCadastro);
} // Fim da função publish_feedbackCadastro.

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



  
