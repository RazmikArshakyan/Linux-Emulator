#include "terminal.h"

Terminal::Terminal()
{
    passwd *pw = getpwuid(getuid());
    char current_dir[FILENAME_MAX];

    while (1) {
        if (pw != nullptr && getcwd(current_dir, sizeof(current_dir)) != nullptr) {
            std::cout << pw->pw_name << ":~" << current_dir << "$ ";
        }

        std::getline(std::cin, command);
        if (command_check() != 0) {
            std::endl(std::cerr << "Error with command");
            break;
        }
        std::endl(std::cout);
    }
}

size_t Terminal::command_check() {
    if (command.empty()) {
        error_handler("Empty command", 2);
        return 0;
    }

    char* command_args[max_args + 1] = {nullptr}; 
    cutter(command.data(), command_args);

    if (strcmp(command_args[0], "cd") == 0) {
        if (command_args[1] == nullptr) {
            error_handler("Usage: cd DIR", 4);
        }
        if (chdir(command_args[1]) != 0) {
            error_handler("Chdir failed", 5);
        }
        return 0;
    }

    pid_t f_pid = fork();
    if (f_pid < 0) {
        error_handler("Fork failed", 1);
    } 
    else if (f_pid == 0) {
        if (execvp(command_args[0], command_args) == -1) {
            error_handler("Execvp failed", 3);
        }
    } 
    else {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                error_handler("Error with command execution", 6);
            }
        } 
        else {
            error_handler("Command execution failed", 7);
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
