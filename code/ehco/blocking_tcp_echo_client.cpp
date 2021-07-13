//https://think-async.com/Asio/asio-1.18.2/src/examples/cpp11/echo/blocking_tcp_echo_client.cpp

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "asio.hpp"

using asio::ip::tcp;

enum { max_length = 1024 };

template<typename... Args>
void log(Args... args){
    ( std::cout << ... << args) << "\n";
}


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;        //创建io_context

    tcp::socket s(io_context);  // 创建socket
    tcp::resolver resolver(io_context); //解析
    asio::connect(s, resolver.resolve(argv[1], argv[2])); // 连接

    log(">>>> 连接server 成功！");

    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length); // 得到一行数据
    size_t request_length = std::strlen(request);

    log(">>>> 开始写入server！");
    asio::write(s, asio::buffer(request, request_length));
    log(">>>> 写入server完成！");

    char reply[max_length];
    log(">>>> 开始从server读取！");
    size_t reply_length = asio::read(s,
        asio::buffer(reply, request_length));
    log(">>>> 读取数据长度为 ",reply_length);
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
