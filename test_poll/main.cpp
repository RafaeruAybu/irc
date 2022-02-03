#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int startup( int port )
{
    // 1. Create sockets
    int sock = socket(AF_INET,SOCK_STREAM,0);//The second parameter here represents TCP
    if( sock < 0 )
    {
    perror("socket fail...\n");
    exit(2);
    }

    // 2. When TIME_WAIT is resolved, the server cannot be restarted; the server can be restarted immediately.
    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);// Address of any type
    local.sin_port = htons(port);// The port number here can also be specified directly as 8080.
    // 3. Binding port number

    if( bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0 )
    {
    perror("bind fail...\n");
    exit(3);
    }

    // 4. Get a listening socket
    if( listen(sock,5) < 0 )
    {
    perror("listen fail...\n");
    exit(4);
    }
    return sock;
}

int main(int argc,char* argv[] )
{
    if( argc != 2 )
    {
    printf("Usage:%s port\n ",argv[0]);
    return 1;
    }

    // 1. Get a listening socket
    int listen_sock = startup(atoi(argv[1]));//Port numbers are passed in as strings and need to be converted to integers


    // 2. Initialization of Structures - List of Structures to Monitor
    struct pollfd fd_list[1024];
    int num = sizeof(fd_list)/sizeof(fd_list[0]);
    int i = 0;

    for(; i < num ; i++  )
    {
    fd_list[i].fd = -1;// File descriptor
    fd_list[i].events = 0;// Set of events to monitor
    fd_list[i].revents = 0;// Ready Event Set of Concerned Descriptors
    }


    // 3. Add read-only events for file descriptors of interest

     i = 0;
    for( ; i < num; i++ )
    {
    if( fd_list[i].fd == -1 )
    {
        fd_list[i].fd = listen_sock;
        fd_list[i].events = POLLIN;// Concern about Read-Only Events
        break;
    }
    }
    while(1)
    {

       //4. Start calling poll and wait for the file descriptor set of interest to be ready
       switch( poll(fd_list,num,3000) )
       {
       case 0:// The state of the denominator has exceeded before it has changed. timeout Time
           printf("timeout...\n");
           continue;
       case -1:// failed
           printf("poll fail...\n");
           continue;
       default: // Succeed
           {
           //   If it is a listener file descriptor, call accept to accept a new connection
           //   If it is a normal file descriptor, read is called to read the data
           int i = 0;
           for( ;i < num; i++ )
           {
               if( fd_list[i].fd == -1 )
               {
               continue;
               }
               if( fd_list[i].fd == listen_sock && ( fd_list[i].revents & POLLIN ))
               {
               // 1. Provide a connection acceptance service if the listening socket is ready to read

               struct sockaddr_in client;
               socklen_t len = sizeof(client);
               int new_sock = accept(listen_sock,(struct sockaddr *)&client,&len);
               if(new_sock < 0)
               {
                   perror("accept fail...\n ");
                   continue;
               }
               //After obtaining the new file descriptor, add the file descriptor to the array for the next time you care about the file descriptor
               int i = 0;
               for( ; i < num; i++ )
               {
                   if( fd_list[i].fd == -1 )//Place the first value in the array at - 1
                   break;
               }
               if( i < num )
               {
                   fd_list[i].fd= new_sock;
                   fd_list[i].events = POLLIN;
               }
               else
               {
                   close(new_sock);
               }
               printf("get a new link![%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
               continue;
               }

                //2. At this point, we are concerned with ordinary file descriptors.
            //   Provide services to read data at this time
            if( fd_list[i].revents & POLLIN  )
            {
                char buf[1024];
                ssize_t s = read(fd_list[i].fd,buf,sizeof(buf)-1);
                if( s < 0 )
                {
                printf("read fail...\n");
                continue;
                }
                else if( s == 0 )
                {
                printf("client quit...\n");
                close(fd_list[i].fd);
                fd_list[i].fd = -1;
                }
                else
                {
                buf[s] = 0;
                printf("client# %s\n",buf);
                //echo to all users, including the the source user.
                for (int i = 1; i < num; i++)
                {
                    if (fd_list[i].fd != -1)
                        write(fd_list[i].fd, buf, s);
                }
                }
            }
           }
           }
           break;
       }
    }
    return 0;
}
