#include <cstdio>
#include <chrono>
#include "asio.hpp"

void print(const asio::error_code & ec){
    printf("hello world \n");
}
int main(){
    asio::io_context io;
    asio::steady_timer t(io,std::chrono::seconds(5));
    t.async_wait(&print); // 这个参数 加 &print和 print,结果都一样
    io.run();
    return 0;
}
