#include <18F4550.h>
#fuses HS, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=8M)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Configuración I2C modo Master 
#include <SSD1306OLED.c>
#include <math.h>
#include <QMC5883.c>

char txt[30];
signed long x_value = 0;
signed long y_value = 0;
signed long z_value = 0;
float heading = 0.0;

void readCompass();

void main()
{ 
   delay_ms(500);
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
   SSD1306_ClearDisplay();
   SSD1306_Display();
   QMC5883_init();
   
   delay_ms(100);
   while(true)
   {
      QMC5883_read_axes(&x_value, &y_value, &z_value);
      heading = QMC5883_read_heading(y_value, x_value);
      
      SSD1306_ClearDisplay();
      sprintf(txt, "x=%Ld",x_value);
      SSD1306_DrawText(0, 0, txt,1);
      sprintf(txt, "y=%Ld",y_value);
      SSD1306_DrawText(0, 25, txt,1);
      sprintf(txt, "z=%Ld",z_value);
      SSD1306_DrawText(0, 50, txt,1);
      sprintf(txt, "H=%5.2f",heading);
      SSD1306_DrawText(60, 0, txt,1);
      SSD1306_Display();
      
      /*
      SSD1306_ClearDisplay();
      sprintf(txt, "ax=%Ld",ax);
      SSD1306_DrawText(0, 0, txt,1);
      sprintf(txt, "ay=%Ld",ay);
      SSD1306_DrawText(0, 25, txt,1);
      sprintf(txt, "az=%Ld",az);
      SSD1306_DrawText(0, 50, txt,1);
      sprintf(txt, "gx=%Ld",gx);
      SSD1306_DrawText(60, 0, txt,1);
      sprintf(txt, "gy=%Ld",gy);
      SSD1306_DrawText(60, 25, txt,1);
      sprintf(txt, "gz=%Ld",gz);
      SSD1306_DrawText(60, 50, txt,1);
      SSD1306_Display();   
      */
   }
}






