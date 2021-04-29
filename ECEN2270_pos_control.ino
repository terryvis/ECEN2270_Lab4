volatile bool var = LOW;

volatile int pulse = 0;
volatile int pwm_out = 0;

volatile int v_ref_max = 5;       // range 0-5V

volatile int left_pending = 0;

// Pins
const int pin_sw = 6;
const int right_forward = 7;
const int right_back = 8;
const int right_pwm = 9;
const int left_pwm = 10;
const int left_forward = 11;
const int left_back = 12;

const int pwm_start = 60; // starting Vref for robot

void moveForward2Feet(int max_pulse) {
  pwm_out = pwm_start; // around 1 V
  pulse = 0;
  int pulse_prev = 0;   // to determine if pulse received in IRQ

  // initialize pins
  digitalWrite(right_forward, HIGH);
  digitalWrite(left_forward, HIGH);
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, LOW);
  
  while(pulse < max_pulse) {
    // check if pulse received
    if (pulse_prev != pulse) {
      pulse_prev = pulse;
      pwm_out += (pulse<255-pwm_start) - (pulse >= max_pulse-255); // adjust PWM output for Vref

      // write Vref output
      analogWrite(right_pwm, pwm_out);
      analogWrite(left_pwm, pwm_out);
    }
  }

  // when done, set all motor outputs low
  motor_output_pins_low();
}

void turnClockwise(int max_pulse) {
  int pwm_out = pwm_start; // around 1 V
  pulse = 0;
  int pulse_prev = 0;   // to determine if pulse received in 

  // initialize pins
  digitalWrite(right_forward, LOW);
  digitalWrite(left_forward, HIGH);
  digitalWrite(right_back, HIGH);
  digitalWrite(left_back, LOW);
  
  while(pulse < max_pulse) {
    // check if pulse received
    if (pulse_prev != pulse) {
      pulse_prev = pulse;
      pwm_out += (pulse<255-pwm_start) - (pulse >= max_pulse-255); // adjust PWM output for Vref

      // write Vref output
      analogWrite(right_pwm, pwm_out);
      analogWrite(left_pwm, pwm_out);
    }
  }

  // when done, set all motor outputs low
  motor_output_pins_low();
}

void turnCounterClockwise(int max_pulse) {
  int pwm_out = pwm_start; // around 1 V
  pulse = 0;
  int pulse_prev = 0;   // to determine if pulse received in 

  // initialize pins
  digitalWrite(right_forward, HIGH);
  digitalWrite(left_forward, LOW);
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, HIGH);
  
  while(pulse < max_pulse) {
    // check if pulse received
    if (pulse_prev != pulse) {
      pulse_prev = pulse;
      pwm_out += (pulse<255-pwm_start) - (pulse >= max_pulse-255); // adjust PWM output for Vref

      // write Vref output
      analogWrite(right_pwm, pwm_out);
      analogWrite(left_pwm, pwm_out);
    }
  }

  // when done, set all motor outputs low
  motor_output_pins_low();
}

void motor_output_pins_low() {
  digitalWrite(right_forward, LOW);
  digitalWrite(left_forward, LOW);
  digitalWrite(right_back, LOW);
  digitalWrite(left_back, LOW);
}

void setup() {  
  Serial.begin(9600);
  
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

  /*
   * 2580 pulses = 2 feet
   * 1200 pulses = 180 deg rotation clockwise (right reverse overperforms)
   * 1420 pulses = 180 deg rotation counterclockwise (left reverse underperforms)
   */
  
  moveForward2Feet(2580);     // move forward 2 feet
  delay(500);
  turnClockwise(1205);        // turn 180 deg clockwise
  delay(500);
  moveForward2Feet(2580);     // move forward 2 feet
  delay(500);
  turnCounterClockwise(1420); // turn 180 deg CCW
}

void ISR_Renc() { // pin 2 ISR
  if (left_pending) { // check if left pulse received as well
    left_pending = 0; // clear flag
    pulse++;          // increment number of pulses
  }
}

void ISR_Lenc() { // pin 3 ISR
  left_pending = 1; // set flag
}
