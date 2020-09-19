#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <altera_avalon_pio_regs.h>
#include "sys/alt_alarm.h"
#include "ledLocation.h"

unsigned int waitOddPress = 1;
unsigned int waitEvenPress = 0;
unsigned int carEntering = 0;
unsigned int carLeaving = 0;
unsigned int timeout = 0;
unsigned int counter = 0;
int y = 0;
int norm = 0;

char* vehicleLeftString = "Vehicle left\n";
char* snapTakenString = "Snapshot taken\n";
char* timeTakenString = "Time in intersection: ";
char* activeCamString = "Camera activated\n";

//set up camera timer
alt_alarm camTimer;


alt_u32 camera_timer_isr(void* cameraContext){
	timeout = 1;
	return 2000;
}

void handle_vehicle_button(int* temp){

	// store interrupt value of BUTTON_0
	int vehicleButton = *temp & 0x04;

	if(waitOddPress && vehicleButton){
		waitOddPress = 0;
		waitEvenPress = 1;
		carEntering = 1;
		carLeaving = 0;
		y = alt_nticks();
	}
	else if(waitEvenPress && vehicleButton){
		waitEvenPress = 0;
		waitOddPress = 1;
		carLeaving = 1;
		carEntering = 0;
		y = alt_nticks() - y;
	}
}

void camera_tlc(void* timerContext, unsigned int state, FILE *transmitter, void* cameraContext){
	int* timerValue = (int*) timerContext;

	switch (state)
	{
	case 0:
		// set timer value for next
		*timerValue = 7000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, GR);

		if(carLeaving == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n", ((float)y/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}

		break;

	case 1:
		// set timer value for next
		*timerValue = 2000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, YR);

		if(carEntering == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", activeCamString);
			fclose(transmitter);
			carEntering = 0;
			alt_alarm_start(&camTimer, 2000, camera_timer_isr, cameraContext);

		}



		if(timeout == 1){
			alt_alarm_stop(&camTimer);
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);
			timeout = 0;
		}

		if(carLeaving == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n", ((float)y/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}
		break;

	case 2:
		// set timer value for next
		*timerValue = 6000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RR);

		if(carEntering == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);
			carEntering = 0;
		}

		if(carLeaving == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n", ((float)y/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}
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
			fprintf(transmitter, "%f \n", ((float)y/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}
		break;

	case 4:
		// set timer value for next
		*timerValue = 2000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RY);

		if(carEntering == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", activeCamString);
			fclose(transmitter);
			carEntering = 0;
			alt_alarm_start(&camTimer, 2000, camera_timer_isr, cameraContext);

		}



		if(timeout == 1){
			alt_alarm_stop(&camTimer);
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);
			timeout = 0;
		}

		if(carLeaving == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n", ((float)y/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}

		if(carLeaving == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n", ((float)y/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}
		break;

	case 5:
		// set timer value for next
		*timerValue = 6000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RR);

		if(carEntering == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", snapTakenString);
			fclose(transmitter);
			carEntering = 0;
		}

		if(carLeaving == 1){
			transmitter = fopen(UART_NAME, "w");
			fprintf(transmitter, "%s", vehicleLeftString);
			fprintf(transmitter, "%s", timeTakenString);
			fprintf(transmitter, "%f \n", ((float)y/alt_ticks_per_second()));
			fclose(transmitter);
			carLeaving = 0;
		}
		break;
	default:
		break;
	}
}