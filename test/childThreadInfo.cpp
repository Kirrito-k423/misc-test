#include <iostream>
#include <thread>
#include <pthread.h>
#include <cstring>
#include <vector>
#include <sched.h>

// 设置线程名称函数
void set_thread_name(const std::string& name) {
    pthread_setname_np(pthread_self(), name.c_str());
}

// 获取线程名称函数
std::string get_thread_name() {
    char thread_name[16];
    pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
    return std::string(thread_name);
}

// 设置CPU亲和性函数
void set_thread_affinity(int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

// 获取CPU亲和性函数
void get_thread_affinity() {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    
    std::cout << "Running on CPUs: ";
    for (int i = 0; i < CPU_SETSIZE; ++i) {
        if (CPU_ISSET(i, &cpuset)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

// 子线程执行函数
void child_thread_function() {
    std::cout << "Child thread started." << std::endl;

    // 检查子线程的线程名称
    std::string child_thread_name = get_thread_name();
    std::cout << "Child thread name: " << child_thread_name << std::endl;

    // 检查子线程的CPU亲和性
    get_thread_affinity();
}

int main() {
    // 设置父线程的线程名称
    std::string parent_thread_name = "ParentThread";
    set_thread_name(parent_thread_name);
    
    // 设置父线程的CPU亲和性
    set_thread_affinity(1); // 设置为CPU 1

    // 打印父线程信息
    std::cout << "Parent thread name: " << get_thread_name() << std::endl;
    std::cout << "Parent thread CPU affinity: ";
    get_thread_affinity();

    // 创建子线程
    std::thread child_thread(child_thread_function);

    // 等待子线程结束
    child_thread.join();

    return 0;
}
