#include<iostream>
#include<unistd.h>

void pipeline(const char* process1, const char* process2 ){
    int fd[2];
    pipe(fd);

    if(fork() != 0 ){
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);

        execlp("/bin/cat", "cat", "main.cpp", nullptr);
        std::cerr << "failed " << process1 << std::endl;
    }else{

    }
}
int main(void){

}