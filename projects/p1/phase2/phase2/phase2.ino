#include "radio.h"
#include "scheduler.h"

#define SET_HIGH_BYTE(x) x>>8
#define SET_LOW_BYTE(x) x & 0x00FF 

volatile uint8_t rxflag = 0;
uint32_t idle_period = 0;

// Packets for commands and IR
radiopacket_t packet;
radiopacket_t ir_packet;

// Our radio's address
uint8_t my_addr[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Analog pins for joystick
int analogServoPinX = A0;
int analogServoPinY = A1;
int buttonPin = A2;

// State variables for button
int buttonState = 0; //state variable, high when the button has been pressed.
int buttonValue = 1000;
int buttonPressed = 0;

int xvalue = 0;  //variable to store the x value of the joystick
int yvalue = 0;  //variable to store the y value of the joystick

int turnIntervalX = 0; // -1 (for ccw rotation), 0 (for neutral) and 1 for (cw rotation)
int turnIntervalY = 0; // -1 (for backwards), 0 (for neutral) and 1 for (forwards motion)

int roombaNumber = 0;
int txflag = 0; // if this is 0, we do not send packets

// Send a movement/rotation command to the Roomba
void sendCommand(int16_t velocity, int16_t rotation) {

  if(txflag == 0) return;
  
  // put some data into the packet
  packet.type = COMMAND;
  memcpy(packet.payload.command.sender_address, my_addr, RADIO_ADDRESS_LENGTH);
  
  pf_command_t* command = &(packet.payload.command);
  command->command = 137;
  command->num_arg_bytes = 4;
  
  command->arguments[0] = SET_HIGH_BYTE(velocity);
  command->arguments[1] = SET_LOW_BYTE(velocity);
  
  command->arguments[2] = SET_HIGH_BYTE(rotation);
  command->arguments[3] = SET_LOW_BYTE(rotation);  
  
  // send the data
  Radio_Transmit(&packet, RADIO_WAIT_FOR_TX);  
  txflag = 0; 
}

// Send an IR command to the Roomba
void sendIRCommand() {

  if(txflag == 0) return;
  
  // put some data into the packet
  ir_packet.type = IR_COMMAND;
  memcpy(ir_packet.payload.command.sender_address, my_addr, RADIO_ADDRESS_LENGTH);
  
  pf_ir_command_t* command = &(ir_packet.payload.ir_command);
  command->ir_command = SEND_BYTE;
  command->ir_data = 'A';
  
  // send the data
  Radio_Transmit(&ir_packet, RADIO_WAIT_FOR_TX);  
  txflag = 0; 
}

// Task for receiving packets from the Roomba
void receivePacket() {
  // wait for the ACK reply to be transmitted back.
  if (rxflag)
  {
    // remember always to read the packet out of the radio, even
    // if you don't use the data.
    if (Radio_Receive(&packet) != RADIO_RX_MORE_PACKETS)
    {
      // if there are no more packets on the radio, clear the receive flag;
      // otherwise, we want to handle the next packet on the next loop iteration.
      rxflag = 0;
    }
  }   
}

// State machine used to send data to the Roomba based on global variables
void writeRoomba() {

  // Send IR packet if button is pressed
  if(buttonState == 1) {
    buttonState = 0;
    sendIRCommand();
  }
  
  // Rotate the Roomba if the joystick is moved in the x direction
  switch(turnIntervalX) {
    case 1:
      sendCommand(0x01F4, 0xFFFF);
      break;
    case -1:
      sendCommand(0x01F4, 0);
  }  
  
  // Move the Roomba if the joystick is moved in the y direction
  switch(turnIntervalY) {
    case -1:
      sendCommand(0x01F4, 0x8000);
      break;
    case 1:
      sendCommand(0xFE0C, 0x8000);
      break;
    default:
      sendCommand(0,0);
      break;
  }
}

// Task for querying analog input from joystick
void getInput() {
  if(txflag == 1) return;
  
  txflag = 1;
  buttonValue = analogRead(buttonPin);

  //check state of button to make sure we don't send to many signals.
  if(buttonPressed == 0 && buttonState == 0){
    if(buttonValue < 10){
      buttonPressed = 1; 
      buttonState = 0; 
    }   
  }
  else{
    if(buttonValue > 10){
      buttonPressed = 0;
      buttonState = 1; 
    }
  }

  //read the value of the joystick
  xvalue = analogRead(analogServoPinX);
  yvalue = analogRead(analogServoPinY);  

  //determine the interval to change the servo by
  if(xvalue < 500){
    turnIntervalX = -1;  
  }
  else if(xvalue > 550){
    turnIntervalX = 1;
  }
  else{
    turnIntervalX = 0; 
  }

  //determine the interval to change the servo by
  if(yvalue < 500){
    turnIntervalY = -1;  
  }
  else if(yvalue > 550){
    turnIntervalY = 1;
  }
  else{
    turnIntervalY = 0; 
  }
}

void setup()
{

  Serial.begin(9600);

  pinMode(13, OUTPUT);
  pinMode(47, OUTPUT);

  // Reset radio
  digitalWrite(47, LOW);
  delay(100);
  digitalWrite(47, HIGH);
  delay(100);

  Radio_Init();

  // configure the receive settings for radio pipe 0
  Radio_Configure_Rx(RADIO_PIPE_0, my_addr, ENABLE);
  // configure radio transceiver settings.
  Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);

  // The address to which the next transmission is to be sent
  Radio_Set_Tx_Addr(ROOMBA_ADDRESSES[roombaNumber]);

  // Enable scheduler
  scheduler_init();

  scheduler_start_task(0, 20, getInput);
  scheduler_start_task(0, 20, writeRoomba);
  scheduler_start_task(0, 20, receivePacket);  
}

void loop() {

  idle_period = scheduler_dispatch(); 

  if(idle_period){
    idle(idle_period); 
  }
}

void radio_rxhandler(uint8_t pipe_number)
{
  rxflag = 1;
}

void idle(uint32_t period) {
  delay(period); 
}
