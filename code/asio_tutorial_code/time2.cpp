/* 
 * 本程序证明 io.run 是一个循环
 * 不停的从I/O execution context 尝试取
 * 一但从I/O execution context 队列的东西被取完，io.run 就结束
 * */
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
