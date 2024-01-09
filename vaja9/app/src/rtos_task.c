#include<rtos_task.h>

#ifdef __cplusplus
extern "C" {
#endif

extern fifo_t keys_fifo;
extern fifo_t sent_fifo;
/***************OPRAVILA_0**************************/

rtos_task_t task_lcd = {
    .last_tick = 0,
    .name = "LCD",
    .active = 1,
    .thred = 0,
    .driver = lcd_driver
};


rtos_task_t task_ura = {
    .last_tick = 0,
    .name = "URA",
    .active = 1,
    .thred = 0,
    .driver = ura_driver
};


rtos_task_t task_text = {
    .last_tick = 0,
    .name = "CALC",
    .active = 1,
    .thred = 0,
    .driver = text_driver
};


rtos_task_t task_tipke = {
    .last_tick = 0,
    .name = "TIPKE",
    .active = 1,
    .thred = 0,
    .driver = buttons_driver
};


rtos_task_t task_led = {
    .last_tick = 0,
    .name = "LED",
    .active = 0,
    .thred = 0,
    .driver = led_driver
};


rtos_task_t task_sent = {
    .last_tick = 0,
    .name = "SENT",
    .active = 0,
    .thred = 0,
    .driver = sent_driver,
};

/*********************/
extern signal_t signal;
/*********************optavila_1************************************/




rtos_task_t task_text1 = {
    .last_tick = 0,
    .name = "LOOK",
    .active = 1,
    .thred = 1,
    .driver = text_driver1
};


rtos_task_t task_tipke1 = {
    .last_tick = 0,
    .name = "TIPKE",
    .active = 1,
    .thred = 1,
    .driver = buttons_driver1
};


rtos_task_t task_adc = {
    .last_tick = 0,
    .name = "ADC",
    .active = 1,
    .thred = 1,
    .driver = adc_read1
};


/***************OPRAVILA_2**************************/




rtos_task_t task_ura2 = {
    .last_tick = 0,
    .name = "URA",
    .active = 1,
    .thred = 2,
    .driver = ura_driver2
};


rtos_task_t task_text2 = {
    .last_tick = 0,
    .name = "CALC",
    .active = 1,
    .thred = 2,
    .driver = text_driver2
};


rtos_task_t task_tipke2 = {
    .last_tick = 0,
    .name = "TIPKE",
    .active = 1,
    .thred = 2,
    .driver = buttons_driver2
};


rtos_task_t task_led2 = {
    .last_tick = 0,
    .name = "LED",
    .active = 1,
    .thred = 2,
    .driver = led_driver2
};

/**********************opravila_3**********************************************/




rtos_task_t task_ura3 = {
    .last_tick = 0,
    .name = "URA",
    .active = 1,
    .thred = 3,
    .driver = ura_driver3
};


rtos_task_t task_calc = {
    .last_tick = 0,
    .name = "CALC",
    .active = 1,
    .thred = 3,
    .driver = calc_driver
};


rtos_task_t task_tipke3 = {
    .last_tick = 0,
    .name = "TIPKE",
    .active = 1,
    .thred = 3,
    .driver = buttons_driver3
};








/*****************FUNKCIJE_0**************************************************/


void ura_driver(void){
    static uint32_t clk = 0;

    clk++;

    if(!(clk%25)){
        ioport_toggle_pin_level(D1_IDX);
    }
}


void text_driver(void){                  // backspace = 8
    static uint8_t position = 0;
    int i,n = 0;
    uint8_t key;
    /// z isklapljanjem procesov


    n = fifo_read(&keys_fifo,&key,1);

    if((key == 27) && n){
        task_text.active = 0;
        task_sent.active = 1;
    }

    if(n){

        switch(key){

            case 127:  //backspace
                position--;
                lcd_string[16+position] = ' ';
                break;
            case  13:  // line fead
                for(i=0; i<16;i++){
                    lcd_string[i] = lcd_string[i+16];
                    lcd_string[i+16] = ' ';

                }
                break;
            case 10:  //carage return
                position = 0;
                break;
            default:
                sprintf(lcd_string+16+position,"%c",(char)key);
                position++;
                lcd_string[16+position] = ' ';

                if(position == 16){
                    position = 0;

                    for(i=0; i<16;i++){
                        lcd_string[i] = lcd_string[i+16];
                        lcd_string[i+16] = ' ';

                    }
                }
        }
    }
}


void buttons_driver(void){
    int butons = 0;

    butons = get_button_press();

    if(butons & Mask_T1){
        fifo_write(&keys_fifo,(const unsigned char *)"T1",2);
    }
    if(butons & Mask_T2){
        fifo_write(&keys_fifo,(const unsigned char *)"T2",2);
    }
    if(butons & Mask_T3){
        fifo_write(&keys_fifo,(const unsigned char *)"T3",2);
    }
    if(butons & Mask_T4){
        fifo_write(&keys_fifo,(const unsigned char *)"T4",2);
    }
}


void led_driver(void){
    uint8_t  check, data_read;
    static uint8_t  count=0;


    if(count>0){
        if((count%5 == 0)  && (count>50)){
            ioport_set_pin_level(D4_IDX, HIGH);
            ioport_set_pin_level(D2_IDX, HIGH);

        }
        else if((count%5 == 3)  && (count>50)){
            ioport_set_pin_level(D4_IDX, LOW);
            ioport_set_pin_level(D2_IDX, LOW);

        }
        else{
        }

        count--;
    }
    else{
        check = fifo_read (&keys_fifo, &data_read, 1);
        if(check == 1){
            count = data_read * 5 +50;

            /*** test neki nouga ***/
            ioport_set_pin_level(D4_IDX, HIGH);
            ioport_set_pin_level(D2_IDX, HIGH);

            count--;

            /**********************************************************/
        }
        else{
        //error reading or buffer empty
        }
    }

}

void sent_driver(void){
    int n;
    uint8_t key;

    n = fifo_read(&keys_fifo,&key,1);

    if((key == 27) && n){
        task_text.active = 1;
        task_sent.active = 0;
    }
    else if(n){
        uart_write(UART,key);
    }
}

/*********************************************funkcije_1********************************************************/

void text_driver1(void){
    int n,i;

    n = sprintf(lcd_string+16,"f = %.2f ",signal.f);

    if(signal.shape==sinus){
        i = sprintf(lcd_string+16+n,"sinus");
        lcd_string[16+n+i] = ' ';
    }
    else{
        i = sprintf(lcd_string+16+n,"triang");
        lcd_string[16+n+i] = ' ';
    }
}


void buttons_driver1(void){
    int butons = 0;

    butons = get_button_press();

    if(butons & Mask_T1){
        signal.shape = sinus;
    }
    if(butons & Mask_T2){
        signal.shape = triang;
    }
   /* if(butons & Mask_T3){

    }
    if(butons & Mask_T4){

    }*/
}

/*********************************funkcije_2******************************************/

void ura_driver2(void){
    static uint32_t clk = 0;

    clk++;

    if(!(clk%12)){
        ioport_toggle_pin_level(D1_IDX);
    }

}


void text_driver2(void){
    uint8_t waiting[KEYS_FIFO_SIZE];
    uint16_t n, i;
    n=0;
    i = 0;

    i = fifo_look(&keys_fifo,waiting);

    for(n = 0; (!(n == i) && (i != 0)); n++){
        sprintf(lcd_string+16+n,"%d",(int)waiting[n]);
    }
    lcd_string[16+i] = ' ';
}


void buttons_driver2(void){
    int butons = 0;
    uint8_t ena = 1, dva = 2, tri = 3, stiri = 4;

    butons = get_button_press();

    if(butons & Mask_T1){
        fifo_write(&keys_fifo,&ena,1);
    }
    if(butons & Mask_T2){
        fifo_write(&keys_fifo,&dva,1);
    }
    if(butons & Mask_T3){
        fifo_write(&keys_fifo,&tri,1);
    }
    if(butons & Mask_T4){
        fifo_write(&keys_fifo,&stiri,1);
    }
}


void led_driver2(void){
    uint8_t  check, data_read;
    static uint8_t  count=0;


    if(count>0){
        if((count%5 == 0)  && (count>50)){
            ioport_set_pin_level(D4_IDX, HIGH);
            ioport_set_pin_level(D2_IDX, HIGH);

        }
        else if((count%5 == 3)  && (count>50)){
            ioport_set_pin_level(D4_IDX, LOW);
            ioport_set_pin_level(D2_IDX, LOW);

        }
        else{
        }

        count--;
    }
    else{
        check = fifo_read (&keys_fifo, &data_read, 1);
        if(check == 1){
            count = data_read * 5 +50;

            /*** test neki nouga ***/
            ioport_set_pin_level(D4_IDX, HIGH);
            ioport_set_pin_level(D2_IDX, HIGH);

            count--;

            /**********************************************************/
        }
        else{
        //error reading or buffer empty
        }
    }

}


/**********************************funkcije_3***************************************************************************/


















uint32_t clk = 0;
void ura_driver3(void){

    clk++;

    if(clk > DAN){
        clk = clk-DAN;
    }
}


void calc_driver(void){
    int sec, min, ure, n;

    sec = clk/10;
    min = sec/60;
    ure = min/60;

    sec = sec%60;
    min = min%60;

    n = sprintf(lcd_string,"ura: %2d:%2d:%2d",ure,min,sec);
    lcd_string[n] = ' ';
}


void buttons_driver3(void){
    int butons = 0;
    bool  hold = 0;

    butons = get_button_press();

    if(butons & Mask_T1){
        clk = clk + URA;
    }
    if(butons & Mask_T2){
        clk = clk + MIN;
    }
    if(butons & Mask_T4){
        clk = 0;
    }

    hold = get_button_state() ^ 0xF;

    if(hold && clk>0){
        clk--;
    }
}






/*******************************************************************IZBIRA PROGRAMA******************************************************************/

rtos_task_t *task_list[10];
rtos_task_t *task_priv = &task_ura;

void change(rtos_task_t *task_list00[10],rtos_task_t *priv){
    int i=0;
    while(task_list00[i]){
        task_list[i] = task_list00[i];
        i++;
    }
    task_priv = priv;
}

/****************************************************************************************************************************************************/

rtos_task_t *task_list0[] = {&task_tipke, &task_text, &task_lcd, &task_sent,&task_led, 0/*null__pointer*/}; //array od tega tipa sprem samo da so naslovi -> konstantna velikost
rtos_task_t *task_list1[] = {&task_tipke1, &task_text1, &task_lcd, &task_adc, 0/*null__pointer*/};
rtos_task_t *task_list2[] = {&task_tipke2, &task_text2, &task_lcd, &task_ura2, 0/*null__pointer*/}; //array od tega tipa sprem samo da so naslovi -> konstantna velikost
rtos_task_t *task_list3[] = {&task_ura3, &task_tipke3, &task_calc, &task_lcd, 0/*null__pointer*/};








#ifdef __cplusplus
}
#endif
