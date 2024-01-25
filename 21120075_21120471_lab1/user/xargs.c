#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_ARGV 32

int main(int argc, char *argv[]) {
  char line[512];
  char *cmd[MAX_ARGV];
  int cmd_argc = 0;
  int pid, status;
  
  if (argc < 2) {
    fprintf(2, "Usage: xargs command [args...]\n");
    exit(1);
  }
  for (int i = 1; i < argc; i++) {
    cmd[cmd_argc++] = argv[i];
  }

  while (1) {
    int i = 0;
    char c;
    int read_success = 0;
    while (read(0, &c, 1) > 0) {
      if (c == '\n') {
        line[i] = '\0';
        read_success = 1;
        break;
      }
      line[i++] = c;
    }

    if (!read_success) {
      break;
    }
    if ((pid = fork()) == 0) {
      cmd[cmd_argc] = line;
      cmd[cmd_argc + 1] = 0;
      exec(cmd[0], cmd);
      fprintf(2, "xargs: exec %s failed\n", cmd[0]);
      exit(1);
    } else if (pid < 0) {
      fprintf(2, "xargs: fork failed\n");
    }
    if (wait(&status) < 0) {
      fprintf(2, "xargs: wait failed\n");
    }
  }

  exit(0);
}
