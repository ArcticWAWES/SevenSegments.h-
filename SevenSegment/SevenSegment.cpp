#include "SevenSegments.h"

const bool nums[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1, 0},
  {1, 1, 1, 1, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 0},
  {1, 0, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 0}
};

// ================= КЛАСС ДЛЯ 1 РАЗРЯДА =================
SevenSegment::SevenSegment(int pins[8], bool ca) {
  for (int i = 0; i < 8; i++) _pins[i] = pins[i];
  commonAnode = ca;
}

void SevenSegment::begin() {
  for (int i = 0; i < 8; i++) {
    pinMode(_pins[i], OUTPUT);
    digitalWrite(_pins[i], commonAnode ? HIGH : LOW);
  }
}

void SevenSegment::clear() {
  for (int i = 0; i < 8; i++) digitalWrite(_pins[i], commonAnode ? HIGH : LOW);
}

void SevenSegment::displayDigit(int digit) {
  if (digit < 0 || digit > 9) return;
  for (int i = 0; i < 8; i++) {
    bool state = nums[digit][i];
    if (commonAnode) state = !state;
    digitalWrite(_pins[i], state);
  }
}

// void SevenSegment::displayDigit(int number) {
//   displayDigit(abs(number) % 10);
// }

void SevenSegment::testPattern(int delayMs) {
  for (int i = 0; i < 8; i++) {
    clear();
    delay(50);
    digitalWrite(_pins[i], commonAnode ? LOW : HIGH);
    delay(delayMs);
  }
  clear();
  delay(100);
  for (int digit = 0; digit <= 9; digit++) {
    displayDigit(digit);
    delay(delayMs);
  }
  clear();
}

// ================= КЛАСС ДЛЯ 4 РАЗРЯДОВ =================
FourSevenSegment::FourSevenSegment(int pins[8], int digits[4], bool commonAnode) : SevenSegment(pins, commonAnode) {
  for (int i = 0; i < 4; i++) {
    _digits[i] = digits[i];
    _buf[i] = -1; // Инициализируем буфер пустыми значениями (-1)
  }
}

void FourSevenSegment::begin() {
  SevenSegment::begin();
  for (int i = 0; i < 4; i++) {
    pinMode(_digits[i], OUTPUT);
    digitalWrite(_digits[i], commonAnode ? LOW : HIGH);
  }
}

void FourSevenSegment::clear() {
  for (int i = 0; i < 4; i++) _buf[i] = -1;
}

void FourSevenSegment::displayDigitIndex(int digit, int idx) {
  if (idx >= 0 && idx < 4) _buf[idx] = digit;
}

void FourSevenSegment::displayDigitAuto(int number) {
  clear();
  if (number < 0 || number > 9999) return;
  for (int i = 0; i < 4; i++) {
    _buf[i] = number % 10;
    number /= 10;
    if (number == 0 && _buf[i] == 0 && i > 0) { _buf[i] = -1; break; } // Гасим ведущие нули
  }
}

void FourSevenSegment::tick() {
  static unsigned long last = 0;
  static int cur = 0;

  if (millis() - last >= 2) {
    last = millis();
    digitalWrite(_digits[cur], commonAnode ? LOW : HIGH); // Гасим прошлый разряд
    cur = (cur + 1) % 4;                                  // Шаг к следующему разряду

    if (_buf[cur] != -1) {
      SevenSegment::displayDigit(_buf[cur]);                // Включаем сегменты
      digitalWrite(_digits[cur], commonAnode ? HIGH : LOW); // Зажигаем текущий разряд
    } else {
      SevenSegment::clear();
    }
  }
}
