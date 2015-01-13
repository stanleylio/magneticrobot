/*
 * LUFA_CDC.c
 *
 * Created: 14/03/2014 01:52:15
 *  Author: Stanley Lio
 */

/************************************************************************
Read on request 3-axis magnetic field strength and send to PC via LUFA
CDC virtual serial port. Send one reading per '\n' received.

For Arduino PRO Micro
************************************************************************/

#include "ArduinoProMicro.h"
#include "VirtualSerial.h"
#include "sltwi.h"
#include "HMC5883L.h"
#include <avr/io.h>
#include <util/delay.h>

USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
	.Config =
	{
		.ControlInterfaceNumber   = 0,
		.DataINEndpoint           =
		{
			.Address          = CDC_TX_EPADDR,
			.Size             = CDC_TXRX_EPSIZE,
			.Banks            = 1,
		},
		.DataOUTEndpoint =
		{
			.Address          = CDC_RX_EPADDR,
			.Size             = CDC_TXRX_EPSIZE,
			.Banks            = 1,
		},
		.NotificationEndpoint =
		{
			.Address          = CDC_NOTIFICATION_EPADDR,
			.Size             = CDC_NOTIFICATION_EPSIZE,
			.Banks            = 1,
		},
	},
};

int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();
	
	uint8_t code = 0;
	uint8_t val = 0;
	
	code = twiSingleByteRead(HMC5883L_ADDR,ID_REG_A,&val);
	if ((TWIRECEIVEGOOD != code) || ('H' != val))
	{
		LED_green_OFF;
		LED_amber_ON;
		for(;;);
	}
	
	twiSingleByteWrite(HMC5883L_ADDR,CFG_REG_A,0b01111000);		// 75Hz refresh rate, 8-average
	//twiSingleByteWrite(HMC5883L_ADDR,CFG_REG_B,0b01000000);		// sensitivity = 820 LSb/Gauss
	//twiSingleByteWrite(HMC5883L_ADDR,CFG_REG_B,0b10000000);		// sensitivity = 440 LSb/Gauss
	twiSingleByteWrite(HMC5883L_ADDR,CFG_REG_B,0b11100000);		// sensitivity = 230 LSb/Gauss
	twiSingleByteWrite(HMC5883L_ADDR,MODE_REG,0);				// mode register, continuous measurement mode
	
	for (;;)
	{
		/* Now that I think of it, moving between the RETURN key and the 
		breadboard 273 times is pretty lame. How about installing a button 
		on the breadboard and send a reading only when pressed? Or a timed
		read - like one reading every two seconds?
		Anyway, just need to wait for the robotic arm...
		*/
		while(CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface))
		{
			LED_green_ON;
			int16_t b = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
			if ('\n' == b&0xFF)
			{
				uint8_t buf[6];
				uint8_t readcount = 0;
				twiMultipleByteRead(HMC5883L_ADDR,DATAX_MSB,buf,6,&readcount);

				int16_t xr = (buf[0] << 8) + buf[1];	// it's in different endianness from the ADXL345
				int16_t yr = (buf[4] << 8) + buf[5];	// and even different order: X,Z,Y, not X,Y,Z!
				int16_t zr = (buf[2] << 8) + buf[3];
				uint8_t sbuf[21];
				snprintf(sbuf,20,"%05d,%05d,%05d\n",xr,yr,zr);
			
				CDC_Device_SendData(&VirtualSerial_CDC_Interface,sbuf,18);
			}
			
			_delay_ms(5);
			LED_green_OFF;
		}

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);		// MUST CALL THESE PERIODICALLY and FREQUENTLY! todo: how frequent?
		USB_USBTask();
	}

    /*while(1)
    {
		while(CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface))
		{
			int16_t b = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
			LED_green_ON;
			CDC_Device_SendByte(&VirtualSerial_CDC_Interface, b & 0xFF);
		}
		LED_green_OFF;
		
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);		// MUST CALL THESE PERIODICALLY and FREQUENTLY! todo: how frequent?
		USB_USBTask();
    }*/
}


void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	LED_amber_INIT;
	LED_amber_OFF;
	LED_green_INIT;
	LED_green_OFF;

	// I2C-related
	set(DDRD,PD0);	// SCL
	set(DDRD,PD1);	// SDA
	TWCR = 0;
	TWBR = 3;		// 400 kHz
	//TWBR = 18;		// 100 kHz
	
	USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	//
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	//
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

