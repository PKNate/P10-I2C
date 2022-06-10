#include <18F4550.h>
#fuses HS, PLL2, CPUDIV1, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device PASS_STRINGS = IN_RAM
#use delay(clock=8M)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Configuración I2C modo Master 
#include <SSD1306OLED.c>
#include <mpu6050.h>
#include <math.h>
#include <functionmpu6050.c>

void readCompass();

void main()
{ 
   delay_ms(500);
   SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
   hmc5883_init();
   delay_ms(100);
   while(true)
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
      
      ang_x = 0.98*(ang_x_prev+(gx/131)*dt) + 0.02*accel_ang_x;
      ang_y = 0.98*(ang_y_prev+(gy/131)*dt) + 0.02*accel_ang_y;
      ang_x_prev=ang_x;
      ang_y_prev=ang_y;
      
      
      SSD1306_ClearDisplay();
      sprintf(txt, "x=%5.2f",ang_x);
      SSD1306_DrawText(0, 0, txt,1);
      sprintf(txt, "y=%5.2f",ang_y);
      SSD1306_DrawText(0, 25, txt,1);
      sprintf(txt, "t=%.6f",dt);
      SSD1306_DrawText(0, 50, txt,1);
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






