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
