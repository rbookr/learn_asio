// asio保证 callback handlers 只会被在执行 io_context::run()的线程上执行
// 因为单线程执行 io_context::run() 不能让callback handlers 同步执行
//
// 我们可以使用 线程池 来运行io_context::run,但是我也需要同步的方法，来访问一些线程安全的资源
// 在同一个 strand 类中分发的 handler 只能一个接一个的执行
#include <cstdio>
#include <thread>
#include <chrono>
#include "asio.hpp"

class printer { 
    public:
        printer(asio::io_context & io)
            :strand_(asio::make_strand(io)),
            timer1_(io,std::chrono::seconds(1)),
            timer2_(io,std::chrono::seconds(2)),
            cnt_(0)
        {
            timer1_.async_wait(
                    asio::bind_executor(strand_,        //strand 保证 注册在此strand上的事件是一个一个执行的
                        [this](const asio::error_code &ec){ this->print1(); } ));
            timer2_.async_wait(
                    asio::bind_executor(strand_,
                        [this](const asio::error_code &ec){ this->print2(); } ));

        }
        ~printer(){
            printf("final cnt is %d\n",cnt_);
        }
        
        void print1(){
            if( cnt_ < 10){
                printf("Timer1 %d\n",cnt_);
                ++cnt_;
                timer1_.expires_at(timer1_.expiry()+std::chrono::seconds(1));
                timer1_.async_wait(asio::bind_executor(strand_, [this](const asio::error_code & ec){
                            this->print1();
                            }));
            }
        }
        void print2(){
            if( cnt_ < 10){
                printf("Timer2 %d\n",cnt_);
                ++cnt_;
                timer2_.expires_at(timer1_.expiry()+std::chrono::seconds(1));
                timer2_.async_wait(asio::bind_executor(strand_, [this](const asio::error_code & ec){
                            this->print2();
                            }));
            }
        }
    private:
        asio::strand<asio::io_context::executor_type> strand_;
        asio::steady_timer timer1_;
        asio::steady_timer timer2_;
        int cnt_;
};
int main(){
    asio::io_context io;
    printer p(io);
    std::thread t([&io]{ io.run(); });
    io.run();
    t.join();
    return 0;
}

