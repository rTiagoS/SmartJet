// ===============================================================================
// --- Inclusão de Bibliotecas ---
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


/* Variáveis para configuração da Rede Wireless ----------- */
char ssid[] = "LIVE TIM_3322_2G";           // your network SSID (name).
char password[] = "KyxqxYSG";           // your network password.

/* Tópicos no broker em que serão publicados os dados ------*/
const char topico_Buffer[] = "/iainovation/espirrinho/buffer"; 

/* Tópicos em que o Espirrinho receberá comandos ---------- */
const char topico_listener[] = "/iainovation/espirrinho/listener";

/* Configuração de Rede do Broker MQTT -------------------- */
const char* mqttServer = "179.191.235.42";
const int mqttPort = 1883;

/* Instanciação de objetos para comunicação com o broker ---*/
WiFiClient SmartJetClient;
PubSubClient client(SmartJetClient);


// ===============================================================================
// --- Protótipo das Funções utilizadas ---
void sensorAproximidade();
void acionaBomba();
void ligaBomba();
void desligaBomba();
void enviaBuffer();
void SetupBroker();
void SetupWiFi();
void reconnect();




// ===============================================================================
// --- Mapeamento de Hardware ---
#define pinoBomba       03
#define pinoProximidade 00


// ===============================================================================
// --- Variáveis Globais ---
int qtde_Acionamentos{1};



void setup(void)
{ /*Begin of setup's function */
  /* - - - Inicialização dos pinos como INPUT ou OUTPUT - - - */
  pinMode(pinoBomba, OUTPUT);
  pinMode(pinoProximidade , INPUT_PULLUP);


  /* - - - LEDS RGB e Microbomba iniciam desligados - - - */
  digitalWrite(pinoBomba, LOW); 
  
  //Serial.begin(115200);

  SetupWiFi();
  SetupBroker();
  
}


void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  sensorAproximidade();
}



/* Atuadores ------------------------------------ */

void ligaBomba()
{
  // Liga o pino da bomba
  digitalWrite(pinoBomba, HIGH);
}

void desligaBomba()
{
  // Desliga o pino da bomba
  digitalWrite(pinoBomba, LOW);
}


/* Sensor de Aproximidade ---------------------  */
void sensorAproximidade()
/*  Caso seja detectado uma mão, a bomba é acionada dentro das regras de acionamento
 *  definida na função acionaBomba(). Caso a mão seja retirada, imediatamente a bomba é desligada e informações
 *  são enviadas ao broker. Existe uma trégua de alguns segundos para um novo acionamento.
 */
{
  delay(800); // Aguarda um tempo para confirmar que tem uma mão aguardando jato de gel.
  
  
  if(digitalRead(pinoProximidade) == LOW) // Se uma mão for detectada, então:
  {
    if (qtde_Acionamentos <= 6)
    {
      ligaBomba(); // Aciona a bomba por um tempo máximo definido.
      delay(1500); // Mantém a bomba acionada por um tempo.
      enviaBuffer(qtde_Acionamentos);
      
      desligaBomba();
      qtde_Acionamentos +=1;
      delay(1000);
    }   
  }
} // Fim da função sensorAproximidade().




/*  setup_wifi function is suppose to stablish a communication between the Wireless Network and Arduino Uno
 *  through the ESP8266-01 using virtual serial as path to exchange information. */
void SetupWiFi()
{ 
 
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) // while the Wireless Network isn't connected:
  { // Begin of while section
    delay(1000);
    //Serial.print(".");
  } // End of while section
 
  //Serial.println("Connected to the WiFi network");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
} /* End of function SetupWiFi() */



/* SetupBroker() set the configurations to connect the SmarJetClient to Broker via mqtt protocol */
void SetupBroker()
{ 
  
  client.setServer(mqttServer, mqttPort); // connect to broker via mqtt protocol
  //client.setCallback(callback);

} /* End of SetupBroker() function */





  /*  reconnect() function is suppose to be called inside de loop region in order to reconnect the Arduino to the
   *  broker if something goes wrong. */
  void reconnect() 
  { 
    while (!client.connected()) // Loop until we're reconnected
    {
      //Serial.print("Connecting to Broker...");
  
      if (client.connect("SmarJetClient_v1"))
      { 
        //Serial.println("connected");
        client.publish(topico_Buffer,"Conectado"); // Once connected, publish an announcement...
        //client.subscribe("/mpu6050/listener/");
      } //End of if section
      else 
      { // Begin of else section
        //Serial.print("failed with state");
        //Serial.print(client.state());
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
