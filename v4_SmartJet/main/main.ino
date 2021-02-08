#include "SmartJet.h"


void setup()
{
  delay(3000);
  Serial.begin(115200);
  Serial.println("teste");

  /* - - - Pinout Initialization Settings - - - */
  pinMode(pump_pin , OUTPUT);
  //pinMode(level0_pin, INPUT_PULLUP);
  //pinMode(level1_pin, INPUT_PULLUP);
  
  digitalWrite(pump_pin, LOW); // Pump start turned down.
  /* - - - - - - - - - - - - - - - - - - - - - - */

  
  load_credentials();
  Serial.print("SSID Após LOADING: ");
  Serial.println(ssid);
  Serial.print("PASS Após LOADING: ");
  Serial.println(password);
  
  is_first_time = (strlen(ssid) == 0);

  if (is_first_time)
  {
    /* - - - Internet Settings - - - */
    initialize_wifi_as_ap();
    initialize_web_server();
    /* - - - - - - - - - - - - - - - -*/
    
    handle_web_server();
    
    ESP.deepSleep(0); // After publish, go into deepSlep.
  }
  else
  {
    /* - - - Load Dispenser Parameters - - - */
    load_email();
    //load_triggers();
    //load_level();
    /* - - - - - - - - - - - - - - - - - - - */

    external_awakening(); // Time to work!
     
    if (is_time_publish())
    {
      /* - - - Internet Settings - - - */
      initialize_wifi_as_client();
      firebase_update_parameters();
      /* - - - - - - - - - - - - - - - */
      
      ESP.deepSleep(0); // After publish, go into deepSlep.
    }
  }
}

void loop()
{
}
