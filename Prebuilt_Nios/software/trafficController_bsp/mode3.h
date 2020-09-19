

#ifndef MODE3_H_
#define MODE3_H_

void configurable_tlc(void* timerContext, unsigned int state, FILE *receiver);
void timeout_data_handler(FILE *receiver);
void uart_receiver(FILE *receiver);
unsigned int check_uart_sequence();

#endif /* MODE3_H_ */

