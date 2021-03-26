//
//  RomixValidator.cpp
//  validate_base
//
//  Created by Braiden Brousseau on 2021-03-26.
//

#include "RomixValidator.hpp"


RomixValidator::RomixValidator() {
}


/**
 Validate a Romix compute object agaist a fixed set of known test cases.
 */
void RomixValidator::validate_static(RomixBase& romix, std::string tag) {
    
    std::cout << "Statically validating " + tag << ":" << std::endl;
    for(auto& p : mTestVectors)
    {
        std::string& input = p.first;
        std::string& output = p.second;
        
        auto bytes = hex_to_bytes(input);
        romix.mix(bytes.data(), 1);
        auto hex = bytes_to_hex(bytes);
        
        assert(output.compare(hex) == 0);
    }
    std::cout << "\tsuccess" << std::endl;
}

/**
 Validate a Romix compute object agaist a fixed set of known test cases.
 */
std::vector<uint8_t> RomixValidator::generate_random_blocks(size_t block_size, size_t n) {
    
    std::vector<uint8_t> data;
    for(size_t i = 0 ; i < block_size * n ; i++)
    {
        data.push_back(rand());
    }
    return data;
}
