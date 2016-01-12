#ifndef MultiSegment_h
#define MultiSegment_h

#include "Arduino.h"


#define MS_SPACE 39
#define DOT 38

class MultiSegment
{
  public:
    MultiSegment(byte digits[], byte segments[], byte dispLen);
    void clear(); // clear display (nothing is displayed)
    void disable(); // disable display (turn off)
    void enable(); // enable display
    void writeInt(int n); // display integer
    void writeStr(char s[]); // display string (if string is longer than display, only first characters are displayed)
    void updateDisplay(unsigned long micros); // update display, using external timer
    void updateDisplay(); // update display using internal timer
  private:
    bool enabled;
    byte *digits;   // array with digits pins
    byte *segments; // array with segment pins
    byte dispLen;   // number of digits
    byte *buffer;   // buffer of currently displayed characters
    byte *intBuffer; // buffer for integer operations
    unsigned long lastUpdate; // last time of display being switched
    byte di; // currently active display
                          // 0                                           9    A   B    C    D    E    F    G    H   I    J    K   L    M   N   O    P    Q   R    S    T    U   V   W     X    Y    Z  -   _  .  
    const byte digs[40] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246, 238, 62, 156, 122, 158, 142, 188, 110, 12, 112, 174, 28, 236, 42, 58, 206, 230, 10, 180, 140, 124, 76, 126, 108, 118, 248, 2, 16, 1, 0};
};

#endif