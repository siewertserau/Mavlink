#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "mavlink_parse_adsb.hxx"

using namespace std;


int check_error_bits(ifstream* f)
{
    int stop = 0;

    if (f->eof())
    {
        perror("stream eofbit. error state");
        // EOF after std::getline() is not the criterion to stop processing
        // data: In case there is data between the last delimiter and EOF,
        // getline() extracts it and sets the eofbit.
        stop = 0;
    }

    if (f->fail()) 
    {
        perror("stream failbit (or badbit). error state");
        stop = 1;
    }

    if (f->bad()) 
    {
        perror("stream badbit. error state");
        stop = 1;
    }

    return stop;

}


int main(int argc, char *argv[])
{
    MavlinkParser parser;

    cout << "ICARUS ADS-B receiver\n";

    //ifstream pipe("adsb-in.dat");

    ifstream pipe("/dev/ttyUSB0");

    if(check_error_bits(&pipe))
    {
        exit(-1);
    }
    else
    {
        cout << "Connected stream to pipe input from ttyUSB0\n";
    }
	
    if (pipe.is_open()) 
    {
        string line;

        cout << "Waiting for input from ttyUSB0\n";

        while (getline(pipe, line)) 
        {
            cout << ".";
	
            for (int i=0; i<line.size(); i++) 
            {
                uint8_t b = line.at(i);

                if (parser.push_byte(b)) 
                {
                    // Message available!
                    while (parser.num_messages_ready() > 0)
                    {
                        switch (parser.next_message_type())
                        {
                            case traffic_report:
                            {
                                cout << "\nREPORT:";
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
    else
    {
        cout << "no USB pipe or not run as root" << endl;
        perror("error while opening pipe");
    }
	
    cout << endl;
    pipe.close();
	
    return 0;
}
