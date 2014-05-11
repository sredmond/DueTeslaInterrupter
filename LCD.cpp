#include "LCD.h"
//Register Select, Enable, DB4, DB5, DB6, DB7
LiquidCrystal lcd(12,11,9,8,7,6); //We hide the LCD from the main body of the program to keep the LCD module relatively standalone.
void setupLCD()
{
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Initiating");
  lcd.setCursor(0,1);
  lcd.print("Liquid Crystal");
}

//Prerequisite: 0 <= x < 16, 0 <= y < 2
void printToLCD(int x, int y, String msg)
{
  lcd.setCursor(x,y);
  lcd.print(msg);
}
void clearLCD()
{
  lcd.clear();
}

