void external_awakening()
{
  Serial.println(" External awakening triggered ");
  
  delay(200); // Just a time delay
  //load_triggers(); // Load from EEPROM the triggers info
  //load_level(); // Load from EEPROM the level info
  
  if (level > 30) // If the level is above the allowed
  {
    digitalWrite(pump_pin, HIGH); // Turn on the pump
    delay(750); //Keep the pump turned on for 750 ms

    digitalWrite(pump_pin, LOW); // TUrn off the pump
    
    triggers += 1; // Keep tracking how many times the pump was triggered
    level = get_liquid_level(); // Keep tracking the liquid level.
    save_triggers(); // Save the triggers info on EEPROM
    save_level(); // Save the level info on EEPROM
  }
  else
  {
    Serial.println(" Low Level Detected. You must recharge it before another use ");
    //Faz o que mais?
  }
} // end of external_awakening function


bool is_time_publish()
{
  if ((triggers != 0) and (triggers % 10 == 0))
  {
    return true;
  }
  else
  {
    return false;
  }
} // end of check_triggers_before_publish()


int get_liquid_level()
{
  // Implementar rotina para fazer leitura dos sensores de nível/
  // FIxando valor apenas para testar.
  return 40;
} // end of get_liquid_level function.
