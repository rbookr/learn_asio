#include <cstdio>
#include <chrono>
#include "asio.hpp"

void print(const asio::error_code & ec,asio::steady_timer *t,int *count){
    //printf("in print &count = %llx\n",count);
    if( *count < 5){
        printf("*count = %d\n",*count);
        ++(*count);
        //修改了新的到期时间
        t->expires_at(t->expiry() + asio::chrono::seconds(1));
        //= 传值补获，得到了原始的count的地址
        auto f= [=](const asio::error_code & ec){
                    //printf("hello\n");
                    //printf("&count = %llx\n",(int *)count);
                    print(ec,t,(int *)count);
                };
        //f(ec);
        t->async_wait(f);
    }
}

int main(){
    int count = 0;
    asio::io_context io;
    asio::steady_timer t(io,std::chrono::seconds(1));
    //printf("in main &count = %llx\n",&count);
    t.async_wait([&](const asio::error_code &ec){ // 一定要用const
                print(ec,&t,&count);
            });
    io.run();
    return 0;
}

