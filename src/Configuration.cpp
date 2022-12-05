#include "Configuration.hpp"

#define ISCONFIG_KEY "IsConfigured"
#define ID_KEY "Id"
#define SSID_KEY "SSID"
#define PASSWORD_KEY "Password"
#define STORE_NAME "beacon"
#define PARTITION_ID __null


Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

bool Configuration::IsConfigured()
{
	auto ret = preferences.begin(STORE_NAME, true, PARTITION_ID);
	if(!ret)
	{
		return false;
	}

	auto c = preferences.getBool(ISCONFIG_KEY);
	preferences.end();

	if(c == true)
	{
		return true;
	}

	return false;
}

void Configuration::Load()
{
	if(IsConfigured())
	{
		preferences.begin(STORE_NAME, false, PARTITION_ID);
		id = preferences.getUShort(ID_KEY);
		ssid = preferences.getString(SSID_KEY);
		password = preferences.getString(PASSWORD_KEY);
		preferences.end();
	}
}

void Configuration::Save()
{
	preferences.begin(STORE_NAME, false, PARTITION_ID);
	preferences.putUShort(ID_KEY, id);
	preferences.putString(SSID_KEY, ssid);
	preferences.putString(PASSWORD_KEY, password);
	preferences.putBool(ISCONFIG_KEY, true);
	preferences.end();
}

const String Configuration::GetSSID()
{
	return ssid;
}

void Configuration::SetSSID(String &s)
{
	if(s.length() > MaxStringConfigLength)
	{
		s = s.substring(0, MaxStringConfigLength - 1);
	}

	ssid = s;
}

const String Configuration::GetPassword()
{
	return password;
}

void Configuration::SetPassword(String &s)
{
	if(s.length() > MaxStringConfigLength)
	{
		s = s.substring(0, MaxStringConfigLength - 1);
	}
	password = s;
}

ushort Configuration::GetID()
{
	return id;
}

void Configuration::SetID(ushort id)
{
	this->id = id;
}

void Configuration::PrintConfiguration(Stream& out, bool includePassword)
{
	out.write('\n');
	out.printf("IsConfigured: %i", IsConfigured());
	out.write('\n');
	out.printf("Id: %i", id);
	out.write('\n');
	out.printf("SSID: %s", ssid);
	out.write('\n');
	if(includePassword)
	{
		out.printf("Password %s\n", password);
	}
	else
	{
		out.printf("Password Length: %i", password.length());
	}
	out.write('\n');
}

String Configuration::ReadToNewLine(Stream& in, bool showAstric)
{
	String s;
	for(byte i=0;i<MaxStringConfigLength;i++)
	{
		while(!in.available())
		{
			delay(100);
		}
		auto c = in.read();
		if(c == '\n')
		{
			s.trim();
			return s;
		}
		else
		{
			s += (char)c;
			if(showAstric)
			{
				in.print('*');
			}
			else
			{
				in.print((char)c);
			}
		}
	}

	return s;
}

bool Configuration::RequestConfiguration(Stream& inOut)
{
	while(!IsConfigured())
	{
		inOut.write('\n');
		long id = -1;
		while(id < 0 || id >= MAXDEVICEID)
		{
			inOut.printf("\nPlease enter an id between 1 and %i: ", MAXDEVICEID - 1);
			auto input = this->ReadToNewLine(inOut);
			id = input.toInt();
			if(id == 0)
			{
				id = -1;
			}
		}
		SetID((ushort)id);

		String in;
		while(in.length() < 1 || in.length() >= MaxStringConfigLength)
		{
			inOut.printf("\nPlease enter SSID: ");
			in = this->ReadToNewLine(inOut);
		}

		SetSSID(in);

		in = "";
		while(in.length() < 1 || in.length() >= MaxStringConfigLength)
		{
			inOut.printf("\nPlease enter your Password: ");
			in = ReadToNewLine(inOut, true);
		}

		SetPassword(in);

		PrintConfiguration(inOut, true);

		inOut.printf("Save Configuration? (y,n)");
		while(!inOut.available())
		{
			delay(100);
		}

		auto c = inOut.read();
		if(c == 'y' || c == 'Y')
		{
			inOut.println("Saving...");
			Save();
			inOut.println("Saved");
		}
	}

	return false;
}