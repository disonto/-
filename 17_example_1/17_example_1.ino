#include <Servo.h>

// Arduino pin assignment

#define PIN_IR 0
#define PIN_SERVO 10
#define PIN_LED  9

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _DIST_MIN 100
#define _DIST_MAX 250

#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)

#define _EMA_ALPHA 0.08

float dist_prev = _DIST_MAX;
int dist_ema;
Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_IR,INPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(57600);

  last_loop_time = 0;
}

void loop()
{
  int dist_raw;
  unsigned long time_curr = millis();
  int a_value, duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IR);

  // Read IR Sensor value !!!
  if (dist_raw < _DIST_MIN) {
    dist_raw = dist_prev;
    digitalWrite(PIN_LED, 1);
  } else if (dist_raw > _DIST_MAX) {
    dist_raw = dist_prev;
    digitalWrite(PIN_LED, 1);
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON      
  }
  // Convert IR sensor value into distance !!!
  // we need distance range filter here !!!

  // map distance into duty
  dist_raw = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;

  dist_ema = _EMA_ALPHA * dist_prev + (1 - _EMA_ALPHA) * dist_ema;

  duty = 0.082*dist_ema*dist_ema - 16.4*dist_ema + 1370;

  if (dist_ema < _DIST_MIN) {
    myservo.writeMicroseconds(_DUTY_MIN);
    duty = _DUTY_MIN;
  }else if (dist_ema > _DIST_MAX) {
    myservo.writeMicroseconds(_DUTY_MAX);
    duty = _DUTY_MAX;
  }else {
    myservo.writeMicroseconds(duty);
  }

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:"); Serial.print(_DIST_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",DIST:"); Serial.print(dist_raw);
  Serial.print(",EMA:"); Serial.print(dist_ema);
  Serial.print(",SERVO:"); Serial.print(duty);
  Serial.print(",MAX : "); Serial.print(_DIST_MAX);
  Serial.println("");

  dist_prev = dist_raw;
}
