#include <iostream>
#include <chrono>
#include "asio.hpp"

using namespace std;

int main(){
    asio::io_context io;
    //asio::steady_timer t(io,std::chrono::seconds(5));
    asio::steady_timer t(io,asio::chrono::seconds(5)); // 没有找到asio::chrono的文档
    t.wait();
    std::cout << "hello world after time left 5 sec!" << std::endl;
    return 0;
}
