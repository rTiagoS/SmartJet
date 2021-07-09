#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif


// Trigger the pump pin. It activates the pump.
void trigger_pump()
{
  delay(50); 
    
  if (level_count >= 30) // If the level is above the allowed
  {
    digitalWrite(pump_pin, HIGH); // Turn on the pump
    delay(175); //Keep the pump turned on for 750 ms

    digitalWrite(pump_pin, LOW); // TUrn off the pump
    
    
    //level = get_liquid_level(); // Keep tracking the liquid level.

  }
  else
  {
    DEBUG_PRINTLN(" Low Level Detected. You must recharge it before another use ");
    //Faz o que mais?
  }
} // end of external_awakening function
/* ------------------------------------------------------------- */


/* Logic function that checks for 5 consecutive triggers */
bool is_time_publish()
{
  //if ((triggers_count != 0) and (triggers_count % 10 == 0))
  if ((triggers_count != 0) and (triggers_count % 5) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
} // end of check_triggers_before_publish()
/* --------------------------------------------------- */


/* Check for low battery level (not used) or low liquid level (actually used) */
bool is_time_to_notifications()
{
  if ((level_count <= 30) or (battery_level <= 25))
  {
    return true;
  }
  else
  {
    return false;
  }
}
/* ------------------------------------------------- */

/* Read the liquid level */
/* not been used */
int get_liquid_level()
{
  bool demo = true;
  if (demo)
  {
    return 100;
  }
  // Read the Liquid Level by sensoring the logic level of the water sensor
  if (digitalRead(level_100) == LOW)
  {
    Serial.println("\n Level: 100");
    return 100;
  }
  else if (digitalRead(level_75) == LOW)
  {
    Serial.println("\n Level: 75");
    return 75;
  }
  else if (digitalRead(level_50) == LOW)
  {
    Serial.println("\n Level: 50");
    return 50;
  }
  else
  {
    Serial.println("\n Level: 25");
    return 25;
  }
} // end of get_liquid_level function.


/* Function that triggers the deep sleep mode */
void go_light_sleep()
{
  DEBUG_PRINTLN(F("\n - - - - -Forced Deep Sleep, wake with GPIO Sensor interrupt - - - - "));

  WiFi.mode(WIFI_OFF); // you must turn the modem off; using disconnect won't work.
 
  DEBUG_PRINTLN(F("\n - - - - CPU going to sleep, pull WAKE_UP_PIN low through sensor to wake it - - - -"));
  
  
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 0); // Wake the ESP if there is a level change from high to low.

  esp_deep_sleep_start();
  delay(10); // it goes to sleep during this delay() and waits for an interrupt
  DEBUG_PRINTLN(F("This will never be printed"));  // the interrupt callback hits before this is executed
}
/* - ---------------------------------------------- */


/* This function allow the user to reset the smartjet from factory default */
void factory_reset()
{
  unsigned long time_out = millis();
  unsigned int count_second{0};
  
  while (digitalRead(WAKE_UP_PIN) == LOW)
  {
    if (count_second >= 15)
    { 
      clear_eeprom();
      ESP.restart();
      break;
    }
    delay(1000);
    //Serial.print("\nTime Count = ");
    //Serial.printf("%d\n", count_second);
    count_second += 1;
  }
} // end factory_reset function
/* --------------------------------------- */

void show_indicator_level_leds()
{
  digitalWrite(LEVEL_LEDS_PIN, HIGH);
  read_battery_level();
  delay(1500);
  digitalWrite(LEVEL_LEDS_PIN, LOW);
}


/* Not been used because the smartjet has been using DC Power Supply, not batteries. */
void read_battery_level()
{
  /*  3.3v --------> 4096                   | 2.4v --------> 100 %             | For 50% => x = 1.2v
   *  2.4v --------> x => x = 2979 (100%).  | x    --------> 75 % => x = 1.8v  | For 25% => x = 0.6v
   *  Level 100: 2979 - 200 <= x <= 2979 + 200
   *  Level 75:  2234 - 200 <= x <= 2234 + 200
   *  Level 50:  1489 - 200 <= x <= 1489 + 200
   *  Level 25:  744 -  200 <= x <= 744 + 200
   */
  battery_level = analogRead(battery_level_pin);
  if ((battery_level >= (2979 - 300)) and (battery_level <= (2979 + 300)))
  {
    battery_level = 100;
  }
  else if ((battery_level >= (2234 - 200)) and (battery_level <= (2234 + 200)))
  {
    battery_level = 75;
  }
  else if ((battery_level >= (1489 - 200)) and (battery_level <= (1489 + 200)))
  {
    battery_level = 50;
  }
  else if ((battery_level >= (744 -  200)) and (battery_level <= (744 + 200)))
  {
    battery_level = 25;
  }
  else
  {
    battery_level = 200;
  }
}
/* --------------------------------------------------------------- */
