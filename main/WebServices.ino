#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif

/* - - - Try to connect to an Access Point provided by a another smartjet master - - -*/
bool connect_to_smartjet_AP()
{
  DEBUG_PRINT("\n * * * Trying to connect to a Smartjet Access Point... ");
  WiFi.disconnect();                    // Disconnect any connection that may exist
  WiFi.mode(WIFI_STA);                  // Set the mode as station (meaning that smartjet acts like a client)
  WiFi.begin("SmartJet", "12345678");   // Credentials of the Smartjet Device Master
  
  int max_attempts = 20;                // maximum number of attempts before throw a error
  while ((WiFi.status() != WL_CONNECTED) and (max_attempts > 0))
  {
    delay(500);                         // Wait 0.5s before another attempt
    DEBUG_PRINT(".");
    max_attempts -= 1;
  }
  if (WiFi.status() == WL_CONNECTED)    
  {
    DEBUG_PRINT("SUCCESS!\n");
    DEBUG_PRINT("WiFi connected, IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    return true;
  }
  else
  {
    DEBUG_PRINT("FAILED!\n");
    return false;
  }
  DEBUG_PRINTLN("\n * * * Exiting Initializing WiFi as client... * * *");
} // end of connect_to_smartjet_ap() function.
/* ------------------------------------------------------------------------------- */


/* The smartjet try a get request in order to receive credentials about e-mail and Wi-Fi credentials. */
String http_request_json_data(const char* serverName)
{
  DEBUG_PRINT("\n * * * REQUESTING USER JSON DATA VIA HTTP GET...  ");
  
  // Try to request a connection to an ESP32 WebServer in order to get the user data

  HTTPClient http;              // Instantiate a object called http from class HTTPClient

  Serial.print("[HTTP] begin...  \n"); 
  http.begin(serverName);       // Setup the connection with the server.

  Serial.print("[HTTP GET... \n");
  // start connection and send HTTP header
  int http_code = http.GET();   // Request via GET method to the WebServer

  // http_code will be negative on error
  if (http_code > 0)
  {
    //HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", http_code);
    String payload;

    // file found at server
    if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY)
    {
      payload = http.getString();
      Serial.println("\n *-*-* Data Received from the Request *-*-*");
      Serial.print(payload);
      request_result = true;  // Variable used to trigger the next if block
      return payload;         // Content received by the get request
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(http_code).c_str());
      Serial.println("\n *-*-* Data Received from the Request *-*-*");
      Serial.print(payload);
      
      request_result = false;
      
      delay(DELAY_BEFORE_RESTART);  // Useful delay before a restart.
      Serial.print("\n Check the connection and try again. Reboting...");
      ESP.restart();
    }
    http.end();
  }
  DEBUG_PRINTLN("DONE!... * * *");
} // end of http_request_json_data function
/* ----------------------------------------------------------------------- */


/* Deserialize the payload json content */
void deserialize_http_payload(String payload)
{ 
  DEBUG_PRINT("\n * * * DESERIALIZING HTTP JSON PAYLOAD... ");
  
  // Json Object Instance and Dynamic Memory Allocation
  DynamicJsonDocument json_user_data(512); // Size computed with ArduinoJson Assistant
                    
  // Deserializing Json Attributes
  deserializeJson(json_user_data, payload);

  // Collect the JSON Content
  String _email                    = json_user_data["tag"];     // Data used to publish into firebase RealTimeDatabase
  String _ssid                     = json_user_data["ssid"];    // Data used to connect to the Wi-Fi
  String _password                 = json_user_data["pass"];    // Data used to connect to the W-Fi
  String _player_id                = json_user_data["idplayer"];  // Data used to publish push notifications with OneSignal service
  unsigned int _smartjets_to_add   = json_user_data["number_smartjets"]; // Not necessary to be used unless the smartjet acts as Master

  // Since we are storing char type at EEPROM, it's necessary to convert types.
  // Note eeprom variable it was declared as global. That's why it's possible access it with '.' notation
  _email.toCharArray(eeprom.email, sizeof(eeprom.email) - 1); 
  _ssid.toCharArray(eeprom.ssid, sizeof(eeprom.ssid) - 1); 
  _password.toCharArray(eeprom.password, sizeof(eeprom.password) - 1); 
  _player_id.toCharArray(eeprom.player_id, sizeof(eeprom.player_id) -1);
  
  eeprom.smartjets_to_add = _smartjets_to_add; // If the smartjets acts as slave, here it will be store 0 at EEPROM by default.
  
  DEBUG_PRINTLN("  DONE! * * *");
  
} // end of deserialize_http_payload function
/* --------------------------------------------------------------------- */


/* Check if the data provided by the GET request is actually valid before save at EEPROM */
bool save_smarjet_struct_data(struct smartjet_data *_data)
{
  DEBUG_PRINT("\n * * * CHECKING HTTP JSON CONTENT... ");
   
  // Check if the credentials is actually valid.
  if (is_credentials_valid())
  {
    DEBUG_PRINTLN("HTTP JSON CONTENT SUCCESSFULLY CHECKED!\n");
    
    save_data(_data); // Save the struct into EEPROM   
    return true;
  }
  else
  {
    DEBUG_PRINTLN(" Credentials INVALID!");
    Serial.print("\n Check the data and try again. Rebooting...");
    delay(DELAY_BEFORE_RESTART);
    ESP.restart();
    return false; // that will never be reached since there is a restart before.
  }
}
/* -------------------------------------------------------------------------- */

/* Initialize the Smartjet as an Access Point. This function should be used by the smartjet master */
bool initialize_wifi_as_ap()
{
  delay(150);
  DEBUG_PRINTLN("\nSetting Dispenser Network Wireless as soft-AP ... ");

  
  //WiFi.softAPConfig(ap_ip, ap_ip, ap_subnet);
  bool result = WiFi.softAP("SmartJet", "12345678"); // Credentials of the Access Point Wi-Fi
  
  delay(500); // Without delay there are issues about IP Adress blank.
  
  if (result == true)
  {
    DEBUG_PRINTLN("\nAccess Point configured succesfully");
  }
  else
  {
    DEBUG_PRINTLN("Setting soft-AP Failed!");
    // Reiniciar o ESP AQUI (?)
  }

  DEBUG_PRINT("Access Point IP Address: ");
  DEBUG_PRINTLN(WiFi.softAPIP());
  return result;
} // end of initialize_wifi_as_ap function.
/* - --------------------------------------------------------------------- */


/* Initialize the smartjet as Web Server. This function should be used by the smartjet master */
/* This function should be triggered only be the smartjet app */
void initialize_web_server()
{
  delay(150);
  DEBUG_PRINT("\nInitializing the Local Web Server...");
  
  /* Set the route and call the appropriate function under posts requests) */
  AsyncCallbackJsonWebHandler *handler = new
  AsyncCallbackJsonWebHandler("/user_credentials", [](AsyncWebServerRequest
  *request, JsonVariant &json) {
    
  DEBUG_PRINTLN("\n\t Handling Post Request ... ");
  DEBUG_PRINT("\n\t * * * DESERIALIZING HTTP JSON PAYLOAD... ");
  StaticJsonDocument<512> json_user_data;
  json_user_data = json.as<JsonObject>();
  
  // Collect the JSON Content
  String _email                    = json_user_data["tag"];
  String _ssid                     = json_user_data["ssid"];
  String _password                 = json_user_data["pass"];
  String _player_id                = json_user_data["idplayer"];
  unsigned int _smartjets_to_add   = json_user_data["number_smartjets"];


  // Since we are storing char type at EEPROM, it's necessary to convert types.
  // Note eeprom variable it was declared as global. That's why it's possible access it with '.' notation
  _email.toCharArray(eeprom.email, sizeof(eeprom.email) - 1);
  _ssid.toCharArray(eeprom.ssid, sizeof(eeprom.ssid) - 1); 
  _password.toCharArray(eeprom.password, sizeof(eeprom.password) - 1); 
  _player_id.toCharArray(eeprom.player_id, sizeof(eeprom.player_id) -1);
  
  // Here the Smartjet acts as Master. Then, smartjets_to_add should be equals ou higher then 0.
  // This variable control the moment that the web server is turned off
  eeprom.smartjets_to_add = _smartjets_to_add; 

  // Debug and Display Section
  DEBUG_PRINTLN("DONE! * * *");
  DEBUG_PRINT("Checking SSID: ");
  DEBUG_PRINTLN(eeprom.ssid);
  DEBUG_PRINT("Checking PASSWORD: ");
  DEBUG_PRINTLN(eeprom.password);
  DEBUG_PRINT("Checking Tag: ");
  DEBUG_PRINTLN(eeprom.email);
  
  is_first_time = false;

  // Set the response to the post request
  String response;
  serializeJson(json_user_data, response); // Serialize the post received before
  request->send(200, "application/json", response); // Send the same json content received under the post request
  Serial.println(response);
  }); // End of the post request route
  
  server.addHandler(handler); // Keep the route asynchronous. 
  
  /* Initialize the web server */
  server.begin(); 
  
  DEBUG_PRINT("WEB SERVER STARTED!\n");
} // end of  initialize_web_server() function.
/* ---------------------------------------------------------------- */



/* Handle get requests by smartjets slave */
/* This function should be triggered by smartjets slaves */
void handle_get_requests()
{
  DEBUG_PRINTLN("\n Now SmartJet starts to listen for incoming get requests ");

  // Route to be called always that there is a get request 
  server.on("/user_credentials", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (eeprom.smartjets_to_add > 0) // This block is executed only there are smartjets not registered yet.
    {
      DEBUG_PRINTLN("\n Handling Get Request ... ");
        
      // Json Object Instance
      DynamicJsonDocument json_user_data(512); // Computed with ArduinoJson Assistant
        
      // Json Payload to the request response
      String payload;
      String pretty_json;
  
      // Json Attributes
      json_user_data["tag"]              = eeprom.email;
      json_user_data["ssid"]             = eeprom.ssid;
      json_user_data["pass"]             = eeprom.password;
      json_user_data["idplayer"]         = eeprom.player_id;
      json_user_data["number_smartjets"] = 0; // Smartjets which make get requests receive 0 by default.
        
      // Serialize Json Attributes
      serializeJson(json_user_data, payload);
        
      // Send the payload when receive a get request
      request->send(200, "application/json", payload);
  
      // The number of smartjets to add reduces -1 because the request was done by some smartjet slave
      eeprom.smartjets_to_add -= 1; 
      Serial.printf("\n There are %i SmartJets remaining to add\n", (eeprom.smartjets_to_add));
      // salvar smartjets_to_add na eeprom? (implementar)
    }
    else if (eeprom.smartjets_to_add == 0)
    {
      DEBUG_PRINTLN("\n All Smartjets were added");
      Serial.print("\n Rebooting ESP...");
      ESP.restart();
    }
  });
}
/* ----------------------------------------------------------- */

/* This function connect the smartjet to the Wi-Fi Network */
bool initialize_wifi_as_client()
{
  DEBUG_PRINT("\nInitializing WiFi as client...");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(eeprom.ssid, eeprom.password);
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(500);
    DEBUG_PRINT(".");
  }
  DEBUG_PRINTLN("");
  DEBUG_PRINT("WiFi connected, IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());

} // end of initialize_wifi_as_client function.
/* ------------------------------------ */


bool is_credentials_valid()
{
  // check the sizeof email, ssid and pass (TO IMPLEMENT)
  return (initialize_wifi_as_client());
}



const char* set_softAP_ssid()
{
  char * _softAP_ssid = (char*)malloc(50 * sizeof(char));
  memset(_softAP_ssid, 0, 50 * sizeof(char));
  sprintf(_softAP_ssid, "SmartJet_%lu", ESP_CHIP_ID);
  return _softAP_ssid;
}
