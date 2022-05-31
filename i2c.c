/* i2c.c
*
* i2c functions using standard linux i2c driver module
*
* Created By: G. Howell
* Created On: 08/07/2020
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "i2c.h"

// handle to i2c device
int hi2c;

/** 
 * @brief 	Opens the I2C bus
 * 
 * @param	bus		I2C bus
 *
 * @return	n/a
 */
void i2cOpen(uint8_t bus)
{	
	char buf[32];
	snprintf(buf, 32, "/dev/i2c-%d", bus);

 	if ((hi2c = open(buf, O_RDWR)) == -1) 
 	{
 		fprintf(stderr, "ERROR: failed to open i2c bus %d\n", bus);
		exit(EXIT_FAILURE);
	}
}

/** 
 * @brief 	Closes the I2C bus
 * 
 * @param	n/a
 *
 * @return	n/a
 */
void i2cClose(void)
{
	close(hi2c);
}

/** 
 * @brief 	Initalises the I2C device address
 * 
 * @param	n/a
 *
 * @return	n/a
 * 
 * \todo look at adding option for master mode
 */
void i2cInit(uint8_t devAddr)
{
	// sets the device address
	if (ioctl(hi2c, I2C_SLAVE, devAddr) == -1) {
		fprintf(stderr, "ERROR: failed to set the i2c address 0x%02x\n", devAddr);
		exit(EXIT_FAILURE);
	}
}

/** 
 * @brief 	Write data to the I2C device
 * 
 * @param	data	data bytes to write
 * @param	dataLen number of bytes to write
 *
 * @return	n/a
 */
void i2cWrite(uint8_t* data, uint32_t dataLen) 
{
	if (write(hi2c, data, dataLen) != dataLen) 
	{
		fprintf(stderr, "ERROR: i2c write failed\n");
		exit(EXIT_FAILURE);
	}
}

/** 
 * @brief 	Reads data from the I2C device
 * 
 * @param	data	read byte data
 * @param	dataLen length of read data in bytes
 *
 * @return	n/a
 */
void i2cRead(uint8_t* data, uint32_t dataLen) 
{
    if (read(hi2c, data, dataLen) != dataLen) 
    {
    	fprintf(stderr, "ERROR: Failed to read i2c data\n");
        exit(EXIT_FAILURE);
    }
}

/** 
 * @brief 	Reads data from from a specific register on the I2C device
 * 
 * @param	regAddr 	read byte data
 * @param	regAddrLen 	length of register address in bytes
 * @param	data 		read data
 * @param	dataLen 	length of data in bytes
 *
 * @return	n/a
 */
void i2cReadReg(uint8_t* regAddr, uint32_t regAddrLen, uint8_t* data, uint32_t dataLen)
{
	i2cWrite(regAddr, regAddrLen);
	i2cRead(data, noBytesRead);
}

/** 
 * @brief 	Reads data from from a specific register on the I2C device
 * 
 * @param	regAddr 	read byte data
 * @param	regAddrLen 	length of register address in bytes
 * @param	data 		read data
 * @param	dataLen 	length of data in bytes
 *
 * @return	n/a
 */
void i2cWriteReg(uint8_t* regAddr, uint32_t regAddrLen, uint8_t* data, uint32_t dataLen)
{
	uint8_t wrData[regAddrLen + dataLen] = {0};

	uint32_t ni;
	for (ni=0; ni<regAddrLen; ni++)
	{
		wrData[ni] = regAddr[ni];
	}
	for (ni=regAddrLen; ni<(regAddrLen+dataLen); ni++)
	{
		wrData[ni] = data[ni-regAddrLen];
	}

	i2cWrite(wrData, regAddrLen+dataLen);
}
