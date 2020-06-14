
#include <XBOXRECV.h>
#include <Servo.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

#define NUM_CONTROLLERS 4
#define CONTROLLER_NOT_FOUND 255

#define BASE 0
#define FORWARD 1
#define UP_DOWN 2
#define JAW 3

#define NUM_SERVOS 4

struct CraneMove {
  Servo servo;
  uint8_t pos;
  uint8_t pin;
};

CraneMove moves[NUM_SERVOS] = {
  { {}, 0, 14 },
  { {}, 0, 15 },
  { {}, 0, 16 },
  { {}, 0, 17 }
};

void initializeCrane() {
  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    moves[i].servo.attach(moves[i].pin);
    moves[i].pos = moves[i].servo.read();
  }
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

void moveCrane(CraneMove& craneMove, int8_t degrees) {
  int16_t newPos = (int16_t)craneMove.pos + degrees;
  Serial.print("Old position: "); Serial.println(craneMove.pos);
  if (newPos < 0) newPos = 0;
  if (newPos > 180) newPos = 180;
  craneMove.pos = newPos;
  craneMove.servo.write(craneMove.pos);
  Serial.print("New position: "); Serial.println(craneMove.pos);
  delay(5);
}

void setup() {
  Serial.begin(115200);
  initializeCrane();
  
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
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
    moveCrane(moves[0], -1);
  }
  
  if(Xbox.getAnalogHat(LeftHatX, controller) < -7500) {
    Serial.print(F("Left. ")); Serial.println(Xbox.getAnalogHat(LeftHatX, controller));
    moveCrane(moves[0], 1);
  }

  if (Xbox.getAnalogHat(LeftHatY, controller) > 7500) {
    Serial.print(F("Up. ")); Serial.println(Xbox.getAnalogHat(LeftHatY, controller));
    moveCrane(moves[1], 1);
  }
  
  if(Xbox.getAnalogHat(LeftHatY, controller) < -7500) {
    Serial.print(F("Down. ")); Serial.println(Xbox.getAnalogHat(LeftHatY, controller));
    moveCrane(moves[1], -1);
  }

  if (Xbox.getAnalogHat(RightHatY, controller) > 7500) {
    Serial.print(F("Forward. ")); Serial.println(Xbox.getAnalogHat(RightHatY, controller));
    moveCrane(moves[2], 1);
  }
  
  if(Xbox.getAnalogHat(RightHatY, controller) < -7500) {
    Serial.print(F("Backward. ")); Serial.println(Xbox.getAnalogHat(RightHatY, controller));
    moveCrane(moves[2], -1);
  }  

  if (Xbox.getButtonPress(L2, controller)) {
    Serial.print(F("Close. ")); Serial.println(Xbox.getButtonPress(L2, controller));
    moveCrane(moves[3], -3);
  }

  if (Xbox.getButtonPress(R2, controller)) {
    Serial.print(F("Open. ")); Serial.println(Xbox.getButtonPress(R2, controller));
    moveCrane(moves[3], 3);
  }
}
