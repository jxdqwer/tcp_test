#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <iostream>
#include <string.h>
#include <unistd.h>

#define PORT 36599
#define BACKLOG 1
#define MAX_MSG_SIZE 256

int main(void)
{
    char msg_buf[MAX_MSG_SIZE]={0};
    std::cout << "this is a thread message test use socket tcp" << std::endl;
    struct sockaddr_in test_ser_addr;
    struct sockaddr_in test_cli_addr;
    /* 设定服务端 */
    socklen_t addrlen=sizeof(struct sockaddr_in);    
    bzero(&test_ser_addr,addrlen);
    test_ser_addr.sin_family=AF_INET;
    test_ser_addr.sin_port=htons(PORT);
    test_ser_addr.sin_addr.s_addr= INADDR_ANY;

    int socket_test_fd=socket(AF_INET,SOCK_STREAM,0);
    bind(socket_test_fd,(struct sockaddr*)&test_ser_addr,sizeof(struct sockaddr_in));
    listen(socket_test_fd,BACKLOG);
    for(;;){
        int cli_socket_fd=accept(socket_test_fd,(struct sockaddr*) &test_cli_addr, &addrlen);
        if(cli_socket_fd<=0){
            printf("accept error");
        }
        else{
            recv(cli_socket_fd, msg_buf, (size_t)MAX_MSG_SIZE, 0); /* 接受数据*/    
            printf("received a connection from %s %d", inet_ntoa(test_cli_addr.sin_addr),test_cli_addr.sin_port);
            printf("%s\n",msg_buf);/*在屏幕上打印出来 */    
            strcpy(msg_buf," hi,I am server!");    
            send(cli_socket_fd, msg_buf, sizeof(msg_buf),0); /*发送的数据*/    
            close(cli_socket_fd);
        }
    }
    return 0;
}