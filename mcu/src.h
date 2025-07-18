/*
 * src.h
 *
 * Created: 7/6/2025 12:13:34 PM
 *  Author: acer
 */ 


#ifndef SRC_H_
#define SRC_H_

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "m_usb.h"

#define F_CPU 16000000UL
#include <util/delay.h>


#define MAX_PWM_FREQ 200000UL
#define MIN_PWM_FREQ 1UL
#define BUFFER_SIZE 32

char inputBuffer[BUFFER_SIZE];
uint8_t bufferIndex = 0;
uint8_t pwm1Started = 0;
uint8_t pwm3Started = 0;
long pre_pwm1_freq  = 0;
long pre_pwm3_freq  = 0;
long dup_pwm1_freq  = 0;
long dup_pwm3_freq  = 0;

void InitPWM_OC1A() {
	DDRB |= (1 << PB5); // D9, PB5 = OC1A output
	DDRD |= (1 << PD5); // TX LED, Motor Enable
	PORTD &= ~(1 << PD5);
	DDRD |= (1 << PD4);  // D4, Motor Direction
	PORTD |= (1 << PD4);
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10); // No prescaler
	ICR1 = 79;  // Default 200 kHz
	OCR1A = ICR1 / 2;
}

void UpdatePWM_OC1A(long freq) {
	if (freq == 0) {
		OCR1A = 0;
		return;
	}

	if (freq < 0) {
		PORTD &= ~(1 << PD4); // Reverse direction (PD4 = 0)
		freq = -freq;         // Use absolute value
		} else {
		PORTD |= (1 << PD4);  // Forward direction (PD4 = 1)
	}

	if (freq < MIN_PWM_FREQ || freq > MAX_PWM_FREQ) return;
	uint32_t top = (F_CPU / (1UL * freq)) - 1;
	if (top > 65535) top = 65535;
	ICR1 = (uint16_t)top;
	OCR1A = ICR1 / 2;
	TCNT1 = 0;
}


void StopPWM_OC1A() {
	TCCR1A = 0;
	TCCR1B = 0;
	PORTB &= ~(1 << PB5);
	PORTD |= (1 << PD5);
	pwm1Started = 0;
}

void InitPWM_OC3A() {
	DDRC |= (1 << PC6); // D5, PC6 = OC3A output (if available)
	DDRD |= (1 << PD7); // D6, Motor Enable
	PORTD &= ~(1 << PD7);
	DDRD |= (1 << PD6); // D12, Motor Direction
	PORTD &= ~(1 << PD6);
	TCCR3A = (1 << COM3A1) | (1 << WGM31);
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS30); // No prescaler
	ICR3 = 79;
	OCR3A = ICR3 / 2;
}

void UpdatePWM_OC3A(long freq) {
	if (freq == 0) {
		OCR3A = 0;
		return;
	}

	if (freq < 0) {
		PORTD |= (1 << PD6);  // Reverse direction (PD6 = 1)
		freq = -freq;         // Use absolute value
		} else {
		PORTD &= ~(1 << PD6); // Forward direction (PD6 = 0)
	}

	if (freq < MIN_PWM_FREQ || freq > MAX_PWM_FREQ) return;
	uint32_t top = (F_CPU / (1UL * freq)) - 1;
	if (top > 65535) top = 65535;
	ICR3 = (uint16_t)top;
	OCR3A = ICR3 / 2;
	TCNT3 = 0;
}


void StopPWM_OC3A() {
	TCCR3A = 0;
	TCCR3B = 0;
	PORTC &= ~(1 << PC6);
	PORTD |= (1 << PD7);
	pwm3Started = 0;
}


#endif /* SRC_H_ */