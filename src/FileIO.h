#ifndef FILEIO_H
#define FILEIO_H

#include <map>
#include <string>
#include <vector>

namespace FileIO {

using KeyValueMap = std::map<std::string, std::string>;

/**
 * @brief Read csv file into map of key-value pairs.
 *
 * @param path Path to the csv file
 * @param out Map to store the key-value pairs
 * @return True if the file opened and was read
 */
bool read(const std::string& path, KeyValueMap& out);

} // namespace FileIO

#endif
