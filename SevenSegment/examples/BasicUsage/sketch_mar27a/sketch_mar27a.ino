//======================================== SEVEN SEGMENT ========================================
//
//
//
//
//
//
//
//
//

#include <SevenSegment.h>

// Пины: A, B, C, D, E, F, G, DP
int pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
SevenSegment display(pins, false);  // false = common cathode

void setup() {
  display.begin();
  
  // Тест всех сегментов и цифр
  display.testPattern(300);
}

void loop() {
  // Показываем цифры от 0 до 9
  for (int i = 0; i <= 9; i++) {
    display.displayDigit(i);
    delay(1000);
  }
  
  // Эффект мигания
  for (int i = 0; i < 3; i++) {
    display.clear();
    delay(200);
    display.displayDigit(8);
    delay(200);
  }
}