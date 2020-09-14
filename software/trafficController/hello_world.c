#include <system.h>
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include "sys/alt_alarm.h"

#define NS_GREEN 0x21 	// LED configuration for G,R state
#define NS_YELLOW 0x22	// LED configuration for Y,R state
#define NS_RED 0x24		// LED configuration for R,R state

#define EW_GREEN 0xC	// LED configuration for R,G state
#define EW_YELLOW 0x14	// LED configuration for R,Y state
#define EW_RED 0x24		// LED configuration for R,R state

// global state variable
unsigned int state = 5;

alt_u32 tlc_timer_isr(void* context){

	int *timeOut = (int*)context;

	//Go to next state
	if (state < 5){
		state++;
	}
	else{
		state = 0;
	}

	return *timeOut;

}


// this function sets the mode value based on inputs from the SWITCHES
int lcd_set_mode(unsigned int previousMode, FILE *lcd){

	// read switch value
	int output = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);

	// update lcd display if necessary
	if(lcd != NULL){
		if(output != previousMode){
			#define ESC 27
			#define CLEAR_LCD_STRING "[2J"
			fprintf(lcd, "%c%s", ESC, CLEAR_LCD_STRING);
			fprintf(lcd, "CURRENT MODE: %d\n", output);
		}
	}

	// return mode
	return output;
}

//the simple controller
void simple_tlc(void* timerContext){

	int* timerValue = (int*) timerContext;

	switch (state)
	{
	case 0:
		// set timer value for next
		*timerValue = 2000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, NS_GREEN);
		break;

	case 1:
		// set timer value for next
		*timerValue = 500;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, NS_YELLOW);
		break;

	case 2:
		// set timer value for next
		*timerValue = 6000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, NS_RED);
		break;

	case 3:
		// set timer value for next
		*timerValue = 2000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, EW_GREEN);
		break;

	case 4:
		// set timer value for next
		*timerValue = 500;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, EW_YELLOW);
		break;

	case 5:
		// set timer value for next
		*timerValue = 6000;
		// set current LEDs
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, EW_RED);
		break;
	default:
		break;
	}
}


int main()
{
	// set up LCD file and open in writing mode
	FILE *lcd;
	lcd = fopen(LCD_NAME, "w");

	// initialize variables and flag
	unsigned int currentMode = 0;
	unsigned int prevMode = 0;
	unsigned int initFlag = 1;

	int lightCol = 500;

	// set up timer
	alt_alarm timer;
	void* timerContext = (void*) &lightCol;

  while(1){

	  // update the mode value
	  currentMode = lcd_set_mode(currentMode, lcd);

	  // check for change in mode
	  if (currentMode != prevMode){

		  // start timer on first startup
		  if (initFlag == 1){
			  alt_alarm_start(&timer, lightCol, tlc_timer_isr, timerContext);
			  initFlag = 0;
		  }

		  // restart timer when mode changed and not restarted
		  else{
			  alt_alarm_stop(&timer);
			  usleep(1000);

			  lightCol = 500;
			  state = 5;

			  alt_alarm_start(&timer, lightCol, tlc_timer_isr, timerContext);
		  }

		  // update previous mode value
		  prevMode = currentMode;
	  }

	  // switch statement to choose the traffic controller based on mode
	  switch(prevMode){

	  case 1 :
		  // run controller for Mode 1
		  simple_tlc(timerContext);
		  break;

	  case 2 :
		  break;

	  case 3 :
		  break;

	  case 4 :
		  break;

	  default :
		  break;
	  }


  }

  // close the LCD file (should not occur)
  fclose(lcd);

  return 0;
}
