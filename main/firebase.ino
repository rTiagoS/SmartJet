// Firebase Functions
/*
bool firebase_first_update()
{
  // Firebase Config
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth); // Initialize the library with Firebase authen and config


  DEBUG_PRINT("\n--------- FIRST UPDATE ON FIREBASE... ");

  // FirebaseJson Object Instance
  FirebaseJson dispenser_actions;
  FirebaseJson dispenser_parameters;

  // Formating the Json Object
  dispenser_parameters.set(String(get_timestamp()), String(level_count));
  dispenser_actions.set("action", "default");

  String dispenser_parameters_fb_path = "";
  String dispenser_actions_fb_path    = "";
  dispenser_parameters_fb_path = String(eeprom.email) + "/" + String(ESP_CHIP_ID) + "/" + "timestamp";
  dispenser_actions_fb_path    = String(eeprom.email) + "/" + String(ESP_CHIP_ID) + "/" + "action";
  
  if (Firebase.updateNode(fbdo, dispenser_parameters_fb_path, dispenser_parameters) and (Firebase.updateNode(fbdo, dispenser_actions_fb_path, dispenser_actions)))
  {
    DEBUG_PRINT(" DONE!\n");
    DEBUG_PRINT("\n Firebase Path: ");
    DEBUG_PRINTLN(fbdo.dataPath());
    return true;
  }
  else
  {
    DEBUG_PRINT("FAILED!\n");
    DEBUG_PRINTLN(fbdo.errorReason());
    delay(500);
    DEBUG_PRINTLN("Trying Again...");
  }

  
  return false;
}
*/

bool firebase_update_parameters()
{
  // Firebase Config
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth); // Initialize the library with Firebase authen and config

  DEBUG_PRINTLN("--------- UPDATING PARAMETERS ON FIREBASE... ----------");

  // FirebaseJson Object Instance
  FirebaseJson dispenser_actions;
  FirebaseJson dispenser_parameters;

  // Formating the Json Object
  dispenser_parameters.set(String(get_timestamp()), level_count);
  dispenser_actions.set("action", "default");

  String dispenser_parameters_fb_path = "";
  String dispenser_actions_fb_path    = "";
  dispenser_parameters_fb_path = String(eeprom.email) + "/" + String(ESP_CHIP_ID) + "/" + "timestamp";
  dispenser_actions_fb_path    = String(eeprom.email) + "/" + String(ESP_CHIP_ID);


if (Firebase.updateNode(fbdo, dispenser_parameters_fb_path, dispenser_parameters) and (Firebase.updateNode(fbdo, dispenser_actions_fb_path, dispenser_actions)))
  {
    DEBUG_PRINT("UPDATED!\n");
    DEBUG_PRINTLN(fbdo.dataPath());
    return true;
  }
  else
  {
    DEBUG_PRINT("FAILED!\n");
    DEBUG_PRINTLN(fbdo.errorReason());
    delay(500);
    DEBUG_PRINTLN("Trying Again...");
  }
  return false;
} // end of firebase_update_parameters function


bool firebase_check_for_delete()
{
  
  DEBUG_PRINTLN("--------- CHECKING FOR ACTIONS... ----------");
  
  // Firebase Config
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth); // Initialize the library with Firebase authen and config

  String dispenser_actions_fb_path = "";
  dispenser_actions_fb_path    = String(eeprom.email) + "/" + String(ESP_CHIP_ID) + "/";


  uint8_t try_again = 1;
  while (try_again <= 3)
  {
    if (Firebase.getJSON(fbdo, dispenser_actions_fb_path))
    {
      DEBUG_PRINTLN("--------- READING PARAMETERS ON FIREBASE... ----------");
      DEBUG_PRINTLN("PATH: " + fbdo.dataPath());
      DEBUG_PRINTLN("TYPE: " + fbdo.dataType());
      DEBUG_PRINT("VALUE: ");
      print_result(fbdo);
      DEBUG_PRINTLN("----------------------DONE!------------------");
      DEBUG_PRINTLN();
      return true;
    }
    else
    {
      DEBUG_PRINT("FAILED!\n");
      DEBUG_PRINTLN(fbdo.errorReason());
      delay(500);
      DEBUG_PRINTLN("Trying Again...");
      try_again += 1;
    }
    return false;
  }
}

void print_result(FirebaseData &data) // melhorar essa função
{
  DEBUG_PRINTLN();
  FirebaseJson &json = data.jsonObject();
  StaticJsonDocument<512> doc; // Size computed by using Arduino Json Assistant

  //Print all object data
  DEBUG_PRINTLN("Pretty printed JSON data: ");
  String jsonStr;
  json.toString(jsonStr, true);

  DEBUG_PRINTLN(jsonStr);
  DEBUG_PRINTLN();

  deserializeJson(doc, jsonStr);

  //Parameters
  String _action = doc["action"];



  if (_action == "\"delete\"")
  {
    DEBUG_PRINTLN( "Action requested: delete");
    clear_eeprom();
    DEBUG_PRINTLN( "Restarting ESP");
    ESP.restart(); // medida provisória

  }
  else if (_action == "default")
  {
    DEBUG_PRINTLN( "Action requested: default");
  }

  /*
    DEBUG_PRINTLN("Iterate Json data: ");
    DEBUG_PRINTLN();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++)
    {
    json.iteratorGet(i, type, key, value);
    DEBUG_PRINT(i);
    DEBUG_PRINT(", ");
    DEBUG_PRINT("Type: ");
    DEBUG_PRINT(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
    if (type == FirebaseJson::JSON_OBJECT)
    {
      DEBUG_PRINT(", Key: ");
      DEBUG_PRINT(key);
    }
    DEBUG_PRINT(", Value: ");
    DEBUG_PRINTLN(value);
    }
    json.iteratorEnd();
  */
}
