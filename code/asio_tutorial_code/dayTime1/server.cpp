#include <ctime>
#include <exception>
#include <cstdio>
#include <string>
#include <iostream>
#include "asio.hpp"

using namespace std;

string make_daytime_string(){
    time_t now = time(0);
    return ctime(&now);
}
const int port = 8089;
int main(){
    try {

        //cout << make_daytime_string() << endl;
        asio::io_context io;
        using tcp = asio::ip::tcp;
        tcp::acceptor acceptor(io,tcp::endpoint(tcp::v4(),port));
        // tcp::v4() 表示接收任何来息 ipv4 地址发起的请求
        // doc :https://think-async.com/Asio/asio-1.18.2/doc/asio/reference/ip__tcp/endpoint.html
        // constructor(ipaddress,port)
        std::cout << "server start !" << std::endl;

        // iterative
        for(;;){
            tcp::socket socket(io); //创建一个socket,插头
            acceptor.accept(socket); //使插头 有能接收的能力
                                    // 阻塞，直到连接
            std::cout << "connect success !" << std::endl;
            // 等待连接
            // Accept a new connection.
            string message = make_daytime_string();
            asio::error_code ignored_error;
            // 向socket 写入
            // socket 是可读可写的
            //The asio::buffer function is used to create a buffer object to represent raw memory, an array of POD elements, a vector of POD elements, or a std::string.
            std::cout << "start write data to socket " << std::endl;
            asio::write(socket,asio::buffer(message),ignored_error);
        }
    }
    catch (std::exception &e ){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
