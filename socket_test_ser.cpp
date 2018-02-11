#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <iostream>
#include <string.h>
#include <unistd.h>

#define PORT 36599
#define BACKLOG 2
#define MAX_MSG_SIZE 256

using namespace std;

class tcp_socket{
    public:
        struct sockaddr_in addr;
        socklen_t addrlen;
        int fd;
        void set_prot(uint16_t in){
            addr.sin_port = in;
        }
        void set_addr(in_addr_t ip){
            addr.sin_addr.s_addr = ip;
        }
        void init_socket(uint16_t port,in_addr_t ip){
            addrlen=sizeof(struct sockaddr_in);
            bzero(&addr,addrlen);
            addr.sin_family=AF_INET;
            set_prot(port);
            set_addr(ip);
            fd=socket(AF_INET,SOCK_STREAM,0);
            if(fd<0){/*创建失败 */    
                fprintf(stderr,"Socket错误:%s\n",strerror(errno));    
                exit(1);
            }
            bind(fd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
            listen(fd,BACKLOG);
        }
        
};

int main(void)
{
    char msg_buf[MAX_MSG_SIZE]={0};
    cout << "启动tcp测试程序" << endl;
    cout << "请输入本地监听端口号：" << endl;
    
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
            fprintf(stderr,"连接失败:%s\n",strerror(errno));
        }
        else{
            recv(cli_socket_fd, msg_buf, (size_t)MAX_MSG_SIZE, 0); /* 接受数据*/
            cout << "来自" << test_cli_addr.sin_port << " " << inet_ntoa(test_cli_addr.sin_addr) << "：" <<endl;
            cout << msg_buf << endl;
            strcpy(msg_buf," hi,I am server!");    
            send(cli_socket_fd, msg_buf, sizeof(msg_buf),0); /*发送的数据*/    
            close(cli_socket_fd);
        }
    }
    return 0;
}