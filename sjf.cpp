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

    process(int id, int at, int bt){
        pid = id;
        arrival_time = at;
        burst_time = bt;
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
        if(p1.burst_time < p2.burst_time){
            return false;
        }else if(p1.burst_time > p2.burst_time){
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
    int next_process = 0;
    int curr_time = 0;

    while(true){
        while(next_process<n && list[next_process].arrival_time <= curr_time){
            ready_queue.push(list[next_process]);
            next_process++;
        }

        // ready_queue is ready now two cases either gap or process
        // gap 
        if(ready_queue.empty()){
            if(next_process == n){
                break;
            }
            int gap_length = list[next_process].arrival_time - curr_time;
            curr_time += gap_length;
            gantt_chart.push_back({-1, gap_length});
        }

        // process
        if(!ready_queue.empty()){
            int pid = ready_queue.top().pid;
            int curr_process;
            for(int i=0; i<n; i++){
                if(list[i].pid == pid){
                    curr_process = i;
                    break;
                }
            }
            list[curr_process].completion_time = curr_time + list[curr_process].burst_time;
            scheduling_length = list[curr_process].completion_time;
            list[curr_process].response_time = curr_time - list[curr_process].arrival_time;
            list[curr_process].turn_around_time = list[curr_process].completion_time - list[curr_process].arrival_time;
            list[curr_process].waiting_time = list[curr_process].turn_around_time - list[curr_process].burst_time;

            gantt_chart.push_back({pid, list[curr_process].burst_time});
            curr_time += list[curr_process].burst_time;
            ready_queue.pop();
        }
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
        process(1, 2, 2),
        process(2, 5, 3),
        process(3, 3, 4),
        process(4, 6, 4)
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