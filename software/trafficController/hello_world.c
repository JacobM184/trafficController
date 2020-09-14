#include <system.h>
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include "sys/alt_alarm.h"

unsigned int state = 0;

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


	int output = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);

	if(lcd != NULL){
		if(output != previousMode){
			#define ESC 27
			#define CLEAR_LCD_STRING "[2J"
			fprintf(lcd, "%c%s", ESC, CLEAR_LCD_STRING);
			fprintf(lcd, "CURRENT MODE: %d\n", output);
		}
	}
	return output;
}

//The simple controller
void simple_tlc(void* timerContext){

	int* timerValue = (int*) timerContext;

	switch (state)
	{
	case 0:
		*timerValue = 500;
		break;
	case 1:
		*timerValue = 6000;
		break;
	case 2:
		*timerValue = 2000;
		break;
	case 3:
		*timerValue = 500;
		break;
	case 4:
		*timerValue = 6000;
		break;
	case 5:
		*timerValue = 2000;
		break;
	default:
		break;
	}
}

int main()
{
  FILE *lcd;
  lcd = fopen(LCD_NAME, "w");
  unsigned int previousMode = 0;

  int lightCol = 6000;
  int currentState = 1;

  alt_alarm timer;
  void* timerContext = (void*) &lightCol;
  alt_timer_start(&timer, lightCol, tlc_timer_isr, timerContext);

  while(1){
	  switch(previousMode){
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

	  previousMode = lcd_set_mode(previousMode, lcd);

  }

  fclose(lcd);

  return 0;
}
