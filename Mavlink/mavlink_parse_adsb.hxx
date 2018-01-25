#ifndef _MAVLINK_PARSE_ADSB_H_
#define _MAVLINK_PARSE_ADSB_H_

#include <inttypes.h>
#include <vector>
#include <iostream>

using namespace std;

#define MIN_MSG_LEN 50

enum msg_type {
	datastream_request = 0,
	traffic_report = 1
};

struct msg_datastream_request_t {
	bool good;
	int seq;
	uint8_t raw[6];
};

struct msg_traffic_report_t {
	bool good;
	int seq;
	uint8_t raw[38];
	uint32_t ICAO_address;
	int32_t lat;
	int32_t lon;
	int32_t altitude;
	uint16_t heading;
	uint16_t hor_velocity;
	int16_t ver_velocity;
	uint16_t validFlags;
	uint16_t squawk;
	uint8_t altitude_type;
	char callsign[9];
	uint8_t emitter_type;
	uint8_t tslc;
};

class MavlinkParser {
	public:
		MavlinkParser();
		bool push_byte(uint8_t b);
		int num_messages_ready();
		int next_message_type();
		msg_traffic_report_t get_next_traffic_report();
		msg_datastream_request_t get_next_datastream_request();
		
	private:
		void parse_buffer();
		vector<int> messages;
		vector<msg_traffic_report_t> traffic_reports;
		vector<msg_datastream_request_t> datastream_requests;
		vector<uint8_t> buffer;
};

#endif
