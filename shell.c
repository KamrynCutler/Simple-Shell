#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;

char *getcwd(char *buf, size_t size);
char cwd[100];
void sighandler(int);
pid_t pid;
//char getenv(const char *name)

int main() {
  
  
    int i = 0;
    int j = 0;
    int k = 1;
    int n = 0;
    bool isBackground = false;
    signal(SIGINT, sighandler);
    char *test;
  
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    char *newenv[MAX_COMMAND_LINE_ARGS];
    	
    while (true) {
			
				isBackground = false;
      
        do{ 
            
            //Modifying shell prompt
            if (getcwd(cwd, sizeof(cwd)) != NULL){
          
            }
            else {
              perror("getcwd()error");
                return 1;
            }
          
              // Print the shell prompt.
              printf("%s> ", cwd);
              fflush(stdout);

              // Read input from stdin and store it in command_line. If there's an
              // error, exit immediately. (If you want to learn more about this line,
              // you can Google "man fgets")

              if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                  fprintf(stderr, "fgets error");
                  exit(0);
              }
 
        }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // TODO:
        // 
        
			  // 0. Modify the prompt to print the current working directory
			  // In do while
			
        // 1. Tokenize the command line input (split it on whitespace)
        arguments[0] = strtok(command_line, delimiters);
         // walk through other tokens 
        i = 0;
         while( arguments[i] != NULL ) {
            arguments[++i] = strtok(NULL, delimiters);
         }
         arguments[i] == NULL;
               
        // 2. Implement Built-In Commands

        // cd: changes the current working directory
        if (strcmp(arguments[0], "cd") == 0 ){
          chdir(arguments[1]);
          //printf("cd function \n");
        } // pwd: prints the current working directory
        else if (strcmp(arguments[0], "pwd") == 0){ 
          if (getcwd(cwd, sizeof(cwd)) == NULL){
            perror("getcwd()error");
            return 1;
          }
          printf("%s \n", cwd);
          fflush(stdout);
          //printf("pwd function \n");
        } // echo: prints a message and the values of environment variables
       else if (strcmp(arguments[0], "echo") == 0){ // Check for $
          while (arguments[k] != NULL){
            printf ("%s ", arguments[k]);
            k++;
          }
         printf("\n");
          //printf("\n echo function \n");
        }
        else if (strcmp(arguments[0], "exit") == 0 ){  // exit: terminates the shell
          exit(0);
        } // env: prints the current values of the environment variables
        else if (strcmp(arguments[0], "env") == 0 ){
          //getenv(arguments[1]);
          printf("%s \n", getenv(arguments[1]));
          //printf("env function \n");
        } // setenv: sets an environment variable
        else if (strcmp(arguments[0], "setenv") == 0){
          newenv[n] = strtok(arguments[1], " = ");
          while(newenv[n] != NULL){
            printf("newenv [%d] = %s \n", n, newenv[n]);
            n++;
            newenv[n] = strtok(NULL, " = ");
          }
          setenv(newenv[0], newenv[1], 1); //printf("setenv function \n");
        }
        else {
          if (strcmp(arguments[i-1], "&") == 0){
            arguments[i-1] = NULL;
            //set a flag
            isBackground = true; 
          }
          
          // 3. Create a child process which will execute the command line input
       
          pid = fork();
 
          if (pid < 0){
            perror("fork failed");
            exit(0);
          }
          else if (pid == 0) { //child
            execvp(arguments[0], arguments);
            exit(0);
          } // 4. The parent process should wait for the child to complete unless its a background process 
          else { //Parent
             //if (strcmp(arguments[i-1], "&") != 0) {
						 if (isBackground != true) {
               wait(NULL); //Until child finishes
             }
          } 
       }

    }
    // This should never be reached.
    return -1;
 }

void sighandler(int signum){
  printf("\nCaught signal %d, coming out...\n", signum);
  //send signal to child process 
  kill(pid, SIGKILL);
}
