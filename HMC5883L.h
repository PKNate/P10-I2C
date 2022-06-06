#include <math.h> 
 
#define HMC5883L_READ_ADDR       0x3D
#define HMC5883L_WRITE_ADDR      0x3C 
                            
#define ConfiguratioRegisterA    0x00 
#define ConfiguratioRegisterB    0x01 
#define ModeRegister             0x02 
#define X_MSB_Reg                0x03 
#define X_LSB_Reg                0x04 
#define Z_MSB_Reg                0x05 
#define Z_LSB_Reg                0x06 
#define Y_MSB_Reg                0x07 
#define Y_LSB_Reg                0x08 
#define Status_Reg               0x09 
#define ID_Reg_A                 0x0A              
#define ID_Reg_B                 0x0B 
#define ID_Reg_C                 0x0C 
 
#define Measurement_Continuous   0x00
#define Measurement_SingleShot   0x01
#define Measurement_Idle         0x03
        
#define declination_angle      73.89 // Determinar segun el lugar donde vivas.
// Determinar la declinacion:  http: //magnetic-declination.com/
// Conversion de unidades:     http: //www.wolframalpha.com/
                                    
// Definimos los pines utilizados para la comunicacion (POR DEFECTO).
#define _SDA PIN_B0
#define _SCL PIN_B1
 
// Configuramos el puerto utilizado para la comunicacion I2C a 400Khz.
#use i2c(Master,Fast, SDA=_SDA, SCL=_SCL)   
                                              
// Variables auxiliares.                                    
register signed long X_axis = 0x00; 
register signed long Y_axis = 0x00;                                  
register signed long Z_axis = 0x00; 
 
// Factor de escala.
float m_scale = 1.0; 
 
// Escribe un registro del HMC5838L.
void HMC5883L_write(unsigned char reg_address, unsigned char value); 
// Lee un registro del HMC5838L.
unsigned char HMC5883L_read(unsigned char reg); 
// Escalamos los valores.
void HMC5883L_set_scale(float gauss); 
// Setea el modo de medicion.
void SetMeasurementMode(unsigned char mode);
// Inicializa el magnetometro HMC5883L.
void HMC5883L_init();  
// Lee los datos sin escalar.
void HMC5883L_read_data_raw(); 
// Escalamos los datos.
void HMC5883L_ReadScaledAxis(); 
// Conversion de los datos a grados.
float HMC5883L_headingDegree();
