/* i2c.h
*
* i2c functions using standard linux i2c driver module
*
* Created By: G. Howell
* Created On: 08/07/2020
*
*/

#ifndef __I2C_H__
#define __I2C_H__

/* open, close and initialisation */
void i2cOpen(uint8_t bus);
void i2cClose(void);
void i2cInit(uint8_t devAddr);

/* basic reading and writing */
void i2cWrite(uint8_t* data, uint32_t noBytes);
void i2cRead(uint8_t* data, uint32_t dataLen);

/* reading and writing to subaddress */
void i2cReadReg(uint8_t* regAddr, uint32_t regAddrLen, uint8_t* data, uint32_t dataLen);
void i2cWriteReg(uint8_t* regAddr, uint32_t regAddrLen, uint8_t* data, uint32_t dataLen);

#endif /* __I2C_H__