
## asio_tutorial_code

下面的代码我抄写自 [Asio Tutorial](https://think-async.com/Asio/asio-1.18.2/doc/asio/tutorial.html) 部分代码有改变，主要是把bind改成lambda，`asio::chrono` 改成`std::chrono`

### steady_timer 延迟

- [time1 延迟](./code/asio_tutorial_code/time1.cpp)
- [time2 async 延迟](./code/asio_tutorial_code/time2.cpp)
- [time3 async 延迟,循环五次](./code/asio_tutorial_code/time3.cpp)
- [time4 async 延迟,循环五次,async_handler 为类的成员函数](./code/asio_tutorial_code/time4.cpp)
- [time5 asio::bind_executor(strand,xx),strand保证调用按顺序执行](./code/asio_tutorial_code/time5.cpp)

### 时间服务器

- [同步的时间client](./code/asio_tutorial_code/dayTime1/client.cpp)
- [同步的时间server](./code/asio_tutorial_code/dayTime1/server.cpp)
- [异步，类封装的时间server](./code/asio_tutorial_code/dayTime1/server.cpp)

学会了
 - 什么是socket
 - 要想使用socket，先要创建一个accpetor
   - acceptor 需要知道接收的ip有哪些，和在哪个端口上接收,这两个数据被封装在一个 endpoint 类中
   - `asio::connect` 需要 endpoints,也可能只要一个 endpoint
   - 根据字面值，如`localhost,myhostname`等来得到直接endpoint,需要一个`resolver.resolve`
   - `asio::connect` 连接到`acceptor.accept` 上 
   - `asio::write,asio::write_some,asio::read,asio::read_some` 显然这些函数都是发生在`连接`之后

下面是我抽象出的相关概念

```plaintext
    Socket
   +-----+
   |     |
   |     |
   +-----+

   acceptor(Socket,handler)
   server-socket                             client-socket
   +-----+                                   +-----+
   |     |    wirte(socket,buffer,handler)   |     |
   | O---+-----------------------------------+--O  | asio::connect(client-socket,endpoints)
   +-----+     read(socket,buffer,handler)   +-----+
   acceptor(io,endpoints).accept(server-socket)

  
  

1. 首先创建一个没有孔的插座,显然server,client都要创建socket
2. acceptor 在插座上打一个孔，然后等待，直到一条线连接到这个也上
3. 有连接了，可以通过 wirte,read等函数来对 socket 进行通信
    - wirte(socket,buffer)
    - read(socket,buffer)
5. 一个socket 应该不能打多个孔，只能打一个孔
6. 想要多个孔，就建立多个socket
```
