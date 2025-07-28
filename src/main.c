/*
project: 01
author: Maxim Mikhaylov 
email: vw75092@umbc.edu 
student id: vw75092 
due date: 3/2/25 11:59:59 pm
description: a simple linux shell designed to perform basic linux commands
*/

#define HISTORY_FILE "project1-vw75092/.421sh"
#define MAX_HISTORY 10
#define MAX_COMMAND_LEN 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"

#define MAX_LINE_LEN 1024
 
//DEFINE THE FUNCTION PROTOTYPES
user_prompt_loop();
char* get_user_command();
char** parse_command(const char *cmd);
execute_command(const char **cmd);

//helper functions
void read_proc_file(const char* cmd);
void clear_history();
void add_to_history(const char* cmd);
void show_history();

int main(int argc, char **argv)
{
    /*
    The main function that checks the number of argument passed to ensure 
    no command-line arguments are passed; if the number of argument is greater 
    than 1 then exit the shell with a message to stderr and return value of 1
    otherwise call the user_prompt_loop() function to get user input repeatedly 
    until the user enters the "exit" command.
    */
  
    if (argc > 1) // # of argument is greater than 1
    {
        fprintf(stderr, "Error: No command-line arguments expected.\n");
        return 1;
    }
    else 
    {
        user_prompt_loop(); // call the user_prompt_loop() function
    }

    return 0;
}

/*
user_prompt_loop():
Get the user input using a loop until the user exits, prompting the user for a command.
Gets command and sends it to a parser, then compares the first element to the two
different commands ("/proc", and "exit"). If it's none of the commands, 
send it to the execute_command() function. If the user decides to exit, then exit 0 or exit 
with the user given value. 
*/

user_prompt_loop()
{
    // initialize variables
    char *command; // user input command
    char **parsed;
    /*
    loop:
        1. prompt the user to type command by printing >>
        2. get the user input using get_user_command() function 
        3. parse the user input using parse_command() function 
        Example: 
            user input: "ls -la"
            parsed output: ["ls", "-la", NULL]
        4. compare the first element of the parsed output to "/proc"and "exit"
        5. if the first element is "/proc" then you have the open the /proc file system 
           to read from it
            i) concat the full command:
                Ex: user input >>/proc /process_id_no/status
                    concated output: /proc/process_id_no/status
            ii) read from the file line by line. you may user fopen() and getline() functions
            iii) display the following information according to the user input from /proc
                a) Get the cpu information if the input is /proc/cpuinfo
                - Cpu Mhz
                - Cache size
                - Cpu cores
                - Address sizes
                b) Get the number of currently running processes from /proc/loadavg
                c) Get how many seconds your box has been up, and how many seconds it has been idle from /proc/uptime
                d) Get the following information from /proc/process_id_no/status
                - the vm size of the virtual memory allocated the vbox 
                - the most memory used vmpeak 
                - the process state
                - the parent pid
                - the number of threads
                - number of voluntary context switches
                - number of involuntary context switches
                e) display the list of environment variables from /proc/process_id_no/environ
                f) display the performance information if the user input is /proc/process_id_no/sched
        6. if the first element is "exit" the use the exit() function to terminate the program
        7. otherwise pass the parsed command to execute_command() function 
        8. free the allocated memory using the free() function
    */

    while (1) // loop
   {
       int fileopen = 0;

       printf(">>"); // 1. prompt the user to type command by printing >>
       command = get_user_command(); // 2. get the user input using get_user_command() function
       parsed = parse_command(command); // 3. parse the user input using parse_command() function
       // 4. compare the first element of the parsed output to "/proc"and "exit"
       if (strcmp(parsed[0], "/proc" ) == 0 || strcmp(parsed[0], "/proc/cpuinfo") == 0 || strcmp(parsed[0], "/proc/loadavg") == 0 || strcmp(parsed[0], "/proc/uptime") == 0 || strcmp(parsed[0], "/proc/process_id_no/status") == 0 || strcmp(parsed[0], "/proc/process_id_no/environs") == 0 || strcmp(parsed[0], "/proc/process_id_no/sched") == 0 ) // 5. if the first element is "/proc"
       {
           // i) concat the full command :
           int count = 0;
           while (parsed[count] != NULL)
           {
               count++;
           }

           size_t full_cmd_size = strlen(parsed[0]) + 1;
           char* full_cmd = malloc(full_cmd_size);
           memcpy(full_cmd, parsed[0], full_cmd_size);
           for (int i = 1; i < count; i++)
           {
               full_cmd_size += strlen(parsed[i]) + 1;
               full_cmd = realloc(full_cmd, full_cmd_size);
               strcat(full_cmd, parsed[i]);
           }
           // ii) read from the file line by line
       

           // iii) display infromation from /proc
           if (strcmp(full_cmd, "/proc/cpuinfo") == 0) //  a) Get the cpu information if /proc/cpuinfo
           {
               read_proc_file(full_cmd);
           }
           else if (strcmp(full_cmd, "/proc/loadavg") == 0) // b) Get the # of currently running processes if /proc/loadavg
           {
               read_proc_file(full_cmd);
           }
           else if (strcmp(full_cmd, "/proc/uptime") == 0) // c) Get how many seconds your box has been up, and how many seconds it has been idle if /proc/uptime
           {
               read_proc_file(full_cmd);
           } 
           else if (strcmp(full_cmd, "/proc/process_id_no/status") == 0) // d) Get the following information from /proc/process_id_no/status
           {
               read_proc_file(full_cmd);
           }
           else if (strcmp(full_cmd, "/proc/process_id_no/environs") == 0) // e) display the list of environment variables from /proc/process_id_no/environ
           {
               read_proc_file(full_cmd);
           }
           else if (strcmp(full_cmd, "/proc/process_id_no/sched") == 0) // f) display the performance information if the user input is /proc/process_id_no/sched
           {
               read_proc_file(full_cmd);
           }
           else 
           {
               read_proc_file(full_cmd);
           }
      
           free(full_cmd);
       }
       else if (strcmp(parsed[0], "exit") == 0) // 6. if the first element is "exit" the use the exit() function
       {
           clear_history();
           free(command); 
           free(parsed);
           exit(0);
       }
       else if (strcmp(parsed[0], "history") == 0)
       {
        show_history();
       }
       else
       {
           execute_command(parsed); // 7. otherwise pass the parsed command to execute_command() function
       }
       add_to_history(command);
       free(command); // 8. free the allocated memory using the free() function
       free(parsed);
   }
}

/*
get_user_command():
Take input of arbitrary size from the user and return to the user_prompt_loop()
*/

char* get_user_command()
{
    size_t input_size = 128;  // initial input size
    size_t i = 0; // index
    char* input = malloc(input_size);
    char c;

    while ((c = fgetc(stdin)) != EOF && c != '\n')
    {
        if (i >= input_size - 1)
        {
            input_size *= 2;
            input = realloc(input, input_size);
        }
        input[i++] = c;
    }
    input[i] = '\0';

    return input;
}

/*
parse_command():
Take command grabbed from the user and parse appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/

char** parse_command(const char *cmd)
{
    size_t args_size = 16;
    size_t arg_count = 0;
    size_t copy_len = strlen(cmd) + 1;

    char **args = malloc(args_size * sizeof(char*));
    char *cmd_copy = malloc(copy_len);
    memcpy(cmd_copy, cmd, copy_len);
    char* token_start = cmd_copy;
    int space;

    while ((space = first_unquoted_space(token_start)) != -1)
    {
        token_start[space] = '\0';

        if (arg_count >= args_size - 1)
        {
            args_size *= 2;
            char** temp = realloc(args, args_size * sizeof(char*));
            if (!temp) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                free(args);
                free(cmd_copy);
                return NULL;
            }
            args = temp;
        }
        args[arg_count++] = unescape(token_start, stderr);
        token_start += space + 1;
    }
    args[arg_count] = unescape(token_start, stderr); // insert last token before NULL
    arg_count++;
    args[arg_count] = NULL; // insert NULL
    free(cmd_copy);
    return args; // freed in user_prompt_loop()
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/

execute_command(const char **cmd)
{
    {
        pid_t pid = fork();
        if (pid < 0) 
        {
            perror("fork failed");
            return;
        }
        else if (pid == 0) 
        {
            // Child process
            execvp(cmd[0], (char* const*)cmd);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
        else 
        {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

void read_proc_file(const char* cmd)
{
    FILE* file = fopen(cmd, "r");

    if (file != NULL)
    {
        char buffer[MAX_LINE_LEN];  // Buffer to store each line

        while (fgets(buffer, sizeof(buffer), file) != NULL) // Read line by line
        {
            printf("%s", buffer);  // Print the line (includes newline if present)
        }

        fclose(file);  // Close the file
    }
    else
    {
        perror("Error opening file");  // Print an error message
    }
}

void clear_history() 
{
    FILE* file = fopen(HISTORY_FILE, "w"); 
    if (file)fclose(file);
}

void add_to_history(const char* cmd) 
{
    if (strcmp(cmd, "history") == 0) return; // Don't store the history command itself

    FILE* file = fopen(HISTORY_FILE, "a"); // Append mode
    if (!file) {
        perror("Error opening history file");
        return;
    }

    fprintf(file, "%s\n", cmd);
    fclose(file);
}

 void show_history() 
{
    FILE* file = fopen(HISTORY_FILE, "r");
    if (!file) {
        printf("No command history found.\n");
        return;
    }

    char history[MAX_HISTORY][MAX_COMMAND_LEN] = { 0 };
    int count = 0;

    while (fgets(history[count % MAX_HISTORY], MAX_COMMAND_LEN, file)) 
    {
        history[count % MAX_HISTORY][strcspn(history[count % MAX_HISTORY], "\n")] = '\0'; // Remove newline
        count++;
    }
    fclose(file);

    int start = count > MAX_HISTORY ? count - MAX_HISTORY : 0;
    for (int i = start; i < count; i++) 
    {
        printf("%d: %s\n", i - start + 1, history[i % MAX_HISTORY]);
    }
    return;
}
