#include<iostream>
#include<thread>

int lock = 0;

class thread_object{
    public:
        void operator()(int& x){
            for(int i = 0; i < 1000000; ++i){
                while(lock != 0){   
                    lock = 1;
                    x += 1;
                    lock = 0;
                }
            }
        }
};

int main(void){
    int x = 0;

    thread_object obj;
    std::thread t(obj, std::ref(x));
    std::thread tt(obj, std::ref(x));

    t.join();
    tt.join();

    std::cout << "final x kii valiue: " << x << std::endl;

}