#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mraa/aio.h>

typedef struct {
	mraa_aio_context signal;
	int reading;
	float temperature;
	char units;
} THERMISTOR;

THERMISTOR *thermistor_init(int aio_pin, char units);
float thermistor_read(THERMISTOR *thermistor);
void thermistor_off(THERMISTOR *thermistor);
