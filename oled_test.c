#include <18F4550.h>
#fuses HSPLL, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=48M)
#use I2C(MASTER, I2C1, FAST, stream = SSD1306_STREAM)
#include <SSD1306OLED.c>

#define x_axis 0
#define y_axis 1

char txt[30];
void drawInterface();
void drawCompass(int16 Azimuth);
void drawTilt(short Axis);

void main()
{ 
   delay_ms(500);
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
   drawInterface();
   
   while(true)
   {
      
      for(int16 i=0; i<360; i++)
      {
         drawCompass(i);
      }
      
   }
   SSD1306_Display();
}

void drawCompass(int16 Azimuth)
{
   int j=20, k=0;
   int drawCompass_offset;
   signed int16 Azimuth_helper;

   //SSD1306_ClearDisplay();
   drawCompass_offset= (Azimuth%5)*2;
   SSD1306_FillRect(0, 0, 128, 16, 0);
   
   for(int i=0; i<128; i+=2)
   {
      Azimuth_helper=Azimuth-32+i/2;
      if(Azimuth_helper<0)
      Azimuth_helper+=360;
      else if(Azimuth_helper>360)
      Azimuth_helper-=360;
      
      if((Azimuth_helper%5)==0)
      {
         if(((Azimuth_helper)%30)==0)
         {
            SSD1306_DrawFastVLine(i-1, 0, 8);
            if(((Azimuth_helper)%90)==0)
            {
               if(i>3 && i<124)
               switch(Azimuth_helper)
               {
                  case 0: SSD1306_DrawChar(i-3, 9, 'N', 1); break;
                  case 90: SSD1306_DrawChar(i-3, 9, 'E', 1); break;
                  case 180:SSD1306_DrawChar(i-3, 9, 'S', 1); break;
                  case 270:SSD1306_DrawChar(i-3, 9, 'W', 1); break;
                  case 360: SSD1306_DrawChar(i-3, 9, 'N', 1); break;
               }
            }
         }
         else 
         SSD1306_DrawFastVLine(i-1, 0, 4);
         /* Debug
         sprintf(txt, "%Ld",Azimuth_helper);
         SSD1306_DrawText(k, j,txt,1);
         j+=10;
         
         if(j>50)
         {
            j=20;
            k+=30;
         }
         */
      }
   }
   /*Debug
   sprintf(txt, "A=%Ld",Azimuth);
   SSD1306_DrawText(90, 50,txt,1);
   SSD1306_DrawFastVLine(63, 10, 4); 
   */
   SSD1306_Display();
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
   SSD1306_FillCircle(63, 31, 2, 1);
   SSD1306_FillRect(0, 0, 127, 16, 0);
   //SSD1306_FillRect(0, 15, 127, 2, 1);
   //SSD1306_DrawRect(0, 0, 127, 16);
   SSD1306_DrawLine(34, 46, 27, 49, 1);
   SSD1306_DrawLine(34, 47, 27, 50, 1);
   SSD1306_DrawLine(100, 49, 93, 46, 1);
   SSD1306_DrawLine(100, 50, 93, 47, 1);
   SSD1306_DrawLine(30, 31, 23, 31, 1);
   SSD1306_DrawLine(30, 32, 23, 32, 1);
   SSD1306_DrawLine(97, 31, 103, 31, 1);
   SSD1306_DrawLine(97, 32, 103, 32, 1);
   SSD1306_DrawText(30, 54, "L",1);
   SSD1306_DrawText(93, 54, "R",1);
   SSD1306_DrawText(0, 54, "x",1);
   SSD1306_Display();
}


