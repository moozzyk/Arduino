#include <Servo.h>
#include <SPI.h>

class ServoWithLimits {
public:
  ServoWithLimits(uint8_t pin, uint8_t min_position = 0, uint8_t max_position = 180):
    pin_(pin), min_position_(min_position), max_position_(max_position)
  {}

  void attach();
  void turn(uint8_t degrees);
  uint8_t get_position();
  void set_position(uint8_t new_position); 

  uint8_t min_position_;
  uint8_t max_position_;

private:
  Servo servo_;
  uint8_t pin_;
  uint8_t position_;
};
