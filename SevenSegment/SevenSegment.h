#ifndef SevenSegments_h
#define SevenSegments_h

#include <Arduino.h>

class SevenSegment {
  protected:
    int _pins[8];
    bool commonAnode; // True — общий анод, False — общий катод
    
  public:
    SevenSegment(int pins[8], bool commonAnode = false);
    void begin();
    void clear();
    void displayDigit(int digit);  // Вывод цифры на 1 разряд
    // void displayDigit(int number); // Вывод числа (только последняя цифра)
    void testPattern(int delayMs = 500);
};

class FourSevenSegment : public SevenSegment {
  private:
    int _digits[4];
    int _buf[4]; // Буфер экрана
    SevenSegment FourSevenSegment = SevenSegment(int pins[8], bool commonAnode);
    
  public:
    _FourSevenSegment(int pins[8], int digits[4], bool commonAnode = true);
    void begin();
    void clear();
    void displayDigitIndex(int digit, int idx); // Вывод цифры на конкретный разряд (0-3)
    void displayDigitAuto(int number);         // Разложение большого числа по разрядам
    void tick();                              // Динамическая индикация (вызывать в loop)
};

#endif

