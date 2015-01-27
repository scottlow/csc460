/*
 * scheduler.h
 *
 * Defintion for a Time Triggered Architecture scheduler. 
 * This is a simple architecture that supports periodic scheduling.
 *
 * created: 27-01-2015
 * author: Simon Diemert, Scott Low
 *
 * Based on: http://nrqm.ca/mechatronics-lab-guide/lab-guide-time-triggered-scheduling/
 */


//TYPES, DEFINES

#define MAXTASKS 10 

//define type for task callbacks. 
typedef void (*task_callback)(); 

typedef struct{
    uint32_t period; 
    uint32_t remaining_time; 
    uint8_t is_running;
    task_callback callback; 
}task_t; 

typedef enum{
    STATUS_OK = 0,
    STATUS_FAIELD = 1
}status_code_t; 

//FUNCTIONS


/**
 * Initalize scheduler, called once at the beginning of the program. 
 */
void scheduler_init(); 

/**
 * Starts a task in the scheduler. 
 *
 * Tasks are called approximately every @param period, it does not provide a guarantee 
 * that the task will be executed on time, all tasks are executed until completetion
 * so if a the previous task takes longer than @param peroid of this task it will 
 * interfer with the execution of this task. 
 *
 * @param delay - The delay (millseconds) from time zero to start the task, 
 *                  sometimes called offset. 
 * @param period - The task will be run every "period" milliseconds. 
 * @param task - The callback funciton that represents the funcitonality for the task.  
 *                  this function is called whenever the task is executed. 
 *
 */
status_code_t scheduler_start_task(int16_t delay, int16_t period, task_callback cb); 


/**
 * Chooses the next task from a list to run. 
 * This function should be called as often as possible 
 * from the main function/loop. 
 */
uint16_t scheduler_dispatch(); 

/**
 * Idle for @param period milliseconds. 
 */
void idle(uint32_t period); 

