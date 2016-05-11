/*
            Classes (Constructors & methods) declared in this file

  Class Raft:
  Class that represents the running Raft server instance

  Class RC:
  Class that represents the running Regional Control (RC) entity.
  This will run a 'regular' host at startup but may later run
  on a AP after a RC failure.

  Class Core:
  The main class of the entire stack, it will check connectivity to all nodes
  and detect failures. It is also responsible for the running Raft intsance,
  APC and the RC. If this node becomes leader it is resposible for spawning the
  RC.
*/
 #define PORT 1153
 #define BUFLEN 2048

#include <getopt.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include "Impl/cc.h"

using namespace std;

/* RAFT */
Raft::Raft(int pid, int port, string address)
:m_pid (pid)
,m_port (port)
,m_address (address)
{
  std::cout << "im raft" << std::endl;

}
Raft::~Raft(){}

/* RC */
RC::RC(int pid, int port, string address)
:m_pid (pid)
,m_port (port)
,m_address (address)
{
  std::cout << "im rc" << std::endl;

}
RC::~RC(){}

/* APC */

//AF_INET (IP), AF_INET6 (IPv6), AF_UNIX (local channel, similar to pipes),
/* CORE */
Core::Core(bool is_bootstrap, Raft raft, RC rc, int argc, char** argv)
:m_raft(raft)
,m_rc(rc)
{
  std::cout << "im core" << std::endl;
  // TODO start components if not already started.


  // TODO if bootstrap start udp listener, for CORE to CORE comm.
  // TODO else connect to bootstrap.

  //int udpPort = 0;
  //string bootstrap_addr = "131.156.145.90";
  //thread t1(Core::connectivity, is_bootstrap, udpPort, bootstrap_addr);
  if (argc > 1) Core::udpClient (argc, argv);
  else Core::udpServer ();

  //t1.join();


}

void
Core::udpServer(){
  //uint16_t udpPort = port;
  //string bootstrap_addr = addr;

  struct sockaddr_in myaddr;            /* our address */
  struct sockaddr_in remaddr;           /* remote address */
  socklen_t addrlen = sizeof(remaddr);  /* length of addresses */
  int recvlen;                          /* # bytes received */
  int fd;                               /* our socket */
  unsigned char buf[BUFLEN];           /* receive buffer */

  /* create a UDP socket */
  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("cannot create socket\n");
    return;
  }

  /* bind the socket to any valid IP address and a specific port */
  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(PORT);

  if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
    perror("bind failed");
    return;
  }

  /* now loop, receiving data and printing what we received */
  for (;;) {
    printf("waiting on port %d\n", PORT);
    recvlen = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&remaddr, &addrlen);
    printf("received %d bytes\n", recvlen);
    if (recvlen > 0) { buf[recvlen] = 0;
      printf("received message: \"%s\"\n", buf); }
  }


}

void
Core::udpClient(int argc, char** argv){
  struct sockaddr_in serv_addr;
    int sockfd, slen=sizeof(serv_addr);
    char buf[BUFLEN];

    if(argc != 2)
    {
      printf("Usage : %s <Server-IP>\n",argv[0]);
      exit(0);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        perror("socket");

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_aton(argv[1], &serv_addr.sin_addr)==0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(1)
    {
        printf("\nEnter data to send(Type exit and press enter to exit) : ");
        scanf("%[^\n]",buf);
        getchar();
        if(strcmp(buf,"exit") == 0)
          exit(0);

        if (sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
            perror("sendto()");
    }

    close(sockfd);


}



















/*
int udpSocket;
if((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
  perror("cannot create socket");
  return 0;
}

struct sockaddr_in myaddr;
memset((char *)&myaddr, 0, sizeof(myaddr));
myaddr.sin_family = AF_INET;
myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
myaddr.sin_port = htons(udpPort);

if (bind(udpSocket, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
 perror("bind failed");
 return 0;
}

inet_pton(AF_INET, bootstrap_addr, &myaddr.sin_addr.s_addr);

 //send the message to server
if(sendto(udpSocket, s.c_str(), s.size(), 0, (struct sockaddr *)&myaddr, sizeof(myaddr))!=s.size()) {
  perror("Mismatch in number of bytes sent");
  exit(EXIT_FAILURE);
}

 //Receive the datagram back from server
int addrLength(sizeof(myaddr)),received(0);
char buffer[256] = {0};
if((received=recvfrom(udpSocket, buffer, 256, 0, (sockaddr *)&myaddr, (socklen_t*)&addrLength)) < 0) {
  perror("Mismatch in number of bytes received");
  exit(EXIT_FAILURE);
}
buffer[received]='\0';
std::cout << "Server (" << inet_ntoa(myaddr.sin_addr) << ") echoed: " << buffer << std::endl;
close(udpSocket);
return 0;
*/



/* Main */
int
main(int argc, char** argv)
{
  //parseArgs(argc, argv);
  bool is_bootstrap_node = true;

  if(is_bootstrap_node){
    Raft raft = Raft(0,0,"");
    RC rc = RC(0,0,"");
    Core(is_bootstrap_node, raft, rc, argc, argv);
  }else{
    //Core(is_bootstrap_node, raft, rc);
  }
  return 0;
}
