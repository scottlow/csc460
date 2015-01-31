#include "scheduler.h"
uint32_t idle_period;
void task1(){
  //erial.println("task1");
  digitalWrite(7, HIGH);
  delay(10);
  digitalWrite(7, LOW);
}

void task2(){
  //Serial.println("task2");
  digitalWrite(6, HIGH);
  delay(30);
  digitalWrite(6, LOW);
}

void idle(uint32_t period){
    digitalWrite(9, HIGH);
    Serial.println("idle");
    delay(period); 
    digitalWrite(9, LOW);
}

void setup() {
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT); 
  pinMode(13, OUTPUT); 
  pinMode(9, OUTPUT);
  pinMode(5, OUTPUT); 
  
  Serial.begin(9600);
  
  scheduler_init();
  
  scheduler_start_task(0, 500, task1);
  scheduler_start_task(200, 500, task2);
}

void loop() {
  
  idle_period = scheduler_dispatch(); 
  
  if(idle_period){
    Serial.println(idle_period);
     idle(idle_period); 
  }

}
