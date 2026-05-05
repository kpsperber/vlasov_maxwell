#include "FileIO.h"

#include <fstream>
#include <string>

namespace FileIO {

/** Trim whitespace from the beginning and end of a string. */
static void trim_inplace(std::string& s) {
    static constexpr char kWs[] = " \t\r";
    const std::size_t i = s.find_first_not_of(kWs);
    if (i == std::string::npos) {
        s.clear();
        return;
    }
    const std::size_t j = s.find_last_not_of(kWs);
    s = s.substr(i, j - i + 1);
}

/** Trim and strip trailing semicolons from the value field (e.g. 0.0;). */
static std::string strip_value(std::string v) {
    for (;;) {
        trim_inplace(v);
        if (v.empty() || v.back() != ';') {
            break;
        }
        v.pop_back();
    }
    return v;
}

void read(const std::string& path, KeyValueMap& out) {
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        trim_inplace(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        const std::size_t pos = line.find(',');
        std::string name = line.substr(0, pos);
        trim_inplace(name);
        out[name] = strip_value(line.substr(pos + 1));
    }
}

} // namespace FileIO
