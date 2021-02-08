#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// ============================================================================= //
// --- Objects Instance --- //

/* WiFiClient Object modeling ---*/
WiFiClient smart_dispenser_wifi_client;
  
// Web Server Object modeling
ESP8266WebServer server(80);
  
/* Soft AP Object network parameters */
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

void external_awakening(); // Instruções ao ser despertado pelo circuito externo
int get_liquid_level(); // Check the liquid level through sensors
bool is_time_publish(); // Be sure the pump was triggered 10 times before publish into Firebase
  
// EEPROM Data Storage Methods
void save_credentials();
void save_email();
void save_level();
void save_triggers();
   
void load_credentials();
void load_email();
void load_level();
void load_triggers();

bool remove_credentials();
bool remove_email();
bool remove_level();
bool remove_triggers();

bool is_credentials_valid();
  
// Web Server Routes 
void handle_post_request();
void handle_web_server();
  
// Publish Method
bool firebase_update_parameters();

// This variable triggers the dispenser recharging mode
bool recharge_mode{false};

// Network Wireless Setup Methods
bool initialize_wifi_as_ap(); // Initialize the ESP8266-01 WiFi Wireless as Access Point
void initialize_web_server(); // Intialize a Web Server to handle posts requests.
bool initialize_wifi_as_client(); // Initialize the ESP8266-01 WiFi Wireless as client
const char* set_softAP_ssid(); // Set the SSID corresponding to the ESP8266-01 HDWID




// =============================================================================== //
// --- Global and Macros Variables ---

// Hardware Pinout
#define  pump_pin     3
#define  level0_pin   2
#define  level1_pin   1
  
// Unique Identifier to the ESP8266-01
unsigned long ESP_CHIP_ID = ESP.getChipId();

// Attributes to Time Management
unsigned long last_connect_try{0}; // Last time I tried to connect to WLAN
//unsigned long time_to_add{0};
unsigned long data_millis{0};
bool is_first_time{false};

// Attributes to Network Wireless Management
unsigned int status = WL_IDLE_STATUS; // Attributes to Current WLAN Status

  
// Firebase Attributes
const char* FIREBASE_HOST = "smartjet-17260-default-rtdb.firebaseio.com";  /* Define the Firebase project host name (required) */
const char * FIREBASE_AUTH = "6ZLOUFM2qaavbGSNwdHZyWIdEhQb3rVZI3DPjVR4";


// Dispenser Attributes
int triggers = 9; // How many triggers. It resets the account when the dispenser got a new recharge
int level = 40; // Dispenser Liquid Level

// Access Point Attributes
const char * softAP_ssid = set_softAP_ssid();
//const char * softAP_password = "12345678";


// User Credentials Attributes
char ssid[33] = "";
char password[65] = "";
char email[40] = "";
