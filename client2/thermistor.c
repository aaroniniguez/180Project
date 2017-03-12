#include "thermistor.h"

THERMISTOR *thermistor_init(int aio_pin, char units)
{
	THERMISTOR *thermistor;
	thermistor = malloc(sizeof(THERMISTOR));

	thermistor->signal = mraa_aio_init(aio_pin);
	thermistor->units = units;

	return thermistor;
}

float thermistor_read(THERMISTOR *thermistor)
{
	int reading;
	float resistance, temperature;
	float B;

	B = 3975.0;

	reading = mraa_aio_read(thermistor->signal);
	resistance = ( 1023.0 - (float) reading ) * ( 10000.0 / (float) reading );

	temperature = resistance / 10000.0;
	temperature = log(temperature);
	temperature = temperature/B;
	temperature = temperature + 1.0/298.15;
	temperature = 1.0/temperature;
	temperature = temperature - 273.15;
	if (thermistor->units == 'F') temperature = temperature * 9/5 + 32;
	if (thermistor->units == 'K') temperature = temperature + 273.15;

	return temperature;
}

void thermistor_off(THERMISTOR *thermistor)
{
	mraa_aio_close(thermistor->signal);
}
