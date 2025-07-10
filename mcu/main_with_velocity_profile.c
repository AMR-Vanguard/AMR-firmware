#include "src.h"

#define DT 0.01  // Time step = 10ms

// Robot motion variables
float acc_c = 0.05;        // Center linear acceleration (m/s²)
float acc_omega = 0.05;    // Angular acceleration (rad/s²)
const float s = 0.57;      // Distance between wheels (m)

// Current and target center velocities
float V_target = 0.0, omega_target = 0.0;
float V_prev = 0.0;

// Wheel velocities
float v_L_current = 0.0, v_R_current = 0.0;
float v_L_target = 0.0, v_R_target = 0.0;

void initialize(void) {
	m_usb_init();
	sei();
}

// Compute target wheel velocities from center V and ω
void compute_wheel_targets() {
	v_L_target = V_target - (s / 2.0) * omega_target;
	v_R_target = V_target + (s / 2.0) * omega_target;
}

// Apply acceleration to each wheel until target is reached
void update_wheel_velocities() {
	float acc_L = 0.0, acc_R = 0.0;

	// Decide motion phase based on V_prev and V_target
	if (V_prev == 0 && V_target != 0) {
		// Starting phase
		acc_R = acc_c + (s / 2.0) * acc_omega;
		acc_L = acc_c - (s / 2.0) * acc_omega;
	} else if (V_target == 0) {
		// Stopping phase
		acc_R = acc_c + (s / 2.0) * acc_omega;
		acc_L = acc_c - (s / 2.0) * acc_omega;
	} else {
		// Moving phase
		acc_R = (s / 2.0) * acc_omega;
		acc_L = -(s / 2.0) * acc_omega;
	}

	// Accelerate/decelerate right wheel
	if (v_R_current < v_R_target) {
		v_R_current += acc_R * DT;
		if (v_R_current > v_R_target) v_R_current = v_R_target;
	} else if (v_R_current > v_R_target) {
		v_R_current -= acc_R * DT;
		if (v_R_current < v_R_target) v_R_current = v_R_target;
	}

	// Accelerate/decelerate left wheel
	if (v_L_current < v_L_target) {
		v_L_current += acc_L * DT;
		if (v_L_current > v_L_target) v_L_current = v_L_target;
	} else if (v_L_current > v_L_target) {
		v_L_current -= acc_L * DT;
		if (v_L_current < v_L_target) v_L_current = v_L_target;
	}
}

void apply_pwm_from_velocities() {
	const double pi = 3.141592653589793;
	double f_L = (12800000.0 / (12.3 * pi)) * v_L_current;
	double f_R = (12800000.0 / (12.3 * pi)) * v_R_current;
	long pwm1_freq = (long)(f_L);
	long pwm3_freq = (long)(f_R);

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
							V_target = atof(token);
							token = strtok(NULL, " ");
							if (token != NULL) {
								omega_target = atof(token);

								// Compute new wheel velocity targets
								compute_wheel_targets();
							}
						}
					} else if (strcmp(inputBuffer, "off") == 0) {
						StopPWM_OC1A();
						StopPWM_OC3A();

						// Reset everything
						V_target = 0;
						omega_target = 0;
						V_prev = 0;
						v_L_target = 0;
						v_R_target = 0;
						v_L_current = 0;
						v_R_current = 0;
					}

					bufferIndex = 0;
				} else if (bufferIndex < BUFFER_SIZE - 1) {
					inputBuffer[bufferIndex++] = received_char;
				} else {
					bufferIndex = 0;
				}
			}
		}

		// Step-by-step acceleration of each wheel
		update_wheel_velocities();
		apply_pwm_from_velocities();

		// Update previous velocity
		V_prev = V_target;

		_delay_ms((int)(DT * 1000));
	}
}
