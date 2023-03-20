#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "extapp_api.h"

#include "inc/peripherals.h"
#include "inc/selector.h"
/*#include <stdlib.h>
TODO: NOW IS ["000","010","011"]. THIS IS BAD, EVIL; FIXME RN
HOWTO FIX: [0,2,3] or 00001011 then 11
OOOR get_pixel, set_pixel
*/

#define size 3
#define r LCD_HEIGHT/size
#define t LCD_WIDTH/size

unsigned char a[r][t];
unsigned int spd=10;
void draw(unsigned char a[r][t]){
for(int i=0;i<r;i++){
	for(int j=0;j<t;j++) {
	extapp_pushRectUniform(size*j,size*i,size-1,size-1,a[i][j]? 0: 0xFFFF);
	}
}
}


void od(){
	int px=t/2;
	int py=r/2;
	for(;;) {
	int key=extapp_getKey(true, NULL);
	if(key==KEY_CTRL_EXE) return;
	extapp_pushRectUniform(size*px,size*py+1,size-1,size-1,0xFFFF);
	if(key==KEY_CTRL_RIGHT && px<t-1) px++;
	if(key==KEY_CTRL_LEFT && px>0) px--;
	if(key==KEY_CTRL_UP && py>0) py--;
	if(key==KEY_CTRL_DOWN && py<r-1) py++;
	if(key==KEY_CTRL_OK){
	a[py][px]=1-a[py][px];
	draw(a);
	}
    extapp_msleep(100);
	}
}

void gol(unsigned char a[r][t]) {
for(;;) {
	unsigned char q[r][t]={};
	for(int i=0;i<r;i++) {
	 	for(int j=0;j<t;j++) {
			if(i>0 && a[i-1][j]) q[i][j]++;
        	if(i<r-1 && a[i+1][j]) q[i][j]++;
        	if(j>0 && a[i][j-1]) q[i][j]++;
        	if(j<t-1 && a[i][j+1]) q[i][j]++;
        	if(i>0 && j>0 && a[i-1][j-1]) q[i][j]++;
        	if(i<r-1 && j>0 && a[i+1][j-1]) q[i][j]++;
        	if(i>0 && j<t-1 && a[i-1][j+1]) q[i][j]++;
        	if(i<r-1 && j<t-1 && a[i+1][j+1]) q[i][j]++;
          }
          }
	for(int i=0;i<r;i++) {
	 	for(int j=0;j<t;j++) {
		if(a[i][j] && !(q[i][j]==3) && !(q[i][j]==2)) a[i][j]=0;
		else if(q[i][j]==3) a[i][j]=1;
		}
	}
	draw(a);
	extapp_msleep(spd*10);
	uint64_t key = extapp_scanKeyboard();
    if(key==SCANCODE_Zero) od();
	if(key==SCANCODE_Home) return;
}
}

void extapp_main(void) {
waitForKeyReleased();
init_display();
for(;;){
int key=extapp_getKey(true, NULL);
    if(key==KEY_CTRL_EXIT || key == KEY_CTRL_MENU) {
    return;
    } else if(key==KEY_CTRL_UP) {
    spd++;
    } else if(key==KEY_CTRL_DOWN && spd>0){
    spd--;
    } else if (key==KEY_CTRL_OK) {
    break;
    }
    char putnum[2];
    sprintf(putnum,"%d",spd);  
    extapp_drawTextLarge(putnum, 0, 20 * 1, 0x0000, 0xFFFF, false);
}

draw(a);
od();
init_display();
gol(a);
}