#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

/* user parameters */

const char* ssid = "LIVE TIM_3322_2G";
const char* password = "KyxqxYSG";

// IDS - unique device id from IoT Manager application: Start IoT Manager, goto "Statistic" and press "Send ids to email"
// Also, you can get ids via MQTT after HELLO message.
// ids will be always changed after IoT Manager reinstall on mobile device

String ids = "c3c768ac-b697-4f7c-9597-0b65d1c81b1f"; // its not real IDS, please change

const char fingerprint[] PROGMEM = "7C 99 51 B3 51 B7 49 61 D0 D8 08 00 9F BF 71 20 67 6E AA FE";


/* end of user parameters */



const char* host = "onesignal.com";
const int httpsPort = 443;
String url = "/api/v1/notifications";

void push(String msg) {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;

  Serial.print("PUSH: connecting to ");
  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("PUSH: try to send push notification...");

  // please, do not change app_id - its IoT Manager id at onesignal.com
  // more info at https://documentation.onesignal.com/v3.0/reference#create-notification
  
  String data = "{\"app_id\": \"1a1152c4-f8cf-41d9-b745-096685cd81af\",\"include_player_ids\":[\"" + ids + "\"],\"data\": {\"foo\": \"bar\"},\"contents\": {\"en\": \"" + msg + "\"}}";

  Serial.print("PUSH: requesting URL: ");
  Serial.println(url);
  
  client.println(String("POST ") + url + " HTTP/1.1");
  client.print("Host:");
  client.println(host);
  client.println("User-Agent: esp8266.Arduino.IoTmanager");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.println(data);

  Serial.println("PUSH: done. Restart esp8266 for push again.");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Push notification for IoT Manager");
  Serial.println();
  Serial.print("PUSH: connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("PUSH: WiFi connected");

  push("Opa Carlos");
}

void loop() {
}
