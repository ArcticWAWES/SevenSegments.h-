#include "SevenSegments.h"

// Таблица: {a, b, c, d, e, f, g, dp}. 1 — сегмент горит.
static const bool NUMS[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1, 0}, // 2
  {1, 1, 1, 1, 0, 0, 1, 0}, // 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // 4
  {1, 0, 1, 1, 0, 1, 1, 0}, // 5
  {1, 0, 1, 1, 1, 1, 1, 0}, // 6
  {1, 1, 1, 0, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1, 0}, // 8
  {1, 1, 1, 1, 0, 1, 1, 0}  // 9
};

// ================== SevenSegment ==================

SevenSegment::SevenSegment(const int pins[8], bool ca) {
  for (int i = 0; i < 8; i++) _pins[i] = pins[i];
  _commonAnode = ca;
}

// --- Конструктор копирования ---
SevenSegment::SevenSegment(const SevenSegment& other) {
  for (int i = 0; i < 8; i++) _pins[i] = other._pins[i];
  _commonAnode = other._commonAnode;
}

// --- Оператор присваивания ---
SevenSegment& SevenSegment::operator=(const SevenSegment& other) {
  if (this == &other) return *this;   // защита от самоприсваивания
  for (int i = 0; i < 8; i++) _pins[i] = other._pins[i];
  _commonAnode = other._commonAnode;
  return *this;
}

// --- Деструктор ---
SevenSegment::~SevenSegment() {
  // ничего освобождать не нужно: массив внутри объекта
}

void SevenSegment::begin() {
  for (int i = 0; i < 8; i++) {
    pinMode(_pins[i], OUTPUT);
    digitalWrite(_pins[i], _commonAnode ? HIGH : LOW); // выключено
  }
}

void SevenSegment::segmentsOff() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(_pins[i], _commonAnode ? HIGH : LOW);
  }
}

void SevenSegment::setSegment(int index, bool on) {
  if (index < 0 || index >= 8) return;
  if (_commonAnode) on = !on;
  digitalWrite(_pins[index], on ? HIGH : LOW);
}

void SevenSegment::clear() {
  segmentsOff();
}

void SevenSegment::applySegments(int digit) {
  if (digit < 0 || digit > 9) {
    segmentsOff();
    return;
  }
  for (int i = 0; i < 8; i++) {
    bool state = NUMS[digit][i];
    if (_commonAnode) state = !state;
    digitalWrite(_pins[i], state);
  }
}

void SevenSegment::displayDigit(int digit) {
  applySegments(digit);
}

void SevenSegment::displayDigit(int number) {
  applySegments(abs(number) % 10);
}

void SevenSegment::testPattern(int delayMs) {
  // Перебор сегментов
  for (int i = 0; i < 8; i++) {
    segmentsOff();
    delay(50);
    digitalWrite(_pins[i], _commonAnode ? LOW : HIGH);
    delay(delayMs);
  }
  segmentsOff();
  delay(100);
  // Перебор цифр
  for (int digit = 0; digit <= 9; digit++) {
    applySegments(digit);
    delay(delayMs);
  }
  segmentsOff();
}

// ================== FourSevenSegment ==================

FourSevenSegment::FourSevenSegment(const int pins[8], const int digits[4], bool ca)
  : SevenSegment(pins, ca) {              // явный вызов конструктора базового класса
  for (int i = 0; i < 4; i++) {
    _digits[i] = digits[i];
    _buf[i]    = -1;
    _dp[i]     = false;
  }
}

// --- Конструктор копирования ---
FourSevenSegment::FourSevenSegment(const FourSevenSegment& other)
  : SevenSegment(other) {                 // копируем базовую часть
  for (int i = 0; i < 4; i++) {
    _digits[i] = other._digits[i];
    _buf[i]    = other._buf[i];
    _dp[i]     = other._dp[i];
  }
}

// --- Оператор присваивания ---
FourSevenSegment& FourSevenSegment::operator=(const FourSevenSegment& other) {
  if (this == &other) return *this;
  SevenSegment::operator=(other);         // копируем базовую часть
  for (int i = 0; i < 4; i++) {
    _digits[i] = other._digits[i];
    _buf[i]    = other._buf[i];
    _dp[i]     = other._dp[i];
  }
  return *this;
}

// --- Деструктор ---
FourSevenSegment::~FourSevenSegment() {
}

void FourSevenSegment::begin() {
  SevenSegment::begin();
  for (int i = 0; i < 4; i++) {
    pinMode(_digits[i], OUTPUT);
    digitalWrite(_digits[i], _commonAnode ? LOW : HIGH); // разряд выключен
  }
}

void FourSevenSegment::clear() {
  for (int i = 0; i < 4; i++) {
    _buf[i] = -1;
    _dp[i]  = false;
  }
}

void FourSevenSegment::allDigitsOff() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(_digits[i], _commonAnode ? LOW : HIGH);
  }
}

void FourSevenSegment::displayDigit(int digit, int idx) {
  if (idx < 0 || idx >= 4) return;
  if (digit < -1 || digit > 10) return;  // -1 = пусто, 10 = минус
  _buf[idx] = digit;
}

void FourSevenSegment::displayDigit(int number) {
  clear();

  bool negative = (number < 0);
  long n = labs((long)number);

  if (n > 9999) return; // не влезает

  // Заполняем справа налево (индекс 3 — младший разряд)
  int idx = 3;
  do {
    _buf[idx--] = (int)(n % 10);
    n /= 10;
  } while (n > 0 && idx >= 0);

  // Знак минус — на первый свободный слева
  if (negative && idx >= 0) {
    _buf[idx] = 10;
  }

  // Гасим ведущие нули (кроме случая, когда число == 0)
  if (number != 0) {
    for (int i = 0; i < 4; i++) {
      if (_buf[i] == 0) _buf[i] = -1;
      else break;
    }
  }
}

void FourSevenSegment::displayFloat(float value, int decimals) {
  clear();

  bool negative = (value < 0);
  float v = fabs(value);

  long mult = 1;
  for (int i = 0; i < decimals; i++) mult *= 10;
  long n = (long)(v * mult + 0.5f);

  if (n > 9999) return;

  int idx = 3;
  int count = 0;
  do {
    _buf[idx] = (int)(n % 10);
    n /= 10;
    count++;
    // Если это разряд, после которого стоит точка
    if (count == decimals && idx > 0) {
      _dp[idx] = true;
    }
    idx--;
  } while (n > 0 && idx >= 0);

  if (negative && idx >= 0) _buf[idx] = 10;

  if (value != 0.0f) {
    for (int i = 0; i < 4; i++) {
      if (_buf[i] == 0) _buf[i] = -1;
      else break;
    }
  }
}

void FourSevenSegment::setDot(int idx, bool on) {
  if (idx >= 0 && idx < 4) _dp[idx] = on;
}

void FourSevenSegment::tick() {
  static unsigned long last = 0;
  static int cur = 0;

  if (millis() - last < 2) return;
  last = millis();

  // 1. Погасить ВСЕ разряды (устраняем ghosting)
  allDigitsOff();

  // 2. Установить сегменты
  int val = _buf[cur];

  if (val == -1) {
    segmentsOff();
  } else if (val == 10) {
    // «минус»: горит только сегмент g (индекс 6)
    segmentsOff();
    setSegment(6, true);
  } else {
    applySegments(val);
  }

  // 3. Точка (dp — индекс 7)
  if (_dp[cur]) {
    setSegment(7, true);
  }

  // 4. Зажечь текущий разряд
  digitalWrite(_digits[cur], _commonAnode ? HIGH : LOW);

  // 5. Перейти к следующему
  cur = (cur + 1) % 4;
}
