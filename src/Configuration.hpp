#pragma once
#include <Arduino.h>
#include <Preferences.h>

#define MaxStringConfigLength 40
#define MAXDEVICEID 65535

class Configuration {
public:
	Configuration();
	~Configuration();
	bool IsConfigured();
	void Load();
	void Save();
	void PrintConfiguration(Stream &out, bool includePassword=false);
	bool RequestConfiguration(Stream &inOut);

	const String GetSSID();
	void SetSSID(String &s);
	const String GetPassword();
	void SetPassword(String &s);
	ushort GetID();
	void SetID(ushort id);

private:
	ushort id;
	String ssid;
	String password;
	Preferences preferences;

	String ReadToNewLine(Stream &in, bool showAstric=false);
};