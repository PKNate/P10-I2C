#include <18F4550.h>
#fuses HS, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=8M)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Configuración I2C modo Master 
#include <SSD1306OLED.c>
#include <mpu6050.h>
#include <math.h>
#include <mpu6050.c>

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
float ang_x, ang_y, accel_ang_x, accel_ang_y;
float ang_x_prev=0, ang_y_prev=0;
char txt[30];

void readTilt();

void main()
{ 
   delay_ms(500);
   setup_timer_0(T0_INTERNAL|T0_DIV_64);
   set_timer0((int16)0);
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
   InitMpu6050();
   
   while(true)
   {
      readTilt();      
      SSD1306_ClearDisplay();
      sprintf(txt, "x=%5.2f",ang_x);
      SSD1306_DrawText(0, 0, txt,1);
      sprintf(txt, "y=%5.2f",ang_y);
      SSD1306_DrawText(0, 25, txt,1);
      sprintf(txt, "t=%.6f",dt);
      SSD1306_DrawText(0, 50, txt,1);
      SSD1306_Display();
   }
}

void readTilt()
{
   timer=get_timer0(); 
   dt=timer/31250;
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
   ang_x_prev=ang_x;
   ang_y_prev=ang_y;
}







