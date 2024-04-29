#include "terminal.h"

Terminal::Terminal()
{
    while (1) {
        std::getline(std::cin, command);
        if (command_check() != 0) {
            std::endl(std::cerr << "Returned with exit code higher than 0");
            break;
        }
    }
}

size_t Terminal::command_check()
{
    pid_t f_pid = fork();
    if (f_pid < 0) {
        error_handler("Fork failed", 1);
    }
    else if (f_pid == 0) {
        char* command_args[max_args + 1];
        cutter(command.data(), command_args);

        execvp(command_args[0], command_args);
        error_handler("Execvp failed", 2);
    }
    else {
        int status;
        wait(&status);
        if (WEXITSTATUS(status) != 0) {
            error_handler("Error with params passed", 3);
        }
    }
    return 0;
}

void Terminal::error_handler(std::string message, int return_code)
{
    std::endl(std::cerr << message);
    exit(return_code);
}

void Terminal::cutter(char command[], char *command_args[])
{
    char *p;
    int arg_count = 0;

    p = command;
    while ((*p) != '\0' && arg_count < max_args) {
        while ((*p) == ' ' && (*p) != '\0') {
            p++;
        }  
        if ((*p) == '\0') {
            break;
        }	
        command_args[arg_count++] = p;		
        while ((*p) != ' ' && (*p) != '\0') {
            p++;
        }  
        if ((*p) == '\0') {
            break;
        } 	
        *p = '\0';			
        p++;			
    }
    command_args[arg_count] = NULL;
}
