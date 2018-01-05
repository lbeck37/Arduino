#include <Arduino.h>
#include <SPI.h>
#include <dog_1701.h>
#include <font_16x32nums.h>
#include <font_6x8.h>
#include <font_8x16.h>
#include <font_8x8.h>
#include <logo_BLH.h>

/*Available functions in dog_1701 Libraray:
  void initialize  (byte p_cs, byte p_si, byte p_clk, byte p_a0, byte p_res, byte type);
  void clear       (void);
  void contrast    (byte contr);
  void view        (byte direction);
  void string      (byte column, byte page, const byte *font_adress, const char *str);
  void rectangle   (byte start_column, byte start_page, byte end_column, byte end_page, byte pattern);
  void picture     (byte column, byte page, const byte *pic_adress);
*/

dog_1701 DOG;

//the following port definitions are used by our demo board "EA PCBARDDOG7565"
int led       = 3;
int led_green = 5;
int led_red   = 3;
void init_backlight(boolean mono);
void mono_backlight(byte brightness);
void gr_backlight(byte green, byte red);


void sample_screen(void);

//initialize the backlight and the display
void setup()
{
  init_backlight(false); //use RGB backlight in this sample code. Please change it to your configuration
  DOG.initialize(10,0,0,9,4,DOGS102);   //SS = 10, 0,0= use Hardware SPI, 9 = A0, 4 = RESET, EA DOGS102-6 (=102x64 dots)
}

//create a sample sceen content
void sample_screen(void)
{
  DOG.clear();  //clear whole display
  DOG.picture(0,0,ea_logo);
  DOG.string(71,0,font_8x16,"DOGS");      //font size 8x16 first page         
  DOG.rectangle(71,2,127,2,0x03);              //show small line (filled pattern = 0x03), to the left and right of 'DOGL128'            
  DOG.string(0,3,font_6x8,"-3.3V single sup.");
  DOG.string(0,4,font_6x8,"-UC1701");
  DOG.string(0,5,font_6x8,"-LED backlights");
  DOG.string(0,6,font_6x8,"-extrem low power");
  DOG.string(0,7,font_6x8,"lcd-module.com");
}

//main loop
void loop()
{
  rgb_backlight(random(255), random(255));    //BL random color (green, red)
  DOG.view(VIEW_BOTTOM);  //default viewing direction
  sample_screen();        //show content
  delay(2000);
  rgb_backlight(random(255), random(255));    //BL random color (green, red)
  DOG.view(VIEW_TOP);    //alternate viewing direction
  sample_screen();
  delay(2000);
}

//The following functions controll the backlight with a PWM. Not needed for the display content
void init_backlight(boolean mono)
{
  if(mono) //EA LED39X41-W, EA LED39X41-A
  {
    pinMode(led,  OUTPUT);
    mono_backlight(255);
  }
  else  //EA LED39X41-GR
  {
    pinMode(led_green, OUTPUT);
    pinMode(led_red,   OUTPUT);
    rgb_backlight(255,0);
  }
}
//Use this funtion for monochrome backlight
void mono_backlight(byte brightness)
{
  analogWrite(led, brightness);  
}
//Use this function for RGB backlight
void rgb_backlight(byte green, byte red)
{
  analogWrite(led_red,  red);  
  analogWrite(led_green, green);  
}
