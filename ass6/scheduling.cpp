#include <bits/stdc++.h>
using namespace std;

struct Process {
    int pid, at, bt, priority;
    int ct=0, tat=0, wt=0;
    int rem;
};

void FCFS(vector<Process> p) {
    sort(p.begin(), p.end(), [](auto &a, auto &b){ return a.at < b.at; });
    int time = 0;
    cout << "\n--- FCFS ---\n";
    for (auto &x : p) {
        if (time < x.at) time = x.at;
        time += x.bt;
        x.ct = time;
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;
        cout << "P" << x.pid << ": CT=" << x.ct << " TAT=" << x.tat << " WT=" << x.wt << endl;
    }
}

void RR(vector<Process> p, int q) {
    cout << "\n--- Round Robin (q=" << q << ") ---\n";
    int n = p.size(), time = 0;
    for (auto &x : p) x.rem = x.bt;
    queue<int> qidx;
    vector<bool> inq(n,false);
    while (true) {
        bool done = true;
        for (int i = 0; i < n; i++) {
            if (p[i].rem > 0) {
                done = false;
                if (p[i].rem > q) {
                    time += q;
                    p[i].rem -= q;
                } else {
                    time += p[i].rem;
                    p[i].rem = 0;
                    p[i].ct = time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    cout << "P" << p[i].pid << ": CT=" << p[i].ct
                         << " TAT=" << p[i].tat
                         << " WT=" << p[i].wt << endl;
                }
            }
        }
        if (done) break;
    }
}

int main() {
    vector<Process> procs = {
        {1, 0, 5, 2},
        {2, 1, 3, 1},
        {3, 2, 8, 3}
    };

    FCFS(procs);
    RR(procs, 2);
    return 0;
}


// 🖥️ What is CPU Scheduling?

// CPU scheduling is the process of deciding which ready process from the ready queue will be allocated the CPU next.

// Since only one process can execute on the CPU at a time, the scheduler decides the execution order to improve system performance.

// ⚙️ Objectives of Scheduling

// Maximize CPU utilization

// Minimize waiting time

// Minimize turnaround time

// Minimize response time

// Maximize throughput

// Ensure fairness

// 🧩 Key Terms
// Term	Definition
// Burst Time (BT)	Time required by the process for execution on CPU.
// Arrival Time (AT)	Time at which process arrives in the ready queue.
// Completion Time (CT)	Time when process completes its execution.
// Turn-Around Time (TAT)	Total time taken from arrival to completion.
// 👉 TAT = CT − AT
// Waiting Time (WT)	Time a process spends waiting in the ready queue.
// 👉 WT = TAT − BT
// Response Time (RT)	Time between arrival and first CPU allocation.
// 🧮 Scheduling Algorithms
// 1️⃣ First Come First Serve (FCFS)

// Non-preemptive algorithm.

// Processes are executed in the order they arrive in the ready queue.

// Implemented using a simple FIFO queue.

// Advantages

// Easy to implement.

// Fair – each process gets CPU in order of arrival.

// Disadvantages

// Leads to convoy effect – short jobs wait for long ones.

// Average waiting time may be large.

// 2️⃣ Shortest Job First (SJF)

// Process with the shortest burst time is executed next.

// Can be preemptive (Shortest Remaining Time First) or non-preemptive.

// Advantages

// Minimizes average waiting time (optimal for a fixed set of processes).

// Disadvantages

// Requires knowledge of burst time in advance.

// May cause starvation for long processes.

// 3️⃣ Priority Scheduling

// Each process is assigned a priority.

// The process with the highest priority (lowest numeric value) executes first.

// Can be preemptive or non-preemptive.

// Advantages

// Suitable for real-time and critical tasks.

// Disadvantages

// Low-priority processes may starve.

// Aging technique can be used to prevent starvation (increase priority over time).

// 4️⃣ Round Robin (RR)

// Each process gets a fixed time quantum (q).

// CPU cycles through all ready processes in a circular queue.

// If a process doesn’t finish in its time slice, it’s preempted and placed at the queue’s end.

// Advantages

// Fair and responsive; ideal for time-sharing systems.

// No starvation.

// Disadvantages

// High context-switch overhead if the quantum is too small.

// If quantum is too large, RR ≈ FCFS.

// 📊 Performance Metrics
// Metric	Formula	Description
// Turn-Around Time	TAT = CT – AT	Total time from submission to completion.
// Waiting Time	WT = TAT – BT	Time spent waiting in ready queue.
// Average TAT	(Σ TAT) / n	Mean turnaround time for all processes.
// Average WT	(Σ WT) / n	Mean waiting time for all processes.
// 🧱 Example
// PID	AT	BT	Priority
// P1	0	5	2
// P2	1	3	1
// P3	2	8	3

// FCFS Order: P1 → P2 → P3
// Round Robin (q = 2): P1 → P2 → P3 → P1 → P2 → P3 …

// Use formulas to compute CT, TAT, WT.

// ⚙️ Comparison Summary
// Algorithm	Type	Preemption	Key Idea	Pros	Cons
// FCFS	Non-Preemptive	❌	Serve in arrival order	Simple	Convoy effect
// SJF	Both	✅/❌	Shortest burst first	Optimal avg WT	Starvation
// Priority	Both	✅/❌	Highest priority first	Flexible	Starvation
// Round Robin	Preemptive	✅	Time quantum rotation	Fair, responsive	Context-switch overhead
// 🧠 Advantages of CPU Scheduling

// Efficient utilization of CPU resources.

// Improves system performance metrics.

// Supports multitasking and responsiveness.

// ⚠️ Disadvantages

// Complex to implement with preemption.

// Requires accurate estimation of burst time.

// Risk of starvation for low-priority tasks.

// 🧩 Viva Questions and Answers
// Question	Answer (Short & Precise)
// Q1: What is CPU scheduling?	Deciding which process in the ready queue gets the CPU next.
// Q2: Why is scheduling required?	To maximize CPU utilization and system throughput.
// Q3: Differentiate between preemptive and non-preemptive scheduling.	Preemptive → CPU can be taken away mid-execution; Non-preemptive → CPU held until process finishes.
// Q4: What is context switching?	Saving and restoring process state when CPU switches from one process to another.
// Q5: What is turnaround time?	Time from process arrival to completion = CT – AT.
// Q6: What is waiting time?	Time process spends waiting = TAT – BT.
// Q7: Which algorithm suffers from the convoy effect?	FCFS.
// Q8: Which algorithm gives minimum average waiting time?	SJF (Shortest Job First).
// Q9: What is starvation?	Continuous postponement of a process due to scheduling preference of others.
// Q10: How can starvation be avoided in priority scheduling?	Using aging – gradually increase waiting process’s priority.
// Q11: Which algorithm is best suited for time-sharing systems?	Round Robin.
// Q12: What is time quantum?	Fixed CPU time allotted to each process in RR scheduling.
// Q13: What happens if the quantum is very large?	RR behaves like FCFS.
// Q14: What happens if the quantum is very small?	Excessive context switches → system overhead.
// Q15: Which performance parameters are used to evaluate scheduling algorithms?	CPU utilization, throughput, waiting time, turnaround time, response time.
// ✅ Conclusion

// CPU scheduling algorithms are essential to efficient process management in multiprogramming systems.
// Each algorithm has trade-offs between fairness, throughput, and response time.
// The best choice depends on the workload type —

// Batch systems: SJF or Priority,

// Interactive systems: Round Robin,

// Simple sequential systems: FCFS.