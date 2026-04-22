#ifndef TIME_H
#define TIME_H

#include <fstream>
using namespace std;

class Time {
    private:
        double t0, tf, dt;
        int Nt, writeInterval;
    
    public:
        double time;
        int timeStep;

        Time();
        Time(const string& filename);
        void read();
        void disp() const;
        void advance();
        bool run() const;
        bool write_now() const;
        double get_dt() const;

};

#endif