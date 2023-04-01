#include "wastils/config.hpp"


int was::load_config(YAML::Node& node, const std::string& filename){
    try {
        node = YAML::LoadFile(filename);
        return 0;

    } catch(const YAML::BadFile& e) {
        std::cerr << e.msg << std::endl;
        return 1;
    } catch(const YAML::ParserException& e) {
        std::cerr << e.msg << std::endl;
        return 1;
    }
}
