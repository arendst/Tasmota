#include "Adafruit_Crickit.h"

//the pwm pins
#define CRICKIT_NUM_PWM 12
static const uint8_t CRICKIT_pwms[CRICKIT_NUM_PWM] = {CRICKIT_SERVO4, CRICKIT_SERVO3, CRICKIT_SERVO2, CRICKIT_SERVO1,
													CRICKIT_MOTOR_B1, CRICKIT_MOTOR_B2, CRICKIT_MOTOR_A1, CRICKIT_MOTOR_A2, 
													CRICKIT_DRIVE4, CRICKIT_DRIVE3, CRICKIT_DRIVE2, CRICKIT_DRIVE1};

//the adc pin
#define CRICKIT_NUM_ADC 8
static const uint8_t CRICKIT_adc[CRICKIT_NUM_ADC] = { CRICKIT_SIGNAL1, CRICKIT_SIGNAL2, CRICKIT_SIGNAL3, CRICKIT_SIGNAL4,
													CRICKIT_SIGNAL5, CRICKIT_SIGNAL6, CRICKIT_SIGNAL7, CRICKIT_SIGNAL8 };

void Adafruit_Crickit::analogWrite(uint8_t pin, uint16_t value, uint8_t width)
{
	(void)width;

	int8_t p = -1;
	for(int i=0; i<CRICKIT_NUM_PWM; i++){
		if(CRICKIT_pwms[i] == pin){
			p = i;
			break;
		}
	}

	if(p > -1){
		uint8_t cmd[] = {(uint8_t)p, (uint8_t)(value >> 8), (uint8_t)value};
		this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_PWM, cmd, 3);
	}
}

uint16_t Adafruit_Crickit::analogRead(uint8_t pin)
{
	uint8_t buf[2];
	int8_t p = -1;
	for(int i=0; i<CRICKIT_NUM_ADC; i++){
		if(CRICKIT_adc[i] == pin){
			p = i;
			break;
		}
	}

	if(p > -1){
		this->read(SEESAW_ADC_BASE, SEESAW_ADC_CHANNEL_OFFSET + p, buf, 2, 500);
		uint16_t ret = ((uint16_t)buf[0] << 8) | buf[1];
	  	delay(1);
		return ret;
	}
	else return 0;
}

void Adafruit_Crickit::setPWMFreq(uint8_t pin, uint16_t freq)
{
	int8_t p = -1;
	for(int i=0; i<CRICKIT_NUM_PWM; i++){
		if(CRICKIT_pwms[i] == pin){
			p = i;
			break;
		}
	}

	if(p > -1){
		uint8_t cmd[] = {(uint8_t)p, (uint8_t)(freq >> 8), (uint8_t)freq};
		this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_FREQ, cmd, 3);
	}
}
