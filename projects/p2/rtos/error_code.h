/**
 * @file   error_code.h
 *
 * @brief Error messages returned in OS_Abort().
 *        Green errors are initialization errors
 *        Red errors are runt time errors
 *
 * CSC 460/560 Real Time Operating Systems - Mantis Cheng
 *
 * @author Scott Craig
 * @author Justin Tanner
 */
#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

enum {

/** GREEN ERRORS -- Initialize time errors. */
    
/** PPP invalid -- Names must be in range [0 .. MAXPROCESS] */
ERR_1_PPP_NAME_OUT_OF_RANGE, //1

/** PERIODIC name is out of range [1 .. MAXNAME] */
ERR_2_CREATE_NAME_OUT_OF_RANGE, //2

/** PERIODIC task assigned name IDLE */
ERR_3_PERIODIC_NAMED_IDLE, //3

/** PERIODIC name already used */
ERR_4_PERIODIC_NAME_IN_USE, //4

/** PERIODIC name not in PPP array */
ERR_5_NAME_NOT_IN_PPP, //5

/** Time interval 0 in PPP array */
ERR_6_TIME_INTERVAL_IN_PPP_0, //6

ERR_7_PERIODIC_WCET_TOO_LARGE, //7

ERR_8_TOO_MANY_PERIODIC_TASKS, //8


/** RED ERRORS -- Run time errors. */

/** User called OS_Abort() */
ERR_RUN_1_USER_CALLED_OS_ABORT, //9

/** Too many tasks created. Only allowed MAXPROCESS at any time.*/
ERR_RUN_2_TOO_MANY_TASKS, //10

/** PERIODIC task still running at end of time slot. */
ERR_RUN_3_PERIODIC_TOOK_TOO_LONG, //11

/** ISR made a request that only tasks are allowed. */
ERR_RUN_4_ILLEGAL_ISR_KERNEL_REQUEST, //12

/** RTOS Internal error in handling request. */
ERR_RUN_5_RTOS_INTERNAL_ERROR, //13

ERR_RUN_6_SERVICE_INIT_ERROR, //14

ERR_RUN_7_TOO_FEW_SERVICES, //15

ERR_RUN_8_SERVICE_ILLEGAL_LEVEL, //16

ERR_RUN_9_SERVICE_ILLEGAL_OPERATION, //17

ERR_RUN_10_PERIODIC_TASK_CONFLICT, //18

};


#endif
