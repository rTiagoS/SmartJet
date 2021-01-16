/** Handle root or redirect to captive portal */
void handleRoot() {
  /*
   * Atualmente não é interessante a funcionalidade do captivePortal para a aplicação.
   * 
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  */
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page; // Constrói o o cabeçalho e corpo da mensagem que será mostrada no navegador.
  Page += F(
            "<!DOCTYPE html><html lang='en'><head>"
            "<meta name='viewport' content='width=device-width'>"
            "<title>SMARJET NETWORK SETUP</title></head><body>"
            "<h1> SMARTJET - WIRELESS NETWORK SETTINGS</h1>");
  if (server.client().localIP() == apIP) // Se o client fazer uma requisição no IP gerado pelo Access Point:
  {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>"); // Avisa que o cliente está na rede AP
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>"); // Avisa que o cliente está pela rede WLAN
  }
  Page += F(
            "<p> Click <a href='/wifi'>here to start the setup</a>.</p>"
            "</body></html>"); // Rota para configuração da conexão WiFi

  server.send(200, "text/html", Page); // Envia cabeçalho e corpo da mensagem para ser visualizada pelo cliente no navegador.
}

/** Wifi config page handler */
void handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += F(
            "<!DOCTYPE html><html lang='en'><head>"
            "<meta name='viewport' content='width=device-width'>"
            "<title>CaptivePortal</title></head><body>"
            "<h1>Wifi config</h1>");
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>");
  }
  Page +=
    String(F(
             "\r\n<br />"
             "<table><tr><th align='left'>SoftAP config</th></tr>"
             "<tr><td>SSID ")) +
    String(softAP_ssid) +
    F("</td></tr>"
      "<tr><td>IP ") +
    toStringIp(WiFi.softAPIP()) +
    F("</td></tr>"
      "</table>"
      "\r\n<br />"
      "<table><tr><th align='left'>WLAN config</th></tr>"
      "<tr><td>SSID ") +
    String(ssid) +
    F("</td></tr>"
      "<tr><td>IP ") +
    toStringIp(WiFi.localIP()) +
    F("</td></tr>"
      "</table>"
      "\r\n<br />"
      "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>");
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      Page += String(F("\r\n<tr><td>SSID ")) + WiFi.SSID(i) + ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F(" *")) + F(" (") + WiFi.RSSI(i) + F(")</td></tr>");
    }
  } else {
    Page += F("<tr><td>No WLAN found</td></tr>");
  }
  Page += F(
            "</table>"
            "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
            "<input type='text' placeholder='network' name='n'/>"
            "<br /><input type='password' placeholder='password' name='p'/>"
            "<br /><input type='submit' value='Connect/Disconnect'/></form>"
            "<p>You may want to <a href='/'>return to the home page</a>.</p>"
            "</body></html>");
  server.send(200, "text/html", Page);
  server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveCredentials();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleCredentialsRequest()
{
  Serial.println("WiFi Credentials Requested");
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  DynamicJsonDocument doc(512);
 
  doc["ssid"] = ssid;
  doc["pass"] = password;
      
  Serial.print(F("Stream..."));
  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);
  Serial.print(F("done."));
}

void handleMasterID()
{
  
  Serial.println("");
  Serial.println("Master ID Requested: ");
  Serial.print(esp_chipID);

  String buf;
  
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  
  DynamicJsonDocument doc(64);
  doc["HWID"] = esp_chipID;
  
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);

  Serial.println("");
  Serial.println(F("ID Entregue."));
}

void handleSaveEmail()
{
  Serial.println("\n Salvando E-mail ...");
  server.arg("email").toCharArray(email, sizeof(email) - 1);
  
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveEmail();
  Serial.println("\n Email Handle Done");
}
