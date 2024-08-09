// ADS Touch Sensor Test Example Program (IC P/N:TS06, 16QFN)
// Code:
// Date: 2024.08.06  Ver.: 0.0.1
// H/W Target: ARDUINO UNO R3, S/W: Arduino IDE  2.3.2
// Author: Park, Byoungbae ( yni2yni@hanmail.net )
// Note: More information? Please ,send to e-mail.
// Uno R3, A4:SDA, A5: SCL, Leonardo 2:SDA,3:SCL, Uno R3, A4:SDA, A5: SCL,
// Register setting values are subject to change without prior notice to improve touch operation.

#include <Wire.h>

#define LF        0x0A //New Line
#define CR        0x0D //Carriage  return
#define SPC       0x20       //Space

#define Sensitivity1  0x00 //ch2,ch1
#define Sensitivity2  0x01 //ch4,ch3
#define Sensitivity3  0x02 //ch6,ch5

#define CTRL1     0x03  
#define CTRL2     0x04
#define Ref_rst   0x05
#define Ch_Hold   0x06 //Touch Key Channel Enable/Disable (Enable = 0x0, Disable=0x1)
#define Cal_Hold  0x07 //Calibration Enable/Disable (Enable = 0x0, Disable=0x1)
#define Dome_en   0x08 //Tact Key Input Enable/Disable (Enable = 0x0, Disable=0x1)

#define Cal_ctrl  0x09  //Calibration Speeed Control 

#define Pat_ctrl  0x0A  //Pattern Function Control 
#define Pat_A0    0x0B  //Pattern A Selection REgister
#define Pat_A1    0x0C  //Pattern A Selection REgister
#define Pat_A2    0x0D  //Pattern A Selection REgister
#define Pat_A3    0x0E  //Pattern A Selection REgister

#define Pat_B0    0x0F  //Pattern B Selection REgister
#define Pat_B1    0x10  //Pattern B Selection REgister
#define Pat_B2    0x11  //Pattern B Selection REgister
#define Pat_B3    0x12  //Pattern B Selection REgister

#define PWM0      0x13  //PWM Control REgister(CH4)
#define PWM1      0x14  //PWM Control REgister(CH5)
#define PWM2      0x15  //PWM Control REgister(CH6)

#define OUTPUT    0x25 //cs1~cs6 Key Output Data

// =============== TS06 I2C Chip ID ================================================
#define TS06_SENSING 0x69 //7bit address: 8bit address 0xD2>>1 //CS1 Pin = Sensing
#define TS06_GND     0x79 //7bit address: 8bit address 0xF2>>1 //CS1 Pin = GND

void  Init_TS06(void); //Initialize TS06 (16QFN)

#define RESET_PIN 7 //Reset pin
#define EN_PIN 6    //I2C_EN Pin

//-------------- Register Test ----------------------------------------------------
void Register_Dump()
{
   byte read_data[1] = {0};

   for (int i = 0; i < 256; i += 16)
   {
      for (int j = 0; j <= 15; j++)
      {
         Wire.beginTransmission(TS06_SENSING); // sned ic slave address
         Wire.write((i + j));                      // sends register address
         Wire.endTransmission();                   // stop transmitting
         Wire.requestFrom(TS06_SENSING, 1);    // data read (1 byte)
         read_data[0] = Wire.read();               //
         print2hex(read_data, 1);                  //
      }
      Serial.write(LF);
      Serial.write(CR);
   }
   delay(500);
}

void print2hex(byte *data, byte length) //Print Hex code
{
   Serial.print("0x");
   for (int i = 0; i < length; i++)
   {
      if (data[i] < 0x10)
      {
         Serial.print("0");
      }
      Serial.print(data[i], HEX);
      Serial.write(SPC);
   }
}

void setup(){
	
  delay(1); // wait for 1[msec]	
  Wire.begin();        // join i2c bus (address optional for master)
  Wire.setClock(200000); // I2C Speed 200[Khz]
  Serial.begin(115200);  // start serial for output (Speed)
  // put your setup code here, to run once:
 
  pinMode(EN_PIN, OUTPUT);
  //SDA, SCL = Hi-z (need to pull-up Resistor)
  delay(200); //Min: wait for 200[msec]
  digitalWrite(EN_PIN, LOW); // I2C_EN pin = low
  delay(1); //wait for 1[msec]
  Init_TS06(); //Initialize TS06
  digitalWrite(EN_PIN, HIGH); // I2C_EN pin = low
  delay(200);    //wait for 200[msec]
  
}

void loop() {

   byte read_data[3] = {0};
   
  // --- I2C_EN = Low, used for battery system ---------------------
  digitalWrite(EN_PIN, LOW); // I2C_EN pin = low
  delay(1); //wait for 1[msec]

 // Touch Key read
  Wire.beginTransmission(TS06_SENSING); // sned ic slave address
  Wire.write(byte(OUTPUT)); // sends register address
  Wire.endTransmission(); // stop transmitting
  Wire.requestFrom(TS06_SENSING,1); // read OUTPUT, 1Byte Read
  read_data[0]=Wire.read();		//OUTPUT Data
 
  // --- I2C_EN = High, used for battery system ---------------------
  digitalWrite(EN_PIN, HIGH); // I2C_EN pin = High
  delay(1); //wait for 1[msec]
  
   Serial.write(10);
   Serial.print(" Touch Sensor Output Data (Hex) ---- > "); // Test Code

   print2hex(read_data, 1); // UART Serial Data (HEX Value)
  
   //Serial.write(SP);
   //Serial.write(LF);
   //Serial.write(CR);
   //delay(5000);   
   //Register_Dump();
   delay(50);   
}

void  Init_TS06(void)
{
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(CTRL2)); // sends register address
   Wire.write(byte(0x1A)); // sends register data Software Reset Enable(Set)
   //Wire.write(byte(0x1E)); // sends register data Software Reset Enable(Set)
   Wire.endTransmission(); // stop transmitting

   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(Sensitivity1)); // sends register address
   Wire.write(byte(0x77)); // sends register data
   Wire.endTransmission(); // stop transmitting
   
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(Sensitivity2)); // sends register address
   Wire.write(byte(0x77)); // sends register data
   Wire.endTransmission(); // stop transmitting
  
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(Sensitivity3)); // sends register address
   Wire.write(byte(0x77)); // sends register data
   Wire.endTransmission(); // stop transmitting

   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(CTRL1)); // sends register address
   Wire.write(byte(0x0A)); // sends register data
   Wire.endTransmission(); // stop transmitting   
    
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(Ref_rst)); // sends register address
   Wire.write(byte(0x00)); // sends register data
   Wire.endTransmission(); // stop transmitting
   
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(Ch_Hold)); // sends register address
   Wire.write(byte(0x00)); // sends register data
   Wire.endTransmission(); // stop transmitting
 
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(Cal_Hold)); // sends register address
   Wire.write(byte(0x00)); // sends register data
   Wire.endTransmission(); // stop transmitting   
   
   //------------ Calibration speed Control Register --------------------------
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(Cal_ctrl)); // sends register address, 0x24h
   Wire.write(byte(0xFD));  //sends register data
   Wire.endTransmission();  //stop

  //================================================================================
   Wire.beginTransmission(TS06_SENSING); // sned ic slave address
   Wire.write(byte(CTRL2)); // sends register address
   Wire.write(byte(0x12)); // sends register data , Software Reset Disable (Clear)
   //LED Drive Disable, Multi Mode, Impedance High, S/W Reset Disable(Clear), Sleep Mode Disable
   
   //Wire.write(byte(0x16)); // sends register data , Software Reset Disable (Clear)
   //LED Drive Disable, Multi Mode, Impedance High, S/W Reset Disable(Clear), Sleep Mode Enable
   Wire.endTransmission(); // stop transmitting   

   }
// End