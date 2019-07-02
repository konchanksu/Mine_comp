#include <stdio.h>
#include <handy.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "Discription.h"
#include "Array.h"
#include "Click.h"

extern int mapsizex_ex;  //配列内のサイズ
extern int mapsizey_ex;  //配列内のサイズ
extern int bombnum_ex;   //爆弾の個数
extern int Coordinatex_ex;//マスの左下の角のx座標
extern int Coordinatey_ex;//マスの左下の角のy座標
extern int boxsize;
extern int boxcenter;

//_D --Discription.h  _A --Array.h  _C --Click.h  _S Stack_Dfs.h
//_ex --extern

int main(){
  srand(time(NULL));
  int x, y;  //クリック位置
  int explain_route;  //説明に行くかどうか
  int level;   //レベル設定
  int remain;  //クリアまで残り
  int flag = 0;  //旗のon off
  int layer1, layer2;  //レイヤー設定
  int arrayx, arrayy;  //配列での座標
  int forecastbom;  //残り爆弾表示用
  hgevent *event;

  boxsize = 30;
  boxcenter = 15;

  HgOpen(1000, 700);
  HgSetEventMask(HG_MOUSE_DOWN);

  layer1 = HgWAddLayer(0);
  layer2 = HgWAddLayer(0);

  explain_route = Opening_D(x,y,event);
  level = Explain_D(x,y,event,explain_route);
  LevelDecide_A(level);

  int MAP[mapsizex_ex][mapsizey_ex];
  int DONOTCHANGE[mapsizex_ex][mapsizey_ex];
  RandomBomb_A(MAP);
  DontChange(MAP,DONOTCHANGE);
  GameUI_D();
  forecastbom = bombnum_ex;
  remain = mapsizex_ex * mapsizey_ex - bombnum_ex;
  Forecastbom_D(forecastbom, layer1);


  //動作
  for(;;){
    while(flag==0){
      HgSetFont(HG_G,20);
      event = HgEvent();
      x = event->x;
      y = event->y;
      flag = Flag_D(flag,layer2,x,y);
      arrayx = ClickX_C(x);
      arrayy = ClickY_C(y);
      remain = ClickDiscription_D(arrayx,arrayy,MAP,DONOTCHANGE,remain);
      printf("%d\n",remain);
      if(remain >= 1000)break;
    }

    while(flag==1){
      event = HgEvent();
      x = event->x;
      y = event->y;
      flag = Flag_D(flag,layer2,x,y);
      arrayx = ClickX_C(x);
      arrayy = ClickY_C(y);
      forecastbom = FlagInstall_A(MAP,arrayx,arrayy,forecastbom);
      Forecastbom_D(forecastbom, layer1);
    }
    if(remain >= 1000)break;
  }
  HgGetChar();
  HgClose();

  return 0;
}