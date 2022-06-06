#include <HMC5883L.h>
// Desarrollo de las funciones de usuario.
 
void HMC5883L_write(unsigned char reg_address, unsigned char value){
 
   i2c_start();                     // Inicializa comunicación I2C, condición de start.
   i2c_write(HMC5883L_WRITE_ADDR);  // Envíamos la dirección del dispositivo + comando para escritura.
   i2c_write(reg_address);          // Indicamos que registro será escrito.
   i2c_write(value);                // Enviamos el nuevo valor al registro.
   I2c_Stop();                      // Condición de stop.
 
}
 
unsigned char HMC5883L_read(unsigned char reg){
 
unsigned char val = 0x00;           // Variable auxiliar. 
    
   i2c_start();                     // Inicializa comunicación I2C, condición de start.
   i2c_write(HMC5883L_WRITE_ADDR);  // Envíamos la dirección del dispositivo + comando para escritura.
   i2c_write(reg);                  // Indicamos que registro será leído.
   i2c_start();                     // Reiniciamos la comunicación.
   i2c_write(HMC5883L_READ_ADDR);   // Envíamos la dirección del dispositivo + comando para lectura.
   val = i2c_read(0);               // Leemos el registro.         
   i2c_stop();                      // Condición de stop.
   return(val);                     // Retornamos con el valor leído.
}
 
void HMC5883L_set_scale(float gauss)                      
{ 
   unsigned char value = 0x05;    
    
    if(gauss == 0.88){value = 0x00; m_scale = 0.73;}  
    else if(gauss == 1.3){value = 0x01; m_scale = 0.92;}  
    else if(gauss == 1.9){value = 0x02; m_scale = 1.22;}  
    else if(gauss == 2.5){value = 0x03; m_scale = 1.52;}   
    else if(gauss == 4.0){value = 0x04; m_scale = 2.27;} 
    else if(gauss == 4.7){value = 0x05; m_scale = 2.56;}  
    else if(gauss == 5.6){value = 0x06; m_scale = 3.03;}    
    else if(gauss == 8.1){value = 0x07; m_scale = 4.35;}        
                                                
   value <<= 5; 
   HMC5883L_write(ConfiguratioRegisterB, value); 
} 
 
void SetMeasurementMode(unsigned char mode){
 
HMC5883L_write(ModeRegister,mode);
 
}
 
void HMC5883L_init(){
 
   HMC5883L_write(ConfiguratioRegisterA, 0x70); 
   HMC5883L_write(ConfiguratioRegisterB, 0xA0); 
   SetMeasurementMode(Measurement_Continuous);
   HMC5883L_set_scale(1.3);
}
 
void HMC5883L_read_data_raw(){
 
unsigned char lsb = 0; 
unsigned char msb = 0; 
    
   i2c_start();                     // Inicializa comunicación I2C, condición de start.
   i2c_Write(HMC5883L_WRITE_ADDR);  // Envíamos la dirección del dispositivo + comando para escritura. 
   i2c_Write(X_MSB_Reg);            // Accedemos a la parte alta del registro que contiene los datos en el eje X.
   i2c_Start();                     // Reiniciamos la comunicación.
   i2c_Write(HMC5883L_READ_ADDR);   // Envíamos la dirección del dispositivo + comando para lectura.
    
   msb = i2c_Read();                // Lee MSB del registro que contiene los datos en el eje X.
   lsb = i2c_Read();                // Lee LSB del registro que contiene los datos en el eje X.
   X_axis = make16(msb,lsb);        // Guarda los datos en una variable auxiliar.
                           
   msb = I2C_Read();                // Lee MSB del registro que contiene los datos en el eje Z.
   lsb = I2C_Read();                // Lee LSB del registro que contiene los datos en el eje Z.
   Z_axis = make16(msb,lsb);        // Guarda los datos en una variable auxiliar.
                   
   msb = I2C_Read();                // Lee MSB del registro que contiene los datos en el eje Y.
   lsb = I2C_Read(0);               // Lee LSB del registro que contiene los datos en el eje Y.
   Y_axis = make16(msb,lsb);        // Guarda los datos en una variable auxiliar.    
                       
   i2c_stop();                      // Condición de stop.
}
 
void HMC5883L_ReadScaledAxis(){
 
   X_axis *= m_scale; 
   Z_axis *= m_scale;                            
   Y_Axis *= m_scale;
}
 
float HMC5883L_headingDegree(){
 
register float heading = 0.00; 
    
   HMC5883L_read_data_raw(); 
   HMC5883L_ReadScaledAxis(); 
   heading = atan2(Y_axis, X_axis); 
   float _declinationAngle = declination_angle/1000.0;
    heading += _declinationAngle;    
                  
    if(heading < 0.0) 
    { 
      heading += (2.0 * PI); 
    } 
    
    if(heading > (2.0 * PI))                
    {                            
      heading -= (2.0 * PI); 
    }                    
                    
   heading *= (180.0 / PI); 
                   
   return heading; 
}
