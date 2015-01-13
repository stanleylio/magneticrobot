/*
 * ArduinoProMicro.h
 *
 * Created: 05/06/2013 9:45:40 PM
 *  Author: Stanley Lio
 */ 


#ifndef ARDUINOPROMICRO_H_
#define ARDUINOPROMICRO_H_

#define F_CPU	16000000UL

#define set(reg,bit)	reg |= _BV(bit);
#define clear(reg,bit)	reg &= ~_BV(bit);

#define D0			PD2
#define D0DDR		DDRD
#define D0PORT		PORTD
#define D0PIN		PIND
#define set_D0		set(D0PORT,D0)
#define clear_D0	clear(D0PORT,D0)

#define D1			PD3
#define D1DDR		DDRD
#define D1PORT		PORTD
#define D1PIN		PIND
#define set_D1		set(D1PORT,D1)
#define clear_D1	clear(D1PORT,D1)

#define D2			PD1
#define D2DDR		DDRD
#define D2PORT		PORTD
#define D2PIN		PIND
#define set_D2		set(D2PORT,D2)
#define clear_D2	clear(D2PORT,D2)

#define D3			PD0
#define D3DDR		DDRD
#define D3PORT		PORTD
#define D3PIN		PIND
#define set_D3		set(D3PORT,D3)
#define clear_D3	clear(D3PORT,D3)

#define D4			PD4
#define D4DDR		DDRD
#define D4PORT		PORTD
#define D4PIN		PIND
#define set_D4		set(D4PORT,D4)
#define clear_D4	clear(D4PORT,D4)

#define D5			PC6
#define D5DDR		DDRC
#define D5PORT		PORTC
#define D5PIN		PINC
#define set_D5		set(D5PORT,D5)
#define clear_D5	clear(D5PORT,D5)

#define D6			PD7
#define D6DDR		DDRD
#define D6PORT		PORTD
#define D6PIN		PIND
#define set_D6		set(D6PORT,D6)
#define clear_D6	clear(D6PORT,D6)

#define D7			PE6
#define D7DDR		DDRE
#define D7PORT		PORTE
#define D7PIN		PINE
#define set_D7		set(D7PORT,D7)
#define clear_D7	clear(D7PORT,D7)

#define D8			PB4
#define D8DDR		DDRB
#define D8PORT		PORTB
#define D8PIN		PINB
#define set_D8		set(D8PORT,D8)
#define clear_D8	clear(D8PORT,D8)

#define D9			PB5
#define D9DDR		DDRB
#define D9PORT		PORTB
#define D9PIN		PINB
#define set_D9		set(D9PORT,D9)
#define clear_D9	clear(D9PORT,D9)

#define D10			PB6
#define D10DDR		DDRB
#define D10PORT		PORTB
#define D10PIN		PINB
#define set_D10		set(D10PORT,D10)
#define clear_D10	clear(D10PORT,D10)

#define D11			PB7
#define D11DDR		DDRB
#define D11PORT		PORTB
#define D11PIN		PINB
#define set_D11		set(D11PORT,D11)
#define clear_D11	clear(D11PORT,D11)

#define D12			PD6
#define D12DDR		DDRD
#define D12PORT		PORTD
#define D12PIN		PIND
#define set_D12		set(D12PORT,D12)
#define clear_D12	clear(D12PORT,D12)

#define D13			PC7
#define D13DDR		DDRC
#define D13PORT		PORTC
#define D13PIN		PINC
#define set_D13		set(D13PORT,D13)
#define clear_D13	clear(D13PORT,D13)

#define D17			PB0
#define D17DDRB		DDRB
#define D17PORT		PORTB
#define D17PIN		PINB
#define set_D17		set(D17PORT,D17)
#define clear_D17	clear(D17PORT,D17)

// the two LEDs
#define LED_amber_INIT		set(DDRB,PB0)
#define LED_amber_DEINIT	clear(DDRB,PB0)
#define LED_green_INIT		set(DDRD,PD5)
#define LED_green_DEINIT	clear(DDRD,PD5)
#define LED_amber_ON		clear(PORTB,PB0)
#define LED_amber_OFF		set(PORTB,PB0)
#define LED_green_ON		clear(PORTD,PD5)
#define LED_green_OFF		set(PORTD,PD5)

#endif /* ARDUINOPROMICRO_H_ */