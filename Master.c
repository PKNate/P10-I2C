#include <18F4550.h>
#fuses INTRC, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#use delay(clock=8M)

#use i2c(Master,fast,sda=PIN_B0,scl=PIN_B1, stream=SSD1306_STREAM)  // Configuración I2C modo Master 
#include <SSD1306OLED.c>


void main()
{
   delay_ms(1000);
   SSD1306_Begin();
   delay_ms(1000);
   while(true)
   {
     SSD1306_ClearDisplay();
     SSD1306_DrawPixel(10, 10);
     SSD1306_Display();
     delay_ms(2000);
     
   }
}



