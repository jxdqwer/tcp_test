#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <iostream>
#include <thread>
#include <mutex>
#include <string.h>
#include <unistd.h>

#define BACKLOG 2
#define MAX_MSG_SIZE 256

//pthread_mutex_t mutex; /* 初始化互斥锁 */

using namespace std;
mutex cli_lock;

class tcp_socket{
    public:
        struct sockaddr_in addr;
        socklen_t addr_len;
        int fd;
        void init_socket(uint16_t port,in_addr_t ip){
            addr_len=sizeof(struct sockaddr_in);
            bzero(&addr,addr_len);
            addr.sin_family=AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = ip;
            fd=socket(AF_INET,SOCK_STREAM,0);
            if(fd<0){/*创建失败 */    
                fprintf(stderr,"Socket错误:%s\n",strerror(errno));    
                exit(1);
            }
            if(bind(fd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))!=0)
            {
                fprintf(stderr,"bind错误:%s\n",strerror(errno));    
                exit(1);
            }
            if(listen(fd,BACKLOG)!=0)
            {
                fprintf(stderr,"listen错误:%s\n",strerror(errno));    
                exit(1);
            }
        }
        void close_socket(){
            close(fd);
        }
};



void sub_server(int fd)
{
    int recv_len = 0;
    char msg[MAX_MSG_SIZE] = "";// 接收缓冲区  
    // 解锁，pthread_mutex_lock()唤醒，不阻塞  
    cli_lock.unlock();
    
    // 接收数据  
    while((recv_len = recv(fd, msg, sizeof(msg), 0)) > 0)
    {  
        printf("recv_buf: %s\n", msg); // 打印数据  
        send(fd, msg, recv_len, 0); // 给客户端回数据  
    }  
      
    printf("client closed!\n");  
    close(fd);  //关闭已连接套接字  
}


int main(void)
{
    in_port_t port = 0;
    tcp_socket ser;
    cout << "请输入端口：" <<endl;
    std::cin >> port;
    ser.init_socket(port,htonl(INADDR_ANY));
    cout << "TCP服务器启动成功！" <<endl;
    pthread_t pthread_id;
    while(1){
        struct sockaddr_in cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        cli_lock.lock();
        int con_fd = accept(ser.fd, (struct sockaddr*)&cli_addr, &cli_addr_len);
        if(con_fd < 0)  
        {  
            fprintf(stderr,"accept错误:%s\n",strerror(errno));
            continue;  
        }
        cout << "cli ip:" << inet_ntoa(cli_addr.sin_addr) << " cli prot:" << cli_addr.sin_port << endl;
        if(con_fd > 0)
        {
            thread cli(sub_server,con_fd);
            cli.detach();
        }
    }
    ser.close_socket();
    return 0;
}