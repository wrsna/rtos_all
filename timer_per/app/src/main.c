#include <asf.h>
#include"lcd.h"
#include"bord.h"


#ifdef __cplusplus
extern "C" {
#endif


//global variables
uint32_t on_time = 0, n = 0;

int main (void){

    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    sysclk_enable_peripheral_clock(ID_PIOB);  ///upa da je prou
    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_TC0);
   /* PIOB -> PIO_PER = 0<<27;
    PIOB->PIO_PDR = PIO_PDR_P27;
    PIOB->PIO_OER = PIO_OER_P27;
    PIOB->PIO_ABSR = PIO_ABSR_P27;
    PIOB->PIO_IER = PIO_IER_P27;*/
    pio_set_peripheral(PIOB,PIO_PERIPH_B,PIO_PB27B_TIOB0);
    pio_set_peripheral(PIOB,PIO_PERIPH_B,PIO_PB25B_TIOA0);   //zaki mi b0 ni delau
    NVIC_EnableIRQ(TC0_IRQn);

    lcd_init();
    bord_init();
    timer_init();


    uint32_t frekvenca = 1;
    uint32_t duty = 50;
    int pritisnjeni = 0;



    /********************* Main loop     ***************************/

    while(1){

        // branje pritiska tipk
        pritisnjeni = get_button_press();

        //spreminjanje parametrov
        if ((pritisnjeni & 1<<0) && (frekvenca<20)) frekvenca++;
        if ((pritisnjeni & 1<<1) && (frekvenca>1 )) frekvenca--;
        if ((pritisnjeni & 1<<2) && (duty<95)) duty += 5;
        if ((pritisnjeni & 1<<3) && (duty>5 )) duty -= 5;

        //izračun
        n = SystemCoreClock/(frekvenca * 32);
        on_time = (n * duty)/100;

        //pisanje vrednosti na lcd
        int i = sprintf(lcd_string,"f %2dHz cycle %2d%%",(int)frekvenca, (int)duty);
        lcd_string[i] = ' ';
        lcd_driver();


    }
}

void TC0_Handler(void){

    //ioport_toggle_pin_level(PIO_PD7_IDX);   ne bom več rabu


    tc_write_ra(TC0, 0, on_time);
    tc_write_rc(TC0, 0, n );
    tc_write_rb(TC0, 0, on_time);

    tc_get_status(TC0, 0);




  ///tu bo rabu samo še 3 vrstive piši v registre in beri status za reset
}


#ifdef __cplusplus
}
#endif
