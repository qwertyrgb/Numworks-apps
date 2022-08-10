#include <stdbool.h>
#include <stdio.h>
#include "extapp_api.h"
#include "inc/peripherals.h"
#include "inc/selector.h"


/*
Bounce Ball app
For Two Numworks
Ball bounces on left on N0100, right on N0110 // Not now
Or you can just change code
*/

void draw_ball(int a,int x,int y,int color){
    for(int i=-a;i<a+1;i++){
        for(int j=-a;j<a+1;j++){
            if(i*i+j*j<a*a) extapp_pushRectUniform(x+i,y+j,1,1,color);
        }
    }
}

void extapp_main(void) {
    waitForKeyReleased();
    init_display();
    int x=-160;
    int y=110;
    int cx=1;
    int cy=2;
    for(;;){
    draw_ball(10,x,y,0);
    extapp_msleep(10);
    draw_ball(10,x,y,0xffff);
    x+=cx;
    y+=cy;
    if(-310>x || x>310) cx=-cx;
    if(10>y || y>230) cy=-cy;
    uint64_t k = extapp_scanKeyboard();
    if (k & SCANCODE_Home) return;
}
}
