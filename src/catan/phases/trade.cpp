#include "catan/phases/trade.hpp"

using namespace ctn::phases;
extern ctn::ProgressManager* prog_ptr;

void trade::on_enter_trade(){
    std::cout << "++++ TRADE ++++" << std::endl;
    prog_ptr->gamestate.resource_select = false;
}