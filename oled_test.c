#include <18F4550.h>
#fuses HSPLL, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=48M)
#use I2C(MASTER, I2C1, FAST, stream = SSD1306_STREAM)
#include <SSD1306OLED.c>
#include <math.h>
#define x_axis 0
#define y_axis 1

float coord_x_prev=0, coord_y_prev=0, vert_x_prev=0, vert_y_prev=0, height_prev=0;

void drawInterface();
void drawCompass(int16 Azimuth);
void drawGyro(int16 Angle, signed int16 height);

void main()
{ 
   signed int j=-10;
   delay_ms(500);
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
   drawInterface();
   SSD1306_Display();
   
   while(true)
   {
      for(signed int16 i=0; i<360; i++)
      {
         drawGyro(i,j);
         drawCompass(i);
         SSD1306_Display();
         j++;
         if(j>10)
         j=-10;
      }
   }
   
}

void drawInterface()
{
   SSD1306_ClearDisplay();
   SSD1306_Display();
   SSD1306_DrawCircle(63, 31, 31);
   SSD1306_DrawCircle(63, 31, 32);
   SSD1306_DrawCircle(63, 31, 33);
   SSD1306_DrawCircle(63, 31, 50);
   SSD1306_DrawCircle(63, 31, 51);
   SSD1306_DrawCircle(63, 31, 54);
   SSD1306_FillRect(0, 0, 127, 16, 0);
   SSD1306_DrawLine(34, 46, 27, 49, 1);
   SSD1306_DrawLine(34, 47, 27, 50, 1);
   SSD1306_DrawLine(100, 49, 93, 46, 1);
   SSD1306_DrawLine(100, 50, 93, 47, 1);
   SSD1306_DrawLine(30, 31, 23, 31, 1);
   SSD1306_DrawLine(30, 32, 23, 32, 1);
   SSD1306_DrawLine(97, 31, 103, 31, 1);
   SSD1306_DrawLine(97, 32, 103, 32, 1);
   SSD1306_DrawChar(30, 54, 'L',1);
   SSD1306_DrawChar(93, 54, 'R',1);
   SSD1306_DrawChar(0, 54, 'x',1);
}

void drawCompass(int16 Azimuth)
{
   int drawCompass_offset;
   signed int16 Azimuth_helper;

   drawCompass_offset= (Azimuth%5)*2;
   SSD1306_FillRect(0, 0, 128, 16, 0);
   
   for(int i=0; i<128; i+=2)
   {
      Azimuth_helper=Azimuth-32+i/2;
      if(Azimuth_helper<0)
      Azimuth_helper+=360;
      else if(Azimuth_helper>=360)
      Azimuth_helper-=360;
      
      if((Azimuth_helper%5)==0)
      {
         if((Azimuth_helper%15)==0)
         {
            SSD1306_DrawFastVLine(i-1, 0, 7);
            
            if(i>6 && i<121)
            if((Azimuth_helper%45)==0)
            switch(Azimuth_helper)
            {
               case 0: SSD1306_DrawChar(i-3, 9, 'N', 1); break;
               case 45: SSD1306_DrawText(i-6, 9, "NE", 1); break;
               case 90: SSD1306_DrawChar(i-3, 9, 'E', 1); break;
               case 135: SSD1306_DrawText(i-6, 9, "SE", 1); break;
               case 180:SSD1306_DrawChar(i-3, 9, 'S', 1); break;
               case 225: SSD1306_DrawText(i-6, 9, "SW", 1); break;
               case 270:SSD1306_DrawChar(i-3, 9, 'W', 1); break;
               case 315: SSD1306_DrawText(i-6, 9, "NW", 1); break;
            }
         }
         else 
         SSD1306_DrawFastVLine(i-1, 0, 4);
      }
   }
}

void drawGyro(int16 Angle, signed int16 height)
{
  float coord_x,coord_y,vert_x,vert_y;
  float rad=Angle*0.017453;
  
  if(height>10) height=10;
  else if (height<-10) height=-10;
  
  coord_x= 25*cos(rad);
  coord_y= 25*sin(rad);
  vert_x=7*cos(rad+1.57);
  vert_y=7*sin(rad+1.57);
  
  SSD1306_DrawLine(63, 31, (int16) 63+coord_x_prev, (int16) 31+coord_y_prev, 0);
  SSD1306_DrawLine(63, 31, (int16) 63-coord_x_prev, (int16) 31-coord_y_prev, 0);
  SSD1306_DrawLine(63, 31, (int16) 63-vert_x_prev, (int16) 31-vert_y_prev, 0);
  SSD1306_DrawLine(55, 31-height_prev, 71, 31-height_prev, 0);
  SSD1306_DrawLine(55, 30-height_prev, 71, 30-height_prev, 0);
  SSD1306_DrawLine(63, 31, (int16) 63+coord_x, (int16) 31+coord_y, 1);
  SSD1306_DrawLine(63, 31, (int16) 63-coord_x, (int16) 31-coord_y, 1);
  SSD1306_DrawLine(63, 31, (int16) 63-vert_x, (int16) 31-vert_y, 1);  
  SSD1306_DrawLine(55, 31-height, 71, 31-height, 1);
  SSD1306_DrawLine(55, 30-height, 71, 30-height, 1);
  
  coord_x_prev=coord_x;
  coord_y_prev=coord_y;
  vert_x_prev=vert_x;
  vert_y_prev=vert_y;
  height_prev=height;
}


