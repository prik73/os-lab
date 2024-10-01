#include<iostream>
#include<thread>

int lock = 0;

class thread_thingy{
    public: 
    void operator()(int& x){
        for(int i = 0; i < 10000; ++i){
            while(lock != 0){
                
                lock = 1;
                x += 1;
                std::cout << x << std::endl;
                lock = 0;
            }
        }
    }
};

int main(void){
    int x = 0;
    thread_thingy o1;
    std::thread t1(o1, std::ref(x));
    std::thread t2(o1, std::ref(x));

    t1.join();
    t2.join();

    std::cout << "final x ki value: " << x << std::endl ;     
    return 0;
}