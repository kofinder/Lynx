// THis code is not my own, I just test and learn from white guys who share his code on linkedin

#include <iostream>
#include <memory>
#include <thread>

int main() {

    std::aligned_storage<sizeof(std::thread), alignof(std::thread)>::type M;

    new(&M) std::thread([] () {
        for(int i = 0; i < 1000; i++) {
            std::cout << '+' << std::flush;
        }
        std::cout << std::endl;
    });

    reinterpret_cast<std::thread*> (&M)->join();

    reinterpret_cast<std::thread*>(&M)->~thread();

    return 0;

}