//===================================================================================================
//                                   INCLUDE HEADER FILES
//===================================================================================================
#include <stdio.h>
#include <unistd.h>
#include "altera_up_avalon_parallel_port.h"

//===================================================================================================
//                                   DEFINE CONSTANTS
//===================================================================================================

//GPIO
#define HIGH 0xFFFFFFFF
#define LOW 0x0

#define HIGH_PITCH 0x0
#define HIGH_ROLL 0x0
#define LOW_PITCH 0x0
#define LOW_ROLL 0x0

//Android
#define CONTROLLER 0
#define CAMERA 1

//Servo
#define PITCH 0
#define ROLL 1
#define PULSE_DELAY 20000
#define PULSE 1000

//===================================================================================================
//                                   DEFINE STRUCTS
//===================================================================================================

//Android Device
typedef struct {
	//identifies the device type
	int type;
	//variables for device orientation
	float pitch, roll;
}Android;

//Servo Motor
typedef struct{
	//Indentify the type of servo
	int type;
	// A floating number between 0 and 1
	// which translates to angles ranging from -90 to +90
	float position;
}Servo;

//===================================================================================================
//                                   GLOBAL VARIABLES
//===================================================================================================

//GPIO
alt_up_parallel_port_dev* gpio;
unsigned int data_in;

//Android Device
Android controller = {CONTROLLER};
Android camera = {CAMERA};

//Servo Motor
Servo servo_pitch = {PITCH};
Servo servo_roll = {ROLL};


//===================================================================================================
//                                   FUNCTION PROTOTYPES
//===================================================================================================

void init_hardware(void);

void adjust_servo(Servo* servo);

void adjust_servo_concurrent(Servo* pitch, Servo* roll);

void get_orientation(Android* android_device);

void orientation_diff(Android* controller, Android* camera);

//===================================================================================================
//                                  MAIN FUNCTION
//===================================================================================================

int main()
{
	printf("Program Started...\n");

	//Initialize Hardware
	init_hardware();


	//Servo Motor Controller
	while(1){

		//Get Orientation of Controller
		get_orientation(&controller);

		//Get Orientation of Camera
		get_orientation(&camera);

		//Calculate the difference in orientation
		orientation_diff(&controller, &camera);

//		//Adjust pitch
//		adjust_servo(&servo_pitch);
//		//Adjust roll
//		adjust_servo(&servo_roll);

		//Adjust servo
		adjust_servo_concurrent(&servo_pitch, &servo_roll);

		//Wait for next update
		usleep(3000000);
	}

	return 0;
}

//===================================================================================================
//                                   FUNCTION IMPLEMENTATION
//===================================================================================================

void init_hardware(void){

	//Initialize GPIO
	gpio = alt_up_parallel_port_open_dev(GPIO_NAME);
	if(gpio == NULL){
		printf("Error opening GPIO\n");
	}
	//Set all GPIO pins to be output
	alt_up_parallel_port_set_all_bits_to_output(gpio);
}


void adjust_servo(Servo* servo){

	if(servo->type == PITCH){

		//TODO
		//SET REQUIRED OUTPUT TO HIGH
		alt_up_parallel_port_write_data(gpio, HIGH);

		//PULSE TIME
		usleep(PULSE + (int)(PULSE * servo->position));

		//SET ALL OUTPUT TO LOW
		alt_up_parallel_port_write_data(gpio, LOW);

		//DELAY BETWEEN PULSE
		usleep(PULSE_DELAY);
	}

	else if (servo->type == ROLL){

		//TODO
		//SET REQUIRED OUTPUT TO HIGH
		alt_up_parallel_port_write_data(gpio, HIGH);

		//PULSE TIME
		usleep(PULSE + (int)(PULSE * servo->position));

		//SET ALL OUTPUT TO LOW
		alt_up_parallel_port_write_data(gpio, LOW);

		//DELAY BETWEEN PULSE
		usleep(PULSE_DELAY);
	}


}

void adjust_servo_concurrent(Servo* pitch, Servo* roll){

	int i;

	if(pitch->position > roll->position){

		for (i=0; i<2; i++) {

			//SET ALL OUTPUT TO HIGH
			alt_up_parallel_port_write_data(gpio, HIGH);

			//PULSE TIME FOR ROLL
			usleep(PULSE + (int)(PULSE * roll->position));
			//SET ROLL SERVO TO LOW
			alt_up_parallel_port_write_data(gpio, LOW_ROLL);

			//REMAINING PULSE TIME FOR PITCH
			usleep(PULSE + (int)(PULSE * (pitch->position - roll->position)));
			//SET PITCH SERVO TO LOW
			alt_up_parallel_port_write_data(gpio, LOW_PITCH);

			//DELAY BETWEEN PULSE
			if(i==0){
				usleep(PULSE_DELAY);
			}
		}

	}

	else if(pitch->position < roll->position){

		for (i=0; i<2; i++) {

			//SET ALL OUTPUT TO HIGH
			alt_up_parallel_port_write_data(gpio, HIGH);

			//PULSE TIME FOR PITCH
			usleep(PULSE + (int)(PULSE * pitch->position));
			//SET ROLL SERVO TO LOW
			alt_up_parallel_port_write_data(gpio, LOW_PITCH);

			//REMAINING PULSE TIME FOR ROLL
			usleep(PULSE + (int)(PULSE * (roll->position - pitch->position)));
			//SET ROLL SERVO TO LOW
			alt_up_parallel_port_write_data(gpio, LOW_ROLL);

			//DELAY BETWEEN PULSE
			if(i==0){
				usleep(PULSE_DELAY);
			}
		}
	}

	//pitch = roll
	else {

		for (i=0; i<2; i++) {

			//SET ALL OUTPUT TO HIGH
			alt_up_parallel_port_write_data(gpio, HIGH);

			//PULSE TIME FOR PITCH = ROLL
			usleep(PULSE + (int)(PULSE * pitch->position));

			//SET BOTH PITCH AND ROLL SERVO TO LOW
			alt_up_parallel_port_write_data(gpio, LOW);

			//DELAY BETWEEN PULSE
			if(i==0){
				usleep(PULSE_DELAY);
			}
		}
	}

}

void orientation_diff(Android* controller, Android* camera){

	//new pitch position
	servo_pitch.position += (controller->pitch - camera->pitch);

	//new roll position
	servo_roll.position += (controller->roll - camera->roll);
}

void get_orientation(Android* android_device){

	if (android_device->type == CONTROLLER){
		//TODO
	}

	else if(android_device->type == CAMERA){
		//TODO
	}
}
