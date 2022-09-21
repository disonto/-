#define PIN_LED 7
unsigned int count;
void setup() {
  pinMode(PIN_LED, OUTPUT);
  count = 0;
}

void loop() {
  digitalWrite(PIN_LED, 0);
  delay(1000);
  while(1) {
    Serial.println(count++);
    digitalWrite(PIN_LED, count%2);
    delay(100);
    if (count == 10) {
      digitalWrite(PIN_LED, 1);
      exit(0);
    }
  }
}
