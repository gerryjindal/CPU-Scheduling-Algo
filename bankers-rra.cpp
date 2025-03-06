// Banker's Resource Request Algorithm
#include <iostream>
#include <vector>

using namespace std;

int main(){
    // GIVEN: 
    // n -> no of process's
    // m -> no of resources
    // available -> available number of resources
    // max -> maximum no of resources a process wants
    // allocation -> no of resources allocated to a process
    int n = 5;
    int m = 4;
    int available[m] = {3, 3, 2, 1};
    vector<int> safe_sequence;

    int max[n][m] = {
        {4, 2, 1, 2},
        {5, 2, 5, 2},
        {2, 3, 1, 6},
        {1, 4, 2, 4},
        {3, 6, 6, 5}
    };
    int allocation[n][m] = {
        {2, 0, 0, 1},
        {3, 1, 2, 1},
        {2, 1, 0, 3},
        {1, 3, 1, 2},
        {1, 4, 3, 2}
    };

    int request_indx;
    int request[m];
    cout << "index of process requesting: ";
    cin >> request_indx;
    cout << "enter the " << m <<" resources requesting(separated by space): ";
    for(int i=0; i<m; i++){
        cin >> request[i];
    } 


    // calc need 
    int need[n][m];
    for (int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Check if request is valid or not
    for(int i=0; i<m; i++){
        if(request[i] > need[request_indx][i] || request[i] > available[i]){
            cout << "Request not valid " <<endl;
            return 0;
        }
    }

    // Updating values 
    for(int i=0; i<m; i++){
        need[request_indx][i] -= request[i];
        available[i] -= request[i];
        allocation[request_indx][i] += request[i];
    } 

    // Now running banker's on updated system
    // Pretending we have granted the requested resource to the process
    int is_completed[n];
    for (int i=0; i<n; i++){
        is_completed[i] = 0;
    }

    int process_executed;
    while(true){
        process_executed = 0;
        for(int i=0; i<n; i++){
            if(is_completed[i] == 0){
                int condition = 1;
            
                for(int k=0; k<m; k++){
                    if(need[i][k]>available[k]){
                        condition = 0;
                    }
                }
                // Process execution
                if(condition){
                    for(int k=0; k<m; k++){
                        available[k] += allocation[i][k];
                    }
                    is_completed[i] =1;
                    cout << " executed process " << i<< endl;
                    safe_sequence.push_back(i);
                    process_executed++;
                }
            }
        }
        if(process_executed == 0){
            break;
        }
    }

    // Check if all completed
    int all_completed = 0;
    for(int i=0; i<n; i++){
        if(is_completed[i] == 0){
            break;
        }else if(i == n-1){
            all_completed = 1;
        }
    }

    // Report if system is in safe or unsafe state
    if(all_completed){
        cout << "Safe state" << endl;
        cout << "Request can be granted" << endl;
        for(auto i: safe_sequence){
            cout << "P" << i << "  ";
        }
        cout << endl;
    }else{
        cout << "Unsafe state" << endl;
        cout << "Request cannot be granted" << endl;
    }

    return 0;
}