

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
						
						handleCommands(token);
						
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