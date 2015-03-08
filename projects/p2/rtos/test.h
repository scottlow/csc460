/**
* tests.h
* 
* Contains several tests as required by Project 2:

    getting time using Now()
    creating and running the three task types
    generating a schedule of periodic tasks
    generating an invalid schedule of periodic tasks
    publishing to a service from all three task levels
    publishing to a service from an interrupt
    subscribing to a service from all three task levels (periodic tasks should fail)

*/

/**
* Tests the Now() functionality of the RTOS. 
*
* Expected that the value returned will be the time since the system has started. 
*/
void test_now(void);

/**
*  Tests creating new system tasks. 
*/
void test_create_system_task(void);


/**
* Tests creation new periodic tasks. 
*/
void test_create_periodic_task(void);

/**
* Tests creation of new round robin tasks. 
*/
void test_create_rr_task(void);

/**
* Tests an invalid schedule of periodic tasks. 
* 
* Expecting the OS to abort with error message:
* 	ERR_RUN_3_PERIODIC_TOOK_TOO_LONG. 
*/
void test_invalid_periodic(void);

/**
* Tests a valid schedule of periodic tasks. 
* 
* Should not fail. Measure output with logic analyzer
* to show the period and execution of each task. 
*/
void test_valid_periodic(void);

/**
* Tests a valid schedule using both periodic and 
* round robin tasks. No system tasks.
*
* Use scope to validate the timing.  
*/
void test_complex_scheduling_1(void);

/**
* Tests a valid schedule using periodic, round robing
* and system level tasks. 
* 
* Use scope to validate the timing. 
*/
void test_complex_scheduling_2(void);

/**
* Test publishing to a service from a system level task. 
*/
void test_publish_system(void);

/**
* Test publishing toa  service from a round robin task. 
*/
void test_publish_rr(void);

/**
* Test publishing to a service from a periodic task. 
*/
void test_publish_periodic(void);

/**
* Test publishing to a service from an interrupt. 
*/
void test_publish_interrupt(void);

/**
* Test subscribing to a service from a round robin task.
* Also test walking up once that service is published to. 	
*/
void test_subscribe_rr(void);

/**
* Tests subscribing to a service from a system task.
* Also tests resuming execution once that task is published to. 
*/
void test_subscribe_system(void);

/**
* Tests subscribing to a service from a periodic task. 
* Expected that this will fail and OS will abort. 
*/
void test_subscribe_periodic(void);
