# netradio


Server端以多播的方式发UDP包，Client端选择接收的数据。
Server端和Client端利用socket通讯，网络层IPV4协议，传输层UDP协议。
Client端fork子进程播放流媒体，利用pipe实现父子进程通讯。
利用Linux系统文件系统作为媒体库，多线程并发发送不同频道内容，流量控制。