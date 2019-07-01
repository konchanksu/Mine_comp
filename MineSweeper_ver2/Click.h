#include <stdio.h>
#include <handy.h>

int mapsizex_ex;  //配列内のサイズ
int mapsizey_ex;  //配列内のサイズ
int bombnum_ex;   //爆弾の個数
int Coordinatex_ex;//マスの左下の角のx座標
int Coordinatey_ex;//マスの左下の角のy座標

//クリックした場所の判断
int ClickX_C(int x){
  int tmpX = 1000 - Coordinatex_ex;
  if(x>Coordinatex_ex && x<tmpX){
    int arrayx;
    arrayx =(x - Coordinatex_ex) / BOXSIZE;
    return arrayx;
  }
  return -1;
}
int ClickY_C(int y){
  int tmpY = 700 - Coordinatey_ex;
  if(y>Coordinatey_ex && y<tmpY){
    int arrayy;
    arrayy =(y - Coordinatey_ex) / BOXSIZE;
    return arrayy;
  }
  return -1;
}
