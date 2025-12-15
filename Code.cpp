#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
using namespace std;

struct Process {
    int id;
    int cpuNeed;
    int memNeed;
    int cpuAlloc;
    int memAlloc;
    int priority;
    int cpuUsage;
    int memUsage;
};

int main() {
    srand((unsigned)time(nullptr));
    const string BR_BLACK = "\033[90m";
    const string RED = "\033[31m";
    const string BR_RED = "\033[91m";
    const string GRN = "\033[32m";
    const string BR_GRN = "\033[92m";
    const string YEL = "\033[33m";
    const string BR_YEL = "\033[93m";
    const string BLU = "\033[34m";
    const string BR_BLU = "\033[94m";
    const string MAG = "\033[35m";
    const string BR_MAG = "\033[95m";
    const string CYN = "\033[36m";
    const string BR_CYN = "\033[96m";
    const string WHT = "\033[37m";
    const string BR_WHT = "\033[97m";
    const string RESET = "\033[0m";

    cout << "Adaptive Resource Allocation Simulator\n\n";
    cout << "INPUT INSTRUCTIONS:\n";
    cout << "1) First enter: <number_of_processes> <total_CPU_available> <total_memory_available>\n";
    cout << "2) Then for each process: <cpuNeed> <memNeed> <priority>\n";
    cout << "3) Finally enter number of iterations (0 = infinite)\n\n";

    int n, totalCPU, totalMEM;
    cin >> n >> totalCPU >> totalMEM;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i].cpuNeed >> p[i].memNeed >> p[i].priority;
        p[i].id = i + 1;
        p[i].cpuAlloc = max(1, p[i].cpuNeed / 2);
        p[i].memAlloc = max(5, p[i].memNeed / 2);
    }

    cout << "Enter iterations (0 = infinite): ";
    int iterations;
    cin >> iterations;

    int iter = 0;

    auto color_for_priority = [&](int pr)->string {
        if (pr >= 5) return BR_MAG;
        if (pr == 4) return BR_BLU;
        if (pr == 3) return BR_CYN;
        if (pr == 2) return BR_GRN;
        if (pr == 1) return BR_WHT;
        return BR_YEL;
    };

    auto run_once = [&]() {
        int usedCPU = 0, usedMEM = 0;
        vector<float> cpuLoads(n), memLoads(n);

        for (int i = 0; i < n; i++) {
            p[i].cpuUsage = rand() % (p[i].cpuNeed + 1);
            p[i].memUsage = rand() % (p[i].memNeed + 1);
            usedCPU += p[i].cpuUsage;
            usedMEM += p[i].memUsage;

            cpuLoads[i] = (float)p[i].cpuUsage / p[i].cpuNeed;
            memLoads[i] = (float)p[i].memUsage / p[i].memNeed;
        }

        for (int i = 0; i < n; i++) {
            if (cpuLoads[i] > 0.7 || memLoads[i] > 0.7) {
                if (p[i].cpuAlloc < p[i].cpuNeed) p[i].cpuAlloc++;
                if (p[i].memAlloc < p[i].memNeed) p[i].memAlloc += 5;
            } else {
                if (p[i].cpuAlloc > 1) p[i].cpuAlloc--;
                if (p[i].memAlloc > 5) p[i].memAlloc -= 5;
            }
        }

        cout << "\nIteration: " << (++iter) << "\n";
        cout << "--------------------------------------------------------------\n";
        cout << setw(8) << "Proc"
             << setw(12) << "CPU_Alloc"
             << setw(12) << "MEM_Alloc"
             << setw(12) << "CPU_Use"
             << setw(12) << "MEM_Use"
             << setw(8) << "Prio" << "\n";
        cout << "--------------------------------------------------------------\n";

        for (int i = 0; i < n; i++) {
            string color = color_for_priority(p[i].priority);
            cout << color;
            cout << setw(8) << ("P" + to_string(p[i].id))
                 << setw(12) << p[i].cpuAlloc
                 << setw(12) << p[i].memAlloc
                 << setw(12) << p[i].cpuUsage
                 << setw(12) << p[i].memUsage
                 << setw(8) << p[i].priority
                 << RESET << "\n";
        }

        cout << "--------------------------------------------------------------\n";
        cout << "System CPU Load: " << usedCPU << " / " << totalCPU << "    ";
        cout << "System MEM Load: " << usedMEM << " / " << totalMEM << "\n";
    };

    if (iterations == 0) {
        while (true) {
            run_once();
            this_thread::sleep_for(chrono::milliseconds(1500));
        }
    } else {
        for (int i = 0; i < iterations; i++) {
            run_once();
            this_thread::sleep_for(chrono::milliseconds(1500));
        }
    }

    return 0;
}
