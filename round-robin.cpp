#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<pair<int, int>> gantt_chart;
int scheduling_length;

class process {
public:

    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turn_around_time;
    int waiting_time;
    int response_time;
    int remaining_time;

    process(int id, int at, int bt){
        pid = id;
        arrival_time = at;
        burst_time = bt;
        remaining_time = bt;
        completion_time = -1;
        turn_around_time = -1;
        waiting_time = -1;
        response_time = -1;
    }

    process(){
        pid = -1;
        arrival_time = -1;
        burst_time = -1;
        completion_time = -1;
        turn_around_time = -1;
        waiting_time = -1;
        response_time = -1;
        remaining_time = -1;
    }

    bool operator < (process p1){
        if(arrival_time < p1.arrival_time){
            return true;
        }else if(arrival_time > p1.arrival_time){
            return false;
        }else if(pid < p1.pid){
            return true;
        }
        return false;
    }
};

void print_table(vector<process>& list){
    cout << "    TABLE" << endl;
    int n = list.size();

    string line = "";
    for(int i=0; i< 113 ; i++){
        line += "-";
    }

    cout << line << endl;
    cout << "|\tPid\t|\tAT\t|\tBT\t|\tCT\t|\tTAT\t|\tWT\t|\tRT\t|" << endl;

    for(auto p: list){
        cout << "|\t"<< p.pid <<"\t|\t" << p.arrival_time <<"\t|\t"<< p.burst_time <<"\t|\t"<< p.completion_time 
        <<"\t|\t"<< p.turn_around_time <<"\t|\t"<< p.waiting_time <<"\t|\t"<< p.response_time <<"\t|" << endl;
    }
    cout << line << endl;
}

// This function is the main working of every scheduling algorithm!!!
void calc_attributes(vector<process>& list, int time_quantum){
    queue<process> ready_queue;
    int n = list.size();
    int next_process = 0;
    int curr_time = 0;

    while(next_process<n && list[next_process].arrival_time <= curr_time){
        ready_queue.push(list[next_process]);
        next_process++;
    }

    while(true){
        // ready_queue is ready now two cases either gap or process
        // gap 
        if(ready_queue.empty()){
            if(next_process == n){
                break;
            }
            int gap_length = list[next_process].arrival_time - curr_time;
            curr_time += gap_length;
            gantt_chart.push_back({-1, gap_length});
            while(next_process<n && list[next_process].arrival_time <= curr_time){
                ready_queue.push(list[next_process]);
                next_process++;
            }
        }

        // process
        if(!ready_queue.empty()){
            // get curr process
            int pid = ready_queue.front().pid;
            int process_index;
            for(int i=0; i<n; i++){
                if(list[i].pid == pid){
                    process_index = i;
                    break;
                }
            }
            process& p = list[process_index];
            // calc time interval
            int time_interval = min(list[process_index].remaining_time, time_quantum);
            // update process attributes
            if(list[process_index].remaining_time == list[process_index].burst_time){
                list[process_index].response_time = curr_time - list[process_index].arrival_time;
            }
            list[process_index].completion_time = curr_time + time_interval;
            scheduling_length = list[process_index].completion_time;
            list[process_index].remaining_time -= time_interval;
            // update gantt chart and curr time
            gantt_chart.push_back({pid, time_interval});
            curr_time += time_interval;
            // update the ready queue
            ready_queue.pop();
            while(next_process<n && list[next_process].arrival_time <= curr_time){
                ready_queue.push(list[next_process]);
                next_process++;
            }
            if(list[process_index].remaining_time != 0){
                ready_queue.push(list[process_index]);
            }                   
        }
    }
    // calc TAT and WT for all process's
    for(auto& p : list){
        p.turn_around_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turn_around_time - p.burst_time;
    }
}

void calc_avg(vector<process>& list){
    int n = list.size();
    float avg_tat = 0;
    float avg_wt = 0;
    float avg_rt = 0;

    for(auto& p: list){
        avg_tat += p.turn_around_time;
        avg_wt += p.waiting_time;
        avg_rt += p.response_time;
    }

    avg_tat = (float)avg_tat/n;
    avg_wt = (float)avg_wt/n;
    avg_rt = (float)avg_rt/n;
    float throughput = (float)n/scheduling_length;

    cout << "Average TAT : " << avg_tat << endl;
    cout << "Average WT : " << avg_wt << endl;
    cout << "Average RT : " << avg_rt << endl;
    cout << "Scheduling Length : " << scheduling_length << endl;
    cout << "Throughput : " << throughput << endl;
}

void print_gantt_chart(vector<process>& list){
    cout << "  GANTT CHART"<< endl;
    int gaps = 0;
    for(auto block : gantt_chart){
        if(block.first == -1){
            gaps++;
        }
    }
    int process_blocks = gantt_chart.size() - gaps;
    
    string line = "";
    for(int i=0; i<16*process_blocks + 8*gaps ; i++){
        line += "-";
    }
    cout << line << endl;

    cout << "|";
    for(auto block : gantt_chart){
        if(block.first == -1){
            cout << "\t|";
        }else{
            cout << "\tP"<< block.first <<"\t|";
        }
    }
    cout << endl;
    cout << line << endl;
    
    cout << 0;
    int curr_time = 0;
    for(auto block: gantt_chart){
        if(block.first == -1){
            cout << "\t";
            curr_time += block.second;
            cout << curr_time;
        }else{
            cout << "\t\t";
            curr_time += block.second;
            cout << curr_time;
        }
    }
    cout << endl;
    gantt_chart.clear();
}

int main(){

    // Case 1
    // Hard coded input ->
    int time_quantum = 2;
    vector<process> list = {
        process(1, 2, 2),
        process(2, 5, 3),
        process(4, 6, 4)
    };
    
    sort(list.begin(), list.end());
    calc_attributes(list, time_quantum);
    
    print_table(list);
    calc_avg(list);
    print_gantt_chart(list);

    // Case 2
    // Taking input from user 
    
    int n;
    cout << "Enter no. of process: ";
    cin >> n;
    cout << "Enter time quantum: ";
    cin >> time_quantum;
    vector<process> list2;

    for(int i=0; i<n; i++){
        int pid, at, bt;
        cout << "Enter the pid, at and bt of process "<< i+1 <<" : ";
        cin >> pid >> at >> bt;
        list2.push_back(process(pid, at, bt));
    }

    sort(list2.begin(), list2.end());
    calc_attributes(list2, time_quantum);

    print_table(list2);
    calc_avg(list2);
    print_gantt_chart(list2);
    
    return 0;
}