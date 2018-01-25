#include <iostream>
#include <fstream>
#include <string>
#include "mavlink_parse_adsb.hxx"

using namespace std;

int main(int argc, char *argv[])
{
	// ifstream pipe("adsb-in.dat");
	ifstream pipe("/dev/ttyUSB0");
	
	if (pipe.is_open()) {
		string line;
		while (getline(pipe,line)) {
		
			MavlinkParser parser;
		
			for (int i=0; i<line.size(); i++) {
				uint8_t b = line.at(i);
				if (parser.push_byte(b)) {
					// Message available!
					while (parser.num_messages_ready() > 0) {
						switch (parser.next_message_type()) {
							case traffic_report:
							{
								msg_traffic_report_t report = parser.get_next_traffic_report();
								cout << report.callsign << "\t | (" << report.lat/1e7 << "," << report.lon/1e7 << ") @ " << report.altitude/1e3 << "m | heading: " << report.heading/1e2 << "deg" << endl;
								break;
							}
							case datastream_request:
							{
								msg_datastream_request_t request = parser.get_next_datastream_request();
								cout << "DATASTREAM REQUEST" << endl;
								break;
							}
						}
					}
				}
			}
		}
	}
	
	cout << endl;
	pipe.close();
	
	return 0;
}
