#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "extapp_api.h"

#include "inc/peripherals.h"
#include "inc/selector.h"
#include <complex.h>
#include <stdlib.h>
//TODO: add square (green) select pixel to center
//TODO: Maybe first float to speed? (is simulated complex faster than complex?)
//TODO: Chaneg colors


char* concat(const char *s1,const char *s2) {
    char *result=malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}
	
void extapp_main(void){
waitForKeyReleased();
init_display();
unsigned int N_iteration=10;
unsigned int Zoom=1;
unsigned int Power=2;
double xpos=-2.5;
double ypos=1.25;
unsigned int type=0;
char typename[2][16]={"Mandelbrot","Burning Ship"};
unsigned int tp=0;
for(;;){
int key=extapp_getKey(true, NULL);
    if(key==KEY_CTRL_EXIT||key==KEY_CTRL_MENU) {
    return;
    } else if(key==KEY_CTRL_RIGHT) {
    	if(tp==0) {
		Zoom*=2;
		N_iteration*=1.2;
		}
		else if(tp==1) Power+=1;
		else if(tp==2) xpos+=1.0/Zoom;
		else if(tp==3) ypos+=1.0/Zoom;
		else if(tp==4) type=1-type;
    } else if(key==KEY_CTRL_LEFT){
        if(tp==0&&Zoom>1){
		Zoom/=2;
		N_iteration/=1.2;
		}
		else if(tp==1&&Power>2) Power-=1;
		else if(tp==2) xpos-=1.0/Zoom;
		else if(tp==3) ypos-=1.0/Zoom;
		else if(tp==4) type=1-type;
    } else if(key==KEY_CTRL_UP&&tp>0) tp--;
      else if(key==KEY_CTRL_DOWN&&tp<4) tp++;
      else if (key==KEY_CTRL_OK) break;
	char Zoomch[4];
    sprintf(Zoomch,"%u",Zoom);
	char putnum[2];
    sprintf(putnum,"%d",Power);
	char xposch[16];
   	sprintf(xposch,"%E",xpos);
	char yposch[16];
    sprintf(yposch,"%E",ypos);
	init_display();
    extapp_drawTextLarge(concat("Zoom: ",Zoomch),0,20,0,(tp?0xffff:0x1f),false);
    extapp_drawTextLarge(concat("Power: ",putnum),0,40,0,(tp==1?0x1f:0xffff),false);
    extapp_drawTextLarge(concat("xpos: ",xposch),0,60,0,(tp==2?0x1f:0xffff),false);
    extapp_drawTextLarge(concat("ypos: ",yposch),0,80,0,(tp==3?0x1f:0xffff),false);
	extapp_drawTextLarge(concat("Type: ",typename[type]),0,100,0,(tp==4?0x1f:0xffff),false);
}
void redraw(void) {
init_display();
for(int x=0;x<320;x++){
for(int y=0;y<241;y++){
double complex z=0.0+0.0*I;
double complex c=(3.5*x/319)/Zoom+xpos+((-2.5*y/241)/Zoom+ypos)*I;
    int i=0;
    while(i<N_iteration && abs(z)<2){
       i+=1;
	   if(type==1) {
	   z=cabs(creal(z))+cabs(cimag(z))*I;
	   for(int pw=1;pw<Power;pw++){
       	z*=z;
	   }
	   }
	   else {
	   double complex tmp=z;
       for(int pw=1;pw<Power;pw++){
       	tmp*=z;
	   }
	   z=tmp;
	   }
	   z+=c;
      }
      int rgb=255*i/N_iteration;
      if(type==1) extapp_pushRectUniform(x,241-y,1,1,((rgb&0xF80000)>>8)|((rgb&0x00FC00)>>5)|((rgb&0x0000F8)>>3));
	  else if(type==0) extapp_pushRectUniform(x,y,1,1,((rgb&0xF80000)>>8)|((rgb&0x00FC00)>>5)|((rgb&0x0000F8)>>3));
}
}
}
redraw();
for(;;){
int key=extapp_getKey(true,NULL);
    if(key==KEY_CTRL_EXIT||key==KEY_CTRL_MENU){
		char Zoomch[4];
    	sprintf(Zoomch,"%u",Zoom);
		char xposch[16];
    	sprintf(xposch,"%E",xpos);
		char yposch[16];
    	sprintf(yposch,"%E",ypos);
		char filecontent_to_write[50];
       strcpy(filecontent_to_write,concat(concat(concat(concat(concat(" ",Zoomch),"\n"),xposch),"\n"),yposch));
		extapp_fileWrite("qwer.py",filecontent_to_write,strlen(filecontent_to_write)+1, EXTAPP_RAM_FILE_SYSTEM);
		return;
    } else if(key==KEY_CHAR_RPAR){
    N_iteration*=2;
    } else if(key==KEY_CHAR_LPAR){
	N_iteration/=2;
	} else if(key==KEY_CTRL_UP){
	if(type==1) ypos-=0.4/Zoom;
	else if(type==0) ypos+=0.4/Zoom;
	} else if(key==KEY_CTRL_DOWN){
	if(type==1) ypos+=0.4/Zoom;
	else if(type==0) ypos-=0.4/Zoom;
	}  else if(key==KEY_CTRL_LEFT){
	xpos-=0.5/Zoom;
	} else if(key==KEY_CTRL_RIGHT){
	xpos+=0.5/Zoom;
	} else if(key==KEY_CHAR_PLUS){
		xpos+=0.875/Zoom;
		ypos-=0.6125/Zoom;
		Zoom*=2;
		N_iteration*=1.1;
	} else if(key == KEY_CHAR_2){
		xpos+=1.3125/Zoom;
		ypos-=0.91875/Zoom;
		Zoom*=4;
		N_iteration*=1.2;
	} else if(key==KEY_CHAR_MINUS){
		xpos-=0.875/Zoom;
		ypos+=0.6125/Zoom;
		Zoom/=2;
		N_iteration/=1.1;
	} else if(key == KEY_CHAR_0){
		xpos=-2.5;
		ypos=1.25;
		Zoom=1;
		N_iteration=10;
	}
	redraw();
}
}
