#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include "AsyncJson.h"
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <FirebaseESP32.h>
#include <FirebaseJson.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Arduino.h>
#include "time.h"



// ============================================================================= //
// --- Objects Instance --- //

/* WiFiClient Object modeling ---*/
WiFiClient smart_dispenser_wifi_client;

// Use WiFiClientSecure class to create TLS connection
WiFiClientSecure client;
  
// Web Server Object modeling
AsyncWebServer server(80);
  
/* Soft AP Object network parameters (NOT USED, but it could be) */
IPAddress ap_ip(172, 217, 28, 1);
IPAddress ap_subnet(255, 255, 255, 0);
  
/* Define the Firebase Data object */
FirebaseData fbdo;

/* Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;



// ============================================================================= //
// --- Cabeçalho das Funções --- //

// Dispenser Actions
void trigger_pump(); // Trigger pump when awakening.
int get_liquid_level(); // Check the liquid level through sensors
void read_battery_level(); // Update the global variable battery_level (100, 75, 25 or 0);

bool is_time_publish(); // Be sure the pump was triggered 5 times before publish into Firebase
bool is_time_to_notifications(); // Be sure the level it below 30% before pushing notifications

void show_indicator_level_leds(); 

// Deep Sleep
void go_light_sleep();

// RTC Methods
void update_RTC_dispenser_parameters();
void load_RTC_dispenser_parameters();
void rtc_reset_count();


// EEPROM Functions

void save_data(struct smartjet_data *_data); // Save
  
void read_data(struct smartjet_data *_data); // Load 

void clear_eeprom(); // Clear EEPROM

void factory_reset(); // Clear EEPROM after certain conditions


bool is_credentials_valid();
  
// Web Server Routes 
void handle_get_requests();

// Firebase Functions
bool firebase_first_update(); // Publish dispenser data into Firebase Real Time Database at the First Time
bool firebase_update_parameters(); // Publish dispenser data into Firebase RTD 
bool firebase_check_for_delete(); // Subscribe into Firebase Real Time Database to check for delete
void print_result(FirebaseData &data); // Check for actions...

// One Signal Functions
void push_notification(String msg);

// This variable triggers the dispenser recharging mode
bool recharge_mode{false};

// Network Wireless Setup Methods
bool initialize_wifi_as_client(); // Try to connect the SmartJet to a WiFi Network .
bool initialize_wifi_as_ap(); // Create an Access Point
bool connect_to_smartjet_AP(); // Try to connect the SmartJet to another SmartJet's Access Point.
void initialize_web_server(); // Intialize a Web Server to handle posts requests.
const char* set_softAP_ssid(); // Set the SSID corresponding to the ESP8266-01 HDWID (IT HAS NOT BEEN USED)
String http_request_json_data(const char* serverName);

unsigned long get_timestamp(); // Timestamp 

//bool save_smarjet_struct_data(struct smartjet_data _data);


// =============================================================================== //
// --- Global and Macros Variables ---

// Hardware Pinout
#define  pump_pin           02
#define WAKE_UP_PIN         04 
#define LEVEL_LEDS_PIN      19
#define level_100           18
#define level_75            05
#define level_50            17
#define level_25            16
#define smartjet_status_pin 23
#define battery_level_pin   15


// =============================================================================== //

unsigned int battery_level{100};
unsigned long ESP_CHIP_ID = ESP.getEfuseMac(); // Unique Identifier to the ESP32 

bool is_first_time{false};
bool connect_result{false};
bool request_result{false};
unsigned int DELAY_BEFORE_RESTART{4000};
// Attributes to Network Wireless Management
unsigned int status = WL_IDLE_STATUS; // Attributes to Current WLAN Status

// Firebase Attributes
//const char* FIREBASE_HOST = "smartjet-17260-default-rtdb.firebaseio.com"; // Tiago Firebase
//const char * FIREBASE_AUTH = "6ZLOUFM2qaavbGSNwdHZyWIdEhQb3rVZI3DPjVR4";  // Tiago Firebase
const char* FIREBASE_HOST = "auth-82bca.firebaseio.com";  // Carlos App Firebase
const char * FIREBASE_AUTH = "IWbMfMsNN8l1MBSoOhVc6LaoJogWTkAwx0USgV9u"; // Carlos App Firebase

//One Signal Attributes

const char* test_root_ca= \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIExTCCBGygAwIBAgIQBBKobZVzLlWBV3PhDkY7CDAKBggqhkjOPQQDAjBKMQsw/\n" \
  "CQYDVQQGEwJVUzEZMBcGA1UEChMQQ2xvdWRmbGFyZSwgSW5jLjEgMB4GA1UEAxMX\n" \
  "Q2xvdWRmbGFyZSBJbmMgRUNDIENBLTMwHhcNMjAwODA0MDAwMDAwWhcNMjEwODA0\n" \
  "MTIwMDAwWjBtMQswCQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDVNh\n" \
  "biBGcmFuY2lzY28xGTAXBgNVBAoTEENsb3VkZmxhcmUsIEluYy4xHjAcBgNVBAMT\n" \
  "FXNuaS5jbG91ZGZsYXJlc3NsLmNvbTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IA\n" \
  "BG1h86rR+QUk/2e6ZreC99Tk9XNW8Eiw41VgseBkJdd88hJVJiNHMBBs2FSaCJd+\n" \
  "NQvaoDWuyEXjsCG61qZDOUyjggMPMIIDCzAfBgNVHSMEGDAWgBSlzjfq67B1DpRn\n" \
  "iLRF+tkkEIeWHzAdBgNVHQ4EFgQUP89YUmkEHK7ywVMw+YOtmjy1IXMwQAYDVR0R\n" \
  "BDkwN4IPKi5vbmVzaWduYWwuY29tghVzbmkuY2xvdWRmbGFyZXNzbC5jb22CDW9u\n" \
  "ZXNpZ25hbC5jb20wDgYDVR0PAQH/BAQDAgeAMB0GA1UdJQQWMBQGCCsGAQUFBwMB\n" \
  "BggrBgEFBQcDAjB7BgNVHR8EdDByMDegNaAzhjFodHRwOi8vY3JsMy5kaWdpY2Vy\n" \
  "dC5jb20vQ2xvdWRmbGFyZUluY0VDQ0NBLTMuY3JsMDegNaAzhjFodHRwOi8vY3Js\n" \
  "NC5kaWdpY2VydC5jb20vQ2xvdWRmbGFyZUluY0VDQ0NBLTMuY3JsMEwGA1UdIARF\n" \
  "MEMwNwYJYIZIAYb9bAEBMCowKAYIKwYBBQUHAgEWHGh0dHBzOi8vd3d3LmRpZ2lj\n" \
  "ZXJ0LmNvbS9DUFMwCAYGZ4EMAQICMHYGCCsGAQUFBwEBBGowaDAkBggrBgEFBQcw\n" \
  "AYYYaHR0cDovL29jc3AuZGlnaWNlcnQuY29tMEAGCCsGAQUFBzAChjRodHRwOi8v\n" \
  "Y2FjZXJ0cy5kaWdpY2VydC5jb20vQ2xvdWRmbGFyZUluY0VDQ0NBLTMuY3J0MAwG\n" \
  "A1UdEwEB/wQCMAAwggEFBgorBgEEAdZ5AgQCBIH2BIHzAPEAdgD2XJQv0XcwIhRU\n" \
  "GAgwlFaO400TGTO/3wwvIAvMTvFk4wAAAXO6w9eTAAAEAwBHMEUCIQDsZZaVswAO\n" \
  "W/QFMWkWqH2U9pj4YAQLeS8lUphBWxzJCwIgXcQMyMkDZWXqI79GQheuYl8BJu2E\n" \
  "QBP7v/jT1/Gxcz4AdwBc3EOS/uarRUSxXprUVuYQN/vV+kfcoXOUsl7m9scOygAA\n" \
  "AXO6w9fCAAAEAwBIMEYCIQDZxiORYbjfX9unI3iW78o88Qi+tRTDBHDeD8EPOOAT\n" \
  "gQIhAPSWGijy/VkvgiiKuApOMfOc9Smq5Mx/wpvrRWqnl6xVMAoGCCqGSM49BAMC\n" \
  "A0cAMEQCIAho5YJCEl9Q6d8yCMWJFcmjF3xcjGAhh/K4+NisZMkzAiAhRoYPhQR2\n" \
  "u+4tBdv36ei2wktWLpchUnaDSwJUKelcPw==\n" \
  "-----END CERTIFICATE-----\n";

const char* host = "onesignal.com";
const int httpsPort = 443;
String ids = "c3c768ac-b697-4f7c-9597-0b65d1c81b1f"; // Carlos IDS
String url = "/api/v1/notifications";


// Access Point Attributes
const char * softAP_ssid = set_softAP_ssid();
const char * softAP_password = "12345678";



// RTC Memory
RTC_DATA_ATTR int triggers_count = 0;
RTC_DATA_ATTR int level_count = 100;

struct smartjet_data
{
  char     ssid[100];
  char     password[100];
  char     email[100];
  char     player_id[100];
  unsigned int smartjets_to_add;
};
