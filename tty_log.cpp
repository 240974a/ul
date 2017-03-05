#include <tty_log.h>
namespace tty
{
	char monitorbuf[128] { 0 };
	sstream log(){ return sstream(monitorbuf); }
	// --
	sstream::~sstream() {
		if (pos != monitorbuf) {
			*pos = 0;
			Serial.println(monitorbuf);
		}
	}
	// --
	sstream& operator << (const sstream& s_, const char* str) {
		sstream& s = const_cast<sstream&>(s_);
		while (*str){ *s.pos++ = *str++; };
		return s;
	}
	sstream& operator << (const sstream& s_, const __FlashStringHelper* flash)
	{
		sstream& s = const_cast<sstream&>(s_);
		size_t rest = monitorbuf + sizeof(monitorbuf)-s.pos - 1;
		size_t length = min(rest, strlen_P((const char*)flash));
		if (length){
			memcpy_P(s.pos, (const char*)flash, length);
			s.pos += length;
		}
		return s;
	}


	template<typename T>
	sstream&  print_signed_value(const sstream& s_, T& ival){
		sstream& s = const_cast<sstream&>(s_);
		if (ival == 0){
			*s.pos++ = '0';
			return s;
		}
		if (ival < 0){
			*s.pos++ = '-';
			ival = -ival;
		}
		auto begin = s.pos;
		while (ival) { *s.pos++ = '0' + ival % 10; ival /= 10; }
		for (auto end = s.pos - 1; begin < end; ++begin, --end) { auto c = *begin; *begin = *end; *end = c; }
		return s;
	}
	template<typename T>
	sstream&  print_unsigned_value(const sstream& s_, T& ival){
		sstream& s = const_cast<sstream&>(s_);
		if (ival == 0){
			*s.pos++ = '0';
			return s;
		}
		auto begin = s.pos;
		while (ival) { *s.pos++ = '0' + ival % 10; ival /= 10; }
		for (auto end = s.pos - 1; begin < end; ++begin, --end) { auto c = *begin; *begin = *end; *end = c; }
		return s;
	}
	// --
	sstream& operator << (const sstream& s, unsigned char      val) { return print_unsigned_value(s, val); }
	sstream& operator << (const sstream& s, unsigned short     val) { return print_unsigned_value(s, val); }
	sstream& operator << (const sstream& s, unsigned int       val) { return print_unsigned_value(s, val); }
	sstream& operator << (const sstream& s, unsigned long      val) { return print_unsigned_value(s, val); }
	sstream& operator << (const sstream& s, unsigned long long val) { return print_unsigned_value(s, val); }

	sstream& operator << (const sstream& s, signed char      val) { return print_signed_value(s, val); }
	sstream& operator << (const sstream& s, signed short     val) { return print_signed_value(s, val); }
	sstream& operator << (const sstream& s, signed int       val) { return print_signed_value(s, val); }
	sstream& operator << (const sstream& s, signed long      val) { return print_signed_value(s, val); }
	sstream& operator << (const sstream& s, signed long long val) { return print_signed_value(s, val); }

	sstream& print_float(const sstream& s_, double val){
		sstream& s = const_cast<sstream&>(s_);
		s.pos += strlen(dtostrf(val, 0, 3, s.pos));
		return s;
	}
	sstream& operator << (const sstream& s, double val)	{ return print_float(s,val);}
	sstream& operator << (const sstream& s, float val){ return print_float(s,val); }

	sstream& operator << (const sstream& s_, const OWAddress& val){
		sstream& s = const_cast<sstream&>(s_);
		for (uint8_t i = 0; i < sizeof(OWAddress) / sizeof(OWAddress*); ++i) {
			auto v = val[i];
			if (i != 0){
				*s.pos++ = ',';
			}
			*s.pos++ = '0';
			*s.pos++ = 'x';
			*s.pos++ = '0' + v % 16; v /= 16;
			*s.pos++ = '0' + v % 16; 
		}
		return s;
	}

}
