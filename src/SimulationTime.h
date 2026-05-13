#ifndef SIMULATION_TIME_H
#define SIMULATION_TIME_H

#include <fstream>
#include <string>

/**
 * @brief Time management data structure
 */
class SimulationTime {
    private:
        double t0, tf, dt;
        int Nt, writeInterval;
    
    public:
        double time;
        int timeStep;

        SimulationTime();
        SimulationTime(const std::string& filename);
        void read(const std::string& filename);
        void disp() const;
        void advance();
        bool run() const;
        bool write_now() const;
        double get_dt() const;

};

#endif // SIMULATION_TIME_H
