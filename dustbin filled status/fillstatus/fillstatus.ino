#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with the I2C address (usually 0x27 or 0x3F for I2C modules)
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns and 2 rows

// Define pins for ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Define the depth of the empty bin (in centimeters)
const int binDepth = 50; // Adjust this based on your bin's actual depth

void setup() {
  // Initialize the LCD with number of columns and rows
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.backlight();
  lcd.print("Dustbin Status:");

  // Set up ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Start serial communication (optional for debugging)
  Serial.begin(9600);
}

void loop() {
  long duration, distance;
  
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  // Calculate how full the bin is as a percentage
  int fillPercent = map(distance, 0, binDepth, 100, 0);

  // Clear the previous value on LCD and print the fill status
  lcd.setCursor(0, 1);
  if (fillPercent >= 90) {
    lcd.print("FULL       ");
  }
  else if(fillPercent < 0)
  {
    lcd.print("EMPTY       ");
  }
  else {
    lcd.print(fillPercent);
    lcd.print("% filled    ");
  }

  // Print distance for debugging in the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Small delay before next reading
  delay(1000);
}
