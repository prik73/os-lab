#include<iostream>
#include<unistd.h>

void pipeline(const char* process1, const char* process2){
    int fd[2];
    pipe(fd);

    
    if(fork() != 0){
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execlp("/usr/bin/cat", "cat" , "pip.cpp", nullptr);
        std::cerr << "failed " << process1 <<std::endl;
    }else{
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execlp("/usr/bin/grep","grep","hello", nullptr);
        std::cerr << "failed " << process2<< std::endl;
    }
}

//hello from mars

int main(void){
    pipeline("cat pipe.cpp", "grep hello");
}