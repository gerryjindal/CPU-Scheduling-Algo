#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// first int represents id of process -1 in case of gaps second int represents the length of block
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
        completion_time = -1;
        turn_around_time = -1;
        waiting_time = -1;
        response_time = -1;
        remaining_time = bt;
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
        }

        if(pid < p1.pid){
            return true;
        }
        return false;
    }

};

// Reverse comparison function in order to convert the max priority queue into min priority queue
struct ready_queue_compare {
    bool operator()(process& p1, process& p2){
        if(p1.remaining_time < p2.remaining_time){
            return false;
        }else if(p1.remaining_time > p2.remaining_time){
            return true;
        }else if(p1.arrival_time < p2.arrival_time){
            return false;
        }else if(p1.arrival_time > p2.arrival_time){
            return true;
        }else if(p1.pid < p2.pid){
            return false;
        }
        return true;
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
// This function calculates completion time, response time, turn around time, waiting time and also
// the structure for the gantt chart.
void calc_attributes(vector<process>& list){
    priority_queue<process, vector<process>, ready_queue_compare> ready_queue;
    int n = list.size();
    int next_process = 0; // index in list of process's 
    int curr_process =0; // pid of current process
    int prev_process =0; // pid of prev process
    int curr_time = 0;
    int process_length = 0;
    bool first_loop = true;

    while(true){
        while(next_process<n && list[next_process].arrival_time <= curr_time){
            ready_queue.push(list[next_process]);
            next_process++;
        }
        // gap
        if(ready_queue.empty()){
            if(next_process == n){
                break;
            }
            curr_process = -1;
            // if Switching is there then push in gantt chart
            // first time don't push because no prev_process
            if((!first_loop) && curr_process != prev_process){
                gantt_chart.push_back({prev_process, process_length});
                process_length = 0;
            }
            int time_interval = list[next_process].arrival_time - curr_time;
            process_length += time_interval;
            curr_time += time_interval;
            prev_process = -1;
        }
        // process
        if(!ready_queue.empty()){
            curr_process = ready_queue.top().pid;
            // Should we push in gantt chart ? 
            if((!first_loop) && curr_process != prev_process){
                gantt_chart.push_back({prev_process, process_length});
                process_length = 0;
            }

            int process_index;
            for(int i=0; i<n; i++){
                if(list[i].pid == curr_process){
                    process_index = i;
                    break;
                }
            }
            int time_interval;
            if(next_process == n){
                time_interval = list[process_index].remaining_time;
            }else{
                time_interval = min(list[process_index].remaining_time, list[next_process].arrival_time - curr_time);
            }
            // response time
            if(list[process_index].remaining_time == list[process_index].burst_time){
                list[process_index].response_time = curr_time - list[process_index].arrival_time;
            }
            list[process_index].completion_time = curr_time + time_interval;
            scheduling_length = list[process_index].completion_time;
            list[process_index].remaining_time -= time_interval;
            if(list[process_index].remaining_time == 0){
                ready_queue.pop();
            }
            process_length += time_interval;
            curr_time += time_interval;
            prev_process = curr_process;
        }
        // first loop
        if(first_loop){
            first_loop = false;
        }
        // last loop
        if(ready_queue.empty() && next_process == n){
            gantt_chart.push_back({curr_process, process_length});
        }
    }
    // calc TAT and WT
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

    vector<process> list = {
        process(1, 0, 2),
        process(2, 1, 7),
        process(3, 2, 4),
        process(4, 3, 1)
    };
    
    sort(list.begin(), list.end());
    calc_attributes(list);
    
    print_table(list);
    calc_avg(list);
    print_gantt_chart(list);

    // Case 2
    // Taking input from user 
    
    int n;
    cout << "Enter no. of process: ";
    cin >> n;
    vector<process> list2;

    for(int i=0; i<n; i++){
        int pid, at, bt;
        cout << "Enter the pid, at and bt of process "<< i+1 <<" : ";
        cin >> pid >> at >> bt;
        list2.push_back(process(pid, at, bt));
    }

    sort(list2.begin(), list2.end());
    calc_attributes(list2);

    print_table(list2);
    calc_avg(list2);
    print_gantt_chart(list2);
    
    return 0;
}