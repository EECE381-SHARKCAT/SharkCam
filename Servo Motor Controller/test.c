//===================================================================================================
//                                   INCLUDE HEADER FILES
//===================================================================================================
#include <stdio.h>
#include <unistd.h>
#include "altera_up_avalon_parallel_port.h"

//===================================================================================================
//                                   DEFINE CONSTANTS
//===================================================================================================
#define HIGH 0xFFFFFFFF
#define LOW 0x0
#define PULSE_DELAY 20000
#define PULSE 1000

//===================================================================================================
//                                   GLOBAL VARIABLES
//===================================================================================================

//GPIO
alt_up_parallel_port_dev* gpio;
unsigned int servo_position;
unsigned int data_in;

//===================================================================================================
//                                   FUNCTION PROTOTYPES
//===================================================================================================
void control_servo(void);
void init_hardware(void);

//===================================================================================================
//                                  MAIN FUNCTION
//===================================================================================================

int main()
{
  printf("Program Started...\n");

  //Initialize Hardware
  init_hardware();

  //Control Servo Motors
  servo_position = 0;
  int i;

  while(1){

	  if(servo_position > 2) servo_position = 0;

	  for(i=0; i<4; i++){
		  control_servo();
	  }

	  servo_position++;

	  usleep(3000000);
  }

  return 0;
}

//===================================================================================================
//                                   FUNCTION IMPLEMENTATION
//===================================================================================================

void control_servo(void){

	//OUTPUT = HIGH
	alt_up_parallel_port_write_data(gpio, HIGH);

	//PULSE TIME
	usleep(PULSE + (PULSE/2) * servo_position);

	//OUTPUT = LOW
	alt_up_parallel_port_write_data(gpio, LOW);

	//DELAY BETWEEN PULSE
	usleep(PULSE_DELAY);

}



void init_hardware(void){

	//Initialize GPIO
	gpio = alt_up_parallel_port_open_dev(GPIO_NAME);
	if(gpio == NULL){
		printf("Error opening GPIO\n");
	}
	//Set all GPIO pins to be output
	alt_up_parallel_port_set_all_bits_to_output(gpio);
}
