#ifndef SevenSegment_h
#define SevenSegment_h

#include <Arduino.h>

class SevenSegment {
  private:
    // Пины сегментов (A, B, C, D, E, F, G, DP)
    int _segPins[8];
    // Пины разрядов (для 4-разрядного индикатора)
    int _digitPins[4];
    // Количество разрядов (1 или 4)
    int _numDigits;
    // Тип индикатора
    bool _commonAnode;
    // Буфер для хранения текущих цифр (макс 4)
    int _buffer[4];
    // Текущий активный разряд для мультиплексирования
    int _currentDigit;

    void _writeSegments(int digit);
    void _selectDigit(int digit);

  public:
    // Конструктор для 1-разрядного индикатора (старый вариант)
    SevenSegment(int segPins[8], bool commonAnode = false);
    
    // Конструктор для 4-разрядного индикатора
    SevenSegment(int segPins[8], int digitPins[4], bool commonAnode = false);

    void begin();
    
    // Установка конкретной цифры в конкретный разряд
    void setDigit(int position, int value);
    
    // Вывод целого числа (например, 1234)
    void print(int number);
    
    // Очистка всего дисплея
    void clear();
    
    // Функция мультиплексирования (вызывать в loop)
    void refresh();
    
    // Тестовая функция
    void testPattern(int delayMs = 500);
};

#endif