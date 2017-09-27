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

int main(int argc, char **argv)
{
	char message_send[100];
	char message_recv[1000];
	char *p;
	int rand_num;
	int i;
	pid_t child;
	int N;	
	int **fd;

	if (argc < 2){
		write(2, "Specify the number of child's as the first program argument\n", 30);
		return -1;
	}
	N = atoi(argv[1]);
	
	fd = malloc(N * sizeof(int *));
	for (i = 0 ; i < N ; i++){
		fd[i] = malloc (2 * sizeof(int));	
		if (pipe(fd[i]) == -1){
			write(2, "Pipe creation failed\n", 30);
			return -1;
		}
	}
	
	for (i = 0 ; i < N ; i++){
		child = fork();	
		if (child < 0) {
			write(2, "Forking error\n", 20);
			return -1;
		} else if( child  == 0){
			close(fd[i][0]);
			rand_num = rand();
			memset(message_send, 0 , sizeof(message_send));
			sprintf(message_send, "%d-%d|", getpid(), rand_num);
			write(fd[i][1], message_send, strlen(message_send));
			close(fd[i][1]);
			exit(0);
		} else {
			close(fd[i][1]);
			printf("I started the pid:%d\n", child);
		}
	}

	for(i = 0 ; i < N ; i++){
		memset(message_recv, 0, sizeof(message_recv));
		read(fd[i][0], message_recv, sizeof(message_recv));
		p = strtok(message_recv, "|");
		while(p != NULL){
			printf("%s\n", p);
			p = strtok(NULL, "|");
		}
		close(fd[i][0]);
	}
}
