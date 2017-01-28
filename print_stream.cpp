#ifdef ESP8266
	#include <ESP8266WiFi.h>
#else
	#include <WiFi.h>
#endif
#include "print_stream.h"
#include "utils/stream_buffer.h"

namespace ul
{
	stream_arena<2048> arena;
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	namespace telnetto
	{
		//how many clients should be able to telnet to this ESP8266
		const static int MAX_SRV_CLIENTS = 1;

		WiFiServer server(23);
		WiFiClient serverClients[MAX_SRV_CLIENTS];

		void setup()
		{
			server.begin();
			server.setNoDelay(true);
		}
		void handle()
		{
			uint16_t i;
			//check if there are any new clients
			if (server.hasClient()){
				for (i = 0; i < MAX_SRV_CLIENTS; i++){
					//find free/disconnected spot
					if (!serverClients[i] || !serverClients[i].connected()){
						if (serverClients[i]) serverClients[i].stop();
						serverClients[i] = server.available();
						ul::telnet().print("New client: "); ul::telnet().println(i);
						continue;
					}
				}
				//no free/disconnected spot so reject
				WiFiClient serverClient = server.available();
				serverClient.stop();
			}
			/*
			//check clients for data
			for (i = 0; i < MAX_SRV_CLIENTS; i++){
				if (serverClients[i] && serverClients[i].connected()){
					if (serverClients[i].available()){
						//get data from the telnet client and push it to the UART
						while (serverClients[i].available()) ul::telnet().write(serverClients[i].read());
					}
				}
			}
			*/
			//check UART for data
			if (!arena.empty()){
				//push UART data to all connected telnet clients
				for (i = 0; i < MAX_SRV_CLIENTS; i++){
					if (serverClients[i] && serverClients[i].connected()){
						while (!arena.empty()){
							size_t sz;  
							serverClients[i].write(arena.get_buffer(sz), sz);
						}
						//delay(1);
					}
				}
			}
		}
	}



	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	telnet_stream t_s_;
	telnet_stream& telnet() { return t_s_; }


	size_t telnet_stream::write(uint8_t c) {
		arena.put_byte(c);
		return 1;
	}
	int telnet_stream::available() {
		return 0;
	}
	int telnet_stream::read() {
		return 0;
	}
	int telnet_stream::peek() {
		return 0;
	}
	void telnet_stream::flush() {
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	bool no_print = false;
	char monitorbuf[128] { 0 };
	sstream log(){ return sstream(monitorbuf); }
	// --
	sstream::~sstream() {
		if (!no_print && pos != monitorbuf) {
			*pos = 0;
			telnet().println(monitorbuf);
		}
	}
	// --
	const sstream& operator << (const sstream& s_, const char* str) {
		sstream& s = const_cast<sstream&>(s_);
		while (*str){ *s.pos++ = *str++; };
		return s_;
	}
	const sstream& operator << (const sstream& s_, const __FlashStringHelper* flash)
	{
		sstream& s = const_cast<sstream&>(s_);
		size_t rest = monitorbuf + sizeof(monitorbuf) - s.pos - 1;
		size_t length = min(rest, strlen_P((const char*)flash));
		if (length){
			memcpy_P(s.pos, (const char*)flash, length);
			s.pos += length;
		}
		return s_;
	}


	template<typename T>
	const sstream&  print_signed_value(const sstream& s_, T& ival){
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
	const sstream&  print_unsigned_value(const sstream& s_, T& ival){
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
	const sstream& operator << (const sstream& s, unsigned char      val) { return print_unsigned_value(s, val); }
	const sstream& operator << (const sstream& s, unsigned short     val) { return print_unsigned_value(s, val); }
	const sstream& operator << (const sstream& s, unsigned int       val) { return print_unsigned_value(s, val); }
	const sstream& operator << (const sstream& s, unsigned long      val) { return print_unsigned_value(s, val); }
	const sstream& operator << (const sstream& s, unsigned long long val) { return print_unsigned_value(s, val); }

	const sstream& operator << (const sstream& s, signed char      val) { return print_signed_value(s, val); }
	const sstream& operator << (const sstream& s, signed short     val) { return print_signed_value(s, val); }
	const sstream& operator << (const sstream& s, signed int       val) { return print_signed_value(s, val); }
	const sstream& operator << (const sstream& s, signed long      val) { return print_signed_value(s, val); }
	const sstream& operator << (const sstream& s, signed long long val) { return print_signed_value(s, val); }

	const sstream& print_float(const sstream& s_, double val){
		sstream& s = const_cast<sstream&>(s_);
		s.pos += strlen(dtostrf(val, 0, 3, s.pos));
		return s;
	}
	const sstream& operator << (const sstream& s, double val)	{ return print_float(s, val); }
	const sstream& operator << (const sstream& s, float val){ return print_float(s, val); }
	const sstream& operator<<(const sstream& s_, const IPAddress& p){
		sstream& s = const_cast<sstream&>(s_);
		for (int i = 0; i <= 3; ++i) {
			auto v = p[i];
			print_unsigned_value(s, v);
			if (i != 3) *s.pos++ = '.';
		}
		return s;
	}

	const sstream& operator << (const sstream& s_, const OWAddress& val){
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
