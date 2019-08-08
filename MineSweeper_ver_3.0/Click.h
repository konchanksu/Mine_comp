#include <stdio.h>
#include <handy.h>

int mapsizex_ex;   //配列内のサイズ
int mapsizey_ex;   //配列内のサイズ
int bombnum_ex;    //爆弾の個数
int Coordinatex_ex;//マスの左下の角のx座標
int Coordinatey_ex;//マスの左下の角のy座標
int boxsize;       //マス一マス分のサイズ
int boxcenter;     //マス一マスの半分のサイズ

//クリックした場所の判断
int ClickX_C(int x){
    int tmpX = 1000 - Coordinatex_ex;
    int arrayx;
    if(x>Coordinatex_ex && x<tmpX) {
        arrayx =(x - Coordinatex_ex) / boxsize;
        return arrayx;
    }
    return -1;
}

int ClickY_C(int y){
    int tmpY = 700 - Coordinatey_ex;
    int arrayy;
    if(y>Coordinatey_ex && y<tmpY) {
            arrayy =(y - Coordinatey_ex) / boxsize;
            return arrayy;
    }
    return -1;
}
