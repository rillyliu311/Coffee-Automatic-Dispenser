#include <LiquidCrystal.h>
#include <Servo.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// define button pins
const int button1Pin = 30;
const int button2Pin = 31;
const int trigPin = A0;
const int echoPin = A1;

// define H-bridge pins
const int A1A = 32;
const int A1B = 33;

// define servo pins
Servo myservo1; // Servo 1 cupdispenser
Servo myservo2; // Servo 2 cupdispenser
Servo myServo3; // Continuous servo
Servo myservo4; // Servo 4 sugar dispenser


const int delayTime = 1000;

void setup() {
  Serial.begin(115200);
  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);

  // initialize button pins
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  
  // Enable the internal pull-up resistors for the buttons
  digitalWrite(button1Pin, HIGH);
  digitalWrite(button2Pin, HIGH);

  // initialize servo pins
  myservo1.attach(6); // Servo 1 attached to pin 9
  myservo2.attach(9); // Servo 2 attached to pin 10
  myServo3.attach(8);  // Servo 3 attached to pin 8
  myservo4.attach(7); // Servo 4 attached to pin 7

  // initialize H-bridge pins
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);

  // initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // print the initial message on the LCD
  lcd.clear();
  lcd.print("How do you want");
  lcd.setCursor(0, 1);
  lcd.print("your coffee?");

}

void loop() {
  // Call function to handle button presses
  int buttonPressed = LCD_Display();
  if (buttonPressed) {
    // Start a timer for 15 seconds
    unsigned long startTime = millis();
    while (millis() - startTime < 3000)
    {
      delay(1000); // Wait for 1 second
    }

    // Call function for the cup dispenser
    cupDispenser();
    delay(2000);

    // Turn on the DC motor
    myServo3.writeMicroseconds(2000);
    delay(2000);

    // Check if button1 is pressed
    if (digitalRead(button1Pin) == HIGH) {
      Button1Pressed();
    }
      // Check if button2 is pressed
     else if (digitalRead(button2Pin) == HIGH) {
        Button2Pressed();
    }   
  }
}


void Button1Pressed() {
  // Wait for an object to be detected
  while (true) {
    // Send ultrasonic signal
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure distance
    int duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;

    // Send distance measurement to serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Check if an object is detected
    if ( distance < 4) { 

      myServo3.writeMicroseconds(2000);
      delay(3000);
      // Stop continuous servo motor for 10 seconds
      myServo3.writeMicroseconds(1500);
      delay(5000);

      // Dispenses Sugar
      myservo4.write(0); // move the servo to 0 degrees
      delay(2000); // wait for 2 seconds
      myservo4.write(90); // move the servo to 90 degrees
      
      // Dispenses Coffee
      CoffeeDispenser();

      // Start continuous servo motor again
      myServo3.writeMicroseconds(2000);
      delay(5000);

      // Stop conveyor belt
      myServo3.writeMicroseconds(1500);


      // Exit the while loop
      break;
    }
  }
}

void Button2Pressed() {
  // Wait for an object to be detected
  while (true) {
    // Send ultrasonic signal
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure distance
    int duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;

    // Send distance measurement to serial monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Check if an object is detected
    if (distance < 4) {

      // Stop continuous servo motor for 10 seconds
      myServo3.writeMicroseconds(1500);
      delay(3000);
      // Start continuous servo motor again
      myServo3.writeMicroseconds(2000);
      delay(3000);
      
      // Dispenses Sugar
      myservo4.write(0); // move the servo to 0 degrees
        delay(2000); // wait for 2 seconds
        myservo4.write(90); // move the servo to 90 degrees

      // Dispenses coffee
      CoffeeDispenser();

      // Exit the while loop
      break;
    }
  }
}


int LCD_Display() {
  bool button1State = digitalRead(button1Pin);
  bool button2State = digitalRead(button2Pin);

  // check if button1 is pressed
  if (!button1State) {
    // switch to "No sugar"
    lcd.clear();
    lcd.print("No sugar");
    delay(delayTime); // wait for 1 second
    return 1;
  }

  // check if button2 is pressed
  else if (!button2State) {
    // switch to "With sugar"
    lcd.clear();
    lcd.print("With sugar");
    delay(delayTime); // wait for 1 second
    return 1;
  }

  // no button pressed
  else {}
  return 0;
}

void cupDispenser() {
  delay(1000); //wait 1s
  myservo1.write(140); myservo2.write(25); //Pull cup back
  delay(1000); //wait 1s
  myservo1.write(70); myservo2.write(95); //push cup forwards
}

void CoffeeDispenser() {
  // make the motor rotate clockwise at half speed
      digitalWrite(A1A, HIGH);
      digitalWrite(A1B, LOW);

      // wait for 10 seconds
      delay(10000);

      // stop the motor
      digitalWrite(A1A, LOW);
      digitalWrite(A1B, LOW);

}