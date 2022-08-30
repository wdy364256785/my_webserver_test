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
	int s_fd;
	int c_fd;
	int n_read;
	char readBuf[128];
	char msg[128] = {0};
	
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	memset(&s_addr, 0, sizeof(struct sockaddr_in));	//数据初始化 
	memset(&c_addr, 0, sizeof(struct sockaddr_in));	//数据初始化
	
	if(argc != 3){
		printf("参数输入错误\n");
		exit(-1);
	}
	
	//1.socket
	s_fd = socket(AF_INET,SOCK_STREAM, 0);	//创建套接字 
	if(s_fd == -1){
		perror("socket");
		exit(-1);
	}
	
	s_addr.sin_family = AF_INET;	//协议 
	s_addr.sin_port = htons(atoi(argv[2]));	//端口号 
	inet_aton(argv[1], &s_addr.sin_addr);	//IP地址 
	//2.bind
	bind(s_fd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in));	//绑定 
	
	//3.listen
	listen(s_fd, 10);	//监听10个连接 
	
	//4.accept
	int clen = sizeof(struct sockaddr_in);
	
	while(1){
		c_fd = accept(s_fd, (struct sockaddr *)&c_addr, &clen);	
		if(c_fd == -1){
			perror("accept");
		}
		
		
		printf("获取连接：%s",inet_ntoa(c_addr.sin_addr));
		
		if(fork() == 0){
			
			if(fork() == 0){
				while(1){
					memset(msg, 0, sizeof(msg));
					printf("请输入消息\n");
					gets(msg);
					//5.write
					write(c_fd, msg, strlen(msg));
				}
			}
		
			//6.read
			while(1){
				memset(readBuf, 0, sizeof(readBuf));
				n_read = read(c_fd, readBuf, 128);
				if(n_read == -1){
					perror("read");	
				}else{
					printf("获取的消息：%d, %s\n",n_read,readBuf);
				}
			} 
			break;
		}	
	}
	
	return 0;
}
