#include <stdio.h>
#include <handy.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "Discription.h"
#include "Array.h"
#include "Click.h"

#define BOXSIZE 30   //マス一マス分のサイズ
#define BOXCENTER 15    //マス一マスの半分のサイズ

extern int mapsizex_ex;   //配列内のサイズ
extern int mapsizey_ex;   //配列内のサイズ
extern int bombnum_ex;    //爆弾の個数
extern int Coordinatex_ex;//マスの左下の角のx座標
extern int Coordinatey_ex;//マスの左下の角のy座標

//_D --Discription.h  _A --Array.h  _C --Click.h 
//_ex --extern

/*
   　　マインスイーパー　配列の振り分け
   　　　旗の付いていないマス
   　　　　爆弾 ... 10
     　開いていない番号のマス ... それぞれの周りにある爆弾の数
     　開いた番号のマス　...　-100

     旗のついたマス
     　爆弾 ... -10
     　開いていない番号のマス（０を除く） ...　-番号
     　開いていない0のマス ... -20
 */

int main(){
    srand(time(NULL));
    //変数の宣言
    int x, y;     //クリック位置
    int explain_route; //説明に行くかどうか
    int level;    //レベル設定
    int remain;   //クリアまで残り~
    int flag = 0; //旗のon off
    int layer1, layer2; //レイヤー設定
    int arrayx, arrayy; //配列での座標
    int forecastbom; //残り爆弾の数（プレーヤーが予測した数）
    hgevent *event;

    HgOpen(1000, 700);

    HgSetEventMask(HG_MOUSE_DOWN);
    layer1 = HgWAddLayer(0);
    layer2 = HgWAddLayer(0);

    //オープニング画面、レベル設定画面
    explain_route = Opening_D(x, y, event);
    level = Explain_D(x, y, event, explain_route);
    LevelDecide_A(level);

    //爆弾の配置
    int MAP[mapsizex_ex][mapsizey_ex];
    int DONOTCHANGE[mapsizex_ex][mapsizey_ex];

    RandomBomb_A(MAP);
    DontChange(MAP,DONOTCHANGE);
    GameUI_D();

    forecastbom = bombnum_ex;
    remain = mapsizex_ex * mapsizey_ex - bombnum_ex;

    Forecastbom_D(forecastbom, layer1);
    HgSleep(0.3);

    //最初のマスを自動で開く動作
    int startopen = StOpen_A(MAP);
    HgSetFont(HG_G,20);

    if(startopen != -1) {
        int starty = startopen/100;
        int startx = startopen - (starty * 100);
        int wherex = Coordinatex_ex + startx * BOXSIZE;
        int wherey = Coordinatey_ex + starty * BOXSIZE;

        HgSetFillColor(HG_WHITE);
        HgBoxFill(wherex, wherey, BOXSIZE, BOXSIZE, 1);

        MAP[startx][starty] = -100;
        remain--;
        remain = dfs_D(startx, starty, MAP, remain);
    }

    //動作
    for(;;) {
        //通常モードの操作
        while(flag == 0) {
            event = HgEvent();
            x = event -> x;
            y = event -> y;

            flag = Flag_D(flag, layer2, x, y);
            arrayx = ClickX_C(x);
            arrayy = ClickY_C(y);
            remain = ClickDiscription_D(arrayx, arrayy, MAP, DONOTCHANGE, remain);;
            //printf("%d\n",remain);
            forecastbom = ForecastbomR_D(forecastbom, MAP);
            Forecastbom_D(forecastbom, layer1);

            if(remain >= 1000) break;
        }

        //旗をつけるモードの操作
        while(flag == 1) {
            event = HgEvent();
            x = event -> x;
            y = event -> y;

            flag = Flag_D(flag, layer2, x, y);
            arrayx = ClickX_C(x);
            arrayy = ClickY_C(y);
            forecastbom = FlagInstall_A(MAP, arrayx, arrayy, forecastbom);
            Forecastbom_D(forecastbom, layer1);
        }

        if(remain >= 1000) break;
    }
    HgGetChar();
    HgClose();

    return 0;
}
