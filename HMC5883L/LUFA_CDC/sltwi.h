/*
 * sltwi.h
 *
 * Created: 28/07/2011 10:01:14 PM
 *  Author: Stanley Lio
 */


#ifndef SLTWI_H_
#define SLTWI_H_

#define TWIRECEIVEGOOD	1

uint8_t twiSingleByteWrite(uint8_t slave_addr, uint8_t reg_addr, uint8_t val);
uint8_t twiSingleByteRead(uint8_t slave_addr, uint8_t reg_addr, uint8_t* val);
uint8_t twiMultipleByteRead(const uint8_t slave_addr, const uint8_t reg_addr, uint8_t* buf, const uint8_t count, uint8_t* readcount);

#endif /* SLTWI_H_ */

