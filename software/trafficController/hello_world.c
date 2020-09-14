#include <system.h>
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include "sys/alt_alarm.h"

alt_u32 tlc_timer_isr(void* context){

	int *timeOut = (int*)context;
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

// this function controllers the behavior of the LEDs
int simple_tlc(){
	
}

int main()
{
  FILE *lcd;
  unsigned int previousMode = 0;

  alt_alarm timer;
  int lightTime = 0;


  void* timerContext = (void*) &lightTime;

  // open lcd file in write mode
  lcd = fopen(LCD_NAME, "w");

  while(1){

	  // update the mode value
	  previousMode = lcd_set_mode(previousMode, lcd);

	  // switch statement to choose the traffic controller based on mode
	  switch(previousMode){
	  case 1 :
		  simple_tlc();
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
