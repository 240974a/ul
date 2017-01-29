#pragma once
#ifdef ESP8266

	#include <Arduino.h>
	#include <IPAddress.h>

	namespace ul
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
		extern bool no_print;
		extern sstream log();
		const sstream& operator << (const sstream&, const char* str);
		const sstream& operator << (const sstream&, const __FlashStringHelper*);
		const sstream& operator << (const sstream&, unsigned char val);
		const sstream& operator << (const sstream&, unsigned short val);
		const sstream& operator << (const sstream&, unsigned int val);
		const sstream& operator << (const sstream&, unsigned long val);
		const sstream& operator << (const sstream&, unsigned long long val);
		const sstream& operator << (const sstream&, signed char val);
		const sstream& operator << (const sstream&, signed short val);
		const sstream& operator << (const sstream&, signed int val);
		const sstream& operator << (const sstream&, signed long val);
		const sstream& operator << (const sstream&, signed long long val);
		const sstream& operator << (const sstream&, double val);
		const sstream& operator << (const sstream&, float val);
		const sstream& operator << (const sstream&, const IPAddress&);

		typedef unsigned char OWAddress[8];
		const sstream& operator << (const sstream&, const OWAddress&);

		class telnet_stream : public Stream
		{
		public:
			virtual size_t write(uint8_t c) override;
			virtual int available() override;
			virtual int read() override;
			virtual int peek() override;
			virtual void flush() override;
		};

		telnet_stream& telnet();


		namespace telnetto
		{
			void setup();
			void handle();
		}

	}
#endif