


#ifndef BORD_H_INCLUDE
#define BORD_H_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

#include<asf.h>


void bord_init(void);
int get_button_press(void);
int get_button_state(void);

void adc_setup(void);
uint32_t adc_read(void);

void timer_init(void);

void pio_per_init(void);


/*******************************/
uint32_t tc_get_mode(
		Tc *p_tc,
		uint32_t ul_channel);
/*********************************/

#endif  //BORD_H_INCLUDE

#ifdef __cplusplus
}
#endif
