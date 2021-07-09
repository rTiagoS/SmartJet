#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif

/* This function allows to clear all the EEPROM memory */
void clear_eeprom()
{
  DEBUG_PRINT("\n Clearing EEPROM... ");
  EEPROM.begin(1024);
  for (int i = 0; i < 1024; i++)
  {
    EEPROM.write(i, 0);
    
  }
  EEPROM.end();
  DEBUG_PRINT(" DONE!\n");
}

void save_data(struct smartjet_data *_data)
{
  /* Write data struct on eeprom. This struct contains:
   *  1. char ssid[100];
   *  2. char password[100];
   *  3. char app_id[100];
   *  4. char email[100];
   *  5. unsigned int smartjets_to_add;
   */
   DEBUG_PRINT("\n Saving smartjet data struct on EEPROM... ");
   char ok[2+1] = "OK";
   EEPROM.begin(1024);
   EEPROM.put(0, (*_data));
   EEPROM.put(0 + sizeof(*_data), ok);
   EEPROM.commit();
   EEPROM.end();
   DEBUG_PRINT (" DONE!\n");
} // end of save_data function


void read_data(struct smartjet_data *_data)
{
  /* Read the data struct saved beforehand at EEPROM and return the address */
  DEBUG_PRINT( " Loading smartjet data.... ");
  
  char ok[2+1];
  EEPROM.begin(1024);
  EEPROM.get(0, *_data);
  EEPROM.get(0 + sizeof(*_data), ok);
  EEPROM.end();
  if (String(ok) != String("OK"))
  {
    DEBUG_PRINT( " FAILED! \n");
    DEBUG_PRINTLN("\n * * * DEBUGGING EEPROM ZONE: * * *");
    DEBUG_PRINT("\n Recovered SSID: ");
    DEBUG_PRINT(_data->ssid);
    DEBUG_PRINT("\n Recovered PASSWORD: ");
    DEBUG_PRINTLN(_data->password);
    DEBUG_PRINTLN("\n * * * EXITING DEBBUGING EEPROM ZONE * * *");
  }
  else
  {
    DEBUG_PRINT( " SUCCESS! \n");
    DEBUG_PRINTLN("\n * * * DEBUGGING EEPROM: * * *");
    DEBUG_PRINT("\n Recovered SSID: ");
    DEBUG_PRINT(_data->ssid);
    DEBUG_PRINT("\n Recovered PASSWORD: ");
    DEBUG_PRINT(_data->password);
    DEBUG_PRINTLN("\n * * * EXITING DEBBUGING EEPROM ZONE * * *");
  }
} // end of read_data function
