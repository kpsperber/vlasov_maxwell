#include "SimulationTime.h"
#include "FileIO.h"
#include <iostream>
#include <string>

void Time::read(const std::string& filename) {
    FileIO::KeyValueMap kv;
    FileIO::read(filename, kv);

    if (kv.count("t0")) {
        t0 = std::stod(kv["t0"]);
    }
    if (kv.count("tf")) {
        tf = std::stod(kv["tf"]);
    }
    if (kv.count("dt")) {
        dt = std::stod(kv["dt"]);
    }
    if (kv.count("Nt")) {
        Nt = std::stoi(kv["Nt"]);
    }
    if (kv.count("writeInterval")) {
        writeInterval = std::stoi(kv["writeInterval"]);
    }
}

void Time::disp() const {
    std::cout << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << " Time Properties" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << t0 << "; t0 (initial time)" << std::endl;
    std::cout << tf << "; tf (final time)" << std::endl;
    std::cout << dt << "; dt (time step increment)" << std::endl;
    std::cout << Nt << "; Nt (number of time steps)" << std::endl;
    std::cout << writeInterval << "; writeInterval (write data every...)" << std::endl;
}

Time::Time() {
    time = t0;
    timeStep = 0;
}

Time::Time(const std::string& filename) {
    read(filename);
    time = t0;
    timeStep = 0;
}

void Time::advance() {
    timeStep = timeStep + 1;
    time = time + dt;
}

bool Time::run() const { return (timeStep < Nt); }

bool Time::write_now() const { return (timeStep % writeInterval == 0); }

double Time::get_dt() const { return dt; }
