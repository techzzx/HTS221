/*
    A simple library for arduino to communicate with HTS221 on the X-NUCLEO-IKS01A1 board.

    Author:zhixinzhao
    Email:techzzx@foxmail.com
*/

#ifndef _HTS221_IKS01A1_H_
#define _HTS221_IKS01A1_H_

#include <Arduino.h>
#include <Wire.h>

#define HTS221_Address  0x5f    // 7-bit I2C slave address without R/W bit

/*
    Register mapping
*/                                          //  Default value
#define HTS221_WHO_AM_I         0x0F        //      BC
#define HTS221_AV_CONF          0x10        //      1B
#define HTS221_CTRL_REG1        0x20        //      0
#define HTS221_CTRL_REG2        0x21        //      0
#define HTS221_CTRL_REG3        0x22        //      0
#define HTS221_STATUS_REG       0x27        //      0
#define HTS221_HUMIDITY_OUT_L   0x28        //      Output
#define HTS221_HUMIDITY_OUT_H   0x29        //      Output
#define HTS221_TEMP_OUT_L       0x2A        //      Output
#define HTS221_TEMP_OUT_H       0x2B        //      Output

#define HTS221_H0_RH_X2         0x30
#define HTS221_H1_RH_X2         0x31
#define HTS221_T0_DEGC_X8       0x32
#define HTS221_T1_DEGC_X8       0x33
#define HTS221_T1_T0_MSB        0x35
#define HTS221_H0_T0_OUT_L      0x36
#define HTS221_H0_T0_OUT_H      0x37
#define HTS221_H1_T0_OUT_L      0x3A
#define HTS221_H1_T0_OUT_H      0x3B
#define HTS221_T0_OUT_L         0x3C
#define HTS221_T0_OUT_H         0x3D
#define HTS221_T1_OUT_L         0x3E
#define HTS221_T1_OUT_H         0x3F

/*
    Register default value
*/
#define HTS221_WHO_AM_I_DEFAULT 0xBC
#define HTS221_AV_CONF_DEFAULT  0x1B

/*
    Register specific setting
*/
    // AV_CONF:AVGT
#define HTS221_AVGT_2   0b000000
#define HTS221_AVGT_4   0b001000
#define HTS221_AVGT_8   0b010000
#define HTS221_AVGT_16  0b011000 // defalut
#define HTS221_AVGT_32  0b100000
#define HTS221_AVGT_64  0b101000
#define HTS221_AVGT_128 0b110000
#define HTS221_AVGT_256 0b111000

    // AV_CONF:AVGH
#define HTS221_AVGH_4   0b000
#define HTS221_AVGH_8   0b001
#define HTS221_AVGH_16  0b010
#define HTS221_AVGH_32  0b011 // defalut
#define HTS221_AVGH_64  0b100
#define HTS221_AVGH_128 0b101
#define HTS221_AVGH_256 0b110
#define HTS221_AVGH_512 0b111

    // CTRL_REG1
#define HTS221_PD           0b10000000  // Power Down control
#define HTS221_BDU          0b100       // Block Data Update control
#define HTS221_ODR_ONE      0b00        // Output Data Rate : One Shot
#define HTS221_ODR_1HZ      0b01        // Output Data Rate : 1Hz
#define HTS221_ODR_7HZ      0b10        // Output Data Rate : 7Hz
#define HTS221_ODR_12_5HZ   0b11        // Output Data Rate : 12.5Hz

    // CTRL_REG2
#define HTS221_BOOT     0b10000000      // Reboot memory content
#define HTS221_HEATER   0b10            // Heater
#define HTS221_ONE_SHOT 0b1             // One shot enable

    // CTRL_REG3
#define HTS221_CTRL_REG3_DEFAULT 0x00

    // STATUS_REG
#define HTS221_H_READY 0b10     // Humidity Data Available
#define HTS221_T_READY 0b01     // Temperature Data Available

class HTS221
{
public:
    HTS221(uint8_t i2cAddr = HTS221_Address);

    bool begin(void);
    bool deviceCheck(void);
    bool startup(void);
    bool setResolution(void);
    bool readCalibration(void);

    const double readTemperature(void);
    const double readHumidity(void);

private:
    uint8_t _i2cAddr;
    uint8_t _H0_rH_x2, _H1_rH_x2;
    uint16_t _T0_degC_x8, _T1_degC_x8;
    int16_t _H0_T0_OUT, _H1_T0_OUT;
    int16_t _T0_OUT, _T1_OUT;

    uint8_t i2cReadByte(uint8_t registerAddr);
    bool i2cWriteByte(uint8_t registerAddr, uint8_t data);
};

#endif  // _HTS221_IKS01A1_H_