#include "Arduino.h"
#include "MultiSegment.h"
 
MultiSegment::MultiSegment(byte digits[], byte segments[], byte dispLen)
{
    this->dispLen = dispLen;
    this->buffer = new byte[dispLen];
    this->intBuffer = new byte[dispLen];
    this->digits = digits;
    this->segments = segments;
    this->di = 0;
    for (byte i=0; i<8; i++)
    {
        pinMode(segments[i], OUTPUT);
    }  
    for (byte j=0; j<dispLen; j++)
    {
        pinMode(digits[j], OUTPUT);
    }
}
void multiSegment();

void MultiSegment::clear()
{
    for (byte i=0; i<dispLen; i++)
    {
        buffer[i] = digs[MS_SPACE];
        intBuffer[i] = MS_SPACE;
    }
}

void MultiSegment::writeInt(int n)
{
  int i = dispLen-1;
  bool isNeg = n<0;
  unsigned int n1 = isNeg ? -n : n;
  
  while(n1!=0)
  {
      intBuffer[i--] = n1%10; //+'0';
      n1=n1/10;
  }
  
  if(isNeg)
  {
      intBuffer[i--] = '-';
  }
  
  for(int t = 0; t < i/2; t++)
  {
      intBuffer[t] ^= intBuffer[i-t-1];
      intBuffer[i-t-1] ^= intBuffer[t];
      intBuffer[t] ^= intBuffer[i-t-1];
  }
  
  if(n == 0)
  {
      intBuffer[dispLen-1] = 0;
  }   
  for (int j=0; j<dispLen; j++)
  {
      buffer[j] = digs[intBuffer[j]];
  }
}

void MultiSegment::writeStr(char s[])
{
    int i=0, ci = 0;
    char c;
    bool ch = false;
    while ((c = s[i]) != '\0')
    {
        if (ci<dispLen || (ci==dispLen && c=='.'))
        {
            if (c >= '0' && c <= '9')
            {
                buffer[ci] = digs[c-48];
                ch = true;
            }
            if (c >= 'A' && c <= 'Z')
            {
                buffer[ci] = digs[c-55];
                ch = true;
            }
            if (c >= 'a' && c <= 'z')
            {
                buffer[ci] = digs[c-87];
                ch = true;
            }
            if (c == '.')
            {
                if (ch == false)
                {
                    buffer[ci] |= digs[DOT];
                }
                else
                {
                    buffer[ci-1] |= digs[DOT];
                    ci--;
                    ch = false;
                }
            }
            ci++;
        }
        i++;
    }
}

void MultiSegment::enable()
{
    enabled = true;
}

void MultiSegment::disable()
{
    enabled = false;
    digitalWrite(digits[di], LOW);
}

void MultiSegment::updateDisplay(unsigned long cmicros)
{  
    if (enabled)
    {
        for (byte j=0; j<8; j++)
        {
            byte t = buffer[di];
            byte b = ((t >> j) & 1 == 1 ? LOW : HIGH);
            digitalWrite(segments[7-j], b);
        }
        digitalWrite(digits[di], HIGH);

        if (cmicros - lastUpdate >= 2000UL)
        {
            lastUpdate = cmicros;
            for (byte j=0; j<8; j++)
            {
              digitalWrite(segments[j], HIGH);
            }
            digitalWrite(digits[di], LOW);
            di++;
            if (di>dispLen-1)
            {
                di=0;
            }    
        }
    }
}

void MultiSegment::updateDisplay()
{
    updateDisplay(micros());
}
