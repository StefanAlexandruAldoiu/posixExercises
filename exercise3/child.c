#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
char *addr;

static void sigHandler(int sig)
{
	printf("ID:%d with value:%d\n", getpid(), *((int *) addr));
}

int main(){

	int fd;
	struct stat sb;

	fd = shm_open("shm", O_RDONLY, 0666);;
	if (fd == -1){
		write(2, "Openning error\n", 20);
		return -1;
	}
	if (fstat(fd, &sb) == -1){
		write(2, "Fstat error\n", 20);
		return -1;
	}
	
	addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED){
		write(2, "Mapping error\n", 20);
		return -1;
	}

	if (close(fd) == -1){
		write(2, "Closing error\n", 20);
		return -1;
	}
	
	if (signal(40, sigHandler) == SIG_ERR) {
		write(2, "Changing signal handler error\n", 30);
		return -1;
	}
	printf("I am the process with the pid:%d\n", getpid());
	while (1){
		fd = 0;
		fd += 3;
		sleep(3);
	};
}
