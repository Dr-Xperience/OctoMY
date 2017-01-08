#ifndef ACTUATORTYPE_HPP
#define ACTUATORTYPE_HPP

enum ActuatorType: uint8_t {
	DC_MOTOR, STEP_MOTOR, RC_SERVO, RELAY, TYPE_COUNT
};

#endif // ACTUATORTYPE_HPP