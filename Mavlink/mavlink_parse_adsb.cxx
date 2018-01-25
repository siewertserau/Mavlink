#include "mavlink_parse_adsb.hxx"
#include "type_conversions.hxx"

// Instantiates MavlinkParser class to read uAvionix ADS-B in data
MavlinkParser::MavlinkParser() {
	// Constructor
}

// Add byte to buffer and attempt to parse messages
bool MavlinkParser::push_byte(uint8_t b) {
	buffer.push_back(b);
	parse_buffer();
	if (messages.size() > 0) return true;
	return false;
}

// Returns the number of messages that have been parsed
int MavlinkParser::num_messages_ready() {
	return messages.size();
}

// Returns the type of next message
int MavlinkParser::next_message_type() {
	if (messages.size() > 0) return messages.at(0);
	else return -1;
}

// Return the next traffic report message
msg_traffic_report_t MavlinkParser::get_next_traffic_report() {
	msg_traffic_report_t report;
	if (traffic_reports.size() > 0) {
		report = traffic_reports.at(0);
		traffic_reports.erase(traffic_reports.begin());
		for (int i=0; i<messages.size(); i++) {
			if (messages.at(i) == traffic_report) {
				messages.erase(messages.begin()+i);
				break;
			}
		}
		report.good = true;
	} else {
		report.good = false;
	}
	return report;
}

// Return the next datastream request message
msg_datastream_request_t MavlinkParser::get_next_datastream_request() {
	msg_datastream_request_t msg;
	if (datastream_requests.size() > 0) {
		msg = datastream_requests.at(0);
		datastream_requests.erase(datastream_requests.begin());
		for (int i=0; i<messages.size(); i++) {
			if (messages.at(i) == datastream_request) {
				messages.erase(messages.begin()+i);
				break;
			}
		}
		msg.good = true;
	} else {
		msg.good = false;
	}
	return msg;
}

// Parse Buffer (the meat of the program)
void MavlinkParser::parse_buffer() {
	int n=0, i=0;
	int N=buffer.size();
	while (i<(N-MIN_MSG_LEN)) {
		uint8_t b = buffer.at(i);
		if (b == 0xfe) { // Start of a new packet
			int payload_len = buffer.at(i+1);
			int msg_id = buffer.at(i+5);
			// Check if there is another 0xfe after message
			if (N > i+payload_len+8) {
				if (((int) buffer.at(i+payload_len+8)) == 0xfe) {
					// Message received!
					if (msg_id == 0xf6 && payload_len == 0x26) {
						// Parse traffic report
						msg_traffic_report_t report;
						for (int j=0; j<38; j++) {
							uint8_t c = buffer.at(i+6+j);
							report.raw[j] = c;
						}
						// Message sequence ID
						report.seq = (int)buffer.at(i+2);
						
						// Parse message specifics
						report.ICAO_address = to_u32(buffer.at(i+6), buffer.at(i+7), buffer.at(i+8), buffer.at(i+9));
						report.lat = to_i32(buffer.at(i+10), buffer.at(i+11), buffer.at(i+12), buffer.at(i+13));
						report.lon = to_i32(buffer.at(i+14), buffer.at(i+15), buffer.at(i+16), buffer.at(i+17));
						report.altitude = to_i32(buffer.at(i+18), buffer.at(i+19), buffer.at(i+20), buffer.at(i+21));
						report.heading = to_u16(buffer.at(i+22), buffer.at(i+23));
						report.hor_velocity = to_u16(buffer.at(i+24), buffer.at(i+25));
						report.ver_velocity = to_i16(buffer.at(i+26), buffer.at(i+27));
						report.validFlags = to_u16(buffer.at(i+28), buffer.at(i+29));
						report.squawk = to_u16(buffer.at(i+30), buffer.at(i+31));
						report.altitude_type = buffer.at(i+32);
						for (int j=0; j<9; j++) {
							report.callsign[j] = buffer.at(i+33+j);
						}
						
						report.emitter_type = buffer.at(36);
						report.tslc = buffer.at(37);
						
						traffic_reports.push_back(report);
						messages.push_back(traffic_report);
					}
					
					else if (msg_id == 0x42 && payload_len == 0x06) {
						// Parse datastream request
						msg_datastream_request_t msg;
						for (int j=0; j<6; j++) {
							uint8_t c = buffer.at(i+6+j);
							msg.raw[j] = c;
						}
						// Message sequence ID
						msg.seq = (int)buffer.at(i+2);
						
						datastream_requests.push_back(msg);
						messages.push_back(datastream_request);
					}
					
					// Keep moving...
					n = i+payload_len+7;
					i += payload_len+7;
				}
			}
		}
		i++;
	}
	
	// Clear buffer upto parsed data
	buffer.erase(buffer.begin(), buffer.begin()+n);
}
