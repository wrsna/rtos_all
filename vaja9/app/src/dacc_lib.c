#include<dacc_lib.h>

#ifdef __cplusplus
extern "C" {
#endif

signal_t signal = {
    .f = 1,
    .shape = sinus
};



















void dacc_setup(void){

    sysclk_enable_peripheral_clock(ID_DACC);
    dacc_reset(DACC);
    dacc_set_transfer_mode(DACC,DACC_MR_WORD_HALF);
    dacc_set_timing(DACC,0x08,0,0x10);
    dacc_set_channel_selection(DACC,1);
    dacc_enable_channel(DACC,1);

}









void TC0_Handler(void){
    uint32_t status = 0;
    static uint8_t i = 0, direction = 1; //??
    uint32_t t;

     t =(SystemCoreClock/32)/( 2*(NS-1)*signal.f );
     tc_write_rc(TC0,0,t);

    status = dacc_get_interrupt_status(DACC);

    if(status != DACC_IMR_TXRDY){
            dacc_write_conversion_data(DACC,signal.shape[i]);
            if(direction)i++;
            else i--;
    }

    if(i==10){
        direction = 0;
        i = i-2;
    }
    else if(i == 0){
        direction = 1;
        i = i+2;
    }
    tc_get_status(TC0, 0);
}













#ifdef __cplusplus
}
#endif
