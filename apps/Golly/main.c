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

//TODO add 1D rules + generations + von neumann...

unsigned char a[r][t];
unsigned int spd=10;
unsigned int grid=0;
unsigned int hex=0;
int born[9]={-1,-1,-1,3,-1,-1,-1,-1,-1};
int survive[9]={-1,-1,2,3,-1,-1,-1,-1,-1};
int Continue=1;

int in(int num,int arr[],int len){
  for(int i=0;i<len;i++){
    if(arr[i]==num)return 1;
  }
  return 0;
}

int min(int a,int b){
  return a>b?b:a;
}

void drawhex(int s,int posx,int posy,int clr){
  s=(float)s;
  posx/=s;
  posy/=s;
  for (int x=-s;x<s;x++) {
    for (int y=-s;y<s;y++) {
	if(abs(y)<sqrt(3)*min(s-abs(x),s/2))extapp_pushRectUniform(x+(0.75*s*sqrt(3)+s/5)*posx+10,y+(1+sqrt(3)*s)*posy+(posx%2)*s+10,1,1,clr);
    }
  }
}

void draw(unsigned char a[r][t]){
  //extapp_pushRectUniform(0,0,320,240,0xFFFF);
  for(int i=0;i<r;i++){
    for(int j=0;j<t;j++){
      if(a[i][j]){
        if(hex){
          drawhex(size-grid,size*j,size*(i-(j-t/2)/2),0);
        }else{
          extapp_pushRectUniform(size*j,size*i,size-grid,size-grid,0);
        }
      }else{
        if(hex){
          drawhex(size-grid,size*j,size*(i-(j-t/2)/2),0xFFFF);
        }else{
          extapp_pushRectUniform(size*j,size*i,size-grid,size-grid,0xFFFF);
        }
      }
    }
  }
}

char*concat(const char*s1,const char*s2){
  char*result=malloc(strlen(s1)+strlen(s2)+1);
  strcpy(result,s1);
  strcat(result,s2);
  return result;
}

void gol(unsigned char a[r][t],int isdraw){
  unsigned char q[r][t]={};
  for (int i=0;i<r;i++) {
    for (int j=0;j<t;j++) {
      if (i>0&&a[i-1][j])q[i][j]++;
      if (i<r-1&&a[i+1][j])q[i][j]++;
      if (j>0&&a[i][j-1])q[i][j]++;
      if (j<t-1&&a[i][j+1]) q[i][j]++;
      if (i>0&&j>0 &&a[i-1][j-1])q[i][j]++;
      if (i<r-1&&j>0&&a[i+1][j-1]&&!hex)q[i][j]++;
      if (i>0&&j<t-1&&a[i-1][j+1]&&!hex)q[i][j]++;
      if (i<r-1&&j<t-1&&a[i+1][j+1])q[i][j]++;
    }
  }
  for(int i=0;i<r;i++){
    for(int j=0;j<t;j++){
      if(a[i][j])a[i][j]=in(q[i][j],survive,sizeof(survive)/4);
      else {
        a[i][j]=in(q[i][j],born,sizeof(born)/4);
      }
    }
  }
  if(isdraw){
    draw(a);
    extapp_msleep(spd*2);
  }
  uint64_t key=extapp_scanKeyboard();
  if(key==SCANCODE_Zero)od();
  if(key==SCANCODE_Home){
    Continue=0;
    return;
  }
}

void od(){
  int px,py;
  if(hex){
    px=tx/2;
    py=rx/2;
  }else{
    px=t/2;
    py=r/2;
  }
  for(;;){
    int key=extapp_getKey(true,NULL);
    if(key==KEY_CTRL_EXE)return;
    if(key==SCANCODE_Home){
      Continue=0;
      return;
    }
    if (key==KEY_CTRL_DEL) {
      memset(a,0,sizeof(a));
      draw(a);
    }
    if(key==KEY_CHAR_9){
      memset(a,0,sizeof(a));
      px-=12;
      py-=12;
      a[py+0][px+24]=1;a[py+1][px+22]=1;a[py+1][px+24]=1;a[py+2][px+12]=1;a[py+2][px+13]=1;a[py+2][px+20]=1;a[py+2][px+21]=1;a[py+2][px+34]=1;a[py+2][px+35]=1;a[py+3][px+11]=1;a[py+3][px+15]=1;a[py+3][px+20]=1;a[py+3][px+21]=1;a[py+3][px+34]=1;a[py+3][px+35]=1;a[py+4][px+0]=1;a[py+4][px+1]=1;a[py+4][px+10]=1;a[py+4][px+16]=1;a[py+4][px+20]=1;a[py+4][px+21]=1;a[py+5][px+0]=1;a[py+5][px+1]=1;a[py+5][px+10]=1;a[py+5][px+14]=1;a[py+5][px+16]=1;a[py+5][px+17]=1;a[py+5][px+22]=1;a[py+5][px+24]=1;a[py+6][px+10]=1;a[py+6][px+16]=1;a[py+6][px+24]=1;a[py+7][px+11]=1;a[py+7][px+15]=1;a[py+8][px+12]=1;a[py+8][px+13]=1;
      draw(a);
    }
    if(key==KEY_CHAR_8){
      px-=12;
      py-=12;
		memset(a,0,sizeof(a));
      a[py+0][px+0]=1;a[py+0][px+1]=1;a[py+0][px+7]=1;a[py+0][px+8]=1;a[py+1][px+0]=1;a[py+1][px+1]=1;a[py+1][px+7]=1;a[py+1][px+8]=1;a[py+3][px+4]=1;a[py+3][px+5]=1;a[py+4][px+4]=1;a[py+4][px+5]=1;a[py+9][px+22]=1;a[py+9][px+23]=1;a[py+9][px+25]=1;a[py+9][px+26]=1;a[py+10][px+21]=1;a[py+10][px+27]=1;a[py+11][px+21]=1;a[py+11][px+28]=1;a[py+11][px+31]=1;a[py+11][px+32]=1;a[py+12][px+21]=1;a[py+12][px+22]=1;a[py+12][px+23]=1;a[py+12][px+27]=1;a[py+12][px+31]=1;a[py+12][px+32]=1;a[py+13][px+26]=1;a[py+17][px+20]=1;a[py+17][px+21]=1;a[py+18][px+20]=1;a[py+19][px+21]=1;a[py+19][px+22]=1;a[py+19][px+23]=1;a[py+20][px+23]=1;
      draw(a);
    }
	 if(key==KEY_CHAR_7){
		memset(a,0,sizeof(a));
      a[py+0][px+21]=1;a[py+1][px+21]=1;a[py+1][px+22]=1;a[py+1][px+23]=1;a[py+2][px+19]=1;a[py+2][px+20]=1;a[py+2][px+24]=1;a[py+3][px+3]=1;a[py+3][px+4]=1;a[py+3][px+8]=1;a[py+3][px+9]=1;a[py+3][px+14]=1;a[py+3][px+18]=1;a[py+3][px+20]=1;a[py+3][px+22]=1;a[py+3][px+24]=1;a[py+4][px+3]=1;a[py+4][px+4]=1;a[py+4][px+7]=1;a[py+4][px+10]=1;a[py+4][px+13]=1;a[py+4][px+15]=1;a[py+4][px+18]=1;a[py+4][px+20]=1;a[py+4][px+22]=1;a[py+4][px+23]=1;a[py+5][px+8]=1;a[py+5][px+9]=1;a[py+5][px+14]=1;a[py+5][px+19]=1;a[py+7][px+2]=1;a[py+7][px+3]=1;a[py+7][px+21]=1;a[py+8][px+1]=1;a[py+8][px+4]=1;a[py+8][px+8]=1;a[py+8][px+9]=1;a[py+8][px+13]=1;a[py+8][px+14]=1;a[py+8][px+20]=1;a[py+8][px+22]=1;a[py+9][px+1]=1;a[py+9][px+4]=1;a[py+9][px+7]=1;a[py+9][px+10]=1;a[py+9][px+13]=1;a[py+9][px+15]=1;a[py+9][px+20]=1;a[py+9][px+22]=1;a[py+10][px+2]=1;a[py+10][px+3]=1;a[py+10][px+7]=1;a[py+10][px+9]=1;a[py+10][px+14]=1;a[py+10][px+19]=1;a[py+10][px+20]=1;a[py+10][px+22]=1;a[py+10][px+23]=1;a[py+11][px+8]=1;a[py+13][px+14]=1;a[py+13][px+15]=1;a[py+13][px+20]=1;a[py+13][px+21]=1;a[py+13][px+25]=1;a[py+13][px+26]=1;a[py+14][px+2]=1;a[py+14][px+3]=1;a[py+14][px+7]=1;a[py+14][px+8]=1;a[py+14][px+10]=1;a[py+14][px+14]=1;a[py+14][px+17]=1;a[py+14][px+20]=1;a[py+14][px+22]=1;a[py+14][px+24]=1;a[py+14][px+26]=1;a[py+15][px+2]=1;a[py+15][px+4]=1;a[py+15][px+7]=1;a[py+15][px+9]=1;a[py+15][px+10]=1;a[py+15][px+16]=1;a[py+15][px+17]=1;a[py+15][px+22]=1;a[py+15][px+24]=1;a[py+16][px+3]=1;a[py+16][px+4]=1;a[py+16][px+20]=1;a[py+16][px+22]=1;a[py+16][px+24]=1;a[py+16][px+26]=1;a[py+17][px+20]=1;a[py+17][px+21]=1;a[py+17][px+25]=1;a[py+17][px+26]=1;a[py+19][px+3]=1;a[py+19][px+4]=1;a[py+19][px+12]=1;a[py+19][px+13]=1;a[py+20][px+3]=1;a[py+20][px+5]=1;a[py+20][px+11]=1;a[py+20][px+14]=1;a[py+21][px+5]=1;a[py+21][px+10]=1;a[py+21][px+12]=1;a[py+21][px+13]=1;a[py+21][px+15]=1;a[py+22][px+1]=1;a[py+22][px+2]=1;a[py+22][px+3]=1;a[py+22][px+4]=1;a[py+22][px+11]=1;a[py+22][px+14]=1;a[py+23][px+0]=1;a[py+23][px+12]=1;a[py+23][px+13]=1;a[py+24][px+0]=1;a[py+24][px+2]=1;a[py+25][px+1]=1;a[py+25][px+2]=1;
		draw(a);
    }
	 if(key==KEY_CHAR_6){
		memset(a,0,sizeof(a));
   	a[py+0][px+37]=1;a[py+0][px+38]=1;a[py+0][px+39]=1;a[py+0][px+43]=1;a[py+0][px+44]=1;a[py+0][px+45]=1;a[py+2][px+35]=1;a[py+2][px+40]=1;a[py+2][px+42]=1;a[py+2][px+47]=1;a[py+3][px+35]=1;a[py+3][px+40]=1;a[py+3][px+42]=1;a[py+3][px+47]=1;a[py+4][px+35]=1;a[py+4][px+40]=1;a[py+4][px+42]=1;a[py+4][px+47]=1;a[py+5][px+37]=1;a[py+5][px+38]=1;a[py+5][px+39]=1;a[py+5][px+43]=1;a[py+5][px+44]=1;a[py+5][px+45]=1;a[py+6][px+2]=1;a[py+6][px+7]=1;a[py+6][px+21]=1;a[py+6][px+22]=1;a[py+6][px+28]=1;a[py+6][px+29]=1;a[py+7][px+0]=1;a[py+7][px+1]=1;a[py+7][px+3]=1;a[py+7][px+4]=1;a[py+7][px+5]=1;a[py+7][px+6]=1;a[py+7][px+8]=1;a[py+7][px+9]=1;a[py+7][px+15]=1;a[py+7][px+16]=1;a[py+7][px+17]=1;a[py+7][px+21]=1;a[py+7][px+22]=1;a[py+7][px+27]=1;a[py+7][px+37]=1;a[py+7][px+38]=1;a[py+7][px+39]=1;a[py+7][px+43]=1;a[py+7][px+44]=1;a[py+7][px+45]=1;a[py+8][px+2]=1;a[py+8][px+7]=1;a[py+8][px+23]=1;a[py+8][px+24]=1;a[py+8][px+30]=1;a[py+8][px+35]=1;a[py+8][px+40]=1;a[py+8][px+42]=1;a[py+8][px+47]=1;a[py+9][px+23]=1;a[py+9][px+24]=1;a[py+9][px+28]=1;a[py+9][px+29]=1;a[py+9][px+35]=1;a[py+9][px+40]=1;a[py+9][px+42]=1;a[py+9][px+47]=1;a[py+10][px+35]=1;a[py+10][px+40]=1;a[py+10][px+42]=1;a[py+10][px+47]=1;a[py+12][px+37]=1;a[py+12][px+38]=1;a[py+12][px+39]=1;a[py+12][px+43]=1;a[py+12][px+44]=1;a[py+12][px+45]=1;a[py+13][px+7]=1;a[py+13][px+8]=1;a[py+13][px+23]=1;a[py+13][px+24]=1;a[py+14][px+7]=1;a[py+14][px+8]=1;a[py+15][px+22]=1;a[py+15][px+26]=1;a[py+16][px+5]=1;a[py+16][px+6]=1;a[py+16][px+7]=1;a[py+16][px+8]=1;a[py+16][px+21]=1;a[py+16][px+26]=1;a[py+17][px+1]=1;a[py+17][px+2]=1;a[py+17][px+4]=1;a[py+17][px+9]=1;a[py+17][px+20]=1;a[py+17][px+22]=1;a[py+17][px+24]=1;a[py+17][px+33]=1;a[py+17][px+34]=1;a[py+17][px+36]=1;a[py+17][px+37]=1;a[py+17][px+39]=1;a[py+18][px+1]=1;a[py+18][px+2]=1;a[py+18][px+4]=1;a[py+18][px+6]=1;a[py+18][px+7]=1;a[py+18][px+9]=1;a[py+18][px+19]=1;a[py+18][px+21]=1;a[py+18][px+23]=1;a[py+18][px+32]=1;a[py+18][px+33]=1;a[py+18][px+36]=1;a[py+18][px+37]=1;a[py+18][px+39]=1;a[py+18][px+40]=1;a[py+19][px+4]=1;a[py+19][px+8]=1;a[py+19][px+9]=1;a[py+19][px+11]=1;a[py+19][px+12]=1;a[py+19][px+17]=1;a[py+19][px+22]=1;a[py+19][px+40]=1;a[py+20][px+4]=1;a[py+20][px+9]=1;a[py+20][px+11]=1;a[py+20][px+12]=1;a[py+20][px+17]=1;a[py+20][px+21]=1;a[py+20][px+32]=1;a[py+20][px+33]=1;a[py+21][px+5]=1;a[py+21][px+6]=1;a[py+21][px+7]=1;a[py+21][px+8]=1;a[py+21][px+32]=1;a[py+21][px+33]=1;a[py+21][px+39]=1;a[py+21][px+40]=1;a[py+22][px+19]=1;a[py+22][px+20]=1;a[py+22][px+39]=1;a[py+22][px+40]=1;a[py+23][px+5]=1;a[py+23][px+6]=1;a[py+23][px+32]=1;a[py+24][px+5]=1;a[py+24][px+6]=1;a[py+24][px+32]=1;a[py+24][px+33]=1;a[py+24][px+35]=1;a[py+24][px+36]=1;a[py+24][px+39]=1;a[py+24][px+40]=1;a[py+25][px+33]=1;a[py+25][px+35]=1;a[py+25][px+36]=1;a[py+25][px+38]=1;a[py+25][px+39]=1;
	 draw(a);
	 }
	 if(key==KEY_CHAR_5){
		memset(a,0,sizeof(a));
   	a[py+0][px+27]=1;a[py+0][px+28]=1;a[py+0][px+38]=1;a[py+0][px+39]=1;a[py+1][px+27]=1;a[py+1][px+29]=1;a[py+1][px+37]=1;a[py+1][px+38]=1;a[py+2][px+27]=1;a[py+2][px+39]=1;a[py+3][px+3]=1;a[py+3][px+4]=1;a[py+3][px+41]=1;a[py+3][px+42]=1;a[py+4][px+3]=1;a[py+4][px+4]=1;a[py+4][px+40]=1;a[py+5][px+2]=1;a[py+5][px+5]=1;a[py+6][px+2]=1;a[py+6][px+3]=1;a[py+6][px+4]=1;a[py+6][px+5]=1;a[py+6][px+39]=1;a[py+6][px+42]=1;a[py+7][px+1]=1;a[py+7][px+3]=1;a[py+7][px+4]=1;a[py+7][px+6]=1;a[py+7][px+31]=1;a[py+7][px+32]=1;a[py+7][px+38]=1;a[py+7][px+39]=1;a[py+8][px+2]=1;a[py+8][px+5]=1;a[py+8][px+30]=1;a[py+8][px+31]=1;a[py+8][px+37]=1;a[py+9][px+32]=1;a[py+9][px+37]=1;a[py+9][px+39]=1;a[py+10][px+1]=1;a[py+10][px+2]=1;a[py+10][px+5]=1;a[py+10][px+6]=1;a[py+10][px+34]=1;a[py+10][px+35]=1;a[py+10][px+38]=1;a[py+11][px+0]=1;a[py+11][px+7]=1;a[py+11][px+34]=1;a[py+11][px+35]=1;a[py+13][px+1]=1;a[py+13][px+6]=1;a[py+14][px+2]=1;a[py+14][px+5]=1;a[py+15][px+3]=1;a[py+15][px+4]=1;a[py+17][px+19]=1;a[py+18][px+6]=1;a[py+18][px+9]=1;a[py+18][px+17]=1;a[py+18][px+21]=1;a[py+19][px+5]=1;a[py+19][px+16]=1;a[py+19][px+33]=1;a[py+19][px+34]=1;a[py+20][px+5]=1;a[py+20][px+9]=1;a[py+20][px+16]=1;a[py+20][px+21]=1;a[py+20][px+31]=1;a[py+20][px+36]=1;a[py+21][px+5]=1;a[py+21][px+6]=1;a[py+21][px+7]=1;a[py+21][px+8]=1;a[py+21][px+16]=1;a[py+21][px+17]=1;a[py+21][px+18]=1;a[py+21][px+19]=1;a[py+21][px+20]=1;a[py+21][px+30]=1;a[py+22][px+30]=1;a[py+22][px+36]=1;a[py+23][px+30]=1;a[py+23][px+31]=1;a[py+23][px+32]=1;a[py+23][px+33]=1;a[py+23][px+34]=1;a[py+23][px+35]=1;
	 draw(a);
	 }
    if(key==KEY_CHAR_PLUS)gol(a,1);
    if(key==KEY_CHAR_MULT){
      for(int i=0;i<100;i++)gol(a,0);
      gol(a,1);
    }
    if(key==KEY_CTRL_RIGHT){
      if (hex&&px<tx-1){
        drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
        px++;
        drawhex(size-grid,size*px,size*py,0x07E0);
      }else if(px<t-1&&!hex) {
        extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
        px++;
        extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,0x07E0);
      }
    }
    if(key==KEY_CTRL_LEFT){
      if(hex&&px>2){
        drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
        px--;
        drawhex(size-grid,size*px,size*py,0x07E0);
      }else if(px>0&&!hex){
        extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
        px--;
        extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,0x07E0);
      }
    }
    if (key==KEY_CTRL_UP) {
      if (hex&&py>2) {
        drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
        py--;
        drawhex(size-grid,size*px,size*py, 0x07E0);
      } else if (py > 0 && !hex) {
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
      } else if (py<r-1&&!hex){
        extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
        py++;
        extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,0x07E0);
      }
    }
    if(key==KEY_CTRL_OK){
      if(hex){
        a[py+(px-t/2)/2][px]=1-a[py+(px-t/2)/2][px];
        drawhex(size-grid,size*px,size*py,a[py+(px-t/2)/2][px]?0:0xffff);
      }else{
        a[py][px]=1-a[py][px];
        extapp_pushRectUniform(size*px,size*py,size-grid,size-grid,a[py][px]?0:0xffff);
      }
    }
    extapp_msleep(10);
  }
}

void extapp_main(void){
  while (extapp_scanKeyboard()) {
    extapp_msleep(10);
  }
  extapp_pushRectUniform(0,0,320,240,0xFFFF);
  unsigned int tp=0;
  unsigned int tptwo=0;
  const char isgrid[2][4]={"No ","Yes"};
  const char ishex[2][4] = {
    "OFF",
    "ON "
  };
  //void menu(void) { //function separated from extapp_main
  for(;;){
    int key=extapp_getKey(true, NULL);
    if (key==KEY_CTRL_EXIT||key==KEY_CTRL_MENU){
      return;
    } else if(key==KEY_CTRL_RIGHT){
      if(tp==0&&spd<99)spd++;
      else if(tp==1)grid=1-grid;
      else if((tp== 2||tp==3)&&tptwo<8)tptwo++;
      else if(tp==4) hex = 1-hex;
    } else if(key==KEY_CTRL_LEFT){
      if (tp == 0 && spd > 0)spd--;
      else if(tp==1)grid=1-grid;
      else if((tp==2||tp==3)&&tptwo>0)tptwo--;
      else if(tp==4)hex=1-hex;
    } else if(key==KEY_CTRL_UP&&tp>0)tp--;
    else if (key==KEY_CTRL_DOWN&&tp<4)tp++;
    else if (key==KEY_CTRL_OK)break;
    else if (key==KEY_CTRL_EXE){
      if(tp==2){
        if (born[tptwo]==-1)born[tptwo]=tptwo;
        else {
          born[tptwo]=-1;
        }
      } else if (tp==3) {
        if (survive[tptwo]==-1)survive[tptwo]=tptwo;
        else {
          survive[tptwo]=-1;
        }
      }
    }
    char putnum[2];
    sprintf(putnum, "%d", spd);
    if (spd < 10) strcpy(putnum, concat(putnum, " "));
    extapp_drawTextLarge(concat("Speed: ",putnum),0,20,0,(tp?0xffff:0x1f),false);
    extapp_drawTextLarge(concat("Grid? ",isgrid[grid]),0,40,0,(tp==1?0x1f:0xffff), false);
    extapp_drawTextLarge("Born: ",0,60,0,0xffff,false);
    extapp_drawTextLarge("0",60,60,0,tp==2&&tptwo==0?0x1f:in(0,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("1",80,60,0,tp==2&&tptwo==1?0x1f:in(1,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("2",100,60,0,tp==2&&tptwo==2?0x1f:in(2,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("3",120,60,0,tp==2&&tptwo==3?0x1f:in(3,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("4",140,60,0,tp==2&&tptwo==4?0x1f:in(4,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("5",160,60,0,tp==2&&tptwo==5?0x1f:in(5,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("6",180,60,0,tp==2&&tptwo==6?0x1f:in(6,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("7",200,60,0,tp==2&&tptwo==7?0x1f:in(7,born,9)?0x7e0:0xffff,false);
    extapp_drawTextLarge("8",220,60,0,tp==2&&tptwo==8? 0x1f:in(8,born,9)?0x7e0:0xffff,false);
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
  while(Continue){
    gol(a,1);
  }
}