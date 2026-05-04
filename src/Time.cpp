#include "Time.h"
#include "FileIO.h"
#include <iostream>

// Helper Functions
void Time::read(const string& filename) {
    KeyValueMap kv;
    read(filename, kv);

    if (kv.count("t0")) {
        t0 = stod(kv["t0"]);
    }
    if (kv.count("tf")) {
        tf = stod(kv["tf"]);
    }
    if (kv.count("dt")) {
        dt = stod(kv["dt"]);
    }
    if (kv.count("Nt")) {
        Nt = stoi(kv["Nt"]);
    }
    if (kv.count("writeInterval")) {
        writeInterval = stoi(kv["writeInterval"]);
    }
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
    // disp();
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

