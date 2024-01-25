#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main() {
  int p2c[2]; 
  int c2p[2];

  pipe(p2c);
  pipe(c2p);
  
  int pid = fork();

  if(pid == 0) { // child
    
    close(p2c[1]); 
    close(c2p[0]);

    char buf[100];
    
    read(p2c[0], buf, sizeof(buf));
    printf("%d: received ping\n", getpid());

    write(c2p[1], "pong", strlen("pong"));

    exit(0);

  } else { // parent

    close(p2c[0]);
    close(c2p[1]);

    write(p2c[1], "ping", strlen("ping"));
    
    char buf[100];
    read(c2p[0], buf, sizeof(buf));
    printf("%d: received %s\n", getpid(), buf);

    exit(0);
  }
}
    
