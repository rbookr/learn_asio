#include <iostream>
#include <memory>
#include <asio.hpp>

#define  boost

class AsioIOServicePool
{
public:
    using IOService = boost::asio::io_context;
    using ExecutorType = boost::asio::io_context::executor_type;
    using Work = boost::asio::executor_work_guard<ExecutorType>;
    using WorkPtr = std::unique_ptr<Work>;

    // the constructor just launches some amount of threads
    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency())
        : ioServices_(size),
          works_(size),
          nextIOService_(0)
    {
        for (std::size_t i = 0; i < size; ++i)
        {
            works_[i] = std::unique_ptr<Work>(new Work(ioServices_[i]));
        }

        // one io_context per thread
        for (std::size_t i = 0; i < ioServices_.size(); ++i)
        {
            threads_.emplace_back([this, i] ()
                                  {
                                      ioServices_[i].run();
                                  });
        }
    }

    AsioIOServicePool(const AsioIOServicePool &) = delete;
    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;

    // 使用 round-robin 的方式返回一个 io_service
    boost::asio::io_service &getIOService()
    {
        auto &service = ioServices_[nextIOService_++];
        if (nextIOService_ == ioServices_.size())
        {
            nextIOService_ = 0;
        }

        return service;
    }

    void stop()
    {
        // Once the work object is destroyed, the service will stop.
        for (auto &work: works_) {
            work.reset();
        }
        for (auto &t: threads_) {
            t.join();
        }
    }

private:
    std::vector<IOService>       ioServices_;
    std::vector<WorkPtr>         works_;
    std::vector<std::thread>     threads_;
    std::size_t                  nextIOService_;
};
