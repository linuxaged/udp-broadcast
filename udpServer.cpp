/* fpont 12/99 */
/* pont.net    */
/* udpServer.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

//for Mac OS X
#include <stdlib.h>

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100

class UdpSocket
{
public:
  UdpSocket() = default;
  ~UdpSocket() = default;

  void Open()
  {
    /* socket creation */
  sd=socket(AF_INET, SOCK_DGRAM, 0);
  if(sd<0) {
    printf("cannot open socket \n");
    exit(1);
  }

  int broadcast = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1) {
          perror("setsockopt (SO_BROADCAST)");
          exit(1);
  }

  /* bind local server port */
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(LOCAL_SERVER_PORT);
  auto rc = bind (sd, (struct sockaddr *) &localAddr,sizeof(localAddr));
  if(rc<0) {
    printf("cannot bind port number\n");
    exit(1);
  }

  }

  void Close()
  {
    if ( sd > 0 )
    {
      close( sd );
    }
  }

  void Receive()
  {
    /* server infinite loop */
      while(1) {

        /* init buffer */
        memset(msg,0x0,MAX_MSG);

        /* receive message */
        socklen_t len = sizeof(remoteAddr);
        int n = recvfrom(sd, msg, MAX_MSG, 0,
         (struct sockaddr *) &remoteAddr, &len);

        if(n<0) {
          printf("cannot receive data \n");
          continue;
        }

        /* print received message */
        printf("from %s:UDP%u : %s \n",
         inet_ntoa(remoteAddr.sin_addr),
         ntohs(remoteAddr.sin_port),msg);

      }/* end of server infinite loop */
  }

private:
  int sd = {-1};
  const int REMOTE_PORT = 1500;
  struct sockaddr_in localAddr;
  struct sockaddr_in remoteAddr;
  struct hostent *h;
  char msg[MAX_MSG];
};


int main(int argc, char *argv[]) {

  UdpSocket udpServer;
  udpServer.Open();
  udpServer.Receive();

return 0;

}
