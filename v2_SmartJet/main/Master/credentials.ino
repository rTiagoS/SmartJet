/** Load WLAN credentials from EEPROM */
void loadCredentials() {
  EEPROM.begin(512);
  EEPROM.get(0, ssid);
  EEPROM.get(0 + sizeof(ssid), password);
  char ok[2 + 1];
  EEPROM.get(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    ssid[0] = 0;
    password[0] = 0;
  }
  Serial.println("Recovered credentials:");
  Serial.println(ssid);
  Serial.println(strlen(password) > 0 ? "********" : "<no password>");
}

/** Store WLAN credentials to EEPROM */
void saveCredentials() {
  EEPROM.begin(512);
  EEPROM.put(0, ssid);
  EEPROM.put(0 + sizeof(ssid), password);
  char ok[2 + 1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);
  EEPROM.commit();
  EEPROM.end();
}

void saveEmail()
{
  EEPROM.begin(512);
  char ok[2+1] = "OK";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok), email);
  char ok_email[2+1] = "ok";
  EEPROM.put(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email), ok_email);
  EEPROM.commit();
  EEPROM.end();
}

void loadEmail()
{
  EEPROM.begin(512);
  char ok[2+1] = "OK";
  char ok_email[2+1] = "ok"; 
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok), email);
  EEPROM.get(0 + sizeof(ssid) + sizeof(password) + sizeof(ok) + sizeof(email), ok_email);
  if (String(ok_email) != String("ok"))
  {
    email[0] = 0;
  }
  EEPROM.end();
  Serial.println("\nRecovered email:");
  Serial.println(email);  
}
