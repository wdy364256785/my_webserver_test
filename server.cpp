#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

//持续读取消息，直到套接字关闭或接收到客户发送的“quit”消息
// 前者返回true,后者返回 false,服务器随后将停止服务

bool Serve(int client_socket)
{
  while (true) {
    int length;
    char* msg;
    //从套接字中读取文本消息的长度，返回值为0表示客户连接已关闭
    if(read(client_socket,&length,sizeof(length)) == 0)
      return true;
    msg = new char[length];
    read(client_socket,msg,length);
    std::cout<<msg<<std::endl;
    if(!strcmp(msg,"quit"))  {delete[] msg,msg = NULL;return false;}
    else delete[] msg, msg = NULL;
  }
}

int main(int argc,char* const argv[])
{
  const char* const socket_name = argv[1];
  ing socket_fd;
  struct sockaddr_un name;
  bool serving = true;
  //创建套接字
  socket_fd = socket(PF_LOCAL,SOCK_STREAM,0);
  //设定服务器性质
  name.sun_family = AF_LOCAL;
  strcpy(name.sun_path,socket_name);
  // 绑定套接字
  bind(socket_fd,(struct sockaddr*)&name,SUN_LEN(&name));
  //侦听客户连接
  listen(socket_fd,5);
  //重复接受连接，直到某个客户发出“quit”消息
  while(serving)
  {
    struct sockaddr_un client_name;
    socklen_t client_name_len;
    int client_socket_fd;
    //接受客户连接请求
    client_socket_fd = accept(socket_fd,(struct sockaddr*)&client_name,&client_name_len);
    serving = Serve(client_socket_fd); //服务连接请求
    close(socket_fd);//关闭客户连接
  }
  close(socket_fd);
  unlink(socket_name); //删除套接字文件
  return 0;
}
