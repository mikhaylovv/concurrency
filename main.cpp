#include <iostream>
#include <assert.h>
#include <thread>
#include <atomic>


int main()
{
    using namespace std::chrono_literals;

    std::atomic_bool start (false);
    // std::atomic_int val (0), flag (0);
    int val = 0, flag = 0;

    std::thread first ([&]() {
        int old = val;
        while(!start.load(std::memory_order_acquire)) continue;
        val = 1;
        val = 11;
        flag = 1;
        val = 12;
    });

    std::thread second ([&] () {
        int w = val;
        while(!start.load(std::memory_order_acquire)) continue;

        int useless = val;
        int _flag = flag;
        int _val = val; 
        if (!( (_flag == 0 && _val == 0) || _val == 12)) {
            std::cout << _flag << " : " << _val << " : " << useless << std::endl;
        }
        if (_flag) {
            assert(_val > 10);
        }
    });

    std::this_thread::sleep_for(10ms);
    start.store(true, std::memory_order_release);
    first.join();
    second.join();

    return 0;
}
