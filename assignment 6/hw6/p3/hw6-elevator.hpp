// multiple rider case attempt
/*
Passenger List: elevator maintains list of passengers (pairs of person_id and destination floor)
Batch Processing: elevator picks up as many passengers as possible from the queue if they are on the same floor, respecting the maximum occupancy
Route Management: elevator manages passengers' routes, moving to each passenger's destination floor and logging the necessary information
Complete Logging: detailed logs for each step including entering and exiting the elevator are added to provide comprehensive traceability
also
Occupancy Declaration: occupancy variable initialized to 0 at the start of the elevator function
Occupancy Management: increment occupancy each time a passenger enters the elevator and decrement each time a passenger exits.
*/

#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <random>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <tuple>
#include <unordered_map>

using namespace std;

// System parameters
const int NUM_FLOORS = 50;
const int NUM_ELEVATORS = 6;
const int MAX_OCCUPANCY = 5;
const int MAX_WAIT_TIME = 5000; // milliseconds

// Global variables
mutex cout_mtx;
mutex queue_mtx;
condition_variable cv;
queue<tuple<int, int, int>> global_queue; // person_id, start_floor, dest_floor
vector<int> elevator_positions(NUM_ELEVATORS, 0);
atomic<int> num_people_serviced(0);
vector<int> global_passengers_serviced(NUM_ELEVATORS, 0);
int npeople;

// Elevator thread: picks up, transports, and drops off passengers
inline void elevator(int id) 
{
    int occupancy = 0; // Initialize occupancy for each elevator
    vector<pair<int, int>> passengers; // pairs of (person_id, dest_floor)
    while (true)
    {
        unique_lock<mutex> lock(queue_mtx);
        cv.wait(lock, []
            {
            return !global_queue.empty() || num_people_serviced.load() >= npeople;
        });

        // Terminate when all have been serviced and queue is empty and no one onboard
        if (num_people_serviced.load() >= npeople && global_queue.empty() && passengers.empty()) 
        {
            break;
        }

        // Batch board up to MAX_OCCUPANCY riders
        while (occupancy < MAX_OCCUPANCY && !global_queue.empty()) 
        {
            auto req = global_queue.front();
            global_queue.pop();
            int pid = get<0>(req);
            int sf  = get<1>(req);
            int df  = get<2>(req);
            passengers.emplace_back(pid, df);
            occupancy++;

            // Log pickup and move to start floor
            {
                lock_guard<mutex> lg(cout_mtx);
                cout << "Elevator " << id << " moving to floor " << sf
                     << " to pick up Person " << pid << endl;
            }
            lock.unlock();
            this_thread::sleep_for(chrono::milliseconds(abs(elevator_positions[id] - sf) * 100));
            elevator_positions[id] = sf;
            lock.lock();
        }
        lock.unlock();

        // Deliver each passenger
        for (auto &p : passengers) 
        {
            int pid = p.first;
            int df  = p.second;

            // Log move and travel to destination
            {
                lock_guard<mutex> lg(cout_mtx);
                cout << "Elevator " << id << " moving to floor " << df
                     << " with Person " << pid << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(abs(elevator_positions[id] - df) * 100));
            elevator_positions[id] = df;

            // Log drop-off and update counters
            {
                lock_guard<mutex> lg(cout_mtx);
                cout << "Person " << pid << " exited elevator " << id
                     << " at floor " << df << endl;
            }
            occupancy--;
            global_passengers_serviced[id]++;
            num_people_serviced++;
        }
        passengers.clear();

        // Notify any waiting elevators or persons
        cv.notify_all();
    }

    // Final summary
    {
        lock_guard<mutex> lg(cout_mtx);
        cout << "Elevator " << id << " has finished servicing all people." << endl;
        cout << "Elevator " << id << " serviced " << global_passengers_serviced[id] << " passengers." << endl;
    }
}

// Person thread: generates request and notifies elevators
inline void person(int id) {
    int curr_floor = rand() % NUM_FLOORS;
    int dest_floor = rand() % NUM_FLOORS;
    while (dest_floor == curr_floor) {
        dest_floor = rand() % NUM_FLOORS;
    }

    // Log request
    {
        lock_guard<mutex> lg(cout_mtx);
        cout << "Person " << id << " wants to go from floor " << curr_floor << " to floor " << dest_floor << endl;
    }
    
    // Enqueue request and notify elevators
    {
        lock_guard<mutex> lg(queue_mtx);
        global_queue.emplace(id, curr_floor, dest_floor);
    }
    cv.notify_all();
}

#endif // ELEVATOR_HPP

