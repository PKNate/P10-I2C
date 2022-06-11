#include <18F4550.h>
#fuses HS, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=8M)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)
#include <SSD1306OLED.c>

#define x_axis 0
#define y_axis 1

void drawInterface();
void drawCompass();
void drawTilt(short Axis);

void main()
{ 
   delay_ms(500);
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

   while(true)
   {
      SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r)
   }
}

void drawInterface()
{
   SSD1306_ClearDisplay();
   
   SSD1306_Display();


}


