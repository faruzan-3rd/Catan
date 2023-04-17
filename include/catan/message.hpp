#ifndef __CTN_MESSAGE_HPP_
#define __CTN_MESSAGE_HPP_

#include <string>

typedef std::string str;

namespace ctn
{
    struct Message{
        str sender;
        int player;
        str verb; 
        str what;
        str context;
        str info;
    };
}

#endif