/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* vl53l1x Header Files */
#include "vl53l1x/VL53L1X_api.h"

/* Driver configuration */
#include "ti_drivers_config.h"

/*************vl53l1x variables declarations***********/
const uint16_t dev = 0x29;
static uint8_t byteData;
static uint8_t sensorState;
static uint16_t wordData;
static uint16_t Distance;
static uint8_t RangeStatus;
static uint8_t dataReady;
static int8_t status;
/********************************************************/

/*function Prototypes*/
static void delay(uint32_t duration);
static void i2cScanner(I2C_Handle i2cHandle);

/*I2C Instance Creation*/
I2C_Handle i2c;
I2C_Params i2cParams;
I2C_Transaction i2cTransaction;

/*
 *  ======== mainThread ========
 */
void* mainThread(void *arg0)
{

    /* Call driver init functions */
    GPIO_init();
    I2C_init();

    // SPI_init();
    // UART_init();
    // Watchdog_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    //GPIO_setConfig(CONFIG_GPIO_pwrACCEL, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    //GPIO_write(CONFIG_GPIO_pwrACCEL, CONFIG_GPIO_LED_ON);

//    while (1)
//    {
//        sleep(1);
//        GPIO_toggle(CONFIG_GPIO_LED_0);
//    }

    /* Create I2C for usage */
    printf("thisis task\n");
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);

    /* Scan the Address of the available I2C devices */
    i2cScanner(i2c);
    // Those basic I2C read functions can be used to check your own I2C functions */
    status = VL53L1_RdByte(i2c, dev, 0x010F, &byteData);
    delay(2);
    //printf("VL53L1X Model_ID: %X\n", byteData);
    status = VL53L1_RdByte(i2c, dev, 0x0110, &byteData);
    delay(2);
    //printf("VL53L1X Module_Type: %X\n", byteData);
    status = VL53L1_RdWord(i2c, dev, 0x010F, &wordData);
    //printf("VL53L1X: %X\n", wordData);
    while (sensorState == 0)
    {
        status = VL53L1X_BootState(i2c, dev, &sensorState);
        delay(2);
    }
    //printf("Chip booted\n");

    /* Initialize and configure the device according to people counting need */
    status = VL53L1X_SensorInit(i2c, dev);
    delay(2);
    status += VL53L1X_SetDistanceMode(i2c, dev, 2); /* 1=short, 2=long */
    delay(2);
    status += VL53L1X_SetTimingBudgetInMs(i2c, dev, 20); /* in ms possible values [20, 50, 100, 200, 500] */
    delay(2);
    status += VL53L1X_SetInterMeasurementInMs(i2c, dev, 20);
    delay(2);
    if (status != 0)
    {
        //printf("Error in Initialization or configuration of the device\n");
    }

    /* This function has to be called to enable the ranging */
    status = VL53L1X_StartRanging(i2c, dev);
    while (1)
    {
        printf("thisis task inside while 1\n");

        GPIO_toggle(CONFIG_GPIO_LED_0);
        //Task_sleep((UInt) arg0);
//        GPIO_write(CONFIG_GPIO_pwrACCEL, 1);
        //i2cScanner (i2c);
        //GPIO_write(CONFIG_GPIO_pwrACCEL, CONFIG_GPIO_LED_OFF);
        delay(500);
        status = VL53L1X_CheckForDataReady(i2c, dev, &dataReady);
        if (dataReady == 0)
        {
            //return;
        }
        dataReady = 0;
        status += VL53L1X_GetRangeStatus(i2c, dev, &RangeStatus);
        status += VL53L1X_GetDistance(i2c, dev, &Distance);

        /* clear interrupt has to be called to enable next interrupt*/
        status += VL53L1X_ClearInterrupt(i2c, dev);

        if (status != 0)
        {
            //printf("Error in operating the device\n");
        }
        printf("Distance : %d\n ", Distance);
    }

    /* Deinitialized I2C */
    //I2C_close (i2c);
}

/*********************************************************************
 * @fn      i2cScanner
 *
 * @brief   scans the i2c bus for devices
 *
 * @param   i2cHandle
 *
 * @return  none
 */
static void i2cScanner(I2C_Handle i2cHandle)
{
    //Scan the bus for I2c devices.
    uint8_t result = 0;
    int nDevices = 0;
    uint8_t Buffer[1];
    Buffer[0] = 0;
    I2C_Transaction i2cTransaction = { 0 };
    uint8_t i;
    for (i = 1; i < 127; i++)
    {
        i2cTransaction.slaveAddress = i;
        i2cTransaction.writeBuf = 0;
        i2cTransaction.writeCount = 1;
        i2cTransaction.readBuf = NULL;
        i2cTransaction.readCount = 0;
        result = I2C_transfer(i2c, &i2cTransaction);
        if (result)
        {
            nDevices++;
            printf("Device add : %02X\n", i);
        }
    }
    printf("found %d devices\n", nDevices);
}

/*********************************************************************
 * @fn      delay
 *
 * @brief   delay in milliseconds
 *
 * @param   duration in milliseconds
 *
 * @return  none
 */
static void delay(uint32_t duration)
{
    //iterator
    uint32_t i = 0;
    //enter while loop and execute NOP cycles
    while (i < (1000 * duration))
    {
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        asm(" NOP");
        i++;
    }

}

