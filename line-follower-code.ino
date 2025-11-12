#include <SparkFun_TB6612.h>

// Motor driver pin definitions
#define AIN1 4
#define AIN2 3
#define BIN1 6
#define BIN2 7
#define PWMA 9
#define PWMB 10
#define STBY 5

// Motor configuration offsets
const int offsetA = 1;
const int offsetB = 1;

// Initialize motors
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);  // Left motor
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);  // Right motor

// PID control variables
int P, D, I, previousError = 0, PIDvalue;
int lsp, rsp;
int lfspeed = 130;

float Kp = 0;
float Kd = 0;
float Ki = 0;

// Hard-coded threshold values (inverted logic assumes black surface is high, white is low)
int threshold[6] = {0, 400, 400, 400, 400, 400};

void setup() {
  Serial.begin(9600);
  delay(1000);  // Stabilization delay
}

void loop() {
  // Inverted logic: white line is now the reference (lower values on white)
  if (analogRead(1) < threshold[1] && analogRead(5) > threshold[5]) {
    motor1.drive(0);          // Sharp right
    motor2.drive(lfspeed);
  } 
  else if (analogRead(5) < threshold[5] && analogRead(1) > threshold[1]) {
    motor1.drive(lfspeed);    // Sharp left
    motor2.drive(0);
  } 
  else if (analogRead(3) < threshold[3]) {
    // Adjust PID gains dynamically based on white surface reading
    Kp = 0.001 * (analogRead(3));  // inverse response
    Kd = 8 * Kp;
    linefollow();  // Run PID control
  }
}

void linefollow() {
  int error = analogRead(4) - analogRead(2);  // Inverted: left minus right for white line
  //if (abs(error) < 5) error = 0;
  P = error;
  I += error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = constrain(lfspeed - PIDvalue, 0, 255);
  rsp = constrain(lfspeed + PIDvalue, 0, 255);

  motor1.drive(lsp);
  motor2.drive(rsp);
}
//previously dumped code
