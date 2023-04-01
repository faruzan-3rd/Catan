#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#include "yaml-cpp/yaml.h"
#include <iostream>

namespace was{

    int load_config(YAML::Node& node, const std::string& filename);

}


#endif
