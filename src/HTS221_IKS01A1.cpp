/*
	A simple library for arduino to communicate with HTS221 on the X-NUCLEO-IKS01A1 board.

	Author:zhixinzhao
	Email:techzzx@foxmail.com
*/

#include "HTS221_IKS01A1.h"

/*
	Constructor
*/
HTS221::HTS221(uint8_t i2cAddr)
{
	_i2cAddr = i2cAddr;
}

/*
	HTS221 sensor init
*/
bool HTS221::begin()
{
	if(deviceCheck())
	{
		if(startup())
		{
			if(setResolution())
			{
				return(readCalibration());
			}
		}
	}
	return false;	//if there has any error in function calling , return false
}

/*
	HTS221 sensor check type
*/
bool HTS221::deviceCheck()
{
	return((HTS221_WHO_AM_I_DEFAULT == i2cReadByte(HTS221_WHO_AM_I)));
}

/*
	HTS221 sensor powerup
*/
bool HTS221::startup()
{
	uint8_t data = 0;
	data |= HTS221_PD;
	data |= HTS221_BDU;
	data |= HTS221_ODR_1HZ;

	return(i2cWriteByte(HTS221_CTRL_REG1, data));
}

/*
	HTS221 sensor set resolution
*/
bool HTS221::setResolution()
{
	uint8_t data = 0;
	data |= HTS221_AVGT_16;
	data |= HTS221_AVGH_32;

	return(i2cWriteByte(HTS221_AV_CONF, data));
}

/*
	Read the coefficients of factory calibration
*/
bool HTS221::readCalibration()
{
	uint8_t buff = 0;
	_H0_rH_x2 = i2cReadByte(HTS221_H0_RH_X2);
	_H1_rH_x2 = i2cReadByte(HTS221_H1_RH_X2);
	
	buff = i2cReadByte(HTS221_T1_T0_MSB);
	_T0_degC_x8  = i2cReadByte(HTS221_T0_DEGC_X8);
	_T0_degC_x8 |= (buff & 0x3) << 8;
	_T1_degC_x8  = i2cReadByte(HTS221_T1_DEGC_X8);
	_T1_degC_x8 |= (buff & 0xc) << 6;

	_H0_T0_OUT  = i2cReadByte(HTS221_H0_T0_OUT_L);
	_H0_T0_OUT |= i2cReadByte(HTS221_H0_T0_OUT_H) << 8;
	_H1_T0_OUT  = i2cReadByte(HTS221_H1_T0_OUT_L);
	_H1_T0_OUT |= i2cReadByte(HTS221_H1_T0_OUT_H) << 8;

	_T0_OUT  = i2cReadByte(HTS221_T0_OUT_L);
	_T0_OUT |= i2cReadByte(HTS221_T0_OUT_H) << 8;
	_T1_OUT  = i2cReadByte(HTS221_T1_OUT_L);
	_T1_OUT |= i2cReadByte(HTS221_T1_OUT_H) << 8;
	
	return true;
}

/*
	Return the value of actual humidity , unit : RH %
*/
const double HTS221::readHumidity()
{
	double humidity = 0.0;
	uint8_t dataReady;
	int16_t H_T_OUT = 0;	//it must be int16_t type, not uint16_t
	double H0_rH, H1_rH;

	H0_rH = _H0_rH_x2 / 2.0;//devided by 2 to get real value
	H1_rH = _H1_rH_x2 / 2.0;
	
	dataReady = i2cReadByte(HTS221_STATUS_REG);

	while(!(dataReady & HTS221_H_READY))	//loop until the humidity data is ready
		dataReady = i2cReadByte(HTS221_STATUS_REG);
		
	H_T_OUT  = i2cReadByte(HTS221_HUMIDITY_OUT_L);
	H_T_OUT |= i2cReadByte(HTS221_HUMIDITY_OUT_H) << 8;
	
	humidity = H0_rH + (H1_rH - H0_rH) * (H_T_OUT - _H0_T0_OUT) / (_H1_T0_OUT - _H0_T0_OUT);

	return (humidity < 100.0 ? humidity : 100.0); //if the value of humidity is greater than 100% , set it to 100%
}

/*
	Return the value of actual temperature , unit : °C
*/
const double HTS221::readTemperature()
{
	double temperature = 0.0;
	uint8_t dataReady;
	int16_t T_OUT = 0;
	double T0_degC, T1_degC;

	T0_degC = _T0_degC_x8 / 8.0;	//devided by 8 to get real value
	T1_degC = _T1_degC_x8 / 8.0;
	
	dataReady = i2cReadByte(HTS221_STATUS_REG);

	while(!(dataReady & HTS221_T_READY))	//loop until the temperature data is ready
		dataReady = i2cReadByte(HTS221_STATUS_REG);
		
	T_OUT  = i2cReadByte(HTS221_TEMP_OUT_L);
	T_OUT |= i2cReadByte(HTS221_TEMP_OUT_H) << 8;
	
	temperature = T0_degC + (T1_degC - T0_degC) * (T_OUT - _T0_OUT) / (_T1_OUT - _T0_OUT);

	return temperature;
}


/*---------------------------------------------------*/

/*
	Read a single byte from specific register in HTS221
*/
uint8_t HTS221::i2cReadByte(uint8_t registerAddr) 
{
	Wire.beginTransmission(_i2cAddr);
	Wire.write(registerAddr);
	Wire.endTransmission(false);

	Wire.requestFrom(_i2cAddr, (uint8_t)1);

	while(!Wire.available()) ;	//Wait for data, Warning: the program might hung in there
	return Wire.read();	//Return one read byte
}

/*
	Write a single byte to specific register in HTS221
*/
bool HTS221::i2cWriteByte(uint8_t registerAddr, uint8_t data) 
{
	Wire.beginTransmission(_i2cAddr);

	while(!Wire.write(registerAddr))
		return false;

	while(!Wire.write(data))
		return false;

	return(0 == Wire.endTransmission(false));
}