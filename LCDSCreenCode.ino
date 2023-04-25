// include the library code:
#include <LiquidCrystal.h>
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
 
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("Hello There!");
}
 
void loop() {
  delay(1500);
  lcd.clear();
  lcd.print("My ex-man brought");
  lcd.setCursor(0,1);
  lcd.print("his new girlfr");
  delay(1500);
  lcd.clear();
  lcd.print("iend, and shes l");
  lcd.setCursor(0,1);
  lcd.print("ike, Oh my God!");
  delay(1500);
  lcd.clear();
  lcd.print("but I'm just go");
  lcd.setCursor(0,1);
  lcd.print("nna shake");
  delay(1500);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
 // lcd.setCursor(0, 1);
  // print the number of seconds since reset:
 // lcd.print(millis() / 1000);
}