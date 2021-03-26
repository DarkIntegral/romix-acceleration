//
//  Utils.hpp
//  validate_base
//
//  Created by Braiden Brousseau on 2021-03-26.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <iomanip>  // std::setw, std::setfill
#include <sstream>  // std::ostringstream
#include <string>   // std::string
#include <vector>   // std::vector


std::string bytes_to_hex(std::vector<uint8_t> bytes);
std::vector<std::string> bytes_to_hex(std::vector<std::vector<uint8_t>> data);

std::vector<uint8_t> hex_to_bytes(std::string hex);
std::vector<std::vector<uint8_t>> hex_to_bytes(std::vector<std::string> hex);


template <typename T>
std::vector<T> random_samples_flat(size_t l)
{
    std::vector<T> result;
    for(size_t i = 0; i < l; ++i)
        result.push_back((T)rand());
    return result;
}

template <typename T>
std::vector<std::vector<T>> random_samples_packed(size_t n, size_t l)
{
    std::vector<std::vector<T>> result;
    for(size_t i = 0 ; i < n; ++i){
        std::vector<T> row;
        for(size_t j = 0 ; j < l; ++j)
            row.emplace_back(rand());
        result.push_back(row);
    }
    return result;
}

template <typename T>
std::vector<std::vector<T>> pack_bytes(std::vector<T>& data, size_t len)
{
    assert(data.size() % len == 0);
    std::vector<std::vector<T>> result;
    
    for(size_t i = 0 ; i < data.size(); ++i)
    {
        if(i%len == 0) result.push_back(std::vector<T>());
        std::vector<T>& row = result.back();
        row.emplace_back(data[i]);
    }
    
    return result;
}

template <typename T>
std::vector<T> flatten_bytes(std::vector<std::vector<T>>& data)
{
    std::vector<T> result;
    for(auto& row: data)
    {
        for(auto& elem: row)
        {
            result.emplace_back(elem);
        }
    }
    return result;
}

#endif /* Utils_hpp */
