/*
 * sltwi.c
 *
 * Created: 28/07/2011 10:00:42 PM
 *  Author: Stanley Lio
 */

#include <avr/io.h>
#include <util/twi.h>
#include <inttypes.h>
#include "sltwi.h"

uint8_t twiSingleByteWrite(uint8_t slave_addr, uint8_t reg_addr, uint8_t val)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);		// START
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_START) return TWSR;
	
	TWDR = (slave_addr << 1) & ~TW_READ;			// WRITE the slave address (next is a WRITE operation)
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_MT_SLA_ACK) return TWSR;
	
	TWDR = reg_addr;								// WRITE the register address
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_MT_DATA_ACK) return TWSR;
	
	TWDR = val;										// WRITE the byte
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_MT_DATA_ACK) return TWSR;
	
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);		// STOP
	return TWIRECEIVEGOOD;
}

uint8_t twiMultipleByteRead(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t *buf, const uint8_t count, uint8_t *readcount)
{
	*readcount = 0;		// C pointers are dangerous. wasting mankind's time.
	TWCR = 0;

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);			// START
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_START) return TWSR;				// START failed (exception)
	
	TWDR = (slave_addr << 1) & ~TW_READ;				// WRITE the slave address (next is a WRITE operation)
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_MT_SLA_ACK) return TWSR;		// slave did not ACK as expected (exception)

	TWDR = reg_addr;									// WRITE the register address
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);					// (wait for slave ACK)
	if (TW_STATUS != TW_MT_DATA_ACK) return TWSR;		// slave did not ACK as expected (exception)
		
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);			// REPEATED START
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_REP_START)	return TWSR;		// REPEATED START failed (exception)

	TWDR = (slave_addr << 1) | TW_READ;					// WRITE the slave address (next is a READ operation)
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);					// (wait for slave ACK)
	if (TW_STATUS != TW_MR_SLA_ACK) return TWSR;		// slave did not ACK as expected (exception)
	
	uint8_t i = 0;
	for (i = 0; i < count - 1; i++)						// the last read requires special treatment (NACK instead of ACK)
	{
		TWCR = _BV(TWEA) | _BV(TWINT) | _BV(TWEN);		// let the TWI hardware do the actual reading (ACK)
		loop_until_bit_is_set(TWCR,TWINT);				// (master ACK)
		if (TW_STATUS != TW_MR_DATA_ACK) return TWSR;	// slave did not ACK as expected
		*(buf + i) = TWDR;
		(*readcount)++;
	}
							
	TWCR = _BV(TWINT) | _BV(TWEN);						// let the TWI hardware handle the actual reading (NACK)
	loop_until_bit_is_set(TWCR,TWINT);					// (master NACK)
	if (TW_STATUS != TW_MR_DATA_NACK) return TWSR;		// master's NACK after the last byte failed for some reason
	*(buf + i) = TWDR;
	(*readcount)++;
						
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);			// STOP
	return TWIRECEIVEGOOD;
}

uint8_t twiSingleByteRead(uint8_t slave_addr, uint8_t reg_addr, uint8_t* val)
{
	*val = 0;
	TWCR = 0;

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);		// START
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_START) return TWSR;			// could not get hold of the bus (exception)

	TWDR = (slave_addr << 1) & ~TW_READ;			// WRITE the slave address (next is a WRITE operation)
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);				// (wait for slave ACK)
	if (TW_STATUS != TW_MT_SLA_ACK) return TWSR;	// slave did not ACK as expected (exception)

	TWDR = reg_addr;								// WRITE the register address
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);				// (wait for slave ACK)
	if (TW_STATUS != TW_MT_DATA_ACK) return TWSR;	// slave did not ACK as expected (exception)

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);		// REPEATED START
	loop_until_bit_is_set(TWCR,TWINT);
	if (TW_STATUS != TW_REP_START) return TWSR;		// REPEATED START failed (exception)
				
	TWDR = (slave_addr << 1) | TW_READ;				// WRITE the slave address (next is a READ operation)
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR,TWINT);				// waiting for slave ACK
	if (TW_STATUS != TW_MR_SLA_ACK) return TWSR;	// slave did not ACK as expected (exception)

	TWCR = _BV(TWINT) | _BV(TWEN);					// let the TWI hardware read
	loop_until_bit_is_set(TWCR,TWINT);				// (master NACK)
	if (TW_STATUS != TW_MR_DATA_NACK) return TWSR;	// master NACK failed (exception)

	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);		// STOP
	*val = TWDR;
	return TWIRECEIVEGOOD;
}
