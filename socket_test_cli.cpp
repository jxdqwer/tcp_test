#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <iostream>
#include <string.h>
#include <unistd.h>

#define SER_PORT 36599
#define CLI_PORT 36590
#define BACKLOG 1
#define MAX_MSG_SIZE 256

int main(void)
{
    std::cout << "this is a thread message test use socket tcp" << std::endl;
    char msg_buf[MAX_MSG_SIZE]={0};
    struct sockaddr_in test_ser_addr;
    struct sockaddr_in test_cli_addr;

    int socket_test_cli_fd=socket(PF_INET,SOCK_STREAM,0);

    if(socket_test_cli_fd<0){/*创建失败 */    
        fprintf(stderr,"socker Error:%s\n",strerror(errno));    
        exit(1);    
    }    

    /* 初始化服务端地址 */
    socklen_t addrlen=sizeof(struct sockaddr_in);    
    bzero(&test_ser_addr,addrlen);
    test_ser_addr.sin_family=AF_INET;
    test_ser_addr.sin_port=htons(SER_PORT);
    test_ser_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    /* 初始化客户端地址 */
    bzero(&test_cli_addr,addrlen);
    test_cli_addr.sin_family=AF_INET;
    test_cli_addr.sin_port=htons(CLI_PORT);
    test_cli_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bind(socket_test_cli_fd,(struct sockaddr*)&test_cli_addr,sizeof(struct sockaddr_in));
    if(connect(socket_test_cli_fd,(struct sockaddr*)&test_ser_addr, addrlen)!=0)/*请求连接*/    
    {    
        /*连接失败 */    
        printf("Connect Error\n");    
        close(socket_test_cli_fd);
        exit(1);
    }
    strcpy(msg_buf," hi,I am client!");
    
    send(socket_test_cli_fd, msg_buf, sizeof(msg_buf),0);
    recv(socket_test_cli_fd, msg_buf, MAX_MSG_SIZE,0); /* 接受数据*/    
    printf("%s\n",msg_buf);/*在屏幕上打印出来 */ 
    close(socket_test_cli_fd);

    return 0;
}