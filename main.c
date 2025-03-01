/*
implement a C program that generates two children, a producer and a consumer. The producer child reads from the keyboard some strings, and it transfers them to the consumer. The consumer child converts the strings into uppercase characters, and it visualizes the strings in standard output. The transfer of the strings (and synchronization) has to take place through pipes.

Observation: The length of the strings transferred through the pipe is variable and not known a priori. It is therefore appropriate to use some communication mechanism/protocol. For instance, it is possible to transfer in the pipe the dimension of the string before the string itself, by using a constant number of characters to code this dimension. 
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define SIZE 1024

int main(){
  pid_t pid;
  char c;
  int fd[2];

  //Azzero il buffer stout
  setbuf(stdout, 0);
  //Da dichiarare prima della fork, così child eredita i fd
  pipe(fd);
  pid = fork();
  
  if (pid>0){
    //PADRE
    pid = fork();
    
      if(pid>0){
	//Padre
      } else if(pid==0){
	//Figlio 2
	int len;
	printf("Ciao! Sono figlio 2:\n");
        close(fd[1]);
	printf("Leggo da figlio 1:\n");
	
	while(read(fd[0], &len, sizeof(len))==sizeof(len)){
	    int bytesRead = 0;
	    for(int i = 0; i < len; i++){
	      read(fd[0], &c, sizeof(c));
	    printf("%c", toupper(c));
		   }
	    }
    close(fd[0]);
    exit(0);
      }
      
    wait(NULL);
    wait(NULL);
    
  } else if(pid==0){
    //FIGLIO 1 (producer)
    //Leggere righe da stdin e mandarle al figlio 2 (consumer)
    //Chiudo la lettura, perché scrivo;
    char tempBuffer[SIZE];
    int len;
    printf("Ciao, sono figlio 1 e scrivo:\n");
    close(fd[0]);
    while(fgets(tempBuffer, SIZE, stdin)!=NULL){
      int len = strlen(tempBuffer);
      if(write(fd[1], &len, sizeof(len))!=sizeof(len)){
	perror("Errore nella scrittura buffer size\n");
	exit(-1);
      }
      if(write(fd[1], tempBuffer, len)!=len){
	perror("Errore nella trasmissione della stringa\n");
      }
	    }
    close(fd[1]);
    exit(0);
    } else {
    perror("Errore nella fork");
    exit(-1);
  }
  return 0;
}
