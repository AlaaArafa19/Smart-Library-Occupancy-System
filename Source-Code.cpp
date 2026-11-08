#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
// [[[[[[Pin Definitions]]]]]] 
const int IR_ENTRY = 27; // Entry IR sensor 
const int IR_EXIT = 25; // Exit IR sensor 
const int RELAY_PIN = 26; // Relay (lighting control) 
const int SDA_PIN = 33; // LCD I2C data 
const int SCL_PIN = 32; // LCD I2C clock 
// [[[[[[State Variables]]]]]] 
int count = 0; 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
// [[[[[[Setup]]]]]] 
void setup() { 
 Serial.begin(115200); 
 Wire.begin(SDA_PIN, SCL_PIN); 
 lcd.init(); 
 lcd.backlight(); 
 lcd.setCursor(0, 0); 
 lcd.print("Library System"); 
 delay(2000); 
 updateDisplay(); 
 pinMode(IR_ENTRY, INPUT); 
 pinMode(IR_EXIT, INPUT); 
 pinMode(RELAY_PIN, OUTPUT); 
 digitalWrite(RELAY_PIN, HIGH); // Start with lights OFF 
} 
// [[[[[[Main Loop]]]]]] 
void loop() { 
 bool entryDetected = digitalRead(IR_ENTRY) == LOW; 
 bool exitDetected = digitalRead(IR_EXIT) == LOW; 
 // [[[[[[Person enters]]]]]] 
 if (entryDetected) { 
 count++; 
 updateDisplay(); 
 delay(500); // Debounce 
 while(digitalRead(IR_ENTRY) == LOW); // Wait for clear 
 } 
 // [[[[[[Person exits]]]]]] 
 if (exitDetected) { 
 if (count > 0) count--;
updateDisplay(); 
 delay(500); 
 while(digitalRead(IR_EXIT) == LOW); 
 } 
 // [[[[[[Smart Lighting: ON when occupied, OFF when empty]]]]]] 
 if (count > 0) { 
 digitalWrite(RELAY_PIN, LOW); // Lights ON 
 } else { 
 digitalWrite(RELAY_PIN, HIGH); // Lights OFF 
 } 
 delay(100); 
} 
// [[[[[[Display Update]]]]]] 
void updateDisplay() { 
 lcd.clear(); 
 lcd.setCursor(0, 0); 
 lcd.print("Count: "); 
 lcd.print(count); 
 lcd.setCursor(0, 1); 
 if (count >= 0 && count <= 3) lcd.print("Status: Empty"); 
 else if (count >= 4 && count <= 7) lcd.print("Status: Medium"); 
 else if (count >= 8) lcd.print("Status: Crowded");