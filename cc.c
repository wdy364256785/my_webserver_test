#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int c_fd;
	int n_read;
	char readBuf[128];
	char msg[128] = {0};
	
	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(struct sockaddr_in));	//数据初始化
	
	if(argc != 3){
		printf("参数输入错误\n");
		exit(-1);
	}
	
	
	//1.socket
	c_fd = socket(AF_INET,SOCK_STREAM, 0);	//创建套接字 
	if(c_fd == -1){
		perror("socket");
		exit(-1);
	}
	
	c_addr.sin_family = AF_INET;	//协议 
	c_addr.sin_port = htons(atoi(argv[2]));	//端口号 
	inet_aton(argv[1], &c_addr.sin_addr);	//IP地址 
	//2.connect
	if(connect(c_fd, (struct sockaddr *)&c_addr, sizeof(struct sockaddr)) == -1){
		perror("connect");
		exit(-1);
	}
	
	while(1){
		if(fork() == 0){
			while(1){
				memset(msg, 0, sizeof(msg));
				printf("请输入消息\n");
				gets(msg);
				//3.write
				write(c_fd, msg, strlen(msg));
			}
		}
		
		while(1){
			memset(readBuf, 0, sizeof(readBuf));
			//4.read
			n_read = read(c_fd, readBuf, 128);
			if(n_read == -1){
				perror("read");	
			}else{
				printf("来自服务端获取的消息：%d, %s\n",n_read,readBuf);
			}
		}
	}

		
	return 0;
}
