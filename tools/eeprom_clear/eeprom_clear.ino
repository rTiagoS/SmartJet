/*
   EEPROM Clear

   Sets all of the bytes of the EEPROM to 0.
   This example code is in the public domain.

*/

#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Apagando memória...");
  EEPROM.begin(1024);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 1024; i++) {
    EEPROM.write(i, 0);
  }

  // turn the LED on when we're done
  EEPROM.end();
  Serial.println("Memória EEPROM Apagada!");
}

void loop() {
}
