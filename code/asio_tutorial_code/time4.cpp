#include <cstdio>
#include <chrono>
#include "asio.hpp"

class printer{
    public:
        printer(asio::io_context &io)
            :time_{io,std::chrono::seconds(1)},
            count_{0}
        {
            time_.async_wait([this](const asio::error_code & ec){
                        this->print();
                    });
        }
        ~printer(){
            printf("Final count is %d\n",count_);
        }

        void print(){
            if( count_ < 5){
                printf("count = %d\n",count_);
                ++count_;
                time_.expires_at(time_.expiry() + std::chrono::seconds(1));
                time_.async_wait([this](const asio::error_code  &ec){
                        this->print();
                });
            }
        }
    private:
        asio::steady_timer time_;
        int count_;
};
int main(){
    asio::io_context io;
    printer p(io);
    io.run();
    return 0;
}
