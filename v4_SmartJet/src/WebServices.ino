bool initialize_wifi_as_ap()
{
  Serial.println("Setting Dispenser Network Wireless as soft-AP ... ");
  WiFi.softAPConfig(ap_ip,ap_ip, ap_subnet);
  bool result = WiFi.softAP(softAP_ssid);
  delay(500); // Without delay there are issues about IP Adress blank.
  if (result == true)
  {
    Serial.println("Access Point configured succesfully");  
  }
  else
  {
    Serial.println("Setting soft-AP Failed!");
  }
  
  Serial.print("Access Point IP Address: ");
  Serial.println(WiFi.softAPIP());
  return result; 
} // end of initialize_wifi_as_ap function.


void initialize_web_server()
{
  Serial.println("Initializing the Local Web Server...");
  
  /* Set the route and call the appropriate function under requests) */
  server.on("/user_credentials", handle_post_request);

  /* Initialize the web server */
  server.begin(); 
} // end of  initialize_web_server() function.


bool initialize_wifi_as_client()
{
  
  Serial.println("Connecting Dispenser to the User's WiFi as client...");
  WiFi.disconnect(); // Disconnect from any previous connection (just to be sure)
  WiFi.begin(ssid, password); // Given ssid and password, connect to the user WiFi.
  int connection_result = WiFi.waitForConnectResult();

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf("Connection Failed! ERROR NUMBER: %d\n", WiFi.status());
    return false; 
  }
  else
  {
    Serial.println("Connected!");
    return true;
  }

} // end of initialize_wifi_as_client function.




bool is_credentials_valid()
{
  // check the sizeof email, ssid and pass.
  return (initialize_wifi_as_client());
}

void handle_web_server()
{
  Serial.println(" Handling Clients Web Server Requests...");
  while (is_first_time)
  {
    server.handleClient();
  } 
}

void handle_post_request()
{
  Serial.println("\n Handling Post Request ... ");
  StaticJsonDocument<256> doc; // Size computed by using Arduino Json Assistant
  deserializeJson(doc, server.arg("plain"));

  //Parameters
  String _email = doc["email"]; 
  String _ssid = doc["ssid"];
  String _password = doc["pass"];
  server.send(204, "text/plain", "ESP_ID"); // Alterar para por o ID do ESP.

  _email.toCharArray(email, sizeof(email) - 1);
  _ssid.toCharArray(ssid, sizeof(ssid) - 1);
  _password.toCharArray(password, sizeof(password) - 1);
  
  if (is_credentials_valid())
  {
    save_credentials();
    is_first_time = false;
  }
  else
  {
    Serial.println(" Please, confirm the credentials informed and try again. ");
  }  
}

const char* set_softAP_ssid()
{
  char * _softAP_ssid = (char*)malloc(50*sizeof(char));
  memset(_softAP_ssid, 0, 50*sizeof(char));
  sprintf(_softAP_ssid, "SmartJet_%lu", ESP_CHIP_ID);
  return _softAP_ssid;
}

bool firebase_update_parameters()
{
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth); // Initialize the library with Firebase authen and config 

  int try_again = 1;
  while (try_again <= 3)  // Loop until update the actual dispenser data
  {
    Serial.println("--------- UPDATING PARAMETERS ON FIREBASE... ----------");
      
    FirebaseJson dispenser_id;
    FirebaseJson dispenser_parameters;

    dispenser_parameters.set("level", String(level));
    dispenser_parameters.set("jets", String(triggers));
    dispenser_id.set(String(ESP_CHIP_ID), dispenser_parameters);

    //if (Firebase.updateNode(fbdo, String("/iacinovation/smartjet/"+String(email)), dispenser_id))
    if (Firebase.updateNode(fbdo, String("/iacinovation/smartjet/tiago,ramos121@gmail,com"), dispenser_id))
    {
      Serial.print("UPDATED!\n");
      Serial.println(fbdo.dataPath());
      return true;
    }
    else
    {
      Serial.print("FAILED!\n");
      Serial.println(fbdo.errorReason());
      delay(500);
      Serial.println("Trying Again...");
      try_again +=1;
    }
  }
  return false;
}
