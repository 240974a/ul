#pragma once
#include <Arduino.h>
namespace tty
{
	// ####################################################
	// SSTREAM
	// ####################################################
	struct sstream
	{
		sstream(char* buf) : pos(buf){}
		~sstream();
		char* pos;
	};
	extern sstream log();
	sstream& operator << (const sstream&, const char* str);
	sstream& operator << (const sstream&, const __FlashStringHelper*);
	sstream& operator << (const sstream&, unsigned char val);
	sstream& operator << (const sstream&, unsigned short val);
	sstream& operator << (const sstream&, unsigned int val);
	sstream& operator << (const sstream&, unsigned long val);
	sstream& operator << (const sstream&, unsigned long long val);
	sstream& operator << (const sstream&, signed char val);
	sstream& operator << (const sstream&, signed short val);
	sstream& operator << (const sstream&, signed int val);
	sstream& operator << (const sstream&, signed long val);
	sstream& operator << (const sstream&, signed long long val);
	sstream& operator << (const sstream&, double val);
	sstream& operator << (const sstream&, float val);

	typedef uint8_t OWAddress[8];
	sstream& operator << (const sstream&, const OWAddress&);
}
