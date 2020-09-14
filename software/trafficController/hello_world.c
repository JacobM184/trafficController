#include <system.h>
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include "sys/alt_alarm.h"

#define NS_GREEN 0x21
#define NS_YELLOW 0x22
#define NS_RED 0x24

#define EW_GREEN 0xC
#define EW_YELLOW 0x14
#define EW_RED 0x24

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

	if (output != previousMode){
		newModeFlag = 1;
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
		*timerValue = 2000;
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, NS_GREEN);
		break;
	case 1:
		*timerValue = 500;
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, NS_YELLOW);
		break;
	case 2:
		*timerValue = 6000;
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, NS_RED);
		break;
	case 3:
		*timerValue = 2000;
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, EW_GREEN);
		break;
	case 4:
		*timerValue = 500;
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, EW_YELLOW);
		break;
	case 5:
		*timerValue = 6000;
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, EW_RED);
		break;
	default:
		break;
	}
}


int main()
{
	FILE *lcd;
	lcd = fopen(LCD_NAME, "w");
	unsigned int currentMode = 0;
	unsigned int prevMode = 0;

	int lightCol = 500;

	alt_alarm timer;
	void* timerContext = (void*) &lightCol;
  while(1){

	  // update the mode value
	  currentMode = lcd_set_mode(Mode, lcd);

	  if (currentMode != prevMode){
		  if (prevMode == 0){
			  alt_alarm_start(&timer, lightCol, tlc_timer_isr, timerContext);
		  }
		  else{
			  alt_alarm_stop(&timer);
			  usleep(1000);

			  lightCol = 500;
			  state = 5;

			  alt_alarm_start(&timer, lightCol, tlc_timer_isr, timerContext);

		  }
	  }

	  // switch statement to choose the traffic controller based on mode
	  switch(prevMode){
	  case 1 :
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

  // close the lcd file (should not occur)
  fclose(lcd);

  return 0;
}
