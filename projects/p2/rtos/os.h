#ifndef _OS_H_  
#define _OS_H_ 

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file os.h
 * \brief A simple RTOS interface  
 *
 * \mainpage A Simple RTOS
 * This is a simple RTOS that supports pre-emptive multithreading.
 * 
 * \b Note: Please don't edit the interface file "os.h".
 *
 * \author Dr. Mantis Cheng
 * \date 5 Feb 2015
 *
 *
 * \section assumptions GLOBAL ASSUMPTIONS 
 *
 *    (ATMEL specific)
 *   - Timer2 and SWI interrupts are reserved.
 * 
 *   - All runtime exceptions (where assumptions are violated) or other  
 *     unrecoverable errors get handled by calling OS_Abort().  
 *   - All unspecified runtime errors have undefined behaviours, 
 *     e.g., stack overflow.
 *
 *   Our RTOS scheduler timer resolution is defined by TICK. Hence, all timing parameters must
 *   be defined in multiples of TICKs. For PERIODIC tasks, its period, wcet and start time
 *   must be a multiple of TICKs. For RR tasks, its quantum is also defined in terms of TICKs.
 *
 *
 * \section policy SCHEDULING POLICY   
 *
 *   There are three scheduling levels: SYSTEM, PERIODIC and RR.
 *   These levels are prioritized with SYSTEM being the highest, and 
 *   RR being the lowest.
 *
 *   Preemption occurs immediately. Whenever preemption is feasible, it takes
 *   place instantly. As soon as a higher priority task becomes ready, it
 *   preempts all lower priority tasks.
 *
 *
 * \section system SYSTEM TASKS
 *
 *   SYSTEM (level) tasks are FCFS; they run to completion, i.e., until they
 *   terminate, block or yield. Thus, they are non-preemptible, not even by
 *   other SYSTEM tasks. They should only be used for critical system level
 *   activities, e.g., error or fault recovery. Running too many SYSTEM tasks 
 *   could affect the real time performance of all other low level tasks, 
 *   including PERIODIC tasks; therefore, use SYSTEM tasks sparingly and keep
 *   their execution time short. 
 *
 *
 * \section periodic PERIODIC TASKS
 *
 *   PERIODIC tasks are scheduled based on a per-task scheduling period,
 *   worst-case execution time (wcet) and start time. Periodic tasks are time-critical;
 *   they are \b NOT allowed to wait on any synchronization primitives, e.g.
 *   Service_Subscribe(). For example,
 *
 *   void p() { ... }
 *
 *   e.g., Task_Create_Periodic( p, 0, 20, 1, 2 );
 *
 *   This will create a PERIODIC task executing the function p() with a period of
 *   20 TICKs (100 msecs, or 10 Hz), a WCET of 1 TICK (5 msec) and will start 2 TICKs
 *   after booting. WCET must be less than the period.
 *
 *   That is, after 10 msecs, this task will execute at time 10, 110, 210, 310, etc., with a
 *   worst case execution time of 5 msec during each period of 100 msec.
 *
 *   It is an \b error (exception) if this task executes more than its wcet during each cycle.
 * 
 *   This task may call Task_Next() before its allowed WCET. The remaining time till the
 *   end of its period is known as its "DELAYED" time.
 *
 *   It is the responsibility of the Application Engineers to ensure that no two PERIODIC
 *   tasks overlapping in execution. It is an \b error if two PERIODIC tasks are ready
 *   at the same time!!!  In other words, from the start of a periodic task until the end
 *   of its wcet, no other periodic task should be scheduled.
 *
 *   It is an \b error if a PERIODIC task executes longer than the allowed WCET.
 *   All timing violations should be caught and then reported.  The task must call Task_Next()
 *   before the expiry of its WCET.
 *
 *   When a PERIODIC task is preempted, its worst-case execution time will be stretched.
 *   In a similar way, when a RR task is preempted, its allowed quantum will be stretched.
 *   In theory, the RTOS should only accumulate the actual execution time; but in practice,
 *   the actual execution time could only be approximated due to the limitation of the
 *   scheduling timer resolution (1 TICK).
 *   
 *   It is an error if a PERIODIC task waits on an Event.
 *
 *
 * \section roundrobin ROUND-ROBIN TASKS
 *   
 *   RR tasks are FIFO, and they run at the lowest priority level. 
 *   If a RR task becomes ready, it will be executed only when there are no other SYSTEM 
 *   or PERIODIC tasks running. Each RR task gets one quantum that is 1 TICK in length.
 *   At the end of its quantum, a RR task is re-enqueued in the RR queue.
 *   However, if a RR task is preempted before its quantum expires, it re-enters at the 
 *   front of its queue.
 *
 *   Only SYSTEM and RR tasks may wait for a service. Any task can send notifications over a service. 
 *
 *
 * \section ipc SERVICES
 *
 *   Services are used to transfer data safely between tasks.  Any number of tasks can wait on a
 *   service by calling Service_Subscribe(), at which point the calling task enters the WAITING state.
 *   A WAITING task does not wake up and return from Service_Subscribe until another task sends a
 *   notification to the service being waited on.  Any running task may send a notification to a 
 *   service by calling Service_Publish().  The notification contains a single 16-bit integer.
 *
 *   There is no way to guarantee that a service will receive a notification before a task's 
 *   time slice expires, so PERIODIC tasks are not permitted to subscribe to services.  If a PERIODIC
 *   task attempts to subscribe to a service, then it is a runtime error.  However, PERIODIC tasks
 *   are permitted to publish notifications to a service.  Interrupts can also publish to a service.
 *
 *   When a notification is published to a service, all subscribers are immediately woken and receive
 *   the message.  Only the latest notificaton is stored.  If a new notification comes in it overwrites
 *   the previous notification.
 *   
 *   Example:
 *   - A RR task that sends sensor values over a radio subscribes to a service
 *   - Three PERIODIC tasks read from three different ultrasonic range finders on a schedule
 *   - When one of the PERIODIC tasks has new data, it publishes a notification to the service
 *   - Every time the RR task receives a notification, it sends the received value over the radio.
 *
 *   (this concept can be extended to RPC across several agents, but this feature is not required
 *   for project 2)
 *
 * \section clock CLOCK AND HIGH RESOLUTION TIMER  
 *    
 *   Every RTOS should maintain a "clock" for simple timing services, e.g., measuring timing
 *   interval between two time epochs. The higher the resolution the clock, the more accurate
 *   the time measurement is. An RTOS also uses a timer for scheduling decision, e.g., when to
 *   run a PERIODIC task. If the scheduler is invoked very often, e.g., every millisecond, it
 *   will create an unacceptable overhead where a high percentage of CPU time is wasted on
 *   on scheduling decision and not executing user tasks. So, typically a 5 milliseconds
 *   scheduling interval is a good compromise between overhead and response time.
 *    
 *   Now() is our system clock which gives a millisecond accuracy for timing measurement.
 *   We use Timer2 for this purpose to generate a scheduling timer interrupt every 5 milliseconds.
 *   However, Now() should provide a time resolution of 1 millisecond. By combining the total
 *   number of TICKs and the current free running counter inside a timer, we can provide a more
 *   precise high resolution timer.  
 *
 *
 * \section boot OS BOOTING
 *
 *   Our RTOS is compiled together with an application. The standard default
 *   "main()" function is defined inside the os.c file, which is essentially
 *   "OS_Init()". Application that uses this RTOS will define a new function
 *   called "r_main()" which will be created as the first SYSTEM level task.
 *     -# main() is called from crt0.S as the very first C function to be
 *        executed inside os.c.
 *     -# Upon completion of RTOS initialization, the application's "r_main()" is
 *        created as the first and only SYSTEM level task.
 *     -# In "r_main()", the rest of the application tasks are then created.
 *     -# In order for all other application tasks to run, your "r_main() task
 *        must either terminate or block. (For example, "r_main()"
 *        may become a "watchdog" task to reset the entire application.)
 *
 */  
#include "stdint.h"
   
/*==================================================================  
 *             T Y P E S   &   C O N S T A N T S  
 *==================================================================  
 */  

/*================
  *    C O N S T A N T S
  *================
  */
/* limits */

/** max. number of processes supported */  
#define MAXPROCESS		8   

/** time resolution */
#define TICK			    5     // resolution of system clock in milliseconds
#define QUANTUM       5     // a quantum for RR tasks

/** thread runtime stack */
#define MAXSTACK      256   // bytes

/* scheduling levels */

/** a scheduling level: system tasks with first-come-first-served policy 
 * \sa \ref system, Task_Create().
 */
#define SYSTEM    3 

/** a scheduling level: periodic tasks with predefined intervals 
 * \sa \ref periodic, Task_Create().
 */
#define PERIODIC  2 

/** A scheduling level: first-come-first-served cooperative tasks
 * \sa \ref sporadic, Task_Create(). 
 */
#define RR        1      

#ifndef NULL
#define NULL     0   /* undefined */
#endif

#define IDLE     0  


/*================
  *    T Y P E S
  *================
  */
/** A service descriptor
 * \sa Service_Init().
 */
typedef struct service SERVICE;  


/*================
  *    G L O B A L S
  *================
  */
 
  
  
/*==================================================================  
 *             A C C E S S    P R O C E D U R E S  
 *==================================================================  
 */  

  /*=====  OS   Initialization ===== */

/** Initialize this RTOS; the standard default "main()" is defined
 *  inside os.c, which is essentially OS_Init().
 * \sa \ref boot.
 *
void OS_Init(void);
 */


/** Abort the execution of this RTOS due to an unrecoverable erorr.
 * \sa \ref assumptions. 
 */
void OS_Abort();  


  /*=====  Task API ===== */

 /**
   * \param f  a parameterless function to be created as a process instance
   * \param arg an integer argument to be assigned to this process instanace
   * \return 0 if not successful; otherwise non-zero.
   * \sa Task_GetArg()
   *
   *  A new process is created to execute the parameterless
   *  function \a f with an initial parameter \a arg, which is retrieved
   *  by a call to Task_GetArg().  If a new process cannot be
   *  created, 0 is returned; otherwise, it returns non-zero.
   *
   * \sa \ref policy
   */
int8_t   Task_Create_System(void (*f)(void), int16_t arg);
int8_t   Task_Create_RR(    void (*f)(void), int16_t arg);

 /**
   * \param f a parameterless function to be created as a process instance
   * \param arg an integer argument to be assigned to this process instanace
   * \param period its execution period in TICKs
   * \param wcet its worst-case execution time in TICKs, must be less than "period"
   * \param start its start time in TICKs
   * \return 0 if not successful; otherwise non-zero.
   * \sa Task_GetArg()
   *
   *  A new process is created to execute the parameterless
   *  function \a f with an initial parameter \a arg, which is retrieved
   *  by a call to Task_GetArg().  If a new process cannot be
   *  created, 0 is returned; otherwise, it returns non-zero.
   *
   * \sa \ref policy
   */
int8_t   Task_Create_Periodic(void(*f)(void), int16_t arg, uint16_t period, uint16_t wcet, uint16_t start);

/** 
 * Terminate the calling process
 *
 *  When a process returns, i.e., it executes its last instruction in 
 *  the associated function/code, it is automatically terminated.
 */
void Task_Terminate();    

/** Voluntarily relinquish the processor. */
void Task_Next();

/** Retrieve the assigned parameter. 
  * \sa Task_Create(). 
  */
int16_t Task_GetArg();          


  /*=====  Events API ===== */

/**
 * \return a non-NULL SERVICE descriptor if successful; NULL otherwise.
 *
 *  Initialize a new, non-NULL SERVICE descriptor.
 */
SERVICE *Service_Init();

/**  
  * \param s an Service descriptor
  * \param v pointer to memory where the received value will be written
  *
  * The calling task waits for the next published value associated with service "s".
  * More than one task may wait for a service. When a new value "v" is published to
  * "s", all waiting tasks resume and obtain the same value. 
  */
void Service_Subscribe( SERVICE *s, int16_t *v );


/**  
  * \param e a Service descriptor
  *
  * The calling task publishes a new value "v" to service "s". All waiting tasks on
  * service "s" will be resumed and receive a copy of this value "v". 
  * Values generated by services without subscribers will be lost.
  */
void Service_Publish( SERVICE *s, int16_t v );


   
  /*=====  System Clock API ===== */
  
/**  
  * Returns the number of milliseconds since OS_Init(). Note that this number
  * wraps around after it overflows as an unsigned integer. The arithmetic
  * of 2's complement will take care of this wrap-around behaviour if you use
  * this number correctly.
  * Let  T = Now() and we want to know when Now() reaches T+1000.
  * Now() is always increasing. Even if Now() wraps around, (Now() - T) always
  * >= 0. As long as the duration of interest is less than the wrap-around time,
  * then (Now() - T >= 1000) would mean we have reached T+1000.
  * However, we cannot compare Now() against T directly due to this wrap-around
  * behaviour.
  * Now() will wrap around every 65536 milliseconds. Therefore, for measurement
  * purposes, it should be used for durations less than 65 seconds.
  */
uint16_t Now();  // number of milliseconds since the RTOS boots.

#ifdef __cplusplus
}
#endif

#endif /* _OS_H_ */
