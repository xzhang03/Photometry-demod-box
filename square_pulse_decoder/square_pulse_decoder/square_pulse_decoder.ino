#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// Array info
const byte lightarraypts = 40;
int lightarray[lightarraypts];
byte currentpt = 0;

// Rejection info (telling user that a lot of numbers have been rejected)
bool rejinform = false;
const byte rejinform_thresh = 500;
byte rejinform_counter = 0;

// Time info
byte cycletime = 2;

// Analog input info
byte AnalogPin = A0;
int Ai;
byte Aithresh = 50;

// Output
int sum;
byte i;
float Ao = 0.0;
float V = 0.0;
float slope = 157.44;
float dcoffset = -10.814;

void setup() {
  // Debug
  // Serial.begin(9600);

  // LCD
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Raw input:");
  lcd.setCursor(0, 1);
  lcd.print("Voltage:");
  
}

void loop() {
  // Debug
  // Serial.println("Collecting data...");
  
  // Sampling
  rejinform = false;
  rejinform_counter = 0;
  
  // Collecting phase
  while (currentpt < lightarraypts){
    // Read
    Ai = analogRead(AnalogPin);

    // Log if there is light
    if (Ai >= Aithresh){
      lightarray[currentpt] = Ai;
      currentpt++;

      // Debug
      // Serial.print("Accept: ");
      // Serial.println(Ai);
    }
    else{
      // Debug
      // Serial.print("Reject: ");
      // Serial.println(Ai);
      rejinform_counter++;

      if ((rejinform_counter >= rejinform_thresh) && !rejinform){
        lcd.setCursor(11,0);
        lcd.print("LOW ");
        
        lcd.setCursor(9, 1);
        lcd.print("LOW ");
        rejinform = true;
      }
    }

    // Debug
    delay(cycletime);
  }

  // Debug
  // Serial.println("Processing data...");
  
  // Processing phase
  sum = 0;
  for (i = 0; i < lightarraypts; i++){
    sum += lightarray[i];
  }
  
  // Serial.print("Sum: ");
  // Serial.println(sum);
  
  Ao = sum / lightarraypts;
  // Serial.print("Average: ");
  // Serial.println(Ao);

  // Convert to voltage
  V = (Ao - dcoffset) / slope;

  // Serial.print("Voltage: ");
  // Serial.println(V);
  
  if (Ao <= 1022){
    // LCD
    lcd.setCursor(11,0);
    lcd.print(round(Ao));
    lcd.setCursor(9, 1);
    lcd.print(V);
  }
  else {
    lcd.setCursor(11,0);
    lcd.print(round(Ao));                                                                                           
    lcd.setCursor(9, 1);
    lcd.print("MAX ");
  }
  /*
  if (Ao > 99){
    lcd.print(Ao);
  }
  else if (Ao > 9){
    lcd.print(Ao);
    lcd.print(" ");
  }
  else{
    lcd.print(Ao);
    lcd.print("  ");
  }
  */

  
  
  // Reset
  currentpt = 0;
  delay(100);
  
}
