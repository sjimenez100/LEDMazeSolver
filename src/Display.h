#ifndef DISPLAY_H
#define DISPLAY_H

class Display {

private:

  const uint8_t SDI_PIN;
  const uint8_t CLK_PIN;
  const uint8_t LE_PIN;

  // shifts byte and enforces LOW clock-pin
  void sendByte(uint8_t data) {

    digitalWrite(CLK_PIN, LOW);
    shiftOut(SDI_PIN, CLK_PIN, MSBFIRST, data);
    digitalWrite(CLK_PIN, LOW);
  }

  // latches data to registers
  void latch() {
    digitalWrite(LE_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(LE_PIN, LOW);
  }

public:

  Display(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin)
    : SDI_PIN(dataPin), CLK_PIN(clockPin), LE_PIN(latchPin) {

    digitalWrite(clockPin, LOW);
    digitalWrite(latchPin, LOW);
  }

  // refreshes LED matrix at desired rate (frames/s)
  void refresh(float rate, uint8_t bitmap[8]) {

    unsigned long delayTime = 1.0 / (8 * rate) * 1000;

    for (int i = 7; i >= 0; i--) {
      sendByte(1 << i);
      sendByte(bitmap[7 - i]);
      latch();
      delay(delayTime);
    }
  }
};

#endif