#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <altera_avalon_pio_regs.h>
#include "ledLocation.h"

unsigned int detectedNLine = 0;
unsigned int checkNew = 0;
char sequence[64];
int timeValues[6] = {500,6000,2000,500,6000,2000};

//returns true or (1) when the UART sequence is valid
unsigned int check_uart_sequence() {
    int numCounter = 1;
    int digitCounter = 0;
    unsigned int commaDetector = 0;


    for (int i = 0; sequence[i] != '\n'; i++) {

        if (commaDetector) {
            if (!isdigit((char)sequence[i])) {
                return 0;
            }
            else {
                commaDetector = 0;
                numCounter++;
            }
        }

        if (!isdigit((char)sequence[i]) && (sequence[i] != '\r')) {
            if (i == 0) {
                return 0;
            }
            else if (sequence[i] == ',') {
                if (digitCounter > 4) {
                    return 0;
                }

                digitCounter = 0;
                commaDetector = 1;
            }
            else {
                return 0;
            }
        }

        if (isdigit((char)sequence[i])) {
            digitCounter++;
        }
    }


    if (numCounter != 6 || digitCounter > 4) {
        return 0;
    }

    return 1;
}

void uart_receiver(FILE *receiver){

	char c = 0;
	int count = 0;
	receiver = fopen(UART_NAME, "r");

	if(receiver != NULL){
		for(;;){

			c = getc(receiver);
			sequence[count] = c;

			if(c == '\n'){
				detectedNLine = 1;
				break;
			}

			count++;

		}

	}
	fclose(receiver);
}


void timeout_data_handler(FILE *receiver) {

    uart_receiver(receiver);

    while (!check_uart_sequence()) {
        printf("Invalid Input! Try Again... \n");
        uart_receiver(receiver);
    }

    char* p = sequence;
    timeValues[0] = atoi(p);

    for (int i = 1; i <= 5; i++) {
        p = strchr(p, ',') + 1;
        timeValues[i] = atoi(p);
    }

}


void configurable_tlc(void* timerContext, unsigned int state, FILE *receiver){
	int* timerValue = (int*) timerContext;


		switch (state)
		{
		case 0:
			// set timer value for next
			*timerValue = timeValues[2];
			// set current LEDs
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, GR);
			break;

		case 1:
			// set timer value for next
			*timerValue = timeValues[3];
			// set current LEDs
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, YR);
			break;

		case 2:

			// set current LEDs
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RR);

			checkNew = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE) & 0x8;

			if(checkNew != 0){
				timeout_data_handler(receiver);
			}

			// set timer value for next
			*timerValue = timeValues[4];

			break;

		case 3:
			// set timer value for next
			*timerValue = timeValues[5];
			// set current LEDs
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RG);


			break;

		case 4:
			// set timer value for next
			*timerValue = timeValues[0];
			// set current LEDs
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RY);
			break;

		case 5:

			// set current LEDs
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, RR);

			checkNew = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE) & 0x8;

			if(checkNew != 0){
				timeout_data_handler(receiver);
			}

			// set timer value for next
			*timerValue = timeValues[1];

			break;
		default:
			break;
		}
}