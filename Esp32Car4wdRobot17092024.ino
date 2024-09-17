#include <ESP32Servo.h>

bool sw1_val = 0;
int ultra_stop_cm = 30;//cm 
int motor_delay = 100;

int inbuild_Led = 2;
int touch_Sensor = 22;
int left_IR = 13;
int right_IR = 15;
int servoPin1 = 26;
int trig = 25;
int echo = 33;
int motor_t1 = 5;  //white
int motor_t2 = 18; //grey
int motor_t3 = 19; //green
int motor_t4 = 21; //yellow

Servo myservo1;

long timeInMicro;

long distanceInCm;

void m_stop () {
  digitalWrite(motor_t1, LOW);
  digitalWrite(motor_t2, LOW);
  digitalWrite(motor_t3, LOW);
  digitalWrite(motor_t4, LOW);
  delay(motor_delay);
}

void m_forward () {
  digitalWrite(motor_t1, LOW);
  digitalWrite(motor_t2, HIGH);
  digitalWrite(motor_t3, HIGH);
  digitalWrite(motor_t4, LOW);
  delay(motor_delay);
}

void m_left () {
  digitalWrite(motor_t1, LOW);
  digitalWrite(motor_t2, LOW);
  digitalWrite(motor_t3, HIGH);
  digitalWrite(motor_t4, LOW);
  delay(motor_delay);
}

void m_right () {
  digitalWrite(motor_t1, LOW);
  digitalWrite(motor_t2, HIGH);
  digitalWrite(motor_t3, LOW);
  digitalWrite(motor_t4, LOW);
  delay(motor_delay);
}

void setup() {
  Serial.begin(115200);
  pinMode(motor_t1,OUTPUT);
  pinMode(motor_t2,OUTPUT);
  pinMode(motor_t3,OUTPUT);
  pinMode(motor_t4,OUTPUT);
  pinMode(left_IR, INPUT);
  pinMode(right_IR, INPUT);
  pinMode(inbuild_Led, OUTPUT);
  pinMode(touch_Sensor, INPUT);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  myservo1.attach(servoPin1);
  myservo1.write(90);
}

void loop() {
  if(digitalRead(touch_Sensor) == 0) {
    if(sw1_val == 1) {
      sw1_val = 0;
      digitalWrite(inbuild_Led, LOW);
    }
    else {
      sw1_val = 1;
      digitalWrite(inbuild_Led, HIGH);
    }
    Serial.println("22 pressed");
    delay(1000);
  }
  else if (sw1_val == 0) {
    if(digitalRead(left_IR) == HIGH && digitalRead(right_IR) == HIGH) {
      Serial.println("Stop");
      m_stop();
    }
    else if (digitalRead(left_IR) == LOW && digitalRead(right_IR) == HIGH) {
      Serial.println("left");
      m_left();
    }
    else if (digitalRead(left_IR) == HIGH && digitalRead(right_IR) == LOW) {
      Serial.println("Right");
      m_right();
    }
    else if (digitalRead(left_IR) == LOW && digitalRead(right_IR) == LOW) {
      Serial.println("Obstracle Following Forward");
      m_forward();
    }
  }
  else if (sw1_val == 1) {
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);
    timeInMicro= pulseIn(echo,HIGH);
    distanceInCm = ((timeInMicro/29)/2);
    Serial.print("Distance: ");
    Serial.print(distanceInCm);
    Serial.println(" cm - Obstacle Avoiding");
    delay(100);
    if (distanceInCm <= ultra_stop_cm) {
      m_stop();
      delay(2000);
      myservo1.write(0);
      delay(2000);
      myservo1.write(180);
      delay(2000);
      myservo1.write(90);
      delay(2000);
      Serial.println("left");
      m_left();
      delay(400);
    }
    else {
      m_forward();
      m_stop();
    }
  }
}
