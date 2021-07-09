#include "SmartJet.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif


struct smartjet_data  eeprom;

void setup()
{
  Serial.begin(115200);
  delay(1000); // Take some time to open up the Serial Monitor

  DEBUG_PRINTLN("\n***** ESP32 Waking Up: Setup Initialization *****\n");
  

  print_wakeup_reason(); // Print the wakeup reason for ESP32

  /* = = = Pinout Initialization Settings = = = */
  pinMode(pump_pin , OUTPUT); // Pump pin
  pinMode(WAKE_UP_PIN, INPUT); // Pin that provides the wake up: obstacle sensor
  //pinMode(LEVEL_LEDS_PIN, OUTPUT); // Pin to activate the levels indicator 
  //pinMode(level_100, INPUT_PULLUP);
  //pinMode(level_75, INPUT_PULLUP);
  //pinMode(level_50, INPUT_PULLUP);
  //pinMode(level_25, INPUT_PULLUP);
  

  digitalWrite(LEVEL_LEDS_PIN, LOW);
  digitalWrite(pump_pin, LOW); // Pump starts off.
  /* = = = = = = = = = = = = = = = = = = = = = = */

  /* = = = READ DATA STRUCT FROM EEPROM = = = */
  read_data(&eeprom);
  
  /* - - - - - - - - - - - - - - - - - - - - - */

  
  is_first_time = (strlen(eeprom.ssid) == 0); // Return true if the EEPROM is empty. 


  
  if (is_first_time)
  {
    /* = = = FIRST TIME CONDITION: REGISTER THE SMARTJETS = = = */
    
    connect_result = connect_to_smartjet_AP(); // Try to connect to a smartjet device (master) in order to get the credentials.

    
    if (connect_result == true)
    {
      /* = = = SMARTJETS ACTING LIKE A SLAVE = = = */
      
      String payload = http_request_json_data("http://192.168.4.1/user_credentials"); // Try to request user credentials (stuffs about Wi-Fi credentials).

      
      if (request_result == true)
      {
        
        deserialize_http_payload(payload); // Update the eeprom struct before check if the data provided are really correct.
        
        // Save the data provided by the content payload into EEPROM only after validate the data.
        save_smarjet_struct_data(&eeprom);
        
        rtc_reset_count(); // Reset parameters count at the first time
        firebase_update_parameters(); // Create a firebase directory for the first time
        
        ESP.restart(); // Restart. Then the smartjet start to work normally.
      }
    }
    else
    {
      /* = = = SMARTJETS ACTING LIKE A MASTER = = = */
      
      /* - - - Web Services Initializatiion - - - */ 
      initialize_wifi_as_ap(); // Initialize the smartjet (master) Wi-Fi  as Access Point     
      initialize_web_server(); //  It starts to listen for incoming posts requests. (Async Web Server)
      
      DEBUG_PRINT("Waiting for a Post Request");

      /*  while the number of smartjets to be registered has not been reached yet, loop. */
      while (is_first_time != false)
      {
        
        DEBUG_PRINT(".");
        delay(500);   
      }
      
      bool save_result{false};
      save_result = save_smarjet_struct_data(&eeprom);
      if (save_result == true)
      {
        rtc_reset_count(); // Reset parameters count at the first time.
        firebase_update_parameters(); // Create directories on RTDB. It's the first publish.
        handle_get_requests(); // Now it can answer for incoming get requests.
      }
      else
      {
        DEBUG_PRINTLN(" Please, confirm the credentials informed and try again. ");
        Serial.print("\n Rebooting ESP...");
        ESP.restart();
      }
    }
  }

    
  else
  {
    /* = = = REACH HERE WHEN THE SMARTJET IS ALREADY REGISTERED = = = */
    
    trigger_pump(); // Every wake up from deep sleep, the smartjet striggers gel
    
    //show_indicator_level_leds(); // NOT USED -> NO BATTERIES WILL BE USED

    //get_liquid_level();          // IT WILL BE USED WHEN SOLVE A CONSISTENT WAY TO MEASURE THE LEVEL.

    update_RTC_dispenser_parameters(); // Update the dispenser parameters at RTC memory
   

    if (is_time_publish()) // Conditions to publish: for each 5 triggers.
    {
      initialize_wifi_as_client(); // Turn on the Wi-Fi connection
      
      firebase_check_for_delete(); // Check into firebase if the user decided for deleting this smartjet.
      
      /* - - - Internet Settings - - - */
      firebase_update_parameters(); // Update the dashboard app.
      /* - - - - - - - - - - - - - - - */
      if (is_time_to_notifications()) // Conditions to push notifications: low battery level or low liquid level
      {
        //search_for_id_player();
        push_notification("Warning!!!: SmartJet Low Level\nPlease, before another use, you must recharge it!");
      }
    }
    
    delay(700);
    
    factory_reset(); // Check for user request to reset the Smart Jet from factory. 
    
    go_light_sleep(); // Actually, it is deep sleep, not light sleep mode.
  }
}
void loop()
{
}
