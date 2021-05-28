#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  
#include <Servo.h>

// Declare the Servo pin 
int servoPin = 12; 
// Create a servo object 
Servo Servo1; 

#define KEY_ROWS 4  // Keypad rows
#define KEY_COLS 4  // Keypad columns
#define LCD_ROWS 4  // LCD rows
#define LCD_COLS 20 // LCD columns

// 設置按鍵模組
char keymap[KEY_ROWS][KEY_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[KEY_ROWS] = {11, 10, 9, 8}; //改 D-pin
byte colPins[KEY_COLS] = {7, 6, 5, 4}; // 改 D-pin

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

String passcode = "1234";   // Setting password //改 密碼
String inputCode = "";      // Save the password
bool acceptKey = true;      // Accepting the password entered
LiquidCrystal_I2C lcd(0x3f, 20, 4);  

void clearRow(byte n) {
  byte last = LCD_COLS - n;
  lcd.setCursor(n, 1); // Change to the 2nd row, then prints "Pin:"

  for (byte i = 0; i < last; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(n, 1);
}

void resetLocker() {
  lcd.clear();
  lcd.print("I m YMSH Securitybox"); //改 字
  lcd.setCursor(0, 1);  // Change to the 2nd row //更換到第二行文字
  lcd.print("Password:");
  lcd.cursor();       // 輸入密碼
  Servo1.write(150); //The servo locks //改 角度大小 初始角度
  acceptKey = true;
  inputCode = "";
}
void unlockdoor(){ //controls servo that locks the door
  Servo1.write(10); //Degree when the lock opens //改 角度大小
  lcd.setCursor(0,1);
  clearRow(0);
  lcd.print("~~~Correct!~~"); //改 字
  delay(1500); //To make sure the LCD receives the message
}

void checkPinCode() {
  acceptKey = false;  // Not accepting any enter for passcode
  clearRow(0);        // Clear from the 0 numer
  lcd.noCursor();
  lcd.setCursor(0, 1);  // Change to the 2nd row
  // Check the passcode
 
  if (inputCode == passcode) {
    
    lcd.print("~~~Correct!~~"); //改 字
    unlockdoor();
 
  } else {
    lcd.print("OH! It's Locked!"); //改 字
    
  }
  delay(5000);
  resetLocker();     // Reset LCD and servo
}



void setup() {
  digitalWrite( 12 , LOW ); 
  Serial.begin(9600);

  lcd.init();       
  lcd.backlight();  
  Servo1.attach(servoPin); 
  Servo1.write(180); //Degree when the servo locks //改 角度大小
  resetLocker();
}

void loop() {  
  char key = keypad.getKey();
 
  if (acceptKey && key != NO_KEY) {
    if (key == '*') {   // Clear
      clearRow(4);  // Clear from the 4th row
      inputCode = "";
    } else if (key == '#') {  // Check passcode
      checkPinCode();
    } else {
      inputCode += key;  // Save
      lcd.print('*');
    }
}
}
