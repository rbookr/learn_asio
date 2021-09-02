#pragma once

#include <array>
#include <memory>


#include "asio.hpp"
//TODO include other

//n


namespace http {
namespace server {
    
    class connection_manager;

    class connection : public std::enable_shared_from_this<connection>
    {
        public:
            connection(const connection &) = delete; // del copy construct
            connection& operator=(const connection &) = delete; //del copy assignment
            
            //Construct a connection with the given socket
            explicit connection(asio::ip::tcp::socket socket,
                        connection_manager & manager,
                        request_hander & hander);
    };
    


}
}
