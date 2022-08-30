#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

void SendMsg(int socket_fd,const char* msg)
{
  int length = strlen(msg) + 1;
  write(socket_fd,&length,sizeof(length));
  write(socket_fd,msg,length);
}
int main(int argc, char const *argv[]) {
  const char* const socket_name = argv[1];
  const char* const msg = argv[2];
  int socket_fd;
  struct sockaddr_un name;
  //创建套接字
  socket_fd = socket(PF_LOCAL,SOCK_STREAM,0);
  //套接字地址中存储服务器名称
  name.sun_family = AF_LOCAL;
  strcpy(name.sun_path,socket_name);
  //连接
  connect(socket_fd,(struct sockaddr*)&name,SUN_LEN(&name));
  //发送消息
  SendMsg(socket_fd,msg);
  close(socket_fd);
  return 0;
}
