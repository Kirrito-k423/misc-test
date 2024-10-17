import os
import multiprocessing

# 打印 CPU 亲和性
def print_cpu_affinity():
    pid = os.getpid()
    affinity = os.sched_getaffinity(pid)
    print(f"Process {pid} is running on CPU(s): {affinity}")

# 设置 CPU 亲和性
def set_cpu_affinity(cpu_id):
    pid = os.getpid()
    os.sched_setaffinity(pid, {cpu_id})
    print(f"Process {pid} is now set to run on CPU {cpu_id}")

# 子进程函数
def worker():
    print("=== In child process ===")
    print_cpu_affinity()  # 打印子进程的 CPU 亲和性

    # 手动设置子进程 CPU 亲和性
    set_cpu_affinity(2)

    # 再次打印子进程的 CPU 亲和性
    print_cpu_affinity()

    print("=== Child process finished ===")

if __name__ == '__main__':
    print("=== In parent process ===")
    print_cpu_affinity()  # 打印父进程的 CPU 亲和性

    # 设置父进程的 CPU 亲和性到 CPU 1
    set_cpu_affinity(1)

    # 创建子进程
    p = multiprocessing.Process(target=worker)
    p.start()

    # 打印子进程创建之后父进程的 CPU 亲和性
    print("=== After fork, still in parent process ===")
    print_cpu_affinity()

    p.join()
    print("=== Parent process finished ===")
