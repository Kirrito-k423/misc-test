#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/wait.h>

// Structure to represent a range of cores
struct CoreIdRange {
    int start;
    int end;
};

// Function to execute a shell command and capture its output
std::string executeCommand(const std::string &command) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// Function to parse PIDs and TIDs from pstree output
std::vector<pid_t> parsePIDsFromPstree(const std::string &pstreeOutput) {
    std::vector<pid_t> pids;
    std::regex pidRegex(R"(\((\d+)\))"); // Matches numbers inside parentheses
    std::smatch match;
    std::string::const_iterator searchStart(pstreeOutput.cbegin());
    while (std::regex_search(searchStart, pstreeOutput.cend(), match, pidRegex)) {
        pids.push_back(std::stoi(match[1]));
        searchStart = match.suffix().first;
    }
    return pids;
}

// Function to bind a process or thread to a core range
void bindToCoreRange(pid_t pid, const CoreIdRange &coreRange) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    for (int core = coreRange.start; core <= coreRange.end; ++core) {
        CPU_SET(core, &cpuset);
    }

    if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_setaffinity failed");
    } else {
        std::cout << "Set affinity for PID/TID " << pid
                  << " to cores " << coreRange.start << "-" << coreRange.end << "\n";
    }
}

// Function to create child processes
void createChildProcesses(int numProcesses) {
    for (int i = 0; i < numProcesses; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            std::cout << "Child process " << getpid() << " created\n";
            while (true) {
                // Simulate work (infinite loop)
            }
        }
    }
}

// Thread function
void* threadFunction(void* arg) {
    long threadIndex = (long)arg;
    std::cout << "Thread " << threadIndex << " running. TID: " << pthread_self() << "\n";
    while (true) {
        // Simulate work (infinite loop)
    }
    return nullptr;
}

// Function to create child threads
void createChildThreads(int numThreads) {
    std::vector<pthread_t> threads(numThreads);
    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&threads[i], nullptr, threadFunction, (void*)(long)i);
        std::cout << "Thread " << i << " created\n";
    }
}

// Main function to set affinity for remaining tasks
void SetAffinityForRemainingTasks(pid_t parentPid, const CoreIdRange &coreRange) {
    // Run pstree to get child processes and threads
    std::string pstreeCommand = "pstree -p " + std::to_string(parentPid) + " -t";
    std::string pstreeOutput = executeCommand(pstreeCommand);

    // Parse PIDs/TIDs from the pstree output
    std::vector<pid_t> pids = parsePIDsFromPstree(pstreeOutput);

    // Bind each PID/TID to the core range
    for (pid_t pid : pids) {
        bindToCoreRange(pid, coreRange);
    }
}

int main() {
    pid_t parentPid = getpid(); // Get the current process PID
    CoreIdRange coreRange = {0, 3}; // Example core range (cores 0-3)

    std::cout << "Setting affinity for tasks under PID " << parentPid << "\n";

    // Create child processes and threads
    createChildProcesses(3);  // Create 3 child processes
    createChildThreads(3);    // Create 3 child threads

    // Allow some time for processes and threads to be created
    sleep(1);

    // Set affinity for all child processes and threads
    SetAffinityForRemainingTasks(parentPid, coreRange);

    // Wait for all child processes to finish (in this case, they run indefinitely)
    wait(nullptr);  // This will wait for any child process to terminate

    return 0;
}
