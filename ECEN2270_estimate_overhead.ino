volatile bool var = LOW;

void setup() {
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), service2, RISING);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, var);
}

void service2() { // pin 2 ISR
  var = !var;
}
