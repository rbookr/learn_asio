// copy write from https://think-async.com/Asio/asio-1.18.2/src/examples/cpp11/echo/async_tcp_echo_server.cpp
#include <iostream>
#include <memory>
#include <string>
#include "asio.hpp"

template<typename... Args>
void log(Args... args){
    ( std::cout << ... << args) << "\n";
}

using asio::ip::tcp;

//会话
class session : public std::enable_shared_from_this<session> {
    public:
        session(tcp::socket socket)
            :socket_{std::move(socket)} //这里用了 move 构造
        {
        }
        void start(){
            do_read();
        }

    private:
        void do_read(){
            auto self = shared_from_this(); //得到一个shared_ptr
            // 如果 session 是一个对象而不是一个指针
            // 因为我们执行的是异步的，所以在handler还没有开始的时候，session对象
            // 就可能已经析构了，
            // 所以必须要用 shared_ptr 来自动管理sesion对象的生命周期
            socket_.async_read_some(asio::buffer(data_,max_length),
                    [this,self](const std::error_code &ec,std::size_t length){
                    //TODO
                        if(!ec || ec === std::error::eof ){
                            log(">>>> 读取数据完毕!");
                            do_write(length);
                        }
                        else 
                            log("read data error :",ec.message()," ",ec.value());
                    });
        }

        void do_write(std::size_t length){
            auto self(shared_from_this());
            asio::async_write(socket_, asio::buffer(data_,length),
                    [this,self](const std::error_code &ec,std::size_t /*not use*/){
                        log(">>>> 读取数据完毕!");
                        if(!ec){
                            do_read();
                        }
                        else 
                            log("wirte data error :",ec.message(),ec.value());
                    });
        }
        tcp::socket socket_;
        enum {max_length = 1024};
        char data_[max_length];
};

class server {
    public:
        server(asio::io_context & io,short port)
            :acceptor_(io,tcp::endpoint(tcp::v4(),port)),
            port_(port)
        { do_accept(); }
    private:
        void do_accept(){
            // 这个async的特点
            // 1. 只有一个handler
            // 2. 无socket
            // 3. handler的第二个参数，会给它一个socket,这个就会用自己创建一个socket了
            // 文档： https://think-async.com/Asio/asio-1.18.2/doc/asio/reference/basic_socket_acceptor/async_accept/overload3.html
            log("server start! wait for connect at port : ",port_);
            acceptor_.async_accept(
                [this](const std::error_code &ec,tcp::socket socket){
                    log("client connected! wait for writing some data to client!");
                    if(!ec){
                        std::make_shared<session>(std::move(socket))->start();
                    }
                });
        }

        //members
        tcp::acceptor acceptor_;
        const int port_;
};

int main(int argc,char * argv[]){
    try {
        if(argc !=2 ){
            log("Usage: async_tcp_echo_server <port> ");
            return 1;
        }
        asio::io_context io;
        server s(io,std::atoi(argv[1]));
        io.run();
    }
    catch(std::exception & e){
        std::cerr << " Exception : " << e.what() << "\n";
    }
    
    return 0;
}
