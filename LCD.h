#ifndef __LCD_H
#include "LiquidCrystal.h"

void setupLCD();
void printToLCD(int x, int y, String message);
void clearLCD();

#define __LCD_H
#endif
