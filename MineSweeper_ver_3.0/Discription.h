#include <stdio.h>
#include <handy.h>

int mapsizex_ex;   //配列内のサイズ
int mapsizey_ex;   //配列内のサイズ
int bombnum_ex;    //爆弾の個数
int Coordinatex_ex;//マスの左下の角のx座標
int Coordinatey_ex;//マスの左下の角のy座標
int boxsize;       //マス一マス分のサイズ
int boxcenter;     //マス一マスの半分のサイズ

int Color_D(int colornum);
int GameOver_No2_D(int remain);
int GameOver_No1_D(int DONOTCHANGE[][mapsizey_ex], int remain);
int GameClear_D(void);
int dfs_D(int arrayx, int arrayy, int MAP[][mapsizey_ex], int remain);
int ForecastbomR_D(int forecastbom, int MAP[][mapsizey_ex]);

//オープニング画面
int Opening_D(int x, int y, hgevent *event){
    int a = 0;

    HgSetFillColor(HG_SKYBLUE);
    HgBoxFill(0, 0, 1000, 700, 0);

    HgSetFont(HG_G, 100);
    HgText(100, 400, "MINE SWEEPER");

    HgSetFillColor(HG_BLUE);
    HgBoxFill(80, 50, 320, 180, 1);

    HgSetFont(HG_G,40);
    HgText(120, 150, "ルール説明");

    HgSetFillColor(HG_BLUE);
    HgBoxFill(580, 50, 320, 180, 1);

    HgSetFont(HG_G,40);
    HgText(605, 150, "GAME START");

    for(;;) {
        event = HgEvent();
        x = event->x;
        y = event->y;
        if(x > 80 && x < 400 && y > 50 && y < 230) {
            HgSetFillColor(HG_RED);
            HgBoxFill(80, 50, 320, 180, 1);

            HgSetFont(HG_G,40);
            HgText(120, 150, "ルール説明");

            HgSleep(0.3);
            HgClear();
            a = 1;
            break;
        }
        if(x > 580 && x < 900 && y > 50 && y < 230) {
            HgSetFillColor(HG_RED);
            HgBoxFill(580, 50, 320, 180, 1);

            HgSetFont(HG_G, 40);
            HgText(605, 150, "GAME START");

            HgSleep(0.3);
            HgClear();
            break;
        }
    }
    return a;
}

//説明 難易度設定画面
int Explain_D(int x, int y, hgevent *event, int a){
    int i, level;
    if(a == 1) {
        //表示画面
        HgSetFillColor(HG_SKYBLUE);
        HgBoxFill(0, 0, 1000, 700, 0);

        HgSetFont(HG_G, 20);

        HgText(430, 620, "ルール説明");

        HgText(100, 580, "マインスイーパーは爆弾処理のゲームです。全ての爆弾の場所を特定しましょう。");
        HgText(100, 550, "明らかにしたい場所をクリックすることでその場所の情報を得ることができます。");
        HgText(440, 520, "情報一覧");

        for(i = 0; i < 6; i++) {
            HgText(100, 400 - i * 40, ":周りに爆弾が%dつあります", i + 1);
        }

        HgText(600, 400, ":周りに爆弾が七つあります");
        HgText(600, 360, ":周りに爆弾が八つあります");
        HgText(600, 320, ":まだ開かれていません");
        HgText(600, 280, ":旗がついています（開きません）");
        HgText(600, 240, ":周りに何もありません（安全です）");


        HgSetFillColor(HG_CYAN);    HgBoxFill(75, 400, 20, 20, 1);
        HgSetFillColor(HG_SKYBLUE); HgBoxFill(75, 360, 20, 20, 1);
        HgSetFillColor(HG_BLUE);    HgBoxFill(75, 320, 20, 20, 1);
        HgSetFillColor(HG_GREEN);   HgBoxFill(75, 280, 20, 20, 1);
        HgSetFillColor(HG_YELLOW);  HgBoxFill(75, 240, 20, 20, 1);
        HgSetFillColor(HG_ORANGE);  HgBoxFill(75, 200, 20, 20, 1);
        HgSetFillColor(HG_RED);     HgBoxFill(575, 400, 20, 20, 1);
        HgSetFillColor(HG_PURPLE);  HgBoxFill(575, 360, 20, 20, 1);
        HgSetFillColor(HG_DGRAY);   HgBoxFill(575, 320, 20, 20, 1);
        HgSetFillColor(HG_DGRAY);   HgBoxFill(575, 280, 20, 20, 1);
        HgSetFillColor(HG_WHITE);   HgCircleFill(585, 290, 10, 1);
        HgSetFillColor(HG_WHITE);   HgBoxFill(575, 240, 20, 20, 1);

        HgBoxFill(300, 80, 380, 80, 1);
        HgSetFont(HG_G, 40);
        HgText(340, 100, "GAME START");

        for(;;) {
            event = HgEvent();
            x = event->x;
            y = event->y;

            if(x > 300 && x < 700 && y > 80 && y < 160) {
                HgSetFillColor(HG_RED);
                HgBoxFill(300, 80, 380, 80, 1);
                HgSetFont(HG_G, 40);
                HgText(340, 100, "GAME START");
                HgSleep(0.3);
                HgClear();
            break;
            }
        }
    }

    HgSetFillColor(HG_SKYBLUE);
    HgBoxFill(0, 0, 1000, 700, 0);

    HgSetFont(HG_G, 40);
    HgText(400, 600, "難易度設定");

    HgSetFillColor(HG_WHITE);
    HgBoxFill(100, 320, 375, 180, 1);
    HgText(240, 430, "EASY");

    HgSetFillColor(HG_GREEN);
    HgBoxFill(525, 320, 375, 180, 1);
    HgText(620, 430, "NORMAL");

    HgSetFillColor(HG_BLUE);
    HgBoxFill(100, 80, 375, 180, 1);
    HgText(240, 190, "HARD");

    HgSetFillColor(HG_PURPLE);
    HgBoxFill(525, 80, 375, 180, 1);
    HgText(590, 190, "VERY HARD");

    for(;;) {
        event = HgEvent();
        x = event->x;
        y = event->y;

        if(x > 100 && x < 475 && y > 320 && y < 500) {
            HgSetFillColor(HG_RED);
            HgBoxFill(100, 320, 375, 180, 1);
            HgText(240, 430, "EASY");

            level = 1;
            HgSleep(0.3);
            HgClear();
            break;
        }

        if(x>100 && x<475 && y>80 && y<260) {
            HgSetFillColor(HG_RED);
            HgBoxFill(100,80,375,180,1);
            HgText(240,190,"HARD");

            level = 3;
            HgSleep(0.3);
            HgClear();
            break;
        }

        if(x>525 && x<900 && y>320 && y<500) {
            HgSetFillColor(HG_RED);
            HgBoxFill(525,320,375,180,1);
            HgText(620,430,"NORMAL");

            level = 2;
            HgSleep(0.3);
            HgClear();
            break;
        }

        if(x>525 && x<900 && y>80 && y<260) {
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

int DontChange(int MAP[][mapsizey_ex], int DONOTCHANGE[mapsizex_ex][mapsizey_ex]){
    int i, j;
    for(i=0; i<mapsizex_ex; i++) {
        for(j=0; j<mapsizey_ex; j++) {
            DONOTCHANGE[i][j] = MAP[i][j];
        }
    }
    return 0;
}

//Game外側
int GameUI_D(void){
    int i,j;

    HgSetFillColor(HG_WHITE);
    HgBoxFill(0,0,1000,700,0);

    HgSetFillColor(HG_GRAY);
    HgBoxFill(Coordinatex_ex-20, Coordinatey_ex-20, mapsizex_ex*boxsize+40, mapsizey_ex*boxsize+40,1);

    //フラッグ用のボタン
    HgSetFillColor(HG_ORANGE);
    HgBoxFill(10,10,20,60,1);

    // グレーの蓋を作る　
    for(i=0; i<mapsizex_ex; i++) {
        for(j=0; j<mapsizey_ex; j++) {
            HgSetFillColor(HG_DGRAY);
            HgBoxFill(Coordinatex_ex+i*30,Coordinatey_ex+j*30,30,30,1);
        }
    }
    return 0;
}

//旗の処理
int Flag_D(int flag, int layer2, int x, int y){
    if(x>10 && x<30) {
        if(y>10 && y<70) {
            if(flag==0) {
                HgLClear(layer2);

                HgWSetFillColor(layer2,HG_BLUE);
                HgWBoxFill(layer2,10,10,20,60,1);

                HgWSetFont(layer2, HG_G, 30);
                HgWText(layer2, 10, 500,"F");

                flag = 1;
            }else if(flag==1) {
                HgLClear(layer2);

                HgWSetFillColor(layer2,HG_ORANGE);
                HgWBoxFill(layer2,10,10,20,60,1);
                flag = 0;
            }
        }
    }
    return flag;
}

int Forecastbom_D(int forecastbom, int layer1){
    HgLClear(layer1);
    HgWSetFont(layer1,HG_G,25);
    HgWText(layer1,700,670,"爆弾残り %d",forecastbom);

    return 0;
}

int ClickDiscription_D(int arrayx, int arrayy, int MAP[][mapsizey_ex], int DONOTCHANGE[][mapsizey_ex], int remain){
    int tmp;
    int wherex, wherey;
    wherex =  Coordinatex_ex + arrayx*boxsize, wherey = Coordinatey_ex + arrayy *boxsize;
    tmp = MAP[arrayx][arrayy];

    Color_D(tmp);
    if(arrayx != -1 && arrayy != -1) {
        if(tmp == 0) {
            MAP[arrayx][arrayy] = -100;
            HgSetFillColor(HG_WHITE);
            HgBoxFill(wherex, wherey, boxsize, boxsize, 1);
            remain--;
            remain = dfs_D(arrayx, arrayy, MAP, remain);
        }
        if(tmp > 0 && tmp <9) {
            HgBoxFill(wherex, wherey, boxsize, boxsize, 1);
            HgText(wherex + 8, wherey + 5, "%d", MAP[arrayx][arrayy]);
            remain--;
        }

        if(tmp == 10) remain = GameOver_No1_D(DONOTCHANGE, remain);
        if(tmp < 0) {return remain;}

        MAP[arrayx][arrayy] = -100;
        if(remain == 0) {
            GameClear_D();
            remain = 1001;
        }
    }
    return remain;
}

//GameOver
int GameOver_No1_D(int DONOTCHANGE[][mapsizey_ex], int remain){
    int i, j;
    int wherex, wherey;
    int colornum;

    for(i = 0; i < mapsizex_ex; i++) {
        for(j = 0; j < mapsizey_ex; j++) {
            wherex = Coordinatex_ex + i * boxsize, wherey = Coordinatey_ex + j * boxsize;
            colornum = DONOTCHANGE[i][j];
            Color_D(colornum);

            if(DONOTCHANGE[i][j] == 10) HgSetFillColor(HG_WHITE);
            if(DONOTCHANGE[i][j] > 0 && DONOTCHANGE[i][j] < 9) {
                HgBoxFill(wherex, wherey, boxsize, boxsize, 1);
                HgText(wherex + 8,wherey + 5, "%d", DONOTCHANGE[i][j]);
            }
            if(DONOTCHANGE[i][j] == 0) {
                HgBoxFill(wherex, wherey, boxsize, boxsize, 1);
            }
            if(DONOTCHANGE[i][j] > 9) {
                HgSetFillColor(HG_WHITE);
                HgBoxFill(wherex, wherey, boxsize, boxsize, 1);
                HgSetFillColor(HG_BLACK);
                HgCircleFill(wherex + boxcenter, wherey + boxcenter, boxcenter, 1);
            }
        }
    }
    remain = GameOver_No2_D(remain);
    return remain;
}

int GameOver_No2_D(int remain){
        HgSetFillColor(HG_WHITE);
        HgBoxFill(30, 275, 960, 150, 1);
        HgSetFont(HG_G, 100);
        HgText(120, 300, " GAME  OVER!");

        remain = 1001;

        return remain;
}

//GameClear
int GameClear_D(void){
        HgSetFillColor(HG_WHITE);
        HgBoxFill(30, 275, 960, 150, 1);
        HgSetFont(HG_G, 100);
        HgText(120, 300, "GAME  CLEAR!!");

        return 0;
}

int dfs_D(int arrayx, int arrayy, int MAP[][mapsizey_ex], int remain){
    int TMP[3] = {-1, 0, 1};
    int nx, ny;
    int wherex, wherey;
    int i, j;

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(TMP[i] == 0 && TMP[j] == 0) continue;
            nx = arrayx + TMP[i], ny = arrayy + TMP[j];
            wherex = Coordinatex_ex + nx * boxsize, wherey = Coordinatey_ex + ny * boxsize;

            if(nx < 0 || nx >= mapsizex_ex) continue;
            if(ny < 0 || ny >= mapsizey_ex) continue;

            if(MAP[nx][ny] < 0 && MAP[nx][ny] > -30) {
                if(MAP[nx][ny] == -20) MAP[nx][ny] = 0;
                else MAP[nx][ny]= -MAP[nx][ny];
            }

            else if(MAP[nx][ny] == 0) {
                remain--;
                HgSetFillColor(HG_WHITE);
                HgBoxFill(wherex, wherey, boxsize, boxsize, 1);

                MAP[nx][ny] = -100;
                remain = dfs_D(nx, ny, MAP, remain);
            }

            else if(MAP[nx][ny] >= 1) {
                int tmp = MAP[nx][ny];

                Color_D(tmp);
                HgBoxFill(wherex, wherey, boxsize, boxsize, 1);
                HgText(wherex + 8, wherey + 5, "%d", MAP[nx][ny]);
                MAP[nx][ny] = -100;

                remain--;
            }
        }
    }
    return remain;
}


int Color_D(int colornum){
    if(colornum == 0) HgSetFillColor(HG_WHITE);
    if(colornum == 1) HgSetFillColor(HG_CYAN);
    if(colornum == 2) HgSetFillColor(HG_SKYBLUE);
    if(colornum == 3) HgSetFillColor(HG_BLUE);
    if(colornum == 4) HgSetFillColor(HG_GREEN);
    if(colornum == 5) HgSetFillColor(HG_YELLOW);
    if(colornum == 6) HgSetFillColor(HG_ORANGE);
    if(colornum == 7) HgSetFillColor(HG_RED);
    if(colornum == 8) HgSetFillColor(HG_PURPLE);
    return 0;
}

int ForecastbomR_D(int forecastbom, int MAP[][mapsizey_ex]){
    int num = 0;
    int i, j;
    for(i = 0; i < mapsizex_ex; i++) {
        for(j = 0; j < mapsizey_ex; j++) {
            if(MAP[i][j] < 0 && MAP[i][j] > -30) num++;
        }
    }

    forecastbom = bombnum_ex - num;
    return forecastbom;
}
