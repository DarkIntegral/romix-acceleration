//
//  main.cpp
//  validate_base
//
//  Created by Braiden Brousseau on 2021-03-26.
//

#include <iostream>     //std::cout
#include <vector>       //std::vector
#include "Utils.hpp"
#include "RomixCpu.hpp"
#include "RomixValidator.hpp"





int main(int argc, const char * argv[]) {
    

    RomixValidator validator;
    RomixCpu romixCpu;
    
    validator.validate_static(romixCpu, "romixCpu");

    return 0;
}
