#ifndef FILEIO_H
#define FILEIO_H

#include <map>
#include <string>

namespace FileIO {

using KeyValueMap = std::map<std::string, std::string>;

/**
 * @brief Read csv file into map of key-value pairs.
 *
 * @param path Path to the csv file
 * @param out Map to store the key-value pairs
 */
void read(const std::string& path, KeyValueMap& out);

} // namespace FileIO

#endif
