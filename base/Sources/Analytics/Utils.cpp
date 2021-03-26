//
//  Utils.cpp
//  validate_base
//
//  Created by Braiden Brousseau on 2021-03-26.
//

#include "Utils.hpp"

std::string bytes_to_hex(std::vector<uint8_t> bytes)
{
    std::ostringstream oss;
    for(const uint8_t &b : bytes){
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(b);
    }
    std::string result = oss.str();
    return result;
}

std::vector<std::string> bytes_to_hex(std::vector<std::vector<uint8_t>> data)
{
    std::vector<std::string> results;
    for(auto& d : data) results.push_back(bytes_to_hex(d));
    return results;
}


std::vector<uint8_t> hex_to_bytes(std::string hex)
{
    // two bytes per hex char
    std::vector<uint8_t> bytes;
    
    for (unsigned int i = 0; i < hex.length(); i += 2) {
      std::string byteString = hex.substr(i, 2);
      char byte = (char) strtol(byteString.c_str(), NULL, 16);
      bytes.push_back(byte);
    }
    
    return bytes;
}

std::vector<std::vector<uint8_t>>  hex_to_bytes(std::vector<std::string> hex)
{
    // two bytes per hex char
    std::vector<std::vector<uint8_t>> result;
    
    for (auto& s: hex) {
        result.emplace_back(hex_to_bytes(s));
    }
    return result;
}




