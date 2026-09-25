#include <SevenSegment.h>

// ВНИМАНИЕ: замените номера пинов на свои!
// Порядок сегментов: A, B, C, D, E, F, G, DP
int segPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// Пины разрядов: D1, D2, D3, D4 (слева направо)
int digitPins[4] = {10, 11, 12, 13};

// true = Common Anode, false = Common Cathode
SevenSegment display(segPins, digitPins, false);

void setup() {
  Serial.begin(9600);
  display.begin();
  display.print(1234);
}

void loop() {
  display.refresh();   // КРИТИЧНО: вызывать постоянно!
}