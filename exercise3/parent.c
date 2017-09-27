#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
pid_t pids[3];


static void sigHandler(int sig)
{
	int wpid, status = 0;
	int i;
	for (i = 0 ; i < 3 ; i++)
		kill(9, pids[i]);
	while ((wpid = wait(&status)) > 0);
	exit(0);
}

int main(int argc, char **argv)
{
	int fd;
	char *addr;
	struct stat sb;
	int rand_num;
	int i;
	pid_t child;
	fd = shm_open("shm", O_RDWR | O_CREAT, 0666);;
	if (fd == -1){
		write(2, "Openning error\n", 20);
		return -1;
	}
	if (ftruncate(fd,4) == -1){
		write(2, "Truncating error\n", 20);
		return -1;
	}
	
	addr = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED){
		write(2, "Mapping error\n", 20);
		return -1;
	}
	if (close(fd) == -1){
		write(2, "Closing error\n", 20);
		return -1;
	}

	if (signal(SIGINT, sigHandler) == SIG_ERR) {
		write(2, "Changing signal handler error\n", 30);
	}
	for (i = 0 ; i < 3 ; i++){
		child = fork();	
		if (child < 0) {
			write(2, "Forking error\n", 20);
			return -1;
		} else if( child  == 0){
			execvp("./child",argv);
			exit(EXIT_FAILURE);
		} else {
			pids[i] = child;
			printf("I started the pid:%d\n", child);
		}
	}

	while (1){
		sleep(1);
		rand_num = rand();
		memcpy(addr, &rand_num, 4);

		for (i = 0 ; i < 3 ; i++){
			kill(pids[i], 40);
		}
	}

}
