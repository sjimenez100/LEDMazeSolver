#ifndef MAX7219_H
#define MAX7219_H


using namespace std;

template<typename T>
class MAX7219 {

private:

  const uint8_t DIN_PIN;
  const uint8_t CLK_PIN;
  const uint8_t CS_PIN;
  const uint8_t rows;

  const uint8_t columns = sizeof(T) * 8;
  const uint8_t columnBlocks = columns / 8;
  const uint8_t rowBlocks = ceil(rows / 8);
  const uint8_t totalBlocks = columnBlocks * rowBlocks;

  void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    uint8_t i;

    for (i = 0; i < 8; i++) {
      if (bitOrder == LSBFIRST)
        digitalWrite(dataPin, !!(val & (1 << i)));
      else
        digitalWrite(dataPin, !!(val & (1 << (7 - i))));

      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
    }
  }

  void sendCommand(uint8_t address, uint8_t data, uint64_t mask, bool flipData = false) {

    for (int blockIndex = totalBlocks - 1; blockIndex >= 0; blockIndex--) {

      bool sel = !!(mask & (1 << blockIndex));

      if (sel) {
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, address);
        shiftOut(DIN_PIN, CLK_PIN, flipData ? LSBFIRST : MSBFIRST, data);
      } else {
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);
        shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);
      }
    }

    latch();
  }


  void sendCommand(uint8_t address, uint8_t data) {

    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, address);
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);

    latch();
  }


  void latch() {
    digitalWrite(CS_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(CS_PIN, HIGH);
  }

  void sendDigit(uint8_t rowIndex, T data) {

    // gives leftmost block index of selected row block
    uint8_t leftBlockIndex = ((rowBlocks - 1) - floor(rowIndex / 8)) * columnBlocks;

    for (int columnBlockIndex = columnBlocks - 1; columnBlockIndex >= 0; columnBlockIndex--) {
      sendCommand(rowIndex % 8 + 1, (uint8_t)(data >> ((columnBlocks - 1) - columnBlockIndex) * 8),
                  1 << leftBlockIndex + columnBlockIndex, true);
    }
  }


public:

  MAX7219(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t rows)
    : DIN_PIN(dataPin), CLK_PIN(clockPin), CS_PIN(latchPin), rows(rows) {}

  void initialize() {
    pinMode(CLK_PIN, OUTPUT);
    pinMode(DIN_PIN, OUTPUT);
    pinMode(CS_PIN, OUTPUT);

    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIN_PIN, LOW);
    digitalWrite(CS_PIN, LOW);

    sendCommand(0x0f, 0x00, 255);  // normal operation (no test)

    sendCommand(0x0c, 0x01, 255);  // normal operations

    sendCommand(0x0b, 0x07, 255);  // all digits

    sendCommand(0x09, 0x00, 255);  // no decode

    sendCommand(0x0a, 0x09, 255);  // duty cycle

    clear();
  }

  void clear() {
    for (int r = 0; r < rows; r++) {
      sendDigit(r, 0);
    }
  }

  void refresh(T bitmap[]) {
    for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
      sendDigit(rowIndex, bitmap[rows - rowIndex - 1]);
    }
  }
};

#endif