#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <altera_avalon_pio_regs.h>
#include "sys/alt_alarm.h"
#include "ledLocation.h"

// initialise flags
unsigned int waitOddPress = 1;
unsigned int waitEvenPress = 0;
unsigned int carEntering = 0;
unsigned int carLeaving = 0;
unsigned int timeout = 0;
unsigned int snapTaken = 0;

// initialise variable to hold time value
int time = 0;

// intialise message strings
char* vehicleLeftString = "Vehicle left\n";
char* snapTakenString = "Snapshot taken\n";
char* timeTakenString = "Time in intersection: ";
char* activeCamString = "Camera activated\n";

//set up camera timer
alt_alarm camTimer;

// set up timer ISR to time for 2 seconds
alt_u32 camera_timer_isr(void* cameraContext){

	// set timeout flag to 1
	timeout = 1;

	return 2000;
}

// Button ISR code for mode 4
void handle_vehicle_button(int* temp){

	// store interrupt value of BUTTON_0
	int vehicleButton = *temp & 0x04;

	// if interrupt on odd press
	if(waitOddPress && vehicleButton){
		// flags to determine odd or even press
		waitOddPress = 0;
		waitEvenPress = 1;

		// flags to represent entering or leaving vehicle
		carEntering = 1;
		carLeaving = 0;

		// get current time value
		time = alt_nticks();
	}
	// if interrupt on even press
	else if(waitEvenPress && vehicleButton){
		// flags to determine odd or even press
		waitEvenPress = 0;
		waitOddPress = 1;

		// flags to represent entering or leaving vehicle
		carLeaving = 1;
		carEntering = 0;

		// get time in intersection by
		// subtracting current time from time at start
		time = alt_nticks() - time;
	}
}

// traffic light controller with camera
void camera_tlc(void* timerContext, unsigned int state, FILE *transmitter, void* cameraContext){
	int* timerValue = (int*) timerContext;

	switch (state)
	{
	case 0:
		// set timer value for next
		*timerValue = 7000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, GR);

		// check for car leaving
		if(carLeaving == 1){
			// open UART file in writing mode
			transmitter = fopen(UART_NAME, "w");

			// transmit leaving message, and time in intersection
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n\n", ((float)time/alt_ticks_per_second()));
			fclose(transmitter);

			// set flag to 0
			carLeaving = 0;
		}

		break;

	case 1:
		// set timer value for next
		*timerValue = 2000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, YR);

		// check for car entering on a yellow
		if(carEntering == 1){

			// open UART file in writing mode
			transmitter = fopen(UART_NAME, "w");

			// print camera activated message
			fprintf(transmitter, "%s", activeCamString);
			fclose(transmitter);

			// set flag to 0
			carEntering = 0;

			// start 2-second timer
			alt_alarm_start(&camTimer, 2000, camera_timer_isr, cameraContext);

		}


		// check if car in intersection >= 2 seconds
		if(timeout == 1){
			// stop timer
			alt_alarm_stop(&camTimer);

			// print snapshot taken message
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);

			timeout = 0;
			snapTaken = 1;

		}

		// check for car leaving
		if(carLeaving == 1){

			if(snapTaken == 0){
				alt_alarm_stop(&camTimer);
				timeout = 0;
			}

			// print vehicle left message, and time in intersection
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n\n", ((float)time/alt_ticks_per_second()));
			fclose(transmitter);

			// set flag to 0
			carLeaving = 0;
		}
		break;

	case 2:
		// set timer value for next
		*timerValue = 6000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RR);

		// check for car entering on red
		if(carEntering == 1){
			// print snapshot taken message
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);

			//set flag to 0
			carEntering = 0;
		}

		// check if car is leaving
		if(carLeaving == 1){
			// print vehicle left message and time in intersection
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n\n", ((float)time/alt_ticks_per_second()));
			fclose(transmitter);

			// set flag to 0
			carLeaving = 0;
		}

		// reset snapTaken flag
		snapTaken = 0;
		break;

	case 3:
		// set timer value for next
		*timerValue = 7000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RG);

		if(carLeaving == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n\n", ((float)time/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}
		break;

	case 4:
		// set timer value for next
		*timerValue = 2000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RY);

		// check if car is entering intersection on yellow
		if(carEntering == 1){
			// print camera activated message
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", activeCamString);
			fclose(transmitter);

			// set flag to 0
			carEntering = 0;

			//start 2-second timer
			alt_alarm_start(&camTimer, 2000, camera_timer_isr, cameraContext);

		}


		// check if car in intersection >= 2 seconds
		if(timeout == 1){
			// stop timer
			alt_alarm_stop(&camTimer);

			// print snapshot taken message
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);

			// set flag to 0
			timeout = 0;
			snapTaken = 1;
		}

		// check if car is leaving
		if(carLeaving == 1){

			if(snapTaken == 0){
				alt_alarm_stop(&camTimer);
				timeout = 0;
			}

			// print vehicle left message, and time in intersection
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n\n", ((float)time/alt_ticks_per_second()));
			fclose(transmitter);

			// set flag to 0
			carLeaving = 0;
		}
		break;

	case 5:
		// set timer value for next
		*timerValue = 6000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RR);

		// check if car entering on red light
		if(carEntering == 1){
			// print snapshot taken message
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);

			// set flag to 0
			carEntering = 0;
		}

		// check if car leaving
		if(carLeaving == 1){
			// print vehicle leaving message, and time in intersection
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n\n", ((float)time/alt_ticks_per_second()));
			fclose(transmitter);

			// set flag to 0
			carLeaving = 0;
		}

		// reset snapTaken flag
		snapTaken = 0;
		break;
	default:
		break;
	}
}
