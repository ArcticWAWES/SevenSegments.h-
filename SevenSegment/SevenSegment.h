#ifndef SevenSegments_h
#define SevenSegments_h

#include <Arduino.h>

class SevenSegment {
  private:
    int  _pins[8];        // Порядок: a, b, c, d, e, f, g, dp
    bool _commonAnode;    // true — общий анод, false — общий катод

  protected:
    void applySegments(int digit);       // Установить сегменты под цифру
    void segmentsOff();                  // Погасить все сегменты
    void setSegment(int index, bool on); // Управление одним сегментом (0..7)

  public:
    // Основной конструктор
    SevenSegment(const int pins[8], bool ca = false);

    // Конструктор копирования
    SevenSegment(const SevenSegment& other);

    // Оператор присваивания
    SevenSegment& operator=(const SevenSegment& other);

    // Деструктор
    ~SevenSegment();

    void begin();
    void clear();
    void displayDigit(int digit);    // Одна цифра 0–9
    void displayDigit(int number);   // Последняя цифра числа
    void testPattern(int delayMs = 500);

    bool isCommonAnode() const { return _commonAnode; }
};

class FourSevenSegment : public SevenSegment {
  private:
    int  _digits[4];      // Пины разрядов. _digits[0] — ЛЕВЫЙ (старший)
    int  _buf[4];         // Буфер: -1 = пусто, 0–9 = цифра, 10 = минус
    bool _dp[4];          // Точка в данном разряде

    void allDigitsOff();  // Погасить все разряды

  public:
    // Основной конструктор с явным вызовом конструктора базового класса
    FourSevenSegment(const int pins[8], const int digits[4], bool ca = false);

    // Конструктор копирования
    FourSevenSegment(const FourSevenSegment& other);

    // Оператор присваивания
    FourSevenSegment& operator=(const FourSevenSegment& other);

    // Деструктор
    ~FourSevenSegment();

    void begin();
    void clear();

    void displayDigit(int digit, int idx);   // Цифра на конкретный разряд (0–3)
    void displayDigit(int number);           // Разложить число по разрядам
    void displayFloat(float value, int decimals = 1);
    void setDot(int idx, bool on);           // Управление точкой в разряде

    void tick();                             // Динамическая индикация — в loop()
};

#endif
