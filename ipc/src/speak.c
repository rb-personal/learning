#include "ipc.h"

static char const * const FIFO_NAME = "american_maid";

int main
(void)
{
  char s[300];
  int num, fd;

  // Create the FIFO
  mknod(FIFO_NAME, S_IFIFO | 0666, 0);
  printf("Waiting for readers\n");

  // Open thie FIFO -- this will block till someone else connects as a reader
  fd = open(FIFO_NAME, O_WRONLY);
  printf("Got a reader -- type some stuff\n");

  while (fgets(s, 300, STDIN_FILENO) != NULL) {
    if ((num = write(fd, s, strlen(s))) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    printf("Speak: wrote %d bytes\n", num);
  }
  return 0;
}
