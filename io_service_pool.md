
多个io_service_pool的使用
学习自 [A thread pool with Boost.Asio - 知乎](https://zhuanlan.zhihu.com/p/87390640)

- 每个线程都有一个io_service
- 每个线程都调用各自的io_service.run()方法
特点
- socket描述符不需要在线程间共享，所以不用同步，简单很多
- 在 event handler 中不能执行阻塞的操作，否则将会阻塞掉io_service所在的线程。
