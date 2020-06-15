#include "ServoWithLimits.h"

void ServoWithLimits::attach() {
  servo_.attach(pin_);
}

void ServoWithLimits::turn(uint8_t degrees) {
  int16_t new_position = (int16_t)get_position() + degrees;
  Serial.print("Old position: "); Serial.println(get_position());
  set_position(new_position);
}

void ServoWithLimits::set_position(uint8_t new_position) {
  if (new_position < min_position_) new_position = min_position_;
  if (new_position > max_position_) new_position = max_position_;
  servo_.write(new_position);
  Serial.print("New position: "); Serial.println(new_position);
}

uint8_t ServoWithLimits::get_position() {
  return servo_.read();
}
