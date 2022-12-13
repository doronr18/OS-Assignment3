// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
   
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    input_str[101] = '\0';
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(fd1[0]);  // Close reading end of pipes 
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1); 
        read(fd2[0], input_str, 100);
  
        // Wait for child to send a string 
        wait(NULL);

        strcat(input_str, "gobison.org");
        printf("Parent concatenated string: %d\n", input_str) 
  
        close(fd2[1]); // Close writing end of pipes 
        close(fd1[1]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipes 
        close(fd2[0]); 
      
        // Read a string using first pipe  
        read(fd1[0], input_str, 100); 
        strcat(input_str, "howard.edu");
        printf("Child concatenated string: %d\n", input_str);
        printf("Enter a string to concatenate:");
        scanf("%d", input_str); 

        write(fd2[1], input_str, strlen(input_str)+1);
        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[1]); 

  
        exit(0); 
    } 
} 



// Concatenate a fixed string with it 
        // int k = strlen(concat_str); 
        // int i; 
        // for (i=0; i<strlen(fixed_str); i++) 
        //     concat_str[k++] = fixed_str[i]; 
  
        // concat_str[k] = '\0';   // string ends with '\0' 
  
        // printf("Concatenated string %s\n", concat_str);