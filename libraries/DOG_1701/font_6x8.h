/*File '6X8.FV' as include
The font was generated with EA FontEditor. Please contact ELECTRONIC ASSEMBLY
for more details (techik@lcd-module.de)

 the array starts with a 8 byte header:
  1st Byte: 'F' first 2 bytes are always FV
  2nd Byte: 'V' for FONT VERTICAL
  3rd Byte: First code to define
  4th Byte: Last  code to define
  5th Byte: Width of character in dots
  6th Byte: Height of character in dots
  7th Byte: Height of character in bytes
  8th Byte: Bytes needed for each character (1..255)
            or 0 for big fonts calculate WidthInDots * HeightInBytes
  After that font data will follow */

#define FONT_6X8_LEN  584

const byte font_6x8[FONT_6X8_LEN] __attribute__((section(".progmem.data"))) = 
{   
	70, 86, 32,127,  6,  8,  1,  6,
    0,  0,  0,  0,  0,  0,
    0,  0, 95,  0,  0,  0,
    0,  7,  0,  7,  0,  0,
   20,127, 20,127, 20,  0,
   36, 42,127, 42, 18,  0,
   35, 19,  8,100, 98,  0,
   54, 73, 86, 32, 80,  0,
    0,  8,  7,  3,  0,  0,
    0, 28, 34, 65,  0,  0,
    0, 65, 34, 28,  0,  0,
   42, 28,127, 28, 42,  0,
    8,  8, 62,  8,  8,  0,
    0,128,112, 48,  0,  0,
    8,  8,  8,  8,  8,  0,
    0,  0, 96, 96,  0,  0,
   32, 16,  8,  4,  2,  0,
   62, 81, 73, 69, 62,  0,
    0, 66,127, 64,  0,  0,
   66, 97, 81, 73, 70,  0,
   33, 65, 73, 77, 51,  0,
   24, 20, 18,127, 16,  0,
   39, 69, 69, 69, 57,  0,
   60, 74, 73, 73, 48,  0,
   65, 33, 17,  9,  7,  0,
   54, 73, 73, 73, 54,  0,
    6, 73, 73, 41, 30,  0,
    0,  0, 20,  0,  0,  0,
    0, 64, 52,  0,  0,  0,
    0,  8, 20, 34, 65,  0,
   20, 20, 20, 20, 20,  0,
    0, 65, 34, 20,  8,  0,
    2,  1, 81,  9,  6,  0,
   62, 65, 93, 89, 78,  0,
  124, 18, 17, 18,124,  0,
  127, 73, 73, 73, 54,  0,
   62, 65, 65, 65, 34,  0,
  127, 65, 65, 65, 62,  0,
  127, 73, 73, 73, 65,  0,
  127,  9,  9,  9,  1,  0,
   62, 65, 73, 73,122,  0,
  127,  8,  8,  8,127,  0,
    0, 65,127, 65,  0,  0,
   32, 64, 65, 63,  1,  0,
  127,  8, 20, 34, 65,  0,
  127, 64, 64, 64, 64,  0,
  127,  2, 28,  2,127,  0,
  127,  4,  8, 16,127,  0,
   62, 65, 65, 65, 62,  0,
  127,  9,  9,  9,  6,  0,
   62, 65, 81, 33, 94,  0,
  127,  9, 25, 41, 70,  0,
   38, 73, 73, 73, 50,  0,
    1,  1,127,  1,  1,  0,
   63, 64, 64, 64, 63,  0,
   31, 32, 64, 32, 31,  0,
   63, 64, 56, 64, 63,  0,
   99, 20,  8, 20, 99,  0,
    3,  4,120,  4,  3,  0,
   97, 81, 73, 69, 67,  0,
    0,127, 65, 65, 65,  0,
    2,  4,  8, 16, 32,  0,
    0, 65, 65, 65,127,  0,
    4,  2,  1,  2,  4,  0,
   64, 64, 64, 64, 64,  0,
    0,  3,  7,  8,  0,  0,
   32, 84, 84, 84,120,  0,
  127, 40, 68, 68, 56,  0,
   56, 68, 68, 68, 40,  0,
   56, 68, 68, 40,127,  0,
   56, 84, 84, 84, 24,  0,
    0,  8,126,  9,  2,  0,
   24,164,164,164,124,  0,
  127,  8,  4,  4,120,  0,
    0, 68,125, 64,  0,  0,
   32, 64, 64, 61,  0,  0,
  127, 16, 40, 68,  0,  0,
    0, 65,127, 64,  0,  0,
  124,  4,120,  4,120,  0,
  124,  8,  4,  4,120,  0,
   56, 68, 68, 68, 56,  0,
  252, 24, 36, 36, 24,  0,
   24, 36, 36, 24,252,  0,
  124,  8,  4,  4,  8,  0,
   72, 84, 84, 84, 36,  0,
    4,  4, 63, 68, 36,  0,
   60, 64, 64, 32,124,  0,
   28, 32, 64, 32, 28,  0,
   60, 64, 48, 64, 60,  0,
   68, 40, 16, 40, 68,  0,
   76,144,144,144,124,  0,
   68,100, 84, 76, 68,  0,
    0,  8, 54, 65,  0,  0,
    0,  0,119,  0,  0,  0,
    0, 65, 54,  8,  0,  0,
    2,  1,  2,  4,  2,  0,
   60, 38, 35, 38, 60,  0
};