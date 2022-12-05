#include <WIFI.h>
#include "Configuration.hpp"

Configuration config;

void setup()
{
	Serial.begin(9600);
	config.Load();
	if(!config.IsConfigured())
	{
		config.RequestConfiguration(Serial);
		config.PrintConfiguration(Serial);
	}
	else
	{
		config.PrintConfiguration(Serial);
	}
}

void loop()
{
	delay(1000);
}