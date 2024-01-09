#ifndef DACC_LIB_H_INCLUDED
#define DACC_LIB_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#define NS 10

#include<bord.h>
#include<asf.h>
#include<dacc.h>

struct signal{
    float f;
    uint32_t *shape;
};

typedef struct signal signal_t; //da ne pišm struct task pa nov tip sprem define

uint32_t sinus[NS];
uint32_t triang[NS];

void dacc_setup(void);






















#ifdef __cplusplus
}
#endif
#endif // DACC_LIB_H_INCLUDED
