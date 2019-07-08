#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int mapsizex_ex;   //配列内のサイズ
int mapsizey_ex;   //配列内のサイズ
int bombnum_ex;    //爆弾の個数
int Coordinatex_ex;//マスの左下の角のx座標
int Coordinatey_ex;//マスの左下の角のy座標
int boxsize;       //マス一マス分のサイズ
int boxcenter;     //マス一マスの半分のサイズ

//プロトタイプ宣言
int RandomBombin_A(void);
int CheckBoxes_A(int MAP[][mapsizey_ex],int arrayx,int arrayy);

//レベルごとの詳細設定
int LevelDecide_A(level){
  if(level == 1) {mapsizex_ex = 9,mapsizey_ex = 9,bombnum_ex = 10,
                  Coordinatex_ex = 365,Coordinatey_ex = 215;}
  if(level == 2) {mapsizex_ex = 16,mapsizey_ex = 9,bombnum_ex = 22,
                  Coordinatex_ex = 260,Coordinatey_ex = 215;}
  if(level == 3) {mapsizex_ex = 20,mapsizey_ex = 20,bombnum_ex = 70,
                  Coordinatex_ex = 200,Coordinatey_ex = 50;}
  if(level == 4) {mapsizex_ex = 30, mapsizey_ex = 20,bombnum_ex = 110,
                  Coordinatex_ex = 50,Coordinatey_ex = 50;}
  return 0;
}

//爆弾をランダムに振り分ける
int RandomBomb_A(int MAP[mapsizex_ex][mapsizey_ex]){
  int i,j;
  int RandBom[bombnum_ex];
  int arrayx, arrayy;
  //爆弾を作成(一次元配列で爆弾の配列の番号をランダムに)
  for(i=0;i<bombnum_ex;i++){
    RandBom[i] = RandomBombin_A();
    for(j=0;j<i;j++){
      //前に振り分けたものと被りがあれば、振り分けなおす
      if(RandBom[i] == RandBom[j]){
        i--;
      }
    }
  }

  //一次元配列で作った情報を二次元配列に分解する
  for(i=0;i<bombnum_ex;i++){
    arrayy = RandBom[i]/100;
    arrayx = RandBom[i] - arrayy*100;

    MAP[arrayx][arrayy] = 10;
  }


  for(i=0;i<mapsizex_ex;i++){
    for(j=0;j<mapsizey_ex;j++){
      CheckBoxes_A(MAP, i,j);
    }
  }
  return 0;
}

//爆弾をランダムに振り分ける関数
int RandomBombin_A(void){
  int tmp;
  int numx, numy;

  numx = rand()%mapsizex_ex;
  rand();
  rand();
  numy = (rand()%mapsizey_ex);
  tmp = numx + (numy)*100;

  return tmp;
}

//爆弾の入っていないます
int CheckBoxes_A(int MAP[][mapsizey_ex],int arrayx, int arrayy){
  int i,j;
  int TMP[3] = {-1,0,1};
  int tm = 0;

  if(MAP[arrayx][arrayy] == 10) return 0;

  MAP[arrayx][arrayy] = 0;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      if(i==1 && j==1) continue;
      else{
        if(arrayx==0 && i==0)continue;
        if(arrayx==mapsizex_ex-1 && i==2)continue;
        if(arrayy==0 && j==0)continue;
        if(arrayy==mapsizey_ex-1 && j==2)continue;
        if(MAP[arrayx+TMP[i]][arrayy+TMP[j]] == 10){
           tm++;
        }
      }
    }
  }
  MAP[arrayx][arrayy] = tm;
  return 0;
}

//旗の設定
int FlagInstall_A(int MAP[][mapsizey_ex],int arrayx,int arrayy,int forecastbom){
  int wherex, wherey;
  int tmp = MAP[arrayx][arrayy];
  wherex =  Coordinatex_ex + arrayx*boxsize, wherey = Coordinatey_ex + arrayy * boxsize;

  if(arrayx != -1 && arrayy != -1){
    if(tmp<0 && tmp>-19){
      MAP[arrayx][arrayy] = -tmp;
      HgSetFillColor(HG_DGRAY);
      HgBoxFill(wherex,wherey,boxsize,boxsize,1);
      forecastbom++;
    }else if(tmp==-20){
      MAP[arrayx][arrayy]=0;
      HgSetFillColor(HG_DGRAY);
      HgBoxFill(wherex,wherey,boxsize,boxsize,1);
      forecastbom++;
    }else if(tmp==0){
      MAP[arrayx][arrayy] = -20;
      HgSetFillColor(HG_WHITE);
      HgCircleFill(wherex+boxcenter,wherey+boxcenter,boxcenter,0);
      forecastbom--;
    }else if(tmp>0){
      MAP[arrayx][arrayy] = -tmp;
      HgSetFillColor(HG_WHITE);
      HgCircleFill(wherex+boxcenter,wherey+boxcenter,boxcenter,0);
      forecastbom--;
    }
  }
  return forecastbom;
}

int FlagNum_A(int MAP[][mapsizey_ex],int layer1){
  int i,j;
  int forecastbom = bombnum_ex;
  for(i=0;i<mapsizex_ex;i++){
    for(j=0;j<mapsizey_ex;j++){
      if(MAP[i][j]<0 && MAP[i][j]>=-20){
        forecastbom--;
      }
    }
  }
  HgLClear(layer1);
  HgWSetFont(layer1,HG_G,25);
  HgWText(layer1,700,670,"爆弾残り %d",forecastbom);

  return forecastbom;
}


//最初の運ゲーをなくす
int StOpen_A(int MAP[][mapsizey_ex]){
  int startopen;
  int i, j;
  int BOMBZERO[600];
  int num = 0;
  for(i=0;i<mapsizex_ex;i++){
    for(j=0;j<mapsizey_ex;j++){
      if(MAP[i][j] == 0){
        BOMBZERO[num] = j * 100 + i;
        num++;
      }
    }
  }
  if(num == 0){
    return -1;
  }

  num = rand()%num+1;
  startopen = BOMBZERO[num];
  return startopen;
}
