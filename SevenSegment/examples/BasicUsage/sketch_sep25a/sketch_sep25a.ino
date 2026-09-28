#include "SevenSegments.h"

// Пины сегментов в порядке: a, b, c, d, e, f, g, dp
// Подставьте СВОИ номера пинов Arduino согласно распайке
int segPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// Пины разрядов: digits[0] — ЛЕВЫЙ (старший), digits[3] — ПРАВЫЙ (младший)
int digitPins[4] = {10, 11, 12, 13};

// ✅ Объявление переменной типа FourSevenSegment
//    с инициализацией через конструктор — глобально
// 3461BS-1 — общий катод, поэтому ca = false
FourSevenSegment display(segPins, digitPins, false);

void setup() {
  display.begin();
  display.displayDigit(1234);
}

void loop() {
  display.tick();   // динамическая индикация, без delay
}
