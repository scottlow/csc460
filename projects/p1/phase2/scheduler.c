/*
 * Implementation of Time Triggered Architecture defined in scheduler.h.
 *
 * created: 27-01-2015
 * author: Simon Diemert, Scott Low
 *
 * Based on: http://nrqm.ca/mechatronics-lab-guide/lab-guide-time-triggered-scheduling/
 */

#include "scheduler.h"
#include <arduino/Arduino.h>

task_t tasks[MAXTASKS];  //array of tasks, indexed by task indexes passed to scheduler_start_tasks()
uint32_t last_runtime; 

void scheduler_init(){
    last_runtime = millis();  //get the current system time and store it.
}

status_code_t scheduler_start_task(int16_t delay, int16_t period, task_callback cb){

    static uint16_t id = 0;  //index into the tasks array. 

    if(id < MAXTASKS){
        tasks[id].remaining_time = delay; 
        tasks[id].period = period; 
        tasks[id].is_running = 1; 
        tasks[id].cb = 1; 
        id++; 
        return STATUS_OK;  //we created the new task, return OK
    }else{
        return STATUS_FAILED; //to many tasks, return FAILED
    }
}

uint16_t scheduler_dispatch(){

    uint8_t i = 0; 

    uint32_t now = millis(); 
    uint32_t elapsed = now - last_runtime; 
    last_runtime = now; 
    task_callback t = NULL; 
    uint32_t idle_time = 0xFFFFFFFF; 

    for(i = 0; i < MAXTASKS; i++){
        if(tasks[i].is_running){
            tasks[i].remaining_time -= elapsed; 
            if(tasks[i].remaining_time <= 0){
                if(t == NULL){
                    t = tasks[i].callback; 
                    tasks[i].remaining_time += tasks[i].period; 
                } 
                idle_time = 0; 
            }
        }else{
            idle_time = min((uint32_t)tasks[i].remaining_time, idle_time); 
        }
    }
    if(t != NULL){
        t(); 
    }
    return idle_time; 
}

void idle(uint32_t period){
    delay(period); 
}
