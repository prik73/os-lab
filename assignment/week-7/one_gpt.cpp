#include <iostream>
#include <vector>

using namespace std;

// Function to check if resources can be allocated to a process
bool canAllocate(int process, const vector<int>& available, const vector<vector<int>>& need) {
    for (int i = 0; i < available.size(); i++) {
        if (need[process][i] > available[i])
            return false;
    }
    return true;
}

// Banker's Algorithm to check if the system is in a safe state
bool isSafeState(int P, int R, vector<vector<int>>& allocation, vector<vector<int>>& max, vector<vector<int>>& need, vector<int>& available) {
    vector<bool> finished(P, false);  // Track finished processes
    vector<int> safeSequence;         // To store the safe sequence
    int processCount = 0;

    // Loop until all processes are finished
    while (processCount < P) {
        bool allocated = false;

        // Find a process that can be allocated resources
        for (int i = 0; i < P; i++) {
            if (!finished[i] && canAllocate(i, available, need)) {
                // Allocate resources and mark process as finished
                for (int j = 0; j < R; j++) {
                    available[j] += allocation[i][j]; // Release resources
                }

                finished[i] = true;
                allocated = true;
                safeSequence.push_back(i);
                processCount++;
                break;
            }
        }

        // If no allocation was possible, the system is not in a safe state
        if (!allocated) {
            cout << "The system is NOT in a safe state." << endl;
            return false;
        }
    }

    // If all processes are finished, the system is in a safe state
    cout << "The system is in a safe state." << endl;
    cout << "Safe sequence: ";
    for (int i = 0; i < safeSequence.size(); i++) {
        cout << "P" << safeSequence[i];
        if (i != safeSequence.size() - 1) cout << " -> ";
    }
    cout << endl;

    return true;
}

int main() {
    int P, R;

    // Input the number of processes and resources
    cout << "Enter the number of processes: ";
    cin >> P;
    cout << "Enter the number of resource types: ";
    cin >> R;

    vector<vector<int>> max(P, vector<int>(R));         // Max matrix
    vector<vector<int>> allocation(P, vector<int>(R));  // Allocation matrix
    vector<vector<int>> need(P, vector<int>(R));        // Need matrix
    vector<int> available(R);                           // Available resources

    // Input the maximum resource matrix
    cout << "Enter the maximum demand matrix:" << endl;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            cin >> max[i][j];
        }
    }

    // Input the allocation matrix
    cout << "Enter the allocation matrix:" << endl;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            cin >> allocation[i][j];
        }
    }

    // Input the available resources vector
    cout << "Enter the available resources:" << endl;
    for (int i = 0; i < R; i++) {
        cin >> available[i];
    }

    // Calculate the need matrix
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Call the Banker's Algorithm to check if the system is in a safe state
    isSafeState(P, R, allocation, max, need, available);

    return 0;
}
