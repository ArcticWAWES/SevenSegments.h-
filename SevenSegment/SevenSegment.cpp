#include "SevenSegment.h"

// Таблица сегментов для цифр 0-9 и знака минус (индекс 10)
// Порядок: A, B, C, D, E, F, G, DP
const bool nums[11][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1, 0}, // 2
  {1, 1, 1, 1, 0, 0, 1, 0}, // 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // 4
  {1, 0, 1, 1, 0, 1, 1, 0}, // 5
  {1, 0, 1, 1, 1, 1, 1, 0}, // 6
  {1, 1, 1, 0, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1, 0}, // 8
  {1, 1, 1, 1, 0, 1, 1, 0}, // 9
  {0, 0, 0, 0, 0, 0, 1, 0}  // 10: Минус
};

// --- Конструктор для 1 разряда ---
SevenSegment::SevenSegment(int segPins[8], bool commonAnode) {
  for (int i = 0; i < 8; i++) {
    _segPins[i] = segPins[i];
  }
  _numDigits = 1;
  _commonAnode = commonAnode;
  _currentDigit = 0;
  _buffer[0] = 0;
}

// --- Конструктор для 4 разрядов ---
SevenSegment::SevenSegment(int segPins[8], int digitPins[4], bool commonAnode) {
  for (int i = 0; i < 8; i++) {
    _segPins[i] = segPins[i];
  }
  for (int i = 0; i < 4; i++) {
    _digitPins[i] = digitPins[i];
    _buffer[i] = 0; // Инициализация нулями
  }
  _numDigits = 4;
  _commonAnode = commonAnode;
  _currentDigit = 0;
}

void SevenSegment::begin() {
  // Настройка пинов сегментов
  for (int i = 0; i < 8; i++) {
    pinMode(_segPins[i], OUTPUT);
    // Выключаем сегменты при старте
    digitalWrite(_segPins[i], _commonAnode ? HIGH : LOW);
  }

  // Настройка пинов разрядов (только если их больше 1)
  if (_numDigits > 1) {
    for (int i = 0; i < _numDigits; i++) {
      pinMode(_digitPins[i], OUTPUT);
      // Выключаем разряды при старте (для общего катода LOW, для анода HIGH)
      digitalWrite(_digitPins[i], _commonAnode ? HIGH : LOW);
    }
  }
}

// Вспомогательная функция: вывод сегментов для одной цифры
void SevenSegment::_writeSegments(int digit) {
  if (digit < 0 || digit > 10) return; // 10 - это минус
  
  for (int i = 0; i < 8; i++) {
    bool state = nums[digit][i];
    if (_commonAnode) state = !state;
    digitalWrite(_segPins[i], state);
  }
}

// Вспомогательная функция: выбор активного разряда
void SevenSegment::_selectDigit(int digit) {
  if (_numDigits == 1) return; // Для 1 разряда ничего не делаем

  // Сначала выключаем все разряды, чтобы не было "призраков"
  for (int i = 0; i < _numDigits; i++) {
    digitalWrite(_digitPins[i], _commonAnode ? HIGH : LOW);
  }
  
  // Включаем нужный разряд
  if (digit >= 0 && digit < _numDigits) {
    digitalWrite(_digitPins[digit], _commonAnode ? LOW : HIGH);
  }
}

void SevenSegment::setDigit(int position, int value) {
  if (position >= 0 && position < _numDigits) {
    _buffer[position] = value;
  }
}

void SevenSegment::print(int number) {
  // Если число отрицательное, обрабатываем знак
  bool isNegative = false;
  if (number < 0) {
    isNegative = true;
    number = -number;
  }

  // Заполняем буфер справа налево
  for (int i = _numDigits - 1; i >= 0; i--) {
    _buffer[i] = number % 10;
    number /= 10;
  }

  // Если было отрицательное число, ставим минус в первый разряд
  if (isNegative && _numDigits > 0) {
    _buffer[0] = 10; // Индекс минуса
  }
}

void SevenSegment::clear() {
  for (int i = 0; i < _numDigits; i++) {
    _buffer[i] = -1; // -1 означает пустой разряд (или 0, если хотите)
  }
  // Если 1 разряд, просто гасим сегменты
  if (_numDigits == 1) {
     for (int i = 0; i < 8; i++) {
        digitalWrite(_segPins[i], _commonAnode ? HIGH : LOW);
     }
  }
}

// САМАЯ ВАЖНАЯ ФУНКЦИЯ ДЛЯ 4-РАЗРЯДНОГО ИНДИКАТОРА
void SevenSegment::refresh() {
  if (_numDigits == 1) {
    // Для 1 разряда просто показываем то, что в буфере
    if (_buffer[0] != -1) _writeSegments(_buffer[0]);
    return;
  }

  // 1. Выключаем текущий разряд (чтобы избежать мерцания)
  _selectDigit(-1); 

  // 2. Если в буфере пусто (-1), переходим к следующему
  if (_buffer[_currentDigit] == -1) {
      _currentDigit++;
      if (_currentDigit >= _numDigits) _currentDigit = 0;
      return; 
  }

  // 3. Устанавливаем сегменты для текущей цифры
  _writeSegments(_buffer[_currentDigit]);

  // 4. Включаем этот разряд
  _selectDigit(_currentDigit);

  // 5. Переходим к следующему разряду для следующего вызова refresh()
  _currentDigit++;
  if (_currentDigit >= _numDigits) _currentDigit = 0;
}

void SevenSegment::testPattern(int delayMs) {
  if (_numDigits == 1) {
    for (int digit = 0; digit <= 10; digit++) {
        _writeSegments(digit);
        delay(delayMs);
    }
    clear();
  } else {
    // Тест для 4 разрядов: заполняем все цифры
    for(int i=0; i<_numDigits; i++) setDigit(i, 8);
    unsigned long start = millis();
    while(millis() - start < 2000) {
        refresh();
    }
    
    // Счет 0000 -> 9999
    for(int i=0; i<10000; i+=1111) {
        print(i);
        unsigned long start2 = millis();
        while(millis() - start2 < 500) {
            refresh(); // Обязательно крутим refresh в цикле
        }
    }
    clear();
  }
}