// ipc.c

#include "ipc.h"

// fork
void exercise0
(void)
{
  pid_t pid;
  int rv;

  switch (pid = fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);

  case 0:
    printf(" CHILD: This is the child process !\n");
    printf(" CHILD: My PID is %d\n", getpid());
    printf(" CHILD: My parent's PID is %d\n", getppid());
    printf(" CHILD: Enter my exit status (make it small): ");
    scanf(" %d", &rv);
    printf(" CHILD: Exiting with return value %d\n", rv);
    exit(rv);

  default:
    printf(" PARENT: This is the parent process !\n");
    printf(" PARENT: My PID is %d\n", getpid());
    printf(" PARENT: My child's PID is %d\n", pid);
    wait(&rv);
    printf(" PARENT: Child's exit status %d\n", WEXITSTATUS(rv));
  }
}

// sigaction
void sigint_handler
(int sig)
{
  write(0, "Ahhh! SIGINT!\n", 14);
}
void exercise1
(void)
{
  printf("Sigaction exercise\n");
  struct sigaction sa;

  sa.sa_handler = sigint_handler; // Signal handler function (SIG_IGN) to ignore signal
  sa.sa_flags   = 0;              // Flags to modify the behavior of the handler
  sigemptyset(&sa.sa_mask);       // Set of signals to block while this guy is handled (sigemptyset(), sigfillset() etc.)

  if (sigaction(SIGINT, &sa, NULL) == -1) { // Register the signal handler using sigaction (not signal)
    perror("sigaction failed");
    exit(EXIT_FAILURE);
  }

  printf("Sleeping (hint, hint..)\n");
  sleep(30);
}

// manipulating shared data
volatile sig_atomic_t got_usr1;
void sigusr1_handler
(int sig)
{
  got_usr1 = 1;
}
void exercise2
(void)
{
  got_usr1 = 0; // atomic update to global, shared memory

  struct sigaction sa;
  sa.sa_handler = sigusr1_handler;
  sa.sa_flags = SA_RESTART; // Restart interrupted system calls (which can be interrupted + restarted)
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction failed");
    exit(EXIT_FAILURE);
  }

  while(!got_usr1){
    printf("Pid %d is working\n", getpid());
    sleep(1);
  }

  printf("Done in by SIGUSR1\n");
}

// pipe()
void exercise3
(void)
{
  int pfds[2];
  char buf[30];

  if (pipe(pfds) == -1) { // pipe() populates the 2 element array with the read (0) and write (1) file descriptors
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  printf("writing to file descriptor #%d\n", pfds[1]);
  write(pfds[1], "test", 5);
  printf("reading from file descript #%d\n", pfds[0]);
  read(pfds[0], buf, 5);
  printf("read \"%s\"\n", buf);
}

// fork() and pipe()
void exercise4
(void)
{
  int pfds[2];
  char buf[30];

  if (-1 == pipe(pfds)) {
    perror("pipe failed");
    exit(EXIT_FAILURE);
  }

  int pid = fork();
  if(pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (0 == pid) {
    // child
    printf("CHILD: writing to the pipe\n");
    write(pfds[1], "test", 5);
    printf("CHILD: exiting\n");
    exit(0);
  }
  printf("PARENT: reading from pipe\n");
  read(pfds[0], buf, 5);
  printf("PARENT: read \"%s\"\n", buf);
  wait(NULL); // wait for child}
}

// implement ls | wc -l
void exercise5
(void)
{
  int pfds[2];
  pipe(pfds);

  if (!fork()) {
    // child
    // close normal stdout
    close(1);

    // make stdout the same as pfds[1] (write fd)
    // **NOTE** dup will replace the first available file descriptor which is stdout b/c we just closed it
    dup(pfds[1]);

    // close the read file descriptor
    close(pfds[0]);

    //
    execlp("ls", "ls", NULL);
  } else {
    // parent
    // close normal stdin
    close(0);

    // make stdin the same as pfds[0] (read fd)
    // **NOTE** again, this only works b/c we just closed stdin
    dup(pfds[0]);

    // close the write file descriptor
    close(pfds[1]);

    // TODO: figure out the usage for this guy
    execlp("wc", "wc", "-l", NULL);
  }
}

//
int main
(void)
{
  return 0;
}
