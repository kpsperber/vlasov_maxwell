#include "Time.h"

#include <iostream>

// Helper Functions
void Time::read(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    string line;
    while(getline(file, line)) {
        auto pos = line.find(',');
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        if (!value.empty() && value[0] == ' ') value.erase(0, 1);
        
        if (key == "t0") {t0 = stoi(value);}
        if (key == "tf") {tf = stoi(value);}
        if (key == "dt") {dt = stof(value);}
        if (key == "Nt") {Nt = stof(value);}
        if (key == "writeInterval") {writeInterval = stof(value);}
    }

    file.close();
}

void Time::disp() const {
    cout << endl;
    cout << string(50, '=') << endl;
    cout << " Time Properties" << endl;
    cout << string(50, '=') << endl;
    cout << t0 << "; t0 (initial time)" << endl;
    cout << tf << "; tf (final time)" << endl;
    cout << dt << "; dt (time step increment)" << endl;
    cout << Nt << "; Nt (number of time steps)" << endl;
    cout << writeInterval << "; writeInterval (write data every...)" << endl;
}

// Constructors
Time::Time() {
    time = t0;
    timeStep = 0;
}

Time::Time(const string& filename) {
    read(filename);
    disp();
    time = t0;
    timeStep = 0;
}

// Other helpful guys
void Time::advance() {
    timeStep = timeStep+1;
    time = time + dt;
}

bool Time::run() const { return (timeStep < Nt);}

bool Time::write_now() const { return (timeStep % writeInterval == 0);}

double Time::get_dt() const { return dt; }

