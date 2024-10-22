#include<iostream>
#include<thread>

#define TRUE 1
#define FALSE 0
#define N 2

int turn = 0;
int interested[N];
int x = 0;

void enterSection(int process){
    int other = 1-process;
    interested[process] = TRUE;
    turn = process;
    while(turn == process && interested[other] == TRUE );
}

void leaveSection(int process){
    interested[process] = FALSE;
}

class thread_object{
    public:
    void operator()(int process){
        for(int i = 0; i < 10000; i++){
            enterSection(process);
            x += 1;
            std::cout << "thread: "<< process << std::endl; 
            leaveSection(process);
        }
    }
};


int main(int argc, char *argv[]){
    thread_object obj;
    std::thread th1(obj, 0);
    std::thread th2(obj, 1);

    th1.join();
    th2.join();

    return 0;
}