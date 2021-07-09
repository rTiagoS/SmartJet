//extern "C"
//{
//  #include "gpio.h"
//  #include "user_interface.h"
//}

#include <ESP8266WiFi.h>
#include <coredecls.h> //crc32()
#include <PolledTimeout.h>
#include <include/WiFiState.h> // WiFiState structure details


//#define DEBUG // prints WiFi connection ifo to serial, uncomment if you want WiFi messages

#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif


#define WAKE_UP_PIN 2 //D3//GPIO00 can also force a serial flash upload with RESET.
#define LED 2 // D4/GPIO2 LED for ESP-01,07 modules;

const char* ssid = "LIVE TIM_3322_2G";
const char* password = "KyxqxYSG";
uint32_t timeout = 30E3;  // 30 second timeout on the WiFi connection

//This structure is stored in RTC memory to save WiFi state, triggers and level of the SmartJet
// and it reconnects twice as fast as the first connection;
// After WiFiState, first field is CRC32, which is calculated based on the rest of structure contents.
// The others fields go after CRC32.
struct nv_s
{
  WiFiState wss; // core's WiFi save state

  struct 
  {
    uint32_t crc32;
    uint32_t triggers; // Stores the sensor triggers count
    uint32_t level; // Stores the liquid level
  } rtcData;
};

static nv_s* nv = (nv_s*)RTC_USER_MEM; // User RTC RAM Area

uint32_t triggers_count = 0; // Keeps track of the number of triggers.
uint32_t level_count = 100; // Keeps track the alcohol liquid level.

void wake_up_callback()
{
  Serial.println(F("Woke from Light Sleep - This is the Callback"));
}

void setup()
{
  nv->rtcData.triggers = 0;
  nv->rtcData.level= 100;
  Serial.begin(115200);
  Serial.println();
  delay(1000);
  //gpio_init(); 
  pinMode(WAKE_UP_PIN, INPUT);
  Serial.println();
  Serial.print(F("\n Reset Reason = "));
  String reset_cause = ESP.getResetReason();
  Serial.println(reset_cause);

  if ((reset_cause == "External System") || (reset_cause == "Power on"))
  {
    Serial.println("I'm awake and starting the routines");
  }
  Serial.println("Actual Triggers: ");
  Serial.print(triggers_count);
  Serial.println("\n Actual Level: ");
  Serial.print(level_count);
}

void loop()
{
  // Read previous triggers_counts from RTC memory, if any
  uint32_t crc_of_data = crc32((uint8_t*) &nv->rtcData, sizeof(nv->rtcData));
  if ((crc_of_data= nv->rtcData.crc32))
  {
    triggers_count = nv->rtcData.triggers; // Read the previous triggers count
    triggers_count++;
    level_count = nv->rtcData.level;
    level_count-=10;
  }
  nv->rtcData.triggers = triggers_count; // update the resent count & CRC
  nv->rtcData.level = level_count;
  updateRTCcrc();
  Serial.println("Actual Triggers: ");
  Serial.print(triggers_count);
  Serial.println("\n Actual Level: ");
  Serial.print(level_count);
  
  init_wifi();
  delay(500);
  Serial.println("Going to sleep now");
  light_sleep();
  delay(200);
  Serial.println("Wake up");

}

void light_sleep()
{
  Serial.println(F("\n - - - - -Forced Light Sleep, wake with GPIO interrupt - - - - "));
  Serial.flush();
  //wifi_station_disconnect();
  WiFi.mode(WIFI_OFF); // you must turn the modem off; using disconnect won't work.
  //wifi_set_opmode_current(NULL_MODE);
  Serial.println(F("\nCPU goiing to sleep, pull WAKE_UP_PIN low through sensor to wake it"));
  
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  
  // only LOLEVEL or HILEVEL interrupts work, no edge, that's an SDK or CPU limitation
  gpio_pin_wakeup_enable(GPIO_ID_PIN(WAKE_UP_PIN), GPIO_PIN_INTR_LOLEVEL); 

  wifi_fpm_set_wakeup_cb(wake_up_callback); // set wakeup callback

  wifi_fpm_open();
  wifi_fpm_do_sleep(0xFFFFFFF);
  delay(10); // it goes to sleep during this delay() and waits for an interrupt
  Serial.println(F("Woke up!"));  // the interrupt callback hits before this is executed
} 

void init_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void updateRTCcrc()
{  // updates the reset count CRC
  nv->rtcData.crc32 = crc32((uint8_t*) &nv->rtcData, sizeof(nv->rtcData));
}
