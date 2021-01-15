#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <string>



// ===============================================================================
// --- Protótipo das Funções utilizadas ---
void sensorAproximidade();
void ligaBomba();
void desligaBomba();
void startWiFiAP();
void startWebServer();
void startWiFiClient();
void setupBroker();
void reconnectBroker();
void saveCredentials();
void loadCredentials();
void handleRoot();
void handleWifi();
void handleWifiSave();
void handleCredentialsRequest();
String retrieveAction(String brokerPayload);
String retrieveID(String  brokerPayload);
void  publish_feedbackCadastro( unsigned long ID);
void  publish_feedbackfirstLogin(const char* topico, unsigned long ID);



// ===============================================================================
// --- Mapeamento de Hardware ---
#define pinoBomba       03
#define pinoProximidade 00


// ===============================================================================
// --- Variáveis Globais ---
int qtde_Acionamentos{1};
bool AindaNaoDesativou{true};
unsigned long tempoReferencia{0};
unsigned long esp_chipID = ESP.getChipId();
bool modoRecarga{false};
bool modoCadastro{false};


// ===============================================================================
// --- Configurações do WiFi Client, softAP, Web Server e Broker ---


/* Set these to your desired softAP credentials. They are not configurable at runtime */
#ifndef APSSID
#define APSSID "SmarJet_2020"
#define APPSK  "12345678"
#endif

const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;

/* Tópicos no broker em que serão publicados os dados ------*/
const char topico_Buffer[] = "/iainovation/espirrinho/buffer"; 


const char* topicoSmartjet = "/iacinovation/smarjet/tiago.ramos121@gmail.com/";



/* Configuração de Rede do Broker MQTT -------------------- */
const char* mqttServer = "179.191.235.42";
const int mqttPort = 1883;

/* Instanciação de objetos para comunicação com o broker ---*/
WiFiClient SmartJetClient;
PubSubClient client(SmartJetClient);



/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[33] = "";
char password[65] = "";


// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(172, 217, 28, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
unsigned long lastConnectTry = 0;

/** Current WLAN status */
unsigned int status = WL_IDLE_STATUS;


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
  
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID  

  startWiFiClient();

  setupBroker();

   
}


void loop()
{
  if ((millis() - tempoReferencia) > 60000)
  {
    if (AindaNaoDesativou) // Esse IF só é TRUE uma vez após o intervalo de 2 minutos.
    {
      WiFi.softAPdisconnect(true); // Desativa o Access Point
      Serial.println("Rede AP Desligada!");
      AindaNaoDesativou = false;
    }
 
    if (!client.connected())
    {
      reconnectBroker();
    }  
    sensorAproximidade();
    
    client.loop();

    if (modoCadastro)
    {
      tempoReferencia = millis();
      startWiFiAP();
      AindaNaoDesativou = true;
      Serial.println("");
      Serial.println("Modo Cadastro Ativado");
      publish_feedbackCadastro(topicoSmartjet, esp_chipID);
      modoCadastro = false;
      
    }
    
  } // fim do if 
  else
  {
    server.handleClient();  // Client Handling
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
  
    if (client.connect("SmarJetClient_v1")) // mudar para incoporar o ID do smarjet.
    { 
      Serial.println("connected");
      client.publish(topicoSmartjet,"Conectado ao broker"); // Once connected, publish an announcement...
      client.subscribe(topicoSmartjet);
      //client.subscribe(topicoRecarga);
      //client.subscribe(topicoCadastro);
      Serial.println("");
      Serial.println("Inscrições Realizadas com Sucesso!");
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
void enviaBuffer(int Acionamentos)
{
  
 /* Envia um buffer para o Broker em forma de array ([x,y]) contendo duas principais informações:
 *  x: 1 (bomba acionada) ou 0 (bomba não acionada).
 *  y: 0 ("cheio") ou 1 ("metade") ou 2 ("quase vazio"). */
 
  static char Buffer[15]; // Por ser um buffer estático, a variável não é destruída ao término da função. Evita bugs.
  memset(Buffer, 0, sizeof(Buffer)); // Limpa o buffer para receber novos dados.
  sprintf(Buffer, "%d", Acionamentos); // Envia os dados x,y para o buffer estático.
  client.publish(topico_Buffer, Buffer); // Publica o buffer no Broker.
  
} // Fim da função enviaBuffer


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


  if (esp_chipID == (retrieveID(s).toInt()))
  {
    if ((retrieveAction(s)).equals("cadastrar"))
    {
      modoCadastro = true; 
      Serial.println("");
      Serial.print("Ativando Modo Cadastro...");
    }
    else if ((retrieveAction(s)).equals("recarregar"))
    {
      modoRecarga = true;
      Serial.println("");
      Serial.print("Ativando Modo Recarga...");
    }
  }
  else if (retrieveID(s) == "all")
  {
    if (retrieveAction(s).equals("login"))
    {
      publish_feedbackfirstLogin(topicoSmartjet, esp_chipID);
    }
  }
}

String retrieveID( String  brokerPayload)
{
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
}

String retrieveAction(String brokerPayload)
{
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
  
}

void  publish_feedbackCadastro(const char* topico, unsigned long ID)
{
   /* 
   * Tópico em que o Smarjet enviará feedback do status da conexão
   */
 
  DynamicJsonDocument doc(64);
  char feedback_modoCadastro[64];
  
  doc["ID"] = ID;
  doc["Feedback"] = "Modo Cadastro Ativado";
  serializeJson(doc, feedback_modoCadastro);

  client.publish(topico,feedback_modoCadastro);
}

void  publish_feedbackfirstLogin(const char* topico, unsigned long ID)
{
   /* 
   * Tópico em que o Smarjet enviará feedback do status da conexão
   */
 
  DynamicJsonDocument doc(64);
  char feedback_firstLogin[64];
  
  doc["ID"] = ID;
  doc["Feedback"] = "13";
  serializeJson(doc, feedback_firstLogin);

  client.publish(topico,feedback_firstLogin);
}






  
