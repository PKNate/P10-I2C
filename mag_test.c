#include <18F4550.h>
#fuses HS, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=8M)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Configuración I2C modo Master 
#include <SSD1306OLED.c>
#include <math.h>
#include <QMC5883.c>

char txt[30];

signed int16 x_min = -1070;
signed int16 x_max = 1392;
signed int16 y_min = -800;
signed int16 y_max = 1402;
signed int16 z_min = -1131;
signed int16 z_max = 2243;
signed int16 x_value;
signed int16 y_value;
signed int16 z_value;
float x_value_float;
float y_value_float;
float z_value_float;
float x_offset;
float y_offset;
float z_offset;
float x_avg_delta;
float y_avg_delta;
float z_avg_delta;
float avg_delta;
float x_scale;
float y_scale;
float z_scale;
float a;

void startCalibration();
void calibrationCompass();
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
      //startCalibration();
      readCompass();

      SSD1306_ClearDisplay();
      sprintf(txt, "A=%5.2f",a);
      SSD1306_DrawText(0, 0, txt,1);
      SSD1306_Display();
   }
}

void calibrationCompass()
{
   x_offset = (float)(x_min + x_max)/2;
   y_offset = (float)(y_min + y_max)/2;
   z_offset = (float)(z_min + z_max)/2;
   x_avg_delta = (float)(x_max - x_min)/2;
   y_avg_delta = (float)(y_max - y_min)/2;
   z_avg_delta = (float)(z_max - z_min)/2;

   avg_delta = (x_avg_delta + y_avg_delta + z_avg_delta) / 3;

   x_scale = avg_delta / x_avg_delta;
   y_scale = avg_delta / y_avg_delta;
   z_scale = avg_delta / z_avg_delta;

   x_value_float = (x_value - x_offset) * x_scale;
   y_value_float = (y_value - y_offset) * y_scale;
   z_value_float = (z_value - z_offset) * z_scale;
   
   x_value = (signed int16) x_value_float;
   y_value = (signed int16) y_value_float;
   z_value = (signed int16) z_value_float;
}

void readCompass()
{
   QMC5883_read_axes(&x_value, &y_value, &z_value);
   calibrationCompass();
   a = atan2(y_value,x_value) * 180.0 / PI;
   a-=15;
   if(a<0)
   a+=360;
}

void startCalibration()
{
   for(int i=0; i<10; i++)
   {
      QMC5883_read_axes(&x_value, &y_value, &z_value);
      if(x_value>x_max)
      x_max=x_value;
      if(y_value>y_max)
      y_max=y_value;
      if(z_value>z_max)
      z_max=z_value;
      if(x_value<x_min)
      x_min=x_value;
      if(y_value<y_min)
      y_min=y_value;
      if(z_value<z_min)
      z_min=z_value;
   }
   
   SSD1306_ClearDisplay();
   sprintf(txt, "xmin=%Ld",x_min);
   SSD1306_DrawText(0, 0, txt,1);
   sprintf(txt, "ymin=%Ld",y_min);
   SSD1306_DrawText(0, 25, txt,1);
   sprintf(txt, "zmin=%Ld",z_min);
   SSD1306_DrawText(0, 50, txt,1);
   sprintf(txt, "xmax=%Ld",x_max);
   SSD1306_DrawText(60, 0, txt,1);
   sprintf(txt, "ymax=%Ld",y_max);
   SSD1306_DrawText(60, 25, txt,1);
   sprintf(txt, "zmax=%Ld",z_max);
   SSD1306_DrawText(60, 50, txt,1);
   SSD1306_Display();
}




