#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

// Shared variables for synchronization
atomic<bool> interested[2] = {false, false}; // Atomic for thread safety
atomic<int> turn = 0;

// Entry section for process
void Entry_Section(int process) {
    int other = 1 - process; 
    interested[process] = true; 
    turn = process; 
    // Wait until the other process is not interested or it's not our turn
    while (interested[other] && turn == other) {
        // Yield the CPU to other threads to avoid busy-waiting
        this_thread::yield(); 
    }
}

// Critical section where actual work is done
void Critical_Section(int process) {
    cout << "Process " << process << " is entering the critical section." << endl;
    // Simulate work in the critical section
    this_thread::sleep_for(chrono::seconds(2)); 
    cout << "Process " << process << " is exiting the critical section." << endl;
}

// Exit section for process
void Exit_Section(int process) {
    interested[process] = false; 
}

// Function that simulates a process
void Process(int process) {
    Entry_Section(process);
    Critical_Section(process);
    Exit_Section(process);
}

int main() {
    thread p1(Process, 0); // Start process 0 (P1)
    thread p2(Process, 1); // Start process 1 (P2)
    
    p1.join(); // Wait for process 0 to complete
    p2.join(); // Wait for process 1 to complete
    
    return 0;
}
