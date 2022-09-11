## 前言
最近想写一个restful api。不要想太多。想太多，拖一拖，就不想搞了。走一步，看一步，别停着。第一步想写的是tiny web server。我虽然知道点网络编成，翻看过《图解HTTP》，但是没有参考，两眼一摸黑。网上找了下参考代码，[《深入理解计算机》第十一章有个tiny web服务器](https://hansimov.gitbook.io/csapp/part3/ch11-network-programming/11.6-putting-it-together-the-tiny-web-server)的[demo代码](https://github.com/scauxun/Tiny-Web-Server)

代码没有什么难度，是最基本的[服务器示例](https://da1234cao.blog.csdn.net/article/details/105069496)。

写代码的时候，我改用[select函数](https://da1234cao.blog.csdn.net/article/details/125462501)来判断一个套接子是否可读。

为了使不同的套接子使用不同的缓冲区，我使用map数据结构。最开始，由于是C代码，所以github上找了一个[rxi/map](https://github.com/rxi/map)，但是不好用。C没有模板，想要写出通用的map很难。所以，代码写了一半，后面转用C++，好使用STL。所以代码乱糟糟的。

## 编译运行

```shell
# 服务端
mkdir build
cd build
cmake ..
make

# 浏览器访问：127.0.0.1：9999
```

## 待解决的问题

**代码中仅处理，GET请求的报文首部的请求行**。

有个不会处理的地方是：**服务端，如何判断已经接收到完整的请求**。

代码中使用了两个缓冲区，一个是IO关联的缓冲区，每次使用read系统调用，从套接字中读取内容到缓冲区；一个是用户缓冲区，从IO关联的缓冲区中读取内容。

理想的做法或许是这样：
* select检测到套接字可读。
* 读取内容，填入IO关联的缓冲区中。
* 判断IO缓冲区中的内容，是否为一个完整的请求。是的话，将IO缓冲区的内容，读取到用户缓冲区。
* 根据用户缓冲区中的内容，进行相应的处理。

关键是，如何判断，缓冲区的内容，是否为一个完整请求。

或者，换句话说，如何知道对方发送完毕，服务的可以处理接受到的信息，并进行回复。

不知道，看到其他参考代码有解决的再说。有了参考，再写代码。另外，以后**尽量不要用C写代码**，浪费生命。