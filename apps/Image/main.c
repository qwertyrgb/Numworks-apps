#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "extapp_api.h"

#include "inc/peripherals.h"
#include "inc/selector.h"
#include "img.h"

void extapp_main(void) {
waitForKeyReleased();
init_display();
for(unsigned int len=0;len<img_len;) {
extapp_pushRectUniform(len/480,(len/2)%240,1,1,img[len++]*256+img[len++]);
}
int key = extapp_getKey(true, NULL);
if(key == KEY_CTRL_EXIT || key == KEY_CTRL_MENU) {
return;
}
}
