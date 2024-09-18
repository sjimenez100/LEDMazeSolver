#ifndef DISPLAY_H
#define DISPLAY_H

template<typename T>
class Display {

private:

  const uint8_t SDI_PIN;
  const uint8_t CLK_PIN;
  const uint8_t LE_PIN;
  const uint8_t rows;

  const uint8_t columns = sizeof(T) * 8;
  const uint8_t columnBlocks = columns / 8;
  const uint8_t rowBlocks = ceil(rows / 8);
  const uint8_t totalBlocks = columnBlocks * rowBlocks;


  void sendCommand(uint8_t address, uint8_t data, uint64_t mask, bool flipData = false) {

    for (int blockIndex = totalBlocks - 1; blockIndex >= 0; blockIndex--) {

      bool sel = !!(mask & (1 << blockIndex));

      if(sel) {
        shiftOut(SDI_PIN, CLK_PIN, MSBFIRST, address);
        shiftOut(SDI_PIN, CLK_PIN, flipData ? LSBFIRST : MSBFIRST, data);
      } else {
        shiftOut(SDI_PIN, CLK_PIN, MSBFIRST, 0);
        shiftOut(SDI_PIN, CLK_PIN, MSBFIRST, 0);
      }
    }

    latch();
  }

  
  void sendCommand(uint8_t address, uint8_t data) {

    shiftOut(SDI_PIN, CLK_PIN, MSBFIRST, address);
    shiftOut(SDI_PIN, CLK_PIN, MSBFIRST, data);

    latch();
  }


  void latch() {
    digitalWrite(LE_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(LE_PIN, HIGH);
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

  Display(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, uint8_t rows)
    : SDI_PIN(dataPin), CLK_PIN(clockPin), LE_PIN(latchPin), rows(rows) {}

  void initialize() {
    pinMode(CLK_PIN, OUTPUT);
    pinMode(SDI_PIN, OUTPUT);
    pinMode(LE_PIN, OUTPUT);

    digitalWrite(CLK_PIN, LOW);
    digitalWrite(SDI_PIN, LOW);
    digitalWrite(LE_PIN, LOW);

    //sendCommand(0x0f, 0x00);  // normal operation (no test)
    sendCommand(0b00001111, 0b00000000, 255);

    //sendCommand(0x0c, 0x01);  // normal operations
    sendCommand(0b00001100, 0b00000001, 255);

    //sendCommand(0x0b, 0x07);  // display all digits
    sendCommand(0b00001011, 0b00000111, 255);

    //sendCommand(0x09, 0x00);  // no decode
    sendCommand(0b00001001, 0b00000000, 255);

    //sendCommand(0x0a, 0x09);  // duty cycle
    sendCommand(0b00001010, 0x0E, 255);

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


  //   // refreshes LED matrix at desired rate (frames/s) (data if fed in a flipped N pattern to the ICs)
  //   void refresh(T bitmap[], unsigned long rate) {

  //     const unsigned long delayTime = 1.0 / (8 * rate) * 1000;

  //     uint8_t totalRowSections = rows/8;
  //     uint8_t totalColumnSections = columns/8;
  //     uint8_t rowSection = 0;

  //     int countR = 0;

  //     for(int r = 0; r < rows; r++){
  //       rowSection = floor((r+1) / 8);

  //       for(int b = 0; b < totalColumnSections; b++){

  //         for(int sec = rowSection; sec < totalRowSections; sec++){
  //           sendByte(1 << 8 * countR);
  //           sendByte(0);
  //         }

  //         //sendByte(1 << 8 * countR);
  //         sendByte((uint8_t)(bitmap[r] & (225 << 8 * b)));

  //         for(int sec = 0; sec < rowSection; sec++){
  //           sendByte(1 << 8 * countR);
  //           sendByte(0);
  //         }
  //       }

  //       countR = !((r+1) % 8) ? 0 : countR + 1;

  //       latch();
  //       delay(delayTime);
  //     }

  //     // for (int i = rows-1; i >= 0; i--) {
  //     //   sendByte(1 << i);
  //     //   sendByte(bitmap[(columns-1) - i]);
  //     //   latch();
  //     //   delay(delayTime);
  //     // }
  //   }
};

#endif