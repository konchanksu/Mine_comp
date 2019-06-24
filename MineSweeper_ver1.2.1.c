#include<stdio.h>
#include<handy.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define BOXSIZE 30
#define BOXSENTER 15

//構造体宣言
struct Mine{
   int mapsizex;  //配列内のサイズ
   int mapsizey;  //配列内のサイズ
   int bombnum;   //爆弾の個数
   int Coordinatex;//マスの左下の角のx座標
   int Coordinatey;//マスの左下の角のy座標
};
struct Mine p;

//プロトタイプ宣言
int Opening(void);
int Explain(void);
int Gamelevel(void);
int RandomBomb(void);
int Flag(int flag, int layer2);
int GameUI(void);
int StOpen(int GAMEMAP[][p.mapsizey]);
int CheckBomb(int GAMEMAP[][p.mapsizey], int bomb);
int CheckBoxes(int GAMEMAP[][p.mapsizey],int arrayx, int arrayy);
int ClickX(int x), ClickY(int y);
int ClickDiscription(int arrayx, int arrayy, int GAMEMAP[][p.mapsizey],int DONTCHANGE[][p.mapsizey],int remain);
int GameOver_No1(int GAMEMAP[][p.mapsizey],int remain);
int GameOver_No2(int remain);
int MineDfs(int x, int y,int GAMEMAP[][p.mapsizey],int remain);
int GameClear(void);
int FlagInstall(int GAMEMAP[][p.mapsizey], int arrayx, int arrayy,int forecastbom);
void dfs(int x, int y,int GAMEMAP[][p.mapsizey]);
int Remainc(int remain, int GAMEMAP[][p.mapsizey]);
int Color(int colornum);
int RemainDiscription(int forecastbom, int layer1);

//メイン関数
int main(){

///////////変数の宣言////////////////////
  int a;  //説明画面に行くかどうか
  int bomb;  //ボム
  int level;  //難易度
  int flag = 0;  //旗の処理
  int i, j;  //ループカウンタ変数
  int x, y;  //クリックした座標
  int arrayx, arrayy;//クリックの座標を配列に直したもの
  int remain;  //クリアまでの残りマス数
  hgevent *event;
  int forecastbom = 0;
  int layer1, layer2;
  int startopen;
  int startx, starty;
////////////////////////////////////////

  srand(time(NULL));

  HgOpen(1000,700);
  HgSetEventMask(HG_MOUSE_DOWN);
  layer1 = HgWAddLayer(0);
  layer2 = HgWAddLayer(0);

  a = Opening();
  if(a==1) Explain();
  level = Gamelevel();

//構造体の初期化
  if(level == 1) p.mapsizex = 9,  p.mapsizey = 9,  p.bombnum = 10,  p.Coordinatex = 365,  p.Coordinatey = 215;
  if(level == 2) p.mapsizex = 16, p.mapsizey = 9,  p.bombnum = 22,  p.Coordinatex = 260,  p.Coordinatey = 215;
  if(level == 3) p.mapsizex = 20, p.mapsizey = 20, p.bombnum = 70,  p.Coordinatex = 200,  p.Coordinatey = 50;
  if(level == 4) p.mapsizex = 30, p.mapsizey = 20, p.bombnum = 110, p.Coordinatex = 50,   p.Coordinatey = 50;

  int GAMEMAP[p.mapsizex][p.mapsizey];
  int DONTCHANGE[p.mapsizex][p.mapsizey];
  int RANDBOM[p.bombnum];

  for(i=0;i<p.bombnum;i++){
    RANDBOM[i] = RandomBomb();
    for(j=0;j<i;j++){
      if(RANDBOM[i] == RANDBOM[j]){
        i--;
      }
    }
  }
  remain = p.mapsizex * p.mapsizey;
  //MAP情報
  for(i=0;i<p.bombnum;i++){
    bomb = RANDBOM[i];
    CheckBomb(GAMEMAP, bomb);
  }
  for(i=0;i<p.mapsizex;i++){
    for(j=0;j<p.mapsizey;j++){
      CheckBoxes(GAMEMAP,i,j);
      DONTCHANGE[i][j] = GAMEMAP[i][j];
    }
  }
  GameUI();
  HgSleep(1.0);
  HgWSetFont(layer1,HG_G,30);
  HgWText(layer1,700,660,"爆弾残り 10");

  startopen = StOpen(GAMEMAP);

  starty = startopen/100;
  startx = startopen - (starty * 100);

  remain = MineDfs(startx, starty, GAMEMAP, remain);
  //ここから動作/////////////////////////
  for(;;){
    //通常モード
    while(flag == 0){
      event = HgEvent();
      x = event->x;
      y = event->y;
      //旗入れ替え
      if(x>10 && x<30){
         if(y>10 && y<70){
           flag = Flag(flag, layer2);
        }
      }
      if(x>p.Coordinatex && x<1000-p.Coordinatex && y>p.Coordinatey && y<700-p.Coordinatey){
        arrayx =ClickX(x);  arrayy =ClickY(y);
        remain = ClickDiscription(arrayx,arrayy,GAMEMAP,DONTCHANGE,remain);
      }
      if(remain>=1000)break;
      if(remain==p.bombnum)break;
    }
    if(remain >= 1000)break;
    if(remain==p.bombnum){GameClear();break;}


    //旗モード
    while(flag == 1){
      event = HgEvent();
      x = event->x;
      y = event->y;
      if(x>p.Coordinatex && x<1000-p.Coordinatex && y>p.Coordinatey && y<700-p.Coordinatey){
        arrayx = ClickX(x);
        arrayy = ClickY(y);
        forecastbom = FlagInstall(GAMEMAP,arrayx,arrayy,forecastbom);
        RemainDiscription(forecastbom,layer1);
      }
      if(x>10 && x<30){
         if(y>10 && y<70){
           flag = Flag(flag, layer2);
        }
      }
    }
    if(flag  == 2)flag = 1;
  }
  HgGetChar();
  HgClose();
  return 0;
}
//関数の定義////////////////////////////////////////////

//オープニング画面
int Opening(void){
//表示画面
  HgSetFillColor(HG_SKYBLUE);
  HgBoxFill(0,0,1000,700,0);

  HgSetFont(HG_G, 100);
  HgText(100,400,"MINE SWEEPER");

  HgSetFillColor(HG_BLUE);
  HgBoxFill(80,50,320,180,1);

  HgSetFont(HG_G,40);
  HgText(120,150,"ルール説明");

  HgSetFillColor(HG_BLUE);
  HgBoxFill(580,50,320,180,1);

  HgSetFont(HG_G,40);
  HgText(605,150,"GAME START");
//ページ移動
  hgevent *event;
  int x, y;
  int a;

  HgSetEventMask(HG_MOUSE_DOWN);

  for(;;){
    event = HgEvent();
    x = event->x;
    y = event->y;

    if(x>80 && x<400 && y>50 && y<230){
      HgSetFillColor(HG_RED);
      HgBoxFill(80,50,320,180,1);

      HgSetFont(HG_G,40);
      HgText(120,150,"ルール説明");

      HgSleep(0.3);
      HgClear();
      a = 1;
      break;
    }
    if(x>580 && x<900 && y>50 && y<230){
      HgSetFillColor(HG_RED);
      HgBoxFill(580,50,320,180,1);

      HgSetFont(HG_G,40);
      HgText(605,150,"GAME START");

      HgSleep(0.3);
      HgClear();
      a = 0;
      break;
    }
  }
  return a;
}

//ルール説明
int Explain(void){
//表示画面
  HgSetFillColor(HG_SKYBLUE);
  HgBoxFill(0,0,1000,700,0);

  HgSetFont(HG_G, 20);

  HgText(430,620,"ルール説明");

  HgText(100,580,"マインスイーパーは爆弾処理のゲームです。全ての爆弾の場所を特定しましょう。");
  HgText(100,550,"明らかにしたい場所をクリックすることでその場所の情報を得ることができます。");
  HgText(440,520,"情報一覧");

  HgText(100,400,":周りに爆弾が一つあります"); HgText(600,400,":周りに爆弾が七つあります");
  HgText(100,360,":周りに爆弾が二つあります"); HgText(600,360,":周りに爆弾が八つあります");
  HgText(100,320,":周りに爆弾が三つあります"); HgText(600,320,":まだ開かれていません");
  HgText(100,280,":周りに爆弾が四つあります"); HgText(600,280,":旗がついています（開きません）");
  HgText(100,240,":周りに爆弾が五つあります"); HgText(600,240,":周りに何もありません（安全です）");
  HgText(100,200,":周りに爆弾が六つあります");

  HgSetFillColor(HG_SKYBLUE);    HgBoxFill(75,400,20,20,1);
  HgSetFillColor(HG_BLUE); HgBoxFill(75,360,20,20,1);
  HgSetFillColor(HG_DBLUE); HgBoxFill(75,320,20,20,1);
  HgSetFillColor(HG_GREEN);  HgBoxFill(75,280,20,20,1);
  HgSetFillColor(HG_YELLOW);HgBoxFill(75,240,20,20,1);
  HgSetFillColor(HG_ORANGE);   HgBoxFill(75,200,20,20,1);
  HgSetFillColor(HG_RED);  HgBoxFill(575,400,20,20,1);
  HgSetFillColor(HG_PURPLE); HgBoxFill(575,360,20,20,1);
  HgSetFillColor(HG_DGRAY);  HgBoxFill(575,320,20,20,1);
  HgSetFillColor(HG_DGRAY);  HgBoxFill(575,280,20,20,1);
  HgSetFillColor(HG_WHITE);  HgCircleFill(585,290,10,1);
  HgSetFillColor(HG_WHITE);  HgBoxFill(575,240,20,20,1);


  HgBoxFill(300,80,380,80,1);
  HgSetFont(HG_G,40);
  HgText(340,100,"GAME START");

//ページ移動
  hgevent *event;
  int x, y;

  HgSetEventMask(HG_MOUSE_DOWN);

  for(;;){
    event = HgEvent();
    x = event->x;
    y = event->y;

    if(x>300 && x<700 && y>80 && y<160){
      HgSetFillColor(HG_RED);
      HgBoxFill(300,80,380,80,1);
      HgSetFont(HG_G,40);
      HgText(340,100,"GAME START");

      HgSleep(0.3);

      break;
    }
  }

  HgClear();
  return 0;
}

//難易度設定
int Gamelevel(void){
//画面表示
  HgSetFillColor(HG_SKYBLUE);
  HgBoxFill(0,0,1000,700,0);

  HgSetFont(HG_G, 40);
  HgText(400,600,"難易度設定");

  HgSetFillColor(HG_WHITE);
  HgBoxFill(100,320,375,180,1);
  HgText(240,430,"EASY");

  HgSetFillColor(HG_GREEN);
  HgBoxFill(525,320,375,180,1);
  HgText(620,430,"NORMAL");

  HgSetFillColor(HG_BLUE);
  HgBoxFill(100,80,375,180,1);
  HgText(240,190,"HARD");

  HgSetFillColor(HG_PURPLE);
  HgBoxFill(525,80,375,180,1);
  HgText(590,190,"VERY HARD");


//ページ移動
  hgevent *event;
  int x, y;
  int level;

  HgSetEventMask(HG_MOUSE_DOWN);


  for(;;){
    event = HgEvent();
    x = event->x;
    y = event->y;

    if(x>100 && x<475 && y>320 && y<500){
      HgSetFillColor(HG_RED);
      HgBoxFill(100,320,375,180,1);
      HgText(240,430,"EASY");

      level = 1;
      HgSleep(0.3);
      HgClear();
      break;
    }

    if(x>100 && x<475 && y>80 && y<260){
      HgSetFillColor(HG_RED);
      HgBoxFill(100,80,375,180,1);
      HgText(240,190,"HARD");

      level = 3;
      HgSleep(0.3);
      HgClear();
      break;
    }

    if(x>525 && x<900 && y>320 && y<500){
      HgSetFillColor(HG_RED);
      HgBoxFill(525,320,375,180,1);
      HgText(620,430,"NORMAL");

      level = 2;
      HgSleep(0.3);
      HgClear();
      break;
    }

    if(x>525 && x<900 && y>80 && y<260){
      HgSetFillColor(HG_RED);
      HgBoxFill(525,80,375,180,1);
      HgText(590,190,"VERY HARD");

      level = 4;
      HgSleep(0.3);
      HgClear();
      break;
    }
  }
  return level;
}

//爆弾をランダムに配置
int RandomBomb(void){
  int tmp;
  int i;
  int numx, numy;

    numx = rand()%p.mapsizex;
    rand();
    rand();
    numy = (rand()%p.mapsizey);
    tmp = numx + (numy)*100;
  return tmp;
}

//旗の処理
int Flag(int flag, int layer2){
  if(flag==0){
    HgLClear(layer2);

    HgWSetFillColor(layer2,HG_BLUE);
    HgWBoxFill(layer2,10,10,20,60,1);

    HgWSetFont(layer2, HG_G, 30);
    HgWText(layer2, 10, 500,"F");

    flag = 2;
  }else  if(flag==1){
    HgLClear(layer2);

    HgWSetFillColor(layer2,HG_ORANGE);
    HgWBoxFill(layer2,10,10,20,60,1);
    flag = 0;
  }
  return flag;
}

//Game外側
int GameUI(void){
  int i,j;

  HgSetFillColor(HG_WHITE);
  HgBoxFill(0,0,1000,700,0);

  HgSetFillColor(HG_GRAY);
  HgBoxFill(p.Coordinatex-20, p.Coordinatey-20, p.mapsizex*BOXSIZE+40, p.mapsizey*BOXSIZE+40 ,1);

  //フラッグ用のボタン
  HgSetFillColor(HG_ORANGE);
  HgBoxFill(10,10,20,60,1);

// グレーの蓋を作る　
  for(i=0;i<p.mapsizex;i++){
    for(j=0;j<p.mapsizey;j++){
        HgSetFillColor(HG_DGRAY);
        HgBoxFill(p.Coordinatex+i*30,p.Coordinatey+j*30,30,30,1);
    }
  }
  return 0;
}

//最初の運ゲーをなくす
int StOpen(int GAMEMAP[][p.mapsizey]){
  int startopen;
  int i, j;
  int BOMBZERO[600];
  int num = 0;
  for(i=0;i<p.mapsizex;i++){
    for(j=0;j<p.mapsizey;j++){
      if(GAMEMAP[i][j] == 0){
        BOMBZERO[num] = j * 100 + i;
        num++;
      }
    }
  }

  num = rand()%num+1;
  startopen = BOMBZERO[num];
  return startopen;
}

//爆弾を配列に取り込む
int CheckBomb(int GAMEMAP[][p.mapsizey], int bomb){
   int arrayx,arrayy;

   arrayy = bomb / 100;
   arrayx = bomb - arrayy * 100;
   GAMEMAP[arrayx][arrayy] = 10;
   return 0;
}

//配列に番号をふる
int CheckBoxes(int GAMEMAP[][p.mapsizey],int arrayx,int arrayy){
  int i,j;
  int TMP[3] = {-1,0,1};
  int tm = 0;

  if(GAMEMAP[arrayx][arrayy] == 10) return 0;

  GAMEMAP[arrayx][arrayy] = 0;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      if(i==1 && j==1) continue;
      else{
        if(arrayx==0 && i==0)continue;
        if(arrayx==p.mapsizex-1 && i==2)continue;
        if(arrayy==0 && j==0)continue;
        if(arrayy==p.mapsizey-1 && j==2)continue;
        if(GAMEMAP[arrayx+TMP[i]][arrayy+TMP[j]] == 10){
           tm++;
        }
      }
    }
  }
  GAMEMAP[arrayx][arrayy] = tm;
  return 0;
}

//クリックした場所の判断
int ClickX(int x){
  int arrayx;
  arrayx =(x - p.Coordinatex) / BOXSIZE; return arrayx;
}
int ClickY(int y){
  int arrayy;
  arrayy =(y - p.Coordinatey) / BOXSIZE; return arrayy;
}

//クリックしたマス目判断
int ClickDiscription(int arrayx, int arrayy,int GAMEMAP[][p.mapsizey],int DONTCHANGE[][p.mapsizey],int remain){
  int tmp;
  int x, y;
  x =  p.Coordinatex + arrayx*BOXSIZE, y = p.Coordinatey + arrayy *BOXSIZE;
  tmp = GAMEMAP[arrayx][arrayy];
  if(tmp == 0) {remain = MineDfs(arrayx,arrayy,GAMEMAP,remain);}
  Color(tmp);
  if(tmp>0 && tmp<9) HgBoxFill(x,y,BOXSIZE,BOXSIZE,1),remain--;
  if(tmp == 0){HgSetFillColor(HG_WHITE); HgBoxFill(x,y,BOXSIZE,BOXSIZE,1);}
  if(tmp == 10)remain = GameOver_No1(DONTCHANGE,remain);
  GAMEMAP[arrayx][arrayy] = -100;
  return remain;
}

//GameOver
int GameOver_No1(int DONTCHANGE[][p.mapsizey],int remain){
  int i, j;
  int x, y;
  int colornum;

  for(j=0;j<p.mapsizey;j++){
    for(i=0;i<p.mapsizex;i++){
      x = p.Coordinatex + i*BOXSIZE, y = p.Coordinatey + j*BOXSIZE;
      colornum = DONTCHANGE[i][j];
      Color(colornum);

      if(DONTCHANGE[i][j] ==10) HgSetFillColor(HG_WHITE);
      if(DONTCHANGE[i][j] >= 0) HgBoxFill(x,y,BOXSIZE,BOXSIZE,1);
      if(DONTCHANGE[i][j] >  9) HgSetFillColor(HG_BLACK),HgCircleFill(x+BOXSENTER,y+BOXSENTER,BOXSENTER,1);
    }
  }
  remain = GameOver_No2(remain);
  return remain;
}
int GameOver_No2(int remain){
  HgGetChar();
  HgSetFillColor(HG_WHITE);
  HgBoxFill(30,275,960,150,1);
  HgSetFont(HG_G,100);
  HgText(120,300," GAME  OVER!");

  remain = 1001;

  return remain;
}

//GameClear
int GameClear(void){
  HgSetFillColor(HG_WHITE);
  HgBoxFill(30,275,960,150,1);
  HgSetFont(HG_G,100);
  HgText(120,300,"GAME  CLEAR!!");

  return 0;
}

//旗の設定
int FlagInstall(int GAMEMAP[][p.mapsizey],int arrayx,int arrayy,int forecastbom){
  int wherex, wherey;
  int tmp = GAMEMAP[arrayx][arrayy];
  wherex =  p.Coordinatex + arrayx*BOXSIZE, wherey = p.Coordinatey + arrayy *BOXSIZE;

  if(tmp<0 && tmp>-19){
    GAMEMAP[arrayx][arrayy] = -tmp;
    HgSetFillColor(HG_DGRAY);
    HgBoxFill(wherex,wherey,BOXSIZE,BOXSIZE,1);
    forecastbom--;
  }else if(tmp==-20){
    GAMEMAP[arrayx][arrayy]=0;
    HgSetFillColor(HG_DGRAY);
    HgBoxFill(wherex,wherey,BOXSIZE,BOXSIZE,1);
    forecastbom--;
  }else if(tmp==0){
    GAMEMAP[arrayx][arrayy] = -20;
    HgSetFillColor(HG_WHITE);
    HgCircleFill(wherex+BOXSENTER,wherey+BOXSENTER,BOXSENTER,0);
    forecastbom++;
  }else if(tmp>0){
    GAMEMAP[arrayx][arrayy] = -tmp;
    HgSetFillColor(HG_WHITE);
    HgCircleFill(wherex+BOXSENTER,wherey+BOXSENTER,BOXSENTER,0);
    forecastbom++;
  }
  return forecastbom;
}

//深さ優先探索
int MineDfs(int x,int y,int GAMEMAP[][p.mapsizey],int remain){
  dfs(x,y,GAMEMAP);
  remain = Remainc(remain,GAMEMAP);
  return remain;
}
void dfs(int x,int y,int GAMEMAP[][p.mapsizey]){
  int TMP[3] = {-1, 0, 1};
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
       int nx = x + TMP[j],ny = y + TMP[i];
       if(GAMEMAP[nx][ny]==0){
         if(nx >= p.mapsizex || nx < 0)continue;
         if(ny >= p.mapsizey || ny < 0)continue;
         else{
           int xx =  p.Coordinatex + nx*BOXSIZE, yy = p.Coordinatey + ny*BOXSIZE;
           HgSetFillColor(HG_WHITE);
           HgBoxFill(xx,yy,BOXSIZE,BOXSIZE,1);
           GAMEMAP[nx][ny] = -100;
           dfs(nx,ny,GAMEMAP);
         }
       }
       else if(GAMEMAP[nx][ny]>0 && GAMEMAP[nx][ny]<10){
         if(nx >= p.mapsizex || nx < 0)continue;
         if(ny >= p.mapsizey || ny < 0)continue;
         else{
           int xx =  p.Coordinatex + nx*BOXSIZE, yy = p.Coordinatey + ny*BOXSIZE;
           int tmp = GAMEMAP[nx][ny];
           Color(tmp);

           HgBoxFill(xx,yy,BOXSIZE,BOXSIZE,1);
           GAMEMAP[nx][ny] = -100;
         }
       }
     }
   }
}
int Remainc(int remain, int GAMEMAP[][p.mapsizey]){
  int rem = p.mapsizex*p.mapsizey;
  for(int i=0;i<p.mapsizex;i++){
    for(int j=0;j<p.mapsizey;j++){
      if(GAMEMAP[i][j]==-100){
        rem--;
      }
    }
  }
  remain = rem;
  return remain;
}

//色を設定する
int Color(int colornum){
  if(colornum == 0)HgSetFillColor(HG_WHITE);
  if(colornum == 1) HgSetFillColor(HG_SKYBLUE);
  if(colornum == 2) HgSetFillColor(HG_BLUE);
  if(colornum == 3) HgSetFillColor(HG_DBLUE);
  if(colornum == 4) HgSetFillColor(HG_GREEN);
  if(colornum == 5) HgSetFillColor(HG_YELLOW);
  if(colornum == 6) HgSetFillColor(HG_ORANGE);
  if(colornum == 7) HgSetFillColor(HG_RED);
  if(colornum == 8) HgSetFillColor(HG_PURPLE);
  return 0;
}

int RemainDiscription(int forecastbom, int layer1){
  int num = p.bombnum - forecastbom;
  HgLClear(layer1);
  HgWSetFont(layer1,HG_G,30);
  HgWText(layer1,700,660,"爆弾残り %d",num);
  return 0;
}
