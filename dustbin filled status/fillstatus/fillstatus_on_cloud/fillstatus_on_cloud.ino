#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with the I2C address (usually 0x27 or 0x3F for I2C modules)
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 columns and 2 rows

// Define pins for the ultrasonic sensors
const int trigPin1 = 9; // Dry waste sensor
const int echoPin1 = 10;
const int trigPin2 = 11; // Wet waste sensor
const int echoPin2 = 12;

// Define the depth of the empty bin (in centimeters)
const int binDepth = 28; // Adjust this based on your bin's actual depth

void setup() {
  // Initialize the LCD with number of columns and rows
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.backlight();
  lcd.print("Dustbin Status:");

  // Set up ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Start serial communication (optional for debugging)
  Serial.begin(9600);
}

void loop() {
  long duration1, duration2, distance1, distance2;
  
  // Trigger the first ultrasonic sensor (dry waste)
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // Trigger the second ultrasonic sensor (wet waste)
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Calculate how full each bin is as a percentage
  int fillPercentDry = map(distance1, 0, binDepth, 100, 0);
  int fillPercentWet = map(distance2, 0, binDepth, 100, 0);

  // Determine the highest fill level
  int highestFillPercent = max(fillPercentDry, fillPercentWet);

  // Clear the previous value on LCD and print the fill status
  lcd.setCursor(0, 1);
  if (highestFillPercent >= 90) {
    lcd.print("FULL         ");
  }
  else if (highestFillPercent < 0) {
    lcd.print("EMPTY        ");
  }
  else {
    lcd.print(highestFillPercent);
    lcd.print("% filled     ");
  }

  // Print distances for debugging in the serial monitor
  Serial.print("Dry Distance: ");
  Serial.print(distance1);
  Serial.print(" cm, Wet Distance: ");
  Serial.print(distance2);
  Serial.print(" cm, Highest Fill: ");
  Serial.print(highestFillPercent);
  Serial.println("%");

  // Small delay before the next reading
  delay(1000);
}
