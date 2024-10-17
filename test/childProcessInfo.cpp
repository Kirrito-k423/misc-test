#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <cstring>
#include <pthread.h>

// 设置进程名称 (使用 prctl 来修改)
#include <sys/prctl.h>

void set_process_name(const std::string &name) {
    prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
}

std::string get_process_name() {
    char name[16];
    prctl(PR_GET_NAME, name, 0, 0, 0);
    return std::string(name);
}

// 设置进程的 CPU 亲和性
void set_cpu_affinity(int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_setaffinity");
    }
}

// 获取进程的 CPU 亲和性
void get_cpu_affinity() {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    if (sched_getaffinity(0, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_getaffinity");
    }

    std::cout << "Running on CPUs: ";
    for (int i = 0; i < CPU_SETSIZE; ++i) {
        if (CPU_ISSET(i, &cpuset)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Parent process name: " << get_process_name() << std::endl;
    std::cout << "Parent process CPU affinity: ";
    get_cpu_affinity();


    std::cout << "Set Parent Process info." << std::endl;

    // 设置父进程的名称和 CPU 亲和性
    set_process_name("ParentProc");
    set_cpu_affinity(1); // 将父进程绑定到 CPU 1

    std::cout << "Parent process name: " << get_process_name() << std::endl;
    std::cout << "Parent process CPU affinity: ";
    get_cpu_affinity();

    pid_t pid = fork(); // 创建子进程

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // 子进程执行部分
        pid_t child_pid = getpid(); // 子进程PID
        pid_t parent_pid_of_child = getppid(); // 子进程的父进程PID

        std::cout << "\nChild process started." << std::endl;
        std::cout << "Child process PID: " << child_pid << std::endl;
        std::cout << "Parent PID of child process: " << parent_pid_of_child << std::endl;

        std::cout << "\nChild process started." << std::endl;

        std::string child_name = get_process_name();
        std::cout << "Child process name: " << child_name << std::endl;

        std::cout << "Child process CPU affinity: ";
        get_cpu_affinity();

        // 在子进程中设置新的名称和亲和性
        set_process_name("ChildProc");
        set_cpu_affinity(2); // 将子进程绑定到 CPU 2

        std::cout << "\nChild process new name: " << get_process_name() << std::endl;
        std::cout << "Child process new CPU affinity: ";
        get_cpu_affinity();
    } else {
        // 父进程等待子进程结束
        wait(NULL);
        std::cout << "\nParent process finished." << std::endl;
    }

    return 0;
}
