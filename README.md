void FourSevenSegment::tick() {
  static unsigned long last = 0;
  static int cur = 0;

  if (millis() - last >= 3) {
    last = millis();

    // 1. Погасить ВСЕ разряды
    for (int i = 0; i < 4; i++) {
      digitalWrite(_digits[i], commonAnode ? LOW : HIGH);
    }

    // 2. Перейти к следующему
    cur = (cur + 1) % 4;

    // 3. Вывести сегменты и зажечь разряд
    if (_buf[cur] != -1) {
      SevenSegment::displayDigit(_buf[cur]);
      digitalWrite(_digits[cur], commonAnode ? HIGH : LOW);
    }
  }
}
