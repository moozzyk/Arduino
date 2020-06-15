
#include <XBOXRECV.h>
#include "ServoWithLimits.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

#define NUM_CONTROLLERS 4
#define CONTROLLER_NOT_FOUND 255

#define NUM_SERVOS 4

ServoWithLimits servos[NUM_SERVOS] = {
  ServoWithLimits(14),
  ServoWithLimits(15),
  ServoWithLimits(16),
  ServoWithLimits(17, 45, 135),
};


void initializeReceiver() {
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));  
}

void initializeServos() {
  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach();
  }
}

void setup() {
  Serial.begin(115200);
  initializeReceiver();
  initializeServos();
}

uint8_t findFirstController() {
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < NUM_CONTROLLERS; i++) {
      if (Xbox.Xbox360Connected[i]) {
        return i;
      }
    }
  }

  return CONTROLLER_NOT_FOUND;
}

void loop() {
  Usb.Task();
  uint8_t controller = findFirstController();
  if (controller == CONTROLLER_NOT_FOUND) {
   if (random(1000) == 0) {
     Serial.println(F("XBox receiver not connected or no controller found."));
    }
    return;
  }

  if (Xbox.getAnalogHat(LeftHatX, controller) > 7500) {
    Serial.print(F("Right. ")); Serial.println(Xbox.getAnalogHat(LeftHatX, controller));
    servos[0].turn(-1);
  }
  
  if(Xbox.getAnalogHat(LeftHatX, controller) < -7500) {
    Serial.print(F("Left. ")); Serial.println(Xbox.getAnalogHat(LeftHatX, controller));
    servos[0].turn(1);
  }

  if (Xbox.getAnalogHat(LeftHatY, controller) > 7500) {
    Serial.print(F("Up. ")); Serial.println(Xbox.getAnalogHat(LeftHatY, controller));
    servos[1].turn(1);
  }
  
  if(Xbox.getAnalogHat(LeftHatY, controller) < -7500) {
    Serial.print(F("Down. ")); Serial.println(Xbox.getAnalogHat(LeftHatY, controller));
    servos[1].turn(-1);
  }

  if (Xbox.getAnalogHat(RightHatY, controller) > 7500) {
    Serial.print(F("Forward. ")); Serial.println(Xbox.getAnalogHat(RightHatY, controller));
    servos[2].turn(1);
  }
  
  if(Xbox.getAnalogHat(RightHatY, controller) < -7500) {
    Serial.print(F("Backward. ")); Serial.println(Xbox.getAnalogHat(RightHatY, controller));
    servos[2].turn(-1);
  }  

  if (Xbox.getButtonPress(L2, controller)) {
    Serial.print(F("Close. ")); Serial.println(Xbox.getButtonPress(L2, controller));
    servos[3].turn(-1);
  }

  if (Xbox.getButtonPress(R2, controller)) {
    Serial.print(F("Open. ")); Serial.println(Xbox.getButtonPress(R2, controller));
    servos[3].turn(1);
  }
  delay(15);
}
