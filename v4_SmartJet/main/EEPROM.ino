void save_credentials()
{
  Serial.println(" Saving credentials... ");
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0 + sizeof(ssid), password);
  char ok[2+1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.commit();
  EEPROM.end();
  Serial.print(" saved!\n");
} // end of save_credentials function

void load_credentials()
{
  Serial.print(" Loading credentials... ");
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0 + sizeof(ssid), password);
  char ok[2+1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.end();
  if (String(ok) != String("OK"))
  {
    Serial.print(" loading credentials failed!\n");
  }
  else
  {
    Serial.print(" loaded!\n");  
  }
  
  Serial.print("\nRecovered SSID: ");
  Serial.print(ssid);
  Serial.print("\nRecovered PASSWORD: ");
  Serial.print(password);
  Serial.println();
  
} // end of load_credentials function.

void save_email()
{
  Serial.println(" Saving credentials... ");
  EEPROM.begin(512);
  char ok[2+1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok), email);
  char ok_email[2+1] = "ok";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email), ok_email);
  EEPROM.commit();
  EEPROM.end();
  Serial.print(" saved!\n");
} // end of save_email function

void load_email()
{
  EEPROM.begin(512);
  char ok[2+1] = "OK";
  char ok_email[2+1] = "ok"; 
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok), email);
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email), ok_email);
  
  EEPROM.end();
  if (String(ok_email) != String("ok"))
  {
    Serial.print(" loading email failed!\n");
  }
  else
  {
    Serial.print(" loaded!\n");  
  }
  Serial.println("\nRecovered email:");
  Serial.print(email);
  Serial.println(); 
} // end of load_email function



void save_level()
{
  Serial.println(" Saving level... ");
  EEPROM.begin(512);
  char ok[2+1] = "OK";
  char ok_email[2+1] = "ok";
  char ok_level[2+1] = "ok";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email), level);
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email) + sizeof(level), ok_level);
  EEPROM.commit();
  EEPROM.end();
  Serial.print(" saved!\n");
} // end of save_level function

void load_level()
{
  Serial.println(" Loading level... ");
  EEPROM.begin(512);
  char ok[2+1]= "OK";
  char ok_email[2+1] = "ok";
  char ok_level[2+1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email), level);
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email) + sizeof(level), ok_level);
  EEPROM.end();
  if (String(ok_level) != String("ok"))
  {
    Serial.print(" loading level failed!\n");
  }
  else
  {
    Serial.print(" loaded!\n");  
  }
  
  Serial.println("\nRecovered  level: ");
  Serial.print(level);
  Serial.println();
} // end of load_level function

void save_triggers()
{
  Serial.println(" Saving triggers... ");
  EEPROM.begin(512);
  char ok[2+1] = "OK";
  char ok_email[2+1] = "ok";
  char ok_level[2+1] = "ok";
  char ok_triggers[2+1] = "ok";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email) + sizeof(level) +
              sizeof(ok_level), triggers);
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email) + sizeof(level) +
              sizeof(ok_level) + sizeof(triggers), ok_triggers);
  EEPROM.commit();
  EEPROM.end();
  Serial.println(" saved!\n");
} // end of save_triigers function.

void load_triggers()
{
  Serial.println(" Loading triggers... ");
  EEPROM.begin(512);
  char ok[2+1];
  char ok_email[2+1]= "ok";
  char ok_level[2+1] = "ok";
  char ok_triggers[2+1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email) + sizeof(level) +
              sizeof(ok_level), triggers);
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email) + sizeof(ok_email) + sizeof(level) +
              sizeof(ok_level) + sizeof(triggers), ok_triggers);
  EEPROM.end();
  if (String(ok_triggers) != String("ok"))
  {
    Serial.print(" loading level failed!\n");
  }
  else
  {
    Serial.print(" loaded!\n");  
  }
  
  Serial.println("\nRecovered  triggers: ");
  Serial.print(triggers);
  Serial.println();
}
