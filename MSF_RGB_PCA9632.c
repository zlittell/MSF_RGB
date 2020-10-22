/**
* @file MSF_RGB_PCA9632.c
* @brief NXP PCA9632 RGB Driver
* @author Zack Littell
* @company Mechanical Squid Factory
* @project MSF_RGB
*/

#include <stdint.h>
#include "MSF_I2C/MSF_I2C.h"

#define PCARESETADDR 0x03
#define PCA_ResetCMD_Length 2
const uint8_t PCA_ResetCMD[PCA_ResetCMD_Length] = {0xA5, 0x5A};
const uint8_t PCA_AUTOINC_NONE = (0u << 5);
const uint8_t PCA_AUTOINC_ALL = (4u << 5);
const uint8_t PCA_AUTOINC_BRIGHTNESS = (5u << 5);
const uint8_t PCA_AUTOINC_GLOBAL = (6u << 5);
const uint8_t PCA_AUTOINC_INDIVIDUALANDGLOBAL = (7u << 5);
const uint8_t PCA_REG_MODE1 = (0u);
const uint8_t PCA_REG_MODE2 = (1u);
const uint8_t PCA_REG_PWM0 = (2u);
const uint8_t PCA_REG_PWM1 = (3u);
const uint8_t PCA_REG_PWM2 = (4u);
const uint8_t PCA_REG_PWM3 = (5u);
const uint8_t PCA_REG_GRPPWM = (6u);
const uint8_t PCA_REG_GRPFREQ = (7u);
const uint8_t PCA_REG_LEDOUT = (8u);
const uint8_t PCA_REG_SUBADR1 = (9u);
const uint8_t PCA_REG_SUBADR2 = (10u);
const uint8_t PCA_REG_SUBADR3 = (11u);
const uint8_t PCA_REG_ALLCALLADR = (12u);
const uint8_t PCA_MODE1_SLEEP = (1u << 4);
const uint8_t PCA_MODE1_SUB1 = (1u << 3);
const uint8_t PCA_MODE1_SUB2 = (1u << 2);
const uint8_t PCA_MODE1_SUB3 = (1u << 1);
const uint8_t PCA_MODE1_ALLCALL = (1u << 0);
const uint8_t PCA_MODE2_DMBLNK = (1u << 5);
const uint8_t PCA_MODE2_INVRT = (1u << 4);
const uint8_t PCA_MODE2_OCH = (1u << 3);
const uint8_t PCA_MODE2_OUTDRV = (1u << 2);
const uint8_t PCA_MODE2_OUTNE = (1u << 0);
const uint8_t PCA_LEDOUT_LDR3_POS = (6u);
const uint8_t PCA_LEDOUT_LDR2_POS = (4u);
const uint8_t PCA_LEDOUT_LDR1_POS = (2u);
const uint8_t PCA_LEDOUT_LDR0_POS = (0u);
const uint8_t PCA_LDRx_OFF = (0u);
const uint8_t PCA_LDRx_ON = (1u);
const uint8_t PCA_LDRx_PWM = (2u);
const uint8_t PCA_LDRx_PWMGRP = (3u);
const uint8_t LED0 = (2u);
const uint8_t LED1 = (3u);
const uint8_t LED2 = (4u);
const uint8_t LED3 = (5u);

/**
	@brief Initialize LED
	@details Initializes LED controller
	@param[in] i2caddr Address of the PCA9632 to initialize
*/
void LED_Init(uint8_t i2caddr)
{
	i2c_send(PCARESETADDR, (uint8_t*)PCA_ResetCMD, PCA_ResetCMD_Length);
	uint8_t config[10] = {
		(PCA_REG_MODE1|PCA_AUTOINC_ALL),	//Start at 0x00 with auto increment
		(PCA_MODE1_ALLCALL),	//MODE1
		(PCA_MODE2_OUTNE),	//MODE2
		0x00,	//PWM0
		0x00,	//PWM1
		0x00,	//PWM2
		0x00,	//PWM3
		0xFF,	//GRPPWM
		0x00,	//GRPFREQ
		((PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR3_POS)|	//LEDOUT
			(PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR2_POS)|
			(PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR1_POS)| 
			(PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR0_POS))};
	i2c_send(i2caddr, config, 10);
}

/**
	@brief Update Single LED Color
	@details Updates a single LED color on the controller
	@param[in] i2caddr Address of LED controller
	@param[in] led_update Index of the LED to update(0-3)
	@param[in] value Value to set led to
	@returns Message attempted sent(1) or not(0)
*/
uint8_t LED_SingleUpdate(uint8_t i2caddr, uint8_t led_update, uint8_t value)
{
	uint8_t message[2] = {PCA_AUTOINC_NONE, value};
	switch (led_update)
	{
		case (0):
		{
			message[0] |= LED0; 
			break;
		}
		case (1):
		{
			message[0] |= LED1;
			break;
		}
		case (2):
		{
			message[0] |= LED2;
			break;
		}
		case (3):
		{
			message[0] |= LED3;
			break;
		}
		default:
		{
			message[0] = 0x00;
			break;
		}
	}
	
	if (message[0])
	{
		i2c_send(i2caddr, message, 2);
		return 1;
	}
	
	return 0;
}

/**
	@brief Update all LEDS on device.
	@details Update all LEDs at once, must be in sequential order from LED0 onward.
	@param[in] i2caddr Address of LED controller
	@param[in] values Pointer to array of values
	@param[in] length Length of values array
*/
void LED_UpdateAll(uint8_t i2caddr, uint8_t *values, uint8_t length)
{
	uint8_t message[length+1];
	message[0] = (PCA_REG_PWM0 | PCA_AUTOINC_ALL);
	uint8_t *mPointer = message+1;
	for (uint8_t i = 0; i<length; i++)
	{
		*mPointer = *values;
		mPointer++;
		values++;
	}
	i2c_send(i2caddr, message, (length+1));
}

/**
	@brief Update brightness
	@details Update the brightness of the LED
	@param[in] i2caddr Address of LED controller
	@param[in] value Value to set brightness
*/
void LED_UpdateBrightness(uint8_t i2caddr, uint8_t value)
{
	uint8_t message[2] = {(PCA_REG_GRPPWM | PCA_AUTOINC_NONE), value};
	i2c_send(i2caddr, message, 2);
}

/**
	@brief LED Enable
	@details Turns on LEDs on controller
	@param[in] i2caddr Address of LED controller
*/
void LED_Enable(uint8_t i2caddr)
{
	uint8_t message[2] = {(PCA_REG_LEDOUT | PCA_AUTOINC_NONE),
	((PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR3_POS)|	//LEDOUT
	(PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR2_POS)|
	(PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR1_POS)|
	(PCA_LDRx_PWMGRP << PCA_LEDOUT_LDR0_POS))};
	i2c_send(i2caddr, message, 2);
}

/**
	@brief LED Disable
	@details Turns off LEDs on controller
	@param[in] i2caddr Address of LED controller
*/
void LED_Disable(uint8_t i2caddr)
{
	uint8_t message[2] = {(PCA_REG_LEDOUT | PCA_AUTOINC_NONE),(0x00)};
	i2c_send(i2caddr, message, 2);
}