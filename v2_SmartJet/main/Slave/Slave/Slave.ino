#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <ArduinoJson.h>



/* Set these to your desired softAP credentials. They are not configurable at runtime */
#ifndef APSSID
#define APSSID  "SmarJet_2020"
#define APPSK   "12345678"
#endif  

const char *softAP_ssid = APSSID;
const char* softAP_password = APPSK;

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[33] = "";
char password[65] = "";

const char* serverCredentials = "http://172.217.28.1/credentials";

/** Last time I tried to connect to WLAN */
unsigned long lastConnectTry = 0;

/** Current WLAN status */
unsigned int status = WL_IDLE_STATUS;

/** Should I connect to WLAN asap? */
boolean connect;



void setup()
{
  delay(1000);
  Serial.begin(115200);

  startAPWiFiClient();

  // Faz uma requisição para o servidor web das credenciais
  httpGETRequest(serverCredentials);

  loadCredentials(); // Load WLAN credentials from network

  //Serial.println("SSID: " + ssid + " PASS: " + password);

  
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
  
  
}

void loop() 
{
  if (connect)
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
  
}

void startAPWiFiClient()
{
  Serial.println("Connecting as wifi client to the Access Point...");
  WiFi.disconnect();
  WiFi.begin(softAP_ssid, softAP_password);
  int connRes = WiFi.waitForConnectResult();
  Serial.print("connRes: ");
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
        strcpy(ssid,buffer_ssid);
        strcpy(password,buffer_password);
        // Output to serial monitor
        Serial.print("SSID:");
        Serial.println(ssid);
        Serial.print("Password:"); 
        Serial.println(password);
        // salva as credenciais na EEPROM
        saveCredentials();
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
