#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#include "yaml-cpp/yaml.h"
#include <iostream>

namespace was{

    // Returns 0 if success, 1 else
    int load_config(YAML::Node& node, const std::string& filename);

}


#endif
