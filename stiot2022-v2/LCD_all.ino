#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin configuration
const int trigPin  = D9;    // Trig pin for ultrasonic
const int echoPin  = D10;   // Echo pin for ultrasonic
const int irSensor = D6;    // IR sensor input (active LOW)
const int buzzer   = A3;    // Buzzer output

bool buzzerOn = false;

// LCD I2C address (use I2C scanner to confirm)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD

void setup() {
  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irSensor, INPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  // // Configure I2C for STM32: SDA=D0, SCL=D1
  // Wire.setSDA(D4);
  // Wire.setSCL(D5);
  // Wire.begin();

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // --- Trigger ultrasonic pulse ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // --- Measure echo duration ---
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- If object is closer than 10cm ---
  if (distance < 10 && distance > 0) {
    buzzerOn = true;
  }

  // --- If IR sensor detects object (active LOW) ---
  if (digitalRead(irSensor) == LOW) {
    buzzerOn = false;
    lcd.clear();  // Clear LCD on IR trigger
  }

  // --- Buzzer and LCD control ---
  if (buzzerOn) {
    tone(buzzer, 1000);   // Activate buzzer
    lcd.setCursor(0, 0);
    lcd.print("WELCOME HOME    ");
  } else {
    noTone(buzzer);       // Turn off buzzer
    if (distance >= 10) {
      lcd.clear();        // Clear LCD when object is far
    }
  }

  delay(200);
}
