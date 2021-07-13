#include <cstdio>
#include <array>
#include <iostream>
#include <exception>
#include <string>
#include "asio.hpp"

using asio::ip::tcp;

const int port = 8089;

int main(int argc,char *argv[]){
    try{

        if(argc != 2){
            std::cerr << "Usage: cline <host> " << std::endl;
            return 1;
        }
        asio::io_context io;
        tcp::resolver resolver(io); //resolver -> 解析器,对endpoint进行解析
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], std::to_string(port));
        for( auto & e : endpoints){
            std::cout << e.service_name() << std::endl;
            std::cout << e.endpoint().address().to_string() << std::endl;
            std::cout << e.endpoint().port() << std::endl;
        }
        tcp::socket socket(io);
        asio::connect(socket, endpoints);
        std::cout <<std::endl << std::endl;
        std::cout << " connect success !" << std::endl;
        for(;;){ //不停的读取
            std::array<char, 128> buf;
            asio::error_code ec;
            size_t  len = socket.read_some(asio::buffer(buf),ec);
            if( ec == asio::error::eof)
                break;
            else if ( ec) {
                throw asio::system_error(ec);
            }
            std::cout << "read data success ! read data length = " << len ;
            std::cout << std::endl;
            std::cout.write(buf.data(), len);
        }
        printf("resolve success !");


    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
