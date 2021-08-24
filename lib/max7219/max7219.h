/*
* The MIT License (MIT)
*
* Copyright (c) JEMRF
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*********************************************************************************
*
* Module     : max7219.h
* Author     : Jonathan Evans
* Description: MAX7219 LED Display Driver
*
********************************************************************************
*/

#ifndef max7219_h
#define max7219_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

//MAX7219
#define REG_DECODE        0x09                        
#define REG_INTENSITY     0x0a                       
#define REG_SCAN_LIMIT    0x0b                       
#define REG_SHUTDOWN      0x0c                      
#define REG_DISPLAY_TEST  0x0f                       

#define INTENSITY_MIN     0x00                       
#define INTENSITY_MAX     0x0f                      
#define MAX_CLK A0
#define MAX_CS A1
#define MAX_DIN A2

/*
*********************************************************************************************************
* LED Segments:         a
*                     ----
*                   f|    |b
*                    |  g |
*                     ----
*                   e|    |c
*                    |    |
*                     ----  o dp
*                       d
*   Register bits:
*      bit:  7  6  5  4  3  2  1  0
*           dp  a  b  c  d  e  f  g
*********************************************************************************************************
* Example : The letter 'I' is represented by illuminating LED's 'b' and 'c' (refer above diagram)
*           Therfore the binary representation of 'I' is as follows
*
*           abcdefg
*           0110000
* 
*           The table below contains all the binary values for the desired font. New font characters
*           can be added or altered as required. 
*           
*           The DP bit is used to switch on the decimal place LED. DP is not included in the below table
*           but is added in the register within the libary depending on the content being displayed.  
*********************************************************************************************************

*/
static const struct {
	char   ascii;
	char   segs;
} MAX7219_Font[] = {
  {'A',(char)0b1110111},{'B',(char)0b1111111},{'C',(char)0b1001110},{'D',(char)0b1111110},{'E',(char)0b1001111},{'F',(char)0b1000111},       
  {'G',(char)0b1011110},{'H',(char)0b0110111},{'I',(char)0b0110000},{'J',(char)0b0111100},{'L',(char)0b0001110},{'N',(char)0b1110110},       
  {'O',(char)0b1111110},{'P',(char)0b1100111},{'R',(char)0b0000101},{'S',(char)0b1011011},{'T',(char)0b0001111},{'U',(char)0b0111110},       
  {'Y',(char)0b0100111},{'[',(char)0b1001110},{']',(char)0b1111000},{'_',char(0b0001000)},{'a',(char)0b1110111},{'b',(char)0b0011111},       
  {'c',(char)0b0001101},{'d',(char)0b0111101},{'e',(char)0b1001111},{'f',(char)0b1000111},{'g',(char)0b1011110},{'h',(char)0b0010111},       
  {'i',(char)0b0010000},{'j',(char)0b0111100},{'l',(char)0b0001110},{'n',(char)0b0010101},{'o',(char)0b1111110},{'p',(char)0b1100111},       
  {'r',(char)0b0000101},{'s',(char)0b1011011},{'t',(char)0b0001111},{'u',(char)0b0011100},{'y',(char)0b0100111},{'-',(char)0b0000001},
  {' ',(char)0b0000000},{'0',(char)0b1111110},{'1',(char)0b0110000},{'2',(char)0b1101101},{'3',(char)0b1111001},{'4',(char)0b0110011},
  {'5',(char)0b1011011},{'6',(char)0b1011111},{'7',(char)0b1110000},{'8',(char)0b1111111},{'9',(char)0b1111011},{(char)'/0',(char)0b0000000}
  };

class MAX7219
{
private:

public: 
  MAX7219(void);
  void Begin(void);
  void DisplayChar (char digit, char character, unsigned int dp);  
  void MAX7219_DisplayTestStart (void);
  void DisplayChar(int digit, char value, bool dp);
  void clearDisplay();
  void MAX7219_Write(volatile byte opcode, volatile byte data);
  void MAX7219_ShutdownStop (void);
  unsigned char MAX7219_LookupCode (char character, unsigned int dp);
  void MAX7219_ShutdownStart (void);
  void Clear (void);
  void MAX7219_DisplayTestStop (void);
  void MAX7219_SetBrightness (char brightness);
  void DisplayText(char *text, int justify);

};

#endif