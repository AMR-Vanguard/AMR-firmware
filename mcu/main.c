
#include "src.h"


void initialize(void) {
	m_usb_init();
	sei();
}

int main(void) {
	initialize();

	while (1) {
		if (m_usb_isconnected()) {
			if (m_usb_rx_available()) {
				char received_char = m_usb_rx_char();

				if (received_char == '\r' || received_char == '\n') {
					inputBuffer[bufferIndex] = '\0';

					if (strncmp(inputBuffer, "set ", 4) == 0) {
						char *token = strtok(&inputBuffer[4], " ");
						if (token != NULL) {
							float v = atof(token);  // linear velocity
							token = strtok(NULL, " ");
							if (token != NULL) {
								float omega = atof(token);  // angular velocity

								// Constants
								float s = 0.57; // distance between wheels, 0.585
								double pi = 3.141592653589793;

								// Velocity for each wheel
								float vl = v - (s / 2.0) * omega;
								float vr = v + (s / 2.0) * omega;

								// Frequency calculation
								double fl = (12800000.0 / (12.3 * pi)) * vl; //12.7
								double fr = (12800000.0 / (12.3 * pi)) * vr;

								// Convert to long for PWM functions
								long pwm1_freq = (long)(fl);
								long pwm3_freq = (long)(fr);
								dup_pwm1_freq = pwm1_freq;
								dup_pwm3_freq = pwm3_freq;

								if (!pwm1Started) {
									InitPWM_OC1A();
									pwm1Started = 1;
								}
								if (!pwm3Started) {
									InitPWM_OC3A();
									pwm3Started = 1;
								}
								
								UpdatePWM_OC1A(pwm1_freq);
								UpdatePWM_OC3A(pwm3_freq);
								pre_pwm1_freq = pwm1_freq;
								pre_pwm3_freq = pwm3_freq;
							}
						}
						} else if (strcmp(inputBuffer, "off") == 0) {
							StopPWM_OC1A();
							StopPWM_OC3A();
					}

					bufferIndex = 0;
					} else if (bufferIndex < BUFFER_SIZE - 1) {
					inputBuffer[bufferIndex++] = received_char;
					} else {
					bufferIndex = 0;
				}
			}
		}
		_delay_ms(10);
	}
}