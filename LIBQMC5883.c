// HMC5883 required Registers

#define W_DATA   0x1A     //'Used to perform a Write operation
#define R_DATA   0x1B     //'Used to perform a Read operation
#define CON_A    0xA0
#define CON_B    0x0B     //'Send continuous Measurement mode.
#define MOD_R    0x09     //'Read/Write Register, Selects the operating mode. Default = Single meVARurement

#define X_MSB    0x01     //'Read Register, Output of X MSB 8-bit value.
#define X_LSB    0x00     //'Read Register, Output of X LSB 8-bit value.
#define Z_MSB    0X05     //'Read Register, Output of Z MSB 8-bit value.
#define Z_LSB    0X04     //'Read Register, Output of Z LSB 8-bit value.
#define Y_MSB    0X03     //'Read Register, Output of Y MSB 8-bit value.
#define Y_LSB    0X02     //'Read Register, Output of Y LSB 8-bit value.

void hmc5883_write(int add, int data)
{
         i2c_start();
         i2c_write(W_DATA);
         i2c_write(add);
         i2c_write(data);
         i2c_stop();
 
}
     
int16 hmc5883_read(int add){
         int retval;
         i2c_start();
         i2c_write(W_DATA);
         i2c_write(add);
         i2c_start();
         i2c_write(R_DATA);
         retval=i2c_read(0);
         i2c_stop();
         return retval;
}
 
void hmc5883_init(){
         
         hmc5883_write(CON_B,  0x01);
         delay_ms(100);
         hmc5883_write(MOD_R,  0b10011001);
         delay_ms(100);
         hmc5883_write(CON_A,  0b10000001);
         delay_ms(100);
}
