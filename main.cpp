#include <sys/types.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <cmath> //isnan
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

const int RETURN_ERR = -1;
const int RETURN_STRFOUND = 0;
const int RETURN_TIMEOUT = 2;
const int RETURN_PIDKILLED = 1;

int main(int argc, char *argv[])
{

    if (argc != 5)
    {
        cerr << "Usage: " << argv[0] << " [PID] [timeout] [logfile] [searchstring]" << endl;
        return 2;
    }

    pid_t pid = stoi(argv[1]);
    if (pid <= 0)
    {
        cerr << "invalid pid" << endl;
        return RETURN_ERR;
    }

    double timeout_duration = stod(argv[2]);
    if( std::isnan(timeout_duration) || std::isinf(timeout_duration))
    {
        cerr << "invalid timeout" << endl;
        return RETURN_ERR;
    }
    chrono::steady_clock::time_point timeout = chrono::steady_clock::now() + chrono::milliseconds((int) timeout_duration * 1000);

    ifstream ifs(argv[3]);
    if (! ifs.is_open())
    {
        cerr << "could not open file" << endl;
        return RETURN_ERR;
    }

    string searchstr(argv[4]);
    cout << "searching for \"" << searchstr << "\"" << endl;

    string line;
    while(ifs.is_open())
    {
        getline(ifs, line);
        // cout << line << endl;


        // If end of file reached, clear the EOF to continue reading
        if (ifs.eof())
        {
            ifs.clear();

            // Don't use 100% CPU
            this_thread::sleep_for(chrono::milliseconds(20));
        }
        // If we "are there", finish up
        else if(line.find(searchstr) != string::npos)
        {
            cout << "Found string in \"" << line << "\"" << endl;
            return RETURN_STRFOUND;
        }

        // Check if log-writing process still exists
        if (kill(pid, 0) != 0)
        {
            cout << "PID " << pid << " is terminated" << endl;
            return RETURN_PIDKILLED;
        }

        // Check for timeout
        if (chrono::steady_clock::now() >= timeout)
        {
            cout << "timeout" << endl;
            return RETURN_TIMEOUT;
        }

    }

    cerr << "file was closed" << endl;
    return RETURN_ERR;
}

