
#ifndef MODE4_H_
#define MODE4_H_

extern unsigned int waitOddPress;
extern unsigned int waitEvenPress;

void camera_tlc(void* timerContext, unsigned int state, FILE *transmitter, void* cameraContext);
void handle_vehicle_button(unsigned int time);
alt_u32 camera_timer_isr(void* cameraContext);

#endif /* MODE4_H_ */
