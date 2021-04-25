volatile bool var = LOW;

volatile int L_pulse_ct = 0;
volatile int R_pulse_ct = 0;

volatile int v_ref_max = 5;       // range 0-5V

// Pins
const int pin_sw = 6;
const int right_forward = 7;
const int right_back = 8;
const int right_pwm = 9;
const int left_pwm = 10;
const int left_forward = 11;
const int left_back = 12;

void moveForward2Feet(int max_pulse) {
  int pwm_out = 60; // around 1 V

  // initialize pins
  digitalWrite(right_forward, HIGH);
  digitalWrite(left_forward, HIGH);
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, LOW);
  
  for(int pulse=0; pulse<max_pulse; pulse++) { // number of encoder pulses
    /*if (pulse < (vref_max*51-150))
      pwm_out++;  // speed up at start
    else if (pulse >= (max_pulse-vref_max*51))
      pwm_out--;  // slow down at end*/

    pwm_out += (pulse<195) - (pulse >= max_pulse-255);

    analogWrite(right_pwm, pwm_out);
    analogWrite(left_pwm, pwm_out);

    delay(1);
  }
}

void motor_output_pins_low() {
  digitalWrite(right_forward, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, LOW);
}

void setup() {  
  // set up interrupts
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), ISR_Renc, RISING);
  attachInterrupt(digitalPinToInterrupt(3), ISR_Lenc, RISING);

  // Set up pins
  pinMode(pin_sw, INPUT_PULLUP);
  pinMode(right_forward, OUTPUT);
  pinMode(right_back, OUTPUT);
  pinMode(right_pwm, OUTPUT);
  pinMode(left_pwm,OUTPUT);
  pinMode(left_forward, OUTPUT);
  pinMode(left_back, OUTPUT);

  pinMode(13, OUTPUT);

  // stop forward/backward motion on both sides
  digitalWrite(right_forward, LOW);
  digitalWrite(right_back, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(left_back, LOW);

  // start PWM for v_ref
  analogWrite(right_pwm, 0);
  analogWrite(left_pwm, 0);
}

void loop() {
  digitalWrite(13, LOW);  // turn LED off
  do{} while (digitalRead(pin_sw) == HIGH); // wait for ON switch
  digitalWrite(13, HIGH); // turn LED on
  delay(1000);            // wait 1 second
  
  moveForward2Feet(3000);
  motor_output_pins_low();
}

void ISR_Renc() { // pin 2 ISR
  R_pulse_ct++;
}

void ISR_Lenc() { // pin 3 ISR
  L_pulse_ct++;
}
