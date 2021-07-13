/* 
 *      Socket
 *     +-----+
 *     |     |
 *     |     |
 *     +-----+
 *
 *     acceptor(Socket,handler)
 *
 *     +-----+               +----+
 *     |     |               |    |
 *     | O---+---------------+--O |
 *     +-----+               +----+
 *
 *    wirte(socket,buffer,handler)
 *
 *  1. 首先创建一个没有孔的插座
 *  2. acceptor 在插座上打一个孔，然后等待，直到一条线连接到这个也上
 *  3. 有连接了，可以通过 wirte,read等函数来对 socket 进行通信
 *      - wirte(socket,buffer)
 *      - read(socket,buffer)
 *  5. 一个socket 应该不能打多个孔，只能打一个孔
 *  6. 想要多个孔，就建立多个socket
 * */
#include <ctime>
#include <memory>
#include <iostream>
#include <string>
#include "asio.hpp"
const int port = 8089;
using asio::ip::tcp;

template<typename... Args>
void log(Args... args){
    ( std::cerr << ... << args) << '\n';
}

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

// 抽象出来的连接
// [Socket] ----> [another]
// 有一个插座和连线
class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
    public:
        using pointer = std::shared_ptr<tcp_connection>;
        static pointer create(asio::io_context &io){
            return pointer(new tcp_connection(io));
        }
        tcp::socket& socket(){
            return socket_;
        }
        void start(){
            log("开始发送数据......");
            message_ = make_daytime_string();
            asio::async_write(socket_, asio::buffer(message_),
                [ptr = shared_from_this(),this](const asio::error_code &ec,size_t len){ this->handle_wirte(ec, len); }
                );
        }
    private:
        tcp_connection(asio::io_context & io)
            :socket_(io)
        {
        }
        void handle_wirte(const asio::error_code &ec,size_t len){
            log("数据发送完成!");
        }
        tcp::socket socket_;
        std::string message_;
};



// server
// 功能：
// 启动的时候创建一个 acceptor
//
class tcp_server{
    public:
        tcp_server(asio::io_context &io)
            : io_context_(io),
            acceptor_(io,tcp::endpoint(tcp::v4(),port))
    {
        start_accept();
    }
    private:
        void start_accept(){
            tcp_connection::pointer new_connection = 
                tcp_connection::create(io_context_);

            log("等待连接......");
            acceptor_.async_accept(
                    new_connection->socket(),
                    [new_connection,this](const asio::error_code &ec){
                        this->handle_accept(new_connection, ec);
                    }
                    );
        }
        void handle_accept(tcp_connection::pointer new_connection,
                const asio::error_code & ec) {
            log("连接成功!");
            if( !ec)
                new_connection->start();
            start_accept();
        }
        asio::io_context &io_context_;
        tcp::acceptor acceptor_;
};

int main(){
    try {
        asio::io_context io;
        tcp_server server(io);
        io.run();
    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
