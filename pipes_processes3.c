#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>



int main(int argc, char **argv)
{
  int pipefd1[2];
  int pipefd2[2];
  int pid, pid2, pid3;
  int status;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  grep_args[1] = argv[1];
  if (argc < 1){
    printf("Please enter a grep argument:\n");
    exit(1);
  }

  pipe(pipefd1);
  pipe(pipefd2);

  pid = fork();

  if (pid == 0){

    dup2(pipefd1[1], 1);

    close(pipefd1[1]);
    close(pipefd1[0]);
    close(pipefd2[0]);
    close(pipefd2[1]);

    execvp(*cat_args, cat_args);

  } 
  else if (pid < 0){
    perror("cat fork error");
  } 
  else{
    pid2 = fork();
    if (pid2 == 0){

      dup2(pipefd1[0], 0); 
      dup2(pipefd2[1], 1);
      
      close(pipefd1[1]);
      close(pipefd1[0]);
      close(pipefd2[0]);
      close(pipefd2[1]);
      
      execvp(*grep_args, grep_args);
    }
    else if(pid2 < 0){
      printf("grep fork error\n");
    }
    else{
      pid3 = fork();
      if (pid3 == 0){
        dup2(pipefd2[0], 0);

        close(pipefd1[1]);
        close(pipefd1[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        execvp(*sort_args, sort_args);
      }
      else if (pid3 < 0){
        perror("sort fork error\n");
      }
    }
  }

  close(pipefd1[1]);
  close(pipefd1[0]);
  close(pipefd2[0]);
  close(pipefd2[1]);

  int i;
  for (i=0; i < 3; i++){
    wait(&status);
  }

  

}