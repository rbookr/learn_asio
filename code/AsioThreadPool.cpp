//来自知乎的一篇文章
// [A thread pool with Boost.Asio - 知乎](https://zhuanlan.zhihu.com/p/87390640)
#include "asio.hpp"
#include <memory>

class AsioIOContextPool {
    public:
        using IOContext    = asio::io_context;
        using ExecutorType = asio::io_context::executor_type;
        using Work         = asio::executor_work_guard<ExecutorType>;
        using WorkPtr      = std::unique_ptr<Work>;
};
