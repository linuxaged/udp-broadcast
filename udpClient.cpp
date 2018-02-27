/* fpont 12/99 */
/* pont.net    */
/* udpClient.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */

//for Mac OS X
#include <stdlib.h>

#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100

class UdpSocket
{
public:
  UdpSocket() = default;
  ~UdpSocket() = default;

  void Open()
  {
    /* socket creation */
    sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd<0) {
      printf("cannot open socket \n");
      exit(1);
    }

    int broadcast = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1) {
            perror("setsockopt (SO_BROADCAST)");
            exit(1);
    }

    /* bind any port */
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(0);

    auto rc = bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr));
    if(rc<0) {
      printf("cannot bind port\n");
      exit(1);
    }
  }

  void InitRemoteAddress()
  {
      /* get server IP address (no check if input is IP address or DNS name */
      h = gethostbyname("255.255.255.255");
      if(h==NULL) {
        printf("unknown host\n");
        exit(1);
      }

      remoteAddr.sin_family = h->h_addrtype;
      memcpy((char *) &remoteAddr.sin_addr.s_addr,
       h->h_addr_list[0], h->h_length);
      remoteAddr.sin_port = htons(REMOTE_PORT);
  }

  void Close()
  {
    if ( sd > 0 )
    {
      close( sd );
    }
  }

  void Send()
  {
    /* send data */

    char buffer[] = {'h', 'e', 'l', 'l', 'o', '\0'};

    while ( true )
    {
      int rc = sendto(sd, buffer, 6, 0,
      (struct sockaddr *) &remoteAddr,
      sizeof(remoteAddr));

      if(rc<0) {
        continue;
      }
    }
  }

private:
  int sd = {-1};
  const int REMOTE_PORT = 1500;
  struct sockaddr_in localAddr;
  struct sockaddr_in remoteAddr;
  struct hostent *h;
};


int main(int argc, char *argv[]) {

  UdpSocket udpClient;

  udpClient.Open();
  udpClient.InitRemoteAddress();
  udpClient.Send();

  return 1;

}
