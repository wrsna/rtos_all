#include<uart_handler.h>

#ifdef __cplusplus
extern "C" {
#endif

extern fifo_t keys_fifo;
//extern char *lcd_string;
extern rtos_task_t *task_list0[];
extern rtos_task_t *task_list1[];
extern rtos_task_t *task_list2[];
extern rtos_task_t *task_list3[];
extern rtos_task_t  task_led2;
extern rtos_task_t  task_ura;



void uart_start(void){
    sam_uart_opt_t uart_setup = {
        .ul_mck = SystemCoreClock,
        .ul_baudrate = 115200,
        .ul_mode =UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO
    };

    NVIC_EnableIRQ(UART_IRQn);

    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_UART);

    pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA8A_URXD | PIO_PA9A_UTXD);
    pio_pull_up(PIOA,PIO_PA8A_URXD | PIO_PA9A_UTXD,PIO_PULLUP);

    uart_init(UART, &uart_setup);

    uart_enable_interrupt(UART, UART_SR_RXRDY);  // glej interupt register na ki naj se enabla interupt nimam še ideje

    NVIC_SetPriority(UART_IRQn,1);

}




void UART_Handler(){
    uint8_t uart_data;
    int i;
    int button_ok;
    uint8_t check[KEYS_FIFO_SIZE];

    if(UART->UART_SR & (UART_SR_FRAME | UART_SR_OVRE | UART_SR_PARE)){

        i = sprintf(lcd_string, "Error: PARE %d   FRAME %d OVER %d", (int)(UART->UART_SR & UART_SR_PARE)>>7, (int)(UART->UART_SR & UART_SR_FRAME)>>6, (int)(UART->UART_SR & UART_SR_OVRE)>>5);
        lcd_string[i] = ' ';
        lcd_driver();
        uart_reset_status(UART);
        while(1){
            button_ok = get_button_press();
            if(button_ok & 1<<3){
                i = sprintf(lcd_string,"                               ");
                lcd_string[i] = ' ';
                break;
         }
        }
    }
    else{
        uart_read(UART, &uart_data);
        fifo_write(&keys_fifo,&uart_data,1);
    }

    i = fifo_look(&keys_fifo,check);

    switch(check[i-1]){
        case '1':
            change(task_list0,&task_ura);
            sprintf(lcd_string,"                                ");
            sysclk_disable_peripheral_clock(ID_TC0);
            break;
        case '2':
            change(task_list1,0);
            sprintf(lcd_string,"                                ");
            sysclk_enable_peripheral_clock(ID_TC0);
            break;
        case '3':
            change(task_list2,&task_led2);
            sprintf(lcd_string,"                                ");
            sysclk_disable_peripheral_clock(ID_TC0);
            break;
        case '4':
            change(task_list3,0);
            sprintf(lcd_string,"                                ");
            sysclk_disable_peripheral_clock(ID_TC0);
            break;
        default :
            break;

    }

}
























#ifdef __cplusplus
}
#endif
