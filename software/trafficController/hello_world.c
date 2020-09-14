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



int main()
{
  FILE *lcd;
  unsigned int previousMode = 0;

  alt_alarm timer;
  int lightCol = 0;


  void* timerContext = (void*) &lightCol;

  lcd = fopen(LCD_NAME, "w");

  while(1){

	  previousMode = lcd_set_mode(previousMode, lcd);


  }

  fclose(lcd);

  return 0;
}
