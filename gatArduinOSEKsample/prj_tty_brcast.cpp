/* --- Button status broadcast of status buttons and timer measurements --- */

#include "Arduino.h"
#include "gttc_types.h"
#include "gttc_timer.h"
#include "prj_cfg.h"
#include "prj_dre.h"
#include "prj_tty_brcast.h"

#ifndef CFG_USE_I2C

uint8_t contButton=BUTTON_BROADCAST_PERIOD_CYCLES;
uint8_t currentPacket=0;

void buttonBroadcast(void) {

  uint8_t i;
  char bufaux[2];

  // Vemos si nos toca refrescarlos o no
  contButton--;
  if (contButton<=0){
    contButton=BUTTON_BROADCAST_PERIOD_CYCLES;
    // Los volcamos por el puerto serie
#ifdef CFG_BROADCAST_DEBUG_TIME
    Serial.print(syncInvocations);
    syncInvocations=0;
    Serial.print(" | ");-
    Serial.print(busyMicros);
#ifdef TIMEBASE_USE_MS
    Serial.print(" | ");
    Serial.print(busyMillis);
    Serial.print(" | ");
    Serial.print(previousMillis);
#endif
    Serial.print(" | ");
    Serial.print(previousMicros);
    Serial.print(" -> ");
#endif
    Serial.print(dre.buttonState[currentPacket]);
    currentPacket++;
    if (currentPacket>=NUM_BUTTONS){
      currentPacket=0;
      Serial.println();
    } else {
      Serial.print(" ");
    }
  }
}

#else

#include <Wire.h>
extern TwoWire Wire;
char bufButtons[NUM_BUTTONS+2];

// callback for sending data
void sendI2cData(void){
  uint8_t i;
  for (i=0;i<NUM_BUTTONS;i++){
    bufButtons[i]=dre.buttonState[i]+'0';
  }
  bufButtons[NUM_BUTTONS]=dre.rpiStatus+'0';
  bufButtons[NUM_BUTTONS+1]='\0';
  Wire.write(bufButtons);
}
#endif

