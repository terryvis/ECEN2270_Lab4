volatile int v_ref = 4;
volatile int pwm_out = 51*v_ref; // range 0-255

// Pins
const int pin_sw = 6;
const int right_forward = 7;
const int right_back = 8;
const int right_pwm = 9;
const int left_pwm = 10;
const int left_forward = 11;
const int left_back = 12;

void setup() {
  // Set up pins
  pinMode(pin_sw, INPUT_PULLUP);
  pinMode(right_forward, OUTPUT);
  pinMode(right_back, OUTPUT);
  pinMode(right_pwm, OUTPUT);
  pinMode(left_pwm,OUTPUT);
  pinMode(left_forward, OUTPUT);
  pinMode(left_back, OUTPUT);

  // stop forward/backward motion on both sides
  digitalWrite(right_forward, LOW);
  digitalWrite(right_back, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_back, LOW);

  // start PWM
  analogWrite(right_pwm, pwm_out);
  analogWrite(left_pwm, pwm_out);
}

void loop() {
  digitalWrite(13, LOW);  // turn LED off
  do{} while (digitalRead(pin_sw) == HIGH); // wait for ON switch
  digitalWrite(13, HIGH); // turn LED on
  delay(1000);            // wait 1 second

  // rotate clockwise 360 degrees
  digitalWrite(right_back, HIGH);
  digitalWrite(left_forward, HIGH);
  delay(2400);

  // stop and wait 1 second
  digitalWrite(right_back, LOW);
  digitalWrite(left_forward, LOW);
  delay(1000);

  // rotate counterclockwise 360 degrees
  digitalWrite(right_forward, HIGH);
  digitalWrite(left_back, HIGH);
  delay(2400);

  // stop before next cycle
  digitalWrite(right_forward, LOW);
  digitalWrite(left_back, LOW);
}
