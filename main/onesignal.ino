void push_notification(String msg)
{

  DEBUG_PRINT("* * * * ONESIGNAL PUSH: connecting to ");
  DEBUG_PRINTLN(host);

  //Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setCACert(test_root_ca);

  if (!client.connect(host, httpsPort))
  {
    DEBUG_PRINTLN("connection failed");
    return;
  }
  DEBUG_PRINTLN("PUSH: try to send push notification...");

  // please, do not change app_id - its IoT Manager id at onesignal.com
  // more info at https://documentation.onesignal.com/v3.0/reference#create-notification

  String data = "{\"app_id\": \"1a1152c4-f8cf-41d9-b745-096685cd81af\",\"include_player_ids\":[\"" + String(eeprom.player_id) + "\"],\"data\": {\"foo\": \"bar\"},\"contents\": {\"en\": \"" + msg + "\"}}";

  DEBUG_PRINTLN(data);
  DEBUG_PRINT("PUSH: requesting URL: ");
  DEBUG_PRINTLN(url);

  client.println(String("POST ") + url + " HTTP/1.1");
  client.print("Host:");
  client.println(host);
  client.println("User-Agent: esp8266.Arduino.IoTmanager");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.println(data);

  ("PUSH: done. Restart esp8266 for push again.");
}

unsigned long get_timestamp()
{
  const char* ntp_server = "pool.ntp.org"; // NTP server to request epoch time

  const long  gmtOffset_sec = 14400; // Manaus GMT
  
  //const int   daylightOffset_sec = 3600;

  configTime(gmtOffset_sec, 0, ntp_server);

  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}
