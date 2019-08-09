#include <stdio.h>
#include <stdlib.h>
#include <handy.h>

#define START 45

int main(){
    int sec = 0;
    int min = 0;
    //int time = 0;
    char hoge;

    HgOpen(200, 200);
    HgSetEventMask(HG_KEY_DOWN);
    HgSetFont(HG_G, 20);

    // 時間を1秒段位で測る(切り上げ)
    while(1){
        if(hoge != 'a'){
            HgWClear();
            if(sec < 10)
                HgText(START, 90, "Time:  %d:0%d", min, sec);
            else
                HgText(START, 90, "Time:  %d:%d", min, sec);
            HgSleep(0.9999);
            sec++;\
            if(sec == 60){
                sec = 0;
                min++;
            }
        }
        else
            break;
    }

    HgGetChar();
    HgClose();
    return 0;
}
