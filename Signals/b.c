#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
//  pid_t pid;

if (kill(atoi(argv[1]),atoi(argv[2]))==-1){

  if (errno == EINVAL) {
	printf("Error: EINVAL\n");    
	perror("Einval");
    exit(1);
  }
if (errno == EPERM) {
	printf("Error: EPERM\n");
    perror("EPERM");
    exit(1);
  }
if (errno == ESRCH) {
	printf("Error: ESRCH\n");
    perror("ESRCH");
    exit(1);
  }
}
  return 0;
}
