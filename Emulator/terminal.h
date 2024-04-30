#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pwd.h>
#include <libgen.h>

class Terminal {
public:
    Terminal();
    size_t command_check();
    
private:
    void error_handler(std::string message, int return_code);
    void cutter(char command[], char *command_args[]);

    std::string command{};
    const int max_args = 1023;	
};

// #include "terminal.cpp"
#endif
