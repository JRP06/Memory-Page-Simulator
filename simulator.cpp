#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

// --------------------------
// Q1: Memory allocation
// --------------------------
vector<int> doAlloc(vector<int> parts, vector<int> procs, int type) {
    vector<int> alloc(procs.size(), -1);

    for (int i = 0; i < procs.size(); i++) {
        int p = procs[i];
        int pick = -1;

        if (type == 0) { 
            // first-fit
            for (int j = 0; j < parts.size(); j++) {
                if (parts[j] >= p) {
                    pick = j;
                    break;
                }
            }
        }
        else if (type == 1) { 
            // best-fit
            int best = INT_MAX;
            for (int j = 0; j < parts.size(); j++) {
                if (parts[j] >= p && parts[j] < best) {
                    best = parts[j];
                    pick = j;
                }
            }
        }
        else { 
            // worst-fit
            int worst = -1;
            for (int j = 0; j < parts.size(); j++) {
                if (parts[j] >= p && parts[j] > worst) {
                    worst = parts[j];
                    pick = j;
                }
            }
        }

        if (pick != -1) {
            alloc[i] = pick;
            parts[pick] -= p;
        }
    }

    return alloc;
}

// ------------------------------------------------------------
// Q2: page + offset
// ------------------------------------------------------------
void pageOffset(long addr) {
    long page = addr / 1024;
    long off = addr % 1024;
    cout << addr << " -> page " << page << ", offset " << off << endl;
}

// ------------------------------------------------------------
// Q3: bits
// ------------------------------------------------------------
int bits(int x) {
    int b = 0;
    while (x > 1) { x /= 2; b++; }
    return b;
}

// ------------------------------------------------------------
// Page replacement helpers (Q4)
// ------------------------------------------------------------

int fifo(const vector<int> & refs, int f) {
    vector<int> fr;
    queue<int> q;
    int faults = 0;

    for (int r : refs) {
        bool hit = false;
        for (int x : fr) if (x == r) hit = true;
        if (hit) continue;

        faults++;
        if (fr.size() < f) {
            fr.push_back(r);
            q.push(r);
        } else {
            int victim = q.front(); q.pop();
            for (int i = 0; i < fr.size(); i++) {
                if (fr[i] == victim) {
                    fr[i] = r;
                    break;
                }
            }
            q.push(r);
        }
    }
    return faults;
}

int lru(const vector<int> &refs, int f) {
    vector<int> fr;
    vector<int> last;
    int faults = 0;

    for (int t = 0; t < refs.size(); t++) {
        int r = refs[t];
        bool hit = false;

        for (int i = 0; i < fr.size(); i++) {
            if (fr[i] == r) {
                hit = true;
                last[i] = t;
            }
        }
        if (hit) continue;

        faults++;
        if (fr.size() < f) {
            fr.push_back(r);
            last.push_back(t);
        } else {
            int idx = 0;
            for (int i = 1; i < last.size(); i++) {
                if (last[i] < last[idx]) idx = i;
            }
            fr[idx] = r;
            last[idx] = t;
        }
    }
    return faults;
}

int optimal(const vector<int> &refs, int f) {
    vector<int> fr;
    int faults = 0;

    for (int i = 0; i < refs.size(); i++) {
        int r = refs[i];
        bool hit = false;

        for (int x : fr) if (x == r) hit = true;
        if (hit) continue;

        faults++;
        if (fr.size() < f) {
            fr.push_back(r);
        } else {
            int idx = -1, far = -1;
            for (int j = 0; j < fr.size(); j++) {
                int next = INT_MAX;
                for (int k = i + 1; k < refs.size(); k++) {
                    if (refs[k] == fr[j]) {
                        next = k;
                        break;
                    }
                }
                if (next == INT_MAX) { idx = j; break; }
                if (next > far) { far = next; idx = j; }
            }
            fr[idx] = r;
        }
    }
    return faults;
}

// ------------------------------------------------------------
// Print Q5 ranking
// ------------------------------------------------------------
void rankQ5() {
    cout << "Optimal: best, no Belady\n";
    cout << "LRU: very good, no Belady\n";
    cout << "Second-chance: ok, CAN have Belady\n";
    cout << "FIFO: worse, has Belady\n";
}


int main() {

    // ---------------- Q1 ----------------
    cout << "\n===== Q1 =====\n";
    vector<int> parts = {300,600,350,200,750,125};
    vector<int> procs = {115,500,358,200,375};

    vector<string> names = {"First-fit","Best-fit","Worst-fit"};

    for (int t = 0; t < 3; t++) {
        auto a = doAlloc(parts, procs, t);
        cout << names[t] << ":\n";
        for (int i = 0; i < a.size(); i++) {
            if (a[i] == -1) cout << "Process " << procs[i] << " KB -> not allocated\n";
            else cout << "Process " << procs[i] << " KB -> partition " << (a[i]+1) << "\n";
        }
        cout << endl;
    }

    // ---------------- Q2 ----------------
    cout << "===== Q2 =====\n";
    pageOffset(3085);
    pageOffset(42095);
    pageOffset(215201);
    pageOffset(650000);
    pageOffset(2000001);
    cout << endl;

    // ---------------- Q3 ----------------
    cout << "===== Q3 =====\n";
    int logicalBits = bits(64) + bits(1024);
    int physicalBits = bits(32) + bits(1024);
    cout << "Logical bits: " << logicalBits << endl;
    cout << "Physical bits: " << physicalBits << endl << endl;

    // ---------------- Q4 ----------------
    cout << "===== Q4 =====\n";
    vector<int> refs = {1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3,6};

    cout << "Frames\tFIFO\tLRU\tOPT\n";
    for (int f = 1; f <= 7; f++) {
        cout << f << "\t" 
             << fifo(refs,f) << "\t"
             << lru(refs,f) << "\t"
             << optimal(refs,f) << "\n";
    }
    cout << endl;

    // ---------------- Q5 ----------------
    cout << "===== Q5 =====\n";
    rankQ5();

    return 0;
}