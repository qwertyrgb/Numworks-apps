#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "extapp_api.h"
#include <stdlib.h>
#include <math.h>

#define size 4
#define r LCD_HEIGHT/size
#define t LCD_WIDTH/size
#define rx r/2
#define tx 3*t/4

unsigned char a[r][t];
unsigned int spd=50;
unsigned int grid=0;
unsigned int hex=0;
int born[9]={-1,-1,-1,3,-1,-1,-1,-1,-1};
int survive[9]={-1,-1,2,3,-1,-1,-1,-1,-1};

void waitForKeyReleased() {
  while (extapp_scanKeyboard()) {
    extapp_msleep(10);
  }
}

int in(int num,int arr[],int len) {
for(int i=0;i<len;i++){
if(arr[i]==num) return 1;
}
return 0;
}

int min(int a,int b) {
return a>b?b:a;
}

void drawhex(int s,int posx,int posy,int clr) {
s=(float)s;
posx/=s;
posy/=s;
for(int x=-s;x<s;x++) {
for(int y=-s;y<s;y++) {
if(abs(y)<sqrt(3)*min(s-abs(x),s/2)) extapp_pushRectUniform(x+(0.75*s*sqrt(3)+s/5)*posx+10,y+(1+sqrt(3)*s)*posy+(posx%2)*s+10,1,1,clr);
}
}
}


void draw(unsigned char a[r][t]){
for(int i=0;i<r;i++){
	for(int j=0;j<t;j++) {
	if(a[i][j]){
	if(hex) {
	drawhex(size-grid,size*j,size*(i-(j-t/2)/2),0);
	}
	else {
	extapp_pushRectUniform(size*j,size*i,size-grid,size-grid,0);
	}
	}
	}
	}
}

char* concat(const char *s1,const char *s2) {
    char *result=malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}
void od(){
	int px,py;
	if(hex) {
	px=tx/2;
	py=rx/2;
	}
	else {
	px=t/2;
	py=r/2;
	}
	for(;;) {
	int key=extapp_getKey(true,NULL);
	if(key==KEY_CTRL_EXE) return;
	if(key==KEY_CTRL_RIGHT){
	if(hex&&px<tx-1) {
	drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
	px++;
	drawhex(size-grid,size*px,size*py,0x07E0);
	} else if(px<t-1&&!hex){
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
	px++;
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,0x07E0);
	}
	}
	if(key==KEY_CTRL_LEFT){
	if(hex&&px>2) {
	drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
	px--;
	drawhex(size-grid,size*px,size*py,0x07E0);
	} else if(px>0&&!hex){
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
	px--;
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,0x07E0);
	}
	}
	if(key==KEY_CTRL_UP){
	if(hex&&py>2) {
	drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
	py--;
	drawhex(size-grid,size*px,size*py,0x07E0);
	} else if(py>0&&!hex) {
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
	py--;
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,0x07E0);
	}
	}
	if(key==KEY_CTRL_DOWN){
	if(hex&&py<rx-5) {
	drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
	py++;
	drawhex(size-grid,size*px,size*py,0x07E0);
	} else if(py<r-1&&!hex){
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
	py++;
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,0x07E0);
	}
	}
	if(key==KEY_CTRL_OK){
	if(hex){
	a[py+(px-t/2)/2][px]=1-a[py+(px-t/2)/2][px];
	drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
	}
	else {
	a[py][px]=1-a[py][px];
	extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
	}
	}
    extapp_msleep(10);
	}
}

void gol(unsigned char a[r][t]) {
for(;;) {
	unsigned char q[r][t]={};
	for(int i=0;i<r;i++) {
	 	for(int j=0;j<t;j++) {
			if(i>0&&a[i-1][j]) q[i][j]++;
        	if(i<r-1&&a[i+1][j]) q[i][j]++;
        	if(j>0&&a[i][j-1]) q[i][j]++;
        	if(j<t-1&&a[i][j+1]) q[i][j]++;
        	if(i>0&&j>0&&a[i-1][j-1]) q[i][j]++;
        	if(i<r-1&&j>0&&a[i+1][j-1]&&!hex) q[i][j]++;
        	if(i>0&&j<t-1&&a[i-1][j+1]&&!hex) q[i][j]++;
        	if(i<r-1&&j<t-1&&a[i+1][j+1]) q[i][j]++;
          }
          }
	for(int i=0;i<r;i++) {
	 	for(int j=0;j<t;j++) {
		if(a[i][j]) a[i][j]=in(q[i][j],survive,sizeof(survive)/4);
		else {
		a[i][j]=in(q[i][j],born,sizeof(born)/4);
		}
	}
	}
	extapp_pushRectUniform(0,0,320,240,0xFFFF);
	draw(a);
	extapp_msleep(spd*2);
	uint64_t key = extapp_scanKeyboard();
    if(key==SCANCODE_Zero) od();
	if(key==SCANCODE_Home) return;
}
}

void extapp_main(void) {
waitForKeyReleased();
extapp_pushRectUniform(0,0,320,240,0xFFFF);
unsigned int tp=0;
unsigned int tptwo=0;
const char isgrid[2][4]={"No ","Yes"};
const char ishex[2][4]={"OFF","ON "};
//void menu(void) { //function separated from extapp_main
for(;;){
int key=extapp_getKey(true, NULL);
    if(key==KEY_CTRL_EXIT||key==KEY_CTRL_MENU) {
    return;
    } else if(key==KEY_CTRL_RIGHT) {
    if(tp==0 && spd<99) spd++;
	else if(tp==1) grid=1-grid;
	else if((tp==2||tp==3)&&tptwo<8) tptwo++;
	else if(tp==4) hex=1-hex;
    } else if(key==KEY_CTRL_LEFT){
    if(tp==0 && spd>0) spd--;
	else if(tp==1) grid=1-grid;
	else if((tp==2||tp==3) && tptwo>0) tptwo--;
	else if(tp==4) hex=1-hex;
	} else if(key==KEY_CTRL_UP&&tp>0) tp--;
    else if(key==KEY_CTRL_DOWN&&tp<4) tp++;
	else if (key==KEY_CTRL_OK) break;
	else if (key==KEY_CTRL_EXE) {
	if(tp==2){
	if(born[tptwo]==-1) born[tptwo]=tptwo;
	else {
	born[tptwo]=-1;
	}
	} else if(tp==3) {
	if(survive[tptwo]==-1) survive[tptwo]=tptwo;
	else {
	survive[tptwo]=-1;
	}
	}
	}
    char putnum[2];
    sprintf(putnum,"%d",spd);
	if(spd<10) strcpy(putnum,concat(putnum," "));
    extapp_drawTextLarge(concat("Speed: ",putnum),0,20,0,(tp?0xffff:0x1f),false);
	extapp_drawTextLarge(concat("Grid? ",isgrid[grid]),0,40,0,(tp==1?0x1f:0xffff),false);
	extapp_drawTextLarge("Born: ",0,60,0,0xffff,false);
	extapp_drawTextLarge("0",60,60,0,tp==2&&tptwo==0?0x1f:in(0,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("1",80,60,0,tp==2&&tptwo==1?0x1f:in(1,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("2",100,60,0,tp==2&&tptwo==2?0x1f:in(2,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("3",120,60,0,tp==2&&tptwo==3?0x1f:in(3,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("4",140,60,0,tp==2&&tptwo==4?0x1f:in(4,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("5",160,60,0,tp==2&&tptwo==5?0x1f:in(5,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("6",180,60,0,tp==2&&tptwo==6?0x1f:in(6,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("7",200,60,0,tp==2&&tptwo==7?0x1f:in(7,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("8",220,60,0,tp==2&&tptwo==8?0x1f:in(8,born,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("Survive: ",0,80,0,0xffff,false);
	extapp_drawTextLarge("0",90,80,0,tp==3&&tptwo==0?0x1f:in(0,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("1",110,80,0,tp==3&&tptwo==1?0x1f:in(1,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("2",130,80,0,tp==3&&tptwo==2?0x1f:in(2,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("3",150,80,0,tp==3&&tptwo==3?0x1f:in(3,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("4",170,80,0,tp==3&&tptwo==4?0x1f:in(4,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("5",190,80,0,tp==3&&tptwo==5?0x1f:in(5,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("6",210,80,0,tp==3&&tptwo==6?0x1f:in(6,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("7",230,80,0,tp==3&&tptwo==7?0x1f:in(7,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("8",250,80,0,tp==3&&tptwo==8?0x1f:in(8,survive,9)?0x7e0:0xffff,false);
	extapp_drawTextLarge("HEX MODE: ",0,100,0,0xffff,false);
	extapp_drawTextLarge(ishex[hex],100,100,0,tp==4?0x1f:hex?0xf800:0x0700,false);
}
extapp_pushRectUniform(0,0,320,240,0xFFFF);
od();
gol(a);
}
