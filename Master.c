#include <18F4550.h>
#fuses HSPLL, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=48M)
#use I2C(MASTER, I2C1, FAST, stream = SSD1306_STREAM)
#include <SSD1306OLED.c>
#include <math.h>
#include <mpu6050.h>
#include <mpu6050.c>
#include <QMC5883.c>

//oled_test variables
float coord_x_prev=0, coord_y_prev=0, vert_x_prev=0, vert_y_prev=0, height_prev=0;

//gyro_test variables
signed int16 ax_offset= -831;
signed int16 ay_offset= -193;
signed int16 az_offset= 881;
signed int16 gx_offset= 730;
signed int16 gy_offset= -122;
signed int16 gz_offset= 47;
signed int16 ax,ay,az;
signed int16 gx,gy,gz;
float timer;
float dt=1;
float ang_x, ang_y, ang_x_full, ang_y_full, accel_ang_x, accel_ang_y;
float height;
float az_m_s2;
float ang_x_prev=0, ang_y_prev=0;
char txt[30];

//mag_test variables
signed int16 x_min = -953;
signed int16 x_max = 1583;
signed int16 y_min = -456;
signed int16 y_max = 1861;
signed int16 z_min = -700;
signed int16 z_max = 2495;
signed int8 a_offset = 90;
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

//oled_test functions
void drawInterface();
void drawCompass(int16 Azimuth);
void drawGyro(int16 Angle, signed int16 height);

//gyro_test functions
void readTilt();
void tiltFullRange();

//mag_test functions
void startCalibration();
void calibrationCompass();
void readCompass();

void main()
{ 
   delay_ms(500);
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
   drawInterface();
   SSD1306_Display();
   
   while(true)
   {
      readTilt(); 
      tiltFullRange();
      readCompass();
      delay_ms(100);
      
      drawGyro((int16)0,(signed int16)0);
      drawCompass((int16)a);
      SSD1306_Display();
      delay_ms(100);
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

void readTilt()
{
   timer=get_timer0(); 
   dt=timer/46875;
   set_timer0((int16)0);
   
   ax=GetdataMpu6050(MPU6050_RA_ACCEL_XOUT_H)+ax_offset;
   ay=GetdataMpu6050(MPU6050_RA_ACCEL_YOUT_H)+ay_offset;
   az=GetdataMpu6050(MPU6050_RA_ACCEL_ZOUT_H)+az_offset;
   gx=GetdataMpu6050(MPU6050_RA_GYRO_XOUT_H)+gx_offset;
   gy=GetdataMpu6050(MPU6050_RA_GYRO_YOUT_H)+gy_offset;
   gz=GetdataMpu6050(MPU6050_RA_GYRO_ZOUT_H)+gz_offset;
   
   accel_ang_x=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
   accel_ang_y=atan(-ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
   
   ang_x = 0.8*(ang_x_prev+(gx/131)*dt) + 0.2*accel_ang_x;
   ang_y = 0.8*(ang_y_prev+(gy/131)*dt) + 0.2*accel_ang_y;
   
   az_m_s2 = az * (9.81/16384.0);
   height = 10*(9.81-az_m_s2);
   
   ang_x_prev=ang_x;
   ang_y_prev=ang_y;
}

void tiltFullRange()
{
   ang_x_full=(-1)*ang_x;
   ang_y_full=(-1)*ang_y;
   
   if(az<0)
   {
      ang_x_full=ang_x+180;
      ang_y_full=ang_y+180;
   }
   
   else
   {
      if(ang_x_full<0)
      ang_x_full+=360;
      
      if(ang_y_full<0)
      ang_y_full+=360;
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
   a+=a_offset;
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


