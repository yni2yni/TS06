//============================================================================
// Program Name: TS06 Touch IC Sample Code
// Copyright (C) 2013 AD Semiconductor Co.,LTD.
// Author: B.B Park (yni2yni@hanmail.net)
// Date :19, November. 2013
// MCU F/W Program Version: Sample C Code (Basic)
// File Name: 
// Target: Silabs C8051F327 28Pin / OSC= 24.0MHz (Int. OSC), 
//         MCS-51 Compatible MCU 
// Tool chain: KEIL C51 C Compiler  Version 8.12a
// H/W board: C8051F327 MCU BOARD Rev. 0 (w/ U4 MCU F/W)
// HOST Program Version:  Hyperterminal , 115200-8-N-1
// UI Compiler Tool: MS Visual Studio 2008 (PC S/W Tool)
// Project name: 
// Folder name: 
// Note:  S/W I2C Program ( GPIO Port)
//============================================================================
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <c8051f326.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
//IC I2C Address ------------------------------------------------------------
#define TS06_Addr 	0xD2  	// I2C Address, Reset Value: 0xD2(Read, Write=Read + 1)
// #define TS06_Addr 	0xF2  	// I2C Address, Reset Value: 0xF2(Read, Write=Read + 1)
                                // ID_SEL CS1/Pin4 -> GND


//MCU  GPIO Port Map.
sbit I2C_CLK	= P0^2;		//I2C Clock ,Open-drain, need pull-up R
sbit I2C_DATA	= P0^1;		//I2C Data , Open-drain, need pull-up R
sbit I2C_EN		= P0^3;		//I2C Enable, Output
sbit ERROR_LED 	= P0^6;		//Error LED, Push-pull

// Disp. LED (Example)
sbit LED_0		= P2^0;		//LED0
sbit LED_1		= P2^0;		//LED1
sbit LED_2		= P2^0;		//LED2
sbit LED_3		= P2^3;		//LED3
sbit LED_4		= P2^0;		//LED4
sbit LED_5		= P2^0;		//LED5
sbit LED_6		= P2^0;		//LED6
sbit LED_7		= P2^0;		//LED7

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
// IIC Function
void i2c_start(void);
void i2c_stop(void);
bit bit_wr(unsigned char temp);
void I2C_Read(unsigned char Chip_Addr, unsigned char Reg_Addr,unsigned char* Data, unsigned char Data_Size);
void I2C_Write(unsigned char Chip_Addr, unsigned char Reg_Addr,unsigned char* Data, unsigned char Data_Size);
void delay_(unsigned char del_t);
void Delay(unsigned int);

//Touch Sensor IC Control
void Initialize_TS06(void);
void READ_TS06(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
unsigned char reg_w_data[8];
unsigned char reg_r_data[22];
unsigned char NbData;
unsigned char i=0;
unsigned char ack;

void Delay(unsigned int j)
{
	unsigned int i,k;
	for(i=0; i<= j; i++)		// 24Mhz= 41.6 nsec
	{k=1;k=0;k=1;k=0;k=0;}	
	// 10 -> 2.083usec, 100 -> 20.8usec, 1000 = 208usec, 5000 = 0.2msec
}

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
 	Delay(5000);
   	Delay(5000);
	//Delay Time (Power on) 
	//100msec
	
 	Initialize_TS06();		
      	
  	//Delay Time 100msec
	Delay(5000);
   	Delay(5000);
	   	   
   	while (1)
   	{
  	Delay(5000);
   	Delay(5000);

	//printf(" Touch Key Output Data -> "); 
  	ERROR_LED = 0; //LED ON
  	Delay(5000);
   	Delay(5000);
	Delay(5000);
   	READ_TS06(); // READ OUTPUT DATA (Touch Sensor)
   	
	//printf(" %c" ,0x0D); // Hex Code : CR
	//printf(" %c" ,0x0A); // Hex Code : LF
	
 	ERROR_LED = 1; // LED OFF
	Delay(5000);
  	Delay(5000);
	Delay(5000);
  	Delay(5000);
  	Delay(5000);
  	Delay(5000);
   	Delay(5000);
   
 	}	 // end while(1)	
} //end main

//-----------------------------------------------------------------------------
// IIC Function (Software I2C Function)
//-----------------------------------------------------------------------------
void delay_(unsigned char del_t)
{
  while(del_t--);		// 24Mhz? = 41.66 nsec    //1000=>41.67usec
}

void i2c_start(void)
{
	I2C_DATA = 1;
	delay_(0);
	I2C_CLK	 = 1;
	delay_(0);	
	I2C_DATA = 0;
	delay_(0);
	I2C_CLK	 = 0;
	delay_(0);
}

void i2c_stop(void)
{
	I2C_CLK  = 0;
	delay_(0);
	I2C_DATA = 0;
	delay_(0);	
	I2C_CLK  = 1;
	delay_(0);
	I2C_DATA = 1;
	delay_(0);
}

bit bit_wr(unsigned char temp)
{
	bit ack; 

	//P0MDOUT =0x0B;		//Change Data Direction for Output ( MCU GPIO Direction Register)

	if(temp&0x80){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0;   //CLK가 High인 ,duration을 맞추기 위해 CLK=1을 4번 씀 
	if(temp&0x40){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0;   
	if(temp&0x20){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0;   
	if(temp&0x10){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0;   

	if(temp&0x08){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0; 
	if(temp&0x04){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0;   
	if(temp&0x02){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0;   
	if(temp&0x01){ I2C_DATA = 1;I2C_DATA = 1;} else {I2C_DATA = 0; I2C_DATA = 0;}
	I2C_CLK = 1;  I2C_CLK = 1;I2C_CLK = 1;  I2C_CLK = 1; I2C_CLK = 0;  

	//P0MDOUT =0x09;		      //Change Data Direction for Input

	I2C_DATA = 1; //delay_(3); 
	I2C_CLK = 1;   //  delay_(0); 

	if(I2C_DATA==1) ack = 0; //fail
	else ack = 1; //succeed 

 	I2C_CLK = 0;  
	
	//P0MDOUT =0x0B; 			//Change Data Direction for Output ( MCU GPIO Direction Register)

	I2C_DATA = 0; 

	return ack;  
}

void I2C_Write(unsigned char Chip_Addr, unsigned char Reg_Addr,unsigned char* Data, unsigned char Data_Size)
{
		unsigned char i =0;	
		i2c_start();
		bit_wr(Chip_Addr); 
		bit_wr(Reg_Addr);
		for(i=0;i<Data_Size;i++){
			bit_wr( *(Data+i) );
		}
		i2c_stop();
}

void I2C_Read(unsigned char Chip_Addr, unsigned char Reg_Addr,unsigned char* Data, unsigned char Data_Size)
{
	unsigned char i, i2c_read,k;

	i2c_start(); 
	bit_wr(Chip_Addr); 
	bit_wr(Reg_Addr);
	i2c_stop();    

	i2c_start();      
	bit_wr(Chip_Addr+1); 

	for(i=0;i<Data_Size;i++){

		//P0MDOUT =0x09;	//Data Port : Input

		I2C_DATA = 1; 
		i2c_read = 0; //Read data temp value

		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x80; else i2c_read &= 0x7F; I2C_CLK =0;  k=1; while(k--); 
		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x40; else i2c_read &= 0xBF; I2C_CLK =0;  k=1; while(k--);
		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x20; else i2c_read &= 0xDF; I2C_CLK =0;  k=1; while(k--);
		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x10; else i2c_read &= 0xEF; I2C_CLK =0;  k=1; while(k--);
		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x08; else i2c_read &= 0xF7; I2C_CLK =0;  k=1; while(k--);
		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x04; else i2c_read &= 0xFB; I2C_CLK =0;  k=1; while(k--);
		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x02; else i2c_read &= 0xFD; I2C_CLK =0;  k=1; while(k--);
		I2C_CLK =1; if(I2C_DATA==1) i2c_read |= 0x01; else i2c_read &= 0xFE; I2C_CLK =0;  k=1; while(k--);

		//P0MDOUT =0x0B;		//Data Port : Output

		if(i==(Data_Size-1))I2C_DATA = 1; else I2C_DATA = 0;
		I2C_CLK = 1; 
		k=1; while(k--);//delay
		I2C_CLK =0;
		*(Data+i) = i2c_read;
	}

	i2c_stop();

}
//-----------------------------------------------------------------------------
// Initialize_ TS06 IC 
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   : None
// This routine initializes the TS06 IC
//-----------------------------------------------------------------------------
void Initialize_TS06(void)
{
	unsigned char rw_data[10];
	
    I2C_EN = 0;  // I2C Enable
	delay_(10);  // I2C Enable Delay Time. about 10us //
	
	//  S/W Reset enable (Reset)
	rw_data[0] = 0x1A; //soft reset bit => bit 3(1: Reset, 0: Reset Clear)
	I2C_Write(TS06_Addr, 0x04,rw_data, 1);
	delay_(1000);
	
	//------ 사용자가 설정하고자 하는  Register Value를  삽입 하시오.-----------------
	
	rw_data[0] = 0x0A; // Register Value
	I2C_Write(TS06_Addr, 0x03,rw_data, 1); // (Chip ID, Register Address, Register Value, 1Byte)
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x05,rw_data, 1);
	delay_(1000);
	
	// Ch enable ( 0x00 : 6 ch enable)
	rw_data[0] = 0x00; // 1: disable, 0: Enable
	I2C_Write(TS06_Addr, 0x06,rw_data, 1);
	delay_(1000);
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x07,rw_data, 1);
	delay_(1000);
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x08,rw_data, 1);
	delay_(1000);

	rw_data[0] = 0xFD; // 
	I2C_Write(TS06_Addr, 0x09,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x0A,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x0B,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x0C,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x0D,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x0E,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x0F,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x10,rw_data, 1);
	delay_(1000);	
		
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x11,rw_data, 1);
	delay_(1000);	
		
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x12,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x13,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x83; // 
	I2C_Write(TS06_Addr, 0x14,rw_data, 1);
	delay_(1000);	
	
	rw_data[0] = 0x00; // 
	I2C_Write(TS06_Addr, 0x15,rw_data, 1);
	delay_(1000);	
	
	//-----------------------------------------------------------------------

	//-------------- Sensitivity --------------------
	// Sensitivity Ch 2,1
	rw_data[0] = 0x55; // (Max: 0x00, Min: 0xFF)
	I2C_Write(TS06_Addr, 0x00,rw_data, 1);
	delay_(1000);
	// Sensitivity Ch 4,3
	rw_data[0] = 0x55; // (Max: 0x00, Min: 0xFF)
	I2C_Write(TS06_Addr, 0x01,rw_data, 1);
	delay_(1000);
	// Sensitivity Ch 6,5
	rw_data[0] = 0x55; // (Max: 0x00, Min: 0xFF)
	I2C_Write(TS06_Addr, 0x02,rw_data, 1);
	delay_(1000);

	
	//  S/W Reset Disable (Reset Clear)
	rw_data[0] = 0x12; //soft reset bit => bit 3(1: Reset, 0: Not Reset)
	I2C_Write(TS06_Addr, 0x04,rw_data, 1);
	delay_(1000);
	
	I2C_EN = 1;  // I2C Disable
	delay_(100); 
		
}

// READ OUTPUT DATA Touch Sensor IC
void READ_TS06 (void)
{
	unsigned char rw_data[10];
	unsigned char LED_CONTROL;

	//init I/O
	LED_CONTROL = 0;
	
	
	I2C_EN = 0;  // I2C Enable
	delay_(10);  // I2C Enable Delay Time. about 10us //

//key output data , output data address: 0x25 (bit [5:0])
//Key On =bit  '1' Key Off = bit '0'
	I2C_Read(TS06_Addr, 0x25,rw_data, 1);

	delay_(5000);
	delay_(5000);
	delay_(5000);
	delay_(5000);
	delay_(5000);
	delay_(5000);
	
//contorl LED
	LED_CONTROL = rw_data[0] & 0x01; //Ch1

	if(LED_CONTROL)
		LED_0 = 0; //on
	else
		LED_0 = 1; //off
	LED_CONTROL = 0;

	LED_CONTROL = rw_data[0] & 0x02; //Ch2
	if(LED_CONTROL)
		LED_1 = 0; //on
	else
		LED_1 = 1; //off
	LED_CONTROL = 0;

	LED_CONTROL = rw_data[0] & 0x04; //Ch3
	if(LED_CONTROL)
		LED_2 = 0; //on
	else
		LED_2 = 1; //off
	LED_CONTROL = 0;

	LED_CONTROL = rw_data[0] & 0x08; //Ch4
	if(LED_CONTROL)
		LED_3 = 0; //on
	else
		LED_3 = 1; //off
	LED_CONTROL = 0;

	LED_CONTROL = rw_data[0] & 0x10; //Ch5
	if(LED_CONTROL)
		LED_4 = 0; //on
	else
		LED_4 = 1; //off
	LED_CONTROL = 0;

	LED_CONTROL = rw_data[0] & 0x20; //Ch6
	if(LED_CONTROL)
		LED_5 = 0; //on
	else
		LED_5 = 1; //off
	LED_CONTROL = 0;

	LED_CONTROL = rw_data[0] & 0x40; //Noise State
	if(LED_CONTROL)
		LED_6 = 0; //on
	else
		LED_6 = 1; //off
	LED_CONTROL = 0;
	
	I2C_EN = 1;  // I2C Disable
	delay_(100);  
	

}
// End Of File ---------------------------------------------------------------

