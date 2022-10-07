#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <assert.h>
#include <sys/ioctl.h>

#define MAX_EVENTS 512

//char* serverIp = "172.31.17.87";
char* serverIp = "192.168.232.251";
int portNo = 9001;
int isLogOn = 0;

struct epoll_event ev;
struct epoll_event events[MAX_EVENTS];

int epfd;
int sockfd;

char html[] = 
"HTTP/1.1 200 OK\r\n"
"Server: F-Stack\r\n"
"Date: Sat, 25 Feb 2017 09:26:33 GMT\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 438\r\n"
"Last-Modified: Tue, 21 Feb 2017 09:44:03 GMT\r\n"
"Connection: keep-alive\r\n"
"Accept-Ranges: bytes\r\n"
"\r\n"
"<!DOCTYPE html>\r\n"
"<html>\r\n"
"<head>\r\n"
"<title>Welcome to F-Stack!</title>\r\n"
"<style>\r\n"
"    body {  \r\n"
"        width: 35em;\r\n"
"        margin: 0 auto; \r\n"
"        font-family: Tahoma, Verdana, Arial, sans-serif;\r\n"
"    }\r\n"
"</style>\r\n"
"</head>\r\n"
"<body>\r\n"
"<h1>Welcome to F-Stack!</h1>\r\n"
"\r\n"
"<p>For online documentation and support please refer to\r\n"
"<a href=\"http://F-Stack.org/\">F-Stack.org</a>.<br/>\r\n"
"\r\n"
"<p><em>Thank you for using F-Stack.</em></p>\r\n"
"</body>\r\n"
"</html>";

/*
char html[] =
"HTTP/1.1 200 OK\r\n"
"Server: F-Stack\r\n"
"Date: Sat, 25 Feb 2017 09:26:33 GMT\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 438\r\n"
"Last-Modified: Tue, 21 Feb 2017 09:44:03 GMT\r\n"
"Connection: keep-alive\r\n"
"Accept-Ranges: bytes\r\n"
"\r\n"
"<!DOCTYPE html>\r\n"
"<html>\r\n"
"<head>\r\n"
"<title>Welcome to nginx site 1!</title>\r\n"
"<style>\r\n"
"    body {\r\n"
"        width: 35em;\r\n"
"        margin: 0 auto;\r\n"
"        font-family: Tahoma, Verdana, Arial, sans-serif;\r\n"
"    }\r\n"
"</style>\r\n"
"</head>\r\n"
"<body>\r\n"
"<p>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa</p>\r\n"
"</body>\r\n"
"</html>";
*/

int loop(void *arg)
{
    /* Wait for events to happen */

    int nevents = epoll_wait(epfd,  events, MAX_EVENTS, 0);
    int i;

    for (i = 0; i < nevents; ++i) {
        if (events[i].data.fd == sockfd) { /* Request came to the listening fd, 
                                              handle new request. */
            while (1) {
                int nclientfd = accept(sockfd, NULL, NULL);
                if (nclientfd < 0) {
                    if (isLogOn)
                      printf("Error accepting new connection.");
                    break;
                }
                
                if (isLogOn)
                  printf("Accepting new connection at fd: %d\n", nclientfd);
                
                /* Add to event list */
                ev.data.fd = nclientfd;
                ev.events  = EPOLLIN;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, nclientfd, &ev) != 0) {
                    printf("ff_epoll_ctl failed:%d, %s\n", errno,
                        strerror(errno));
                    break;
                }
            }
        } else { /* Request came to a fd different from listening fd */
            if (events[i].events & EPOLLERR ) {
                /* Simply close socket */
                epoll_ctl(epfd, EPOLL_CTL_DEL,  events[i].data.fd, NULL);
                if (isLogOn)
                  printf("Error at fd %d, so closing it.\n", events[i].data.fd);
                
                close(events[i].data.fd);
            } else if (events[i].events & EPOLLIN) {
                char buf[256];
                size_t readlen = read(events[i].data.fd, buf, sizeof(buf));
                if(readlen > 0) {
                    if (isLogOn)
                      printf("Got request from client. Sending response to client fd: %d\n", events[i].data.fd);
                    
                    size_t writelen = write(events[i].data.fd, html, sizeof(html) - 1);
                    
                    if (isLogOn)
                      printf("Done writing %ld bytes \n", writelen);
                } else {
                    if (isLogOn)
                      printf("Did not get request from client. Delete and close the fd: %d\n", events[i].data.fd);
                    
                    epoll_ctl(epfd, EPOLL_CTL_DEL,  events[i].data.fd, NULL);
                    close( events[i].data.fd);
                }
            } else {
                printf("unknown event: %8.8X\n", events[i].events);
            }
        }
    }
}

int main(int argc, char * argv[])
{


	printf("Argc is %d\n", argc);



    if (argc != 4) {
      printf("Usage: %s portNo isLogOn(on=1,off=0)\n", argv[0]);
      exit(1);
    }

    portNo = atoi(argv[1]);
    //isLogOn is for printing exra debug info incase debug is needed
    isLogOn = atoi(argv[2]);
    serverIp = strdup(argv[3]);
    
    printf("Launching server on port %d and IP is %s\n", portNo, serverIp);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    
    printf("sockfd:%d\n", sockfd);
    if (sockfd < 0) {
        printf("Listen socket creation failed\n");
        exit(1);
    }
    
    int on = 1;
    ioctl(sockfd, FIONBIO, &on);
    // make_socket_non_blocking(sockfd);

    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(portNo);
    inet_aton(serverIp, &my_addr.sin_addr);
    bzero(&(my_addr.sin_zero), 8);

    int ret = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if (ret < 0) {
        printf("Bind failed, error code: %d \n", errno);
        exit(1);
    }

    ret = listen(sockfd, MAX_EVENTS);
    if (ret < 0) {
        printf("Listen failed\n");
        exit(1);
    }

    assert((epfd = epoll_create1(0)) > 0);
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    
    while(1) {
      //printf("Looping start. ");
      loop(NULL);
      //printf("Looping end.\n");
    }

    return 0;
}
