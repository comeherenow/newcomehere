#include <stdio.h>
#include <termio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/* 게임 시작을 위한 기본적인 함수 */
void start(); // 게임 시작해서 이름 받는 함수
void stage();  //map.txt 파일에서 맵을 불러와 배열에 저장하는 함수
void whereisplayer();  // 맵을 출력하기 전 창고지기의 위치를 찾는 함수
void print_stage(int input);  // 맵 배열을 출력하는 함수
int getch();  //옵션 값 입력받는 함수
void move();  // 방향키 움직일 때 창고지기의 좌표 정하는 함수
int nstage_check();  // 맵이 clear 되었는지 확인하는 함수

void save();
void clean(int num);
void undo();
void movesave();
void save_stage(int);
void load_stage();
void print_load();
void how_long_you_play();
void show_me_display();
void check_time();

int undocount=0; //undo횟수

char name[11];  // 이름 입력받는 배열

int stage_num = -1;  // 맵 고유번호
int stage_num_save = -1;

int house_num[5][1];  // 창고 위치 저장 배열
int house_num_save[5][1];

int box_num[5][1];  // 상자의 위치 저장 배열
int box_num_save[5][1];

char map[5][30][30];  // 맵 저장 배열
char mapforsave[5][30][30];

char house[5][30][30]; // 보관장소
char houseforsave[5][30][30];

char box[5][30][30]; // 상자
char boxforsave[5][30][30];

int playerx,playery;  // 창고지기 위치 저장 변수
char input_char;  // 옵션키 입력받는 변수

char mvsave[5][30][30]; //undo 하기전 움직임 저장

char base_map[5][30][30]; //초기 맵상태 저장
char base_box[5][30][30];
char base_house[5][30][30];

double gap=0, exitgap=0; // 시간 측정 변수

clock_t startgame=0,endgame=0,stop=0,stopend=0;

/**********************MAIN*************************/
int main(){

int size=0;

int eachrank[size];
float times[5][eachrank[size]+1];


  start();
  stage();
  save(1);

  if (box_num[stage_num][0]!=house_num[stage_num][0]){
    printf("맵 오류 : 박스 개수와 보관장소 개수 불일치\n프로그램을 종료합니다.");
    return 0;
  }
  print_stage(stage_num=0);
  startgame=clock();
  movesave();
  whereisplayer();
  while(1)
  {
    input_char=getch();

    if(input_char == 'h'||input_char == 'j'||input_char == 'k'||input_char == 'l'){
      movesave();
      move();
      if (nstage_check()==0)
        break;

    }
    if(input_char == 's'){
      save_stage(stage_num);
      printf("저장완료");
      printf("\n(Command)  %c\n", input_char);
      continue;
    }
    if(input_char == 'e'){
      endgame=clock();
      how_long_you_play();
      save_stage(stage_num);
      system("clear");
      printf("\n");
      printf("S E E    Y O U    %s\n",name );
      printf("\n");
      printf("\n(Command)  %c\n", input_char);
      printf("\n");
      return 0;
    }
    if(input_char == 'f'){
      system("clear");
      load_stage();
      print_load();
      continue;
    }
    if(input_char == 'u'){
      printf("\n(Command)  %c\n", input_char);
      undo();
    }
    if(input_char == 'n'){
        printf("\n(Command)  %c\n", input_char);
        clean(2);
        save(2);
        whereisplayer();
        print_stage(stage_num=0);
    }
    if(input_char == 'r'){
        printf("\n(Command)  %c\n", input_char);
        clean(2);
        save(2);
        whereisplayer();
        print_stage(stage_num);
    }
    if(input_char == 'd'){
      printf("\n(Command)  %c\n", input_char);
      show_me_display();
      print_stage(stage_num);
      continue;
    }
    if(input_char == 't'){
      check_time();
      printf("\n(Command)  %c\n", input_char);
      print_stage(stage_num);
      continue;
    }
  }

  size++;
  return 0;
}
/*******************start*******************/

void start(){
  int i=0;
  printf("Start...\nInput name : ");
  scanf("%s",name);

  if(strlen(name)>10){
    printf("10자 이상 입력할 수 업습니다.\n");
    exit(1);
  }
  else {
    while (name[i]!='\0'){
      if((('a'<=name[i])&&(name[i]<='z'))||(('A'<=name[i])&&(name[i]<='Z')));

      else {
        printf("한글은 입력할 수 없습니다.\n");
        exit(1);
      }
    i++;
   }
 }
}

/****************맵읽기(stage)****************/

void stage(){
  int x = 0, y = 0;
  char ch;

  FILE *fp;

  fp=fopen("map.txt","r");
  while(fscanf(fp,"%c",&ch) != EOF){
    if (ch=='m'){ //m이 입력되었을 때 stage_num 증가시켜 맵 번호 할당
      stage_num++;
      x=0;
      y=-1;
      continue;
    }
    if((ch=='a') || (ch == 'p')){
      continue;
    }
    if (ch=='\n'){
      y++;
      x=0;
      continue;
    }
    if(ch=='e'){
      break;
    }
    if(ch=='O'){
      house[stage_num][y][x]=ch;
      house_num[stage_num][0]++;
    }
    if(ch=='$'){
      box[stage_num][y][x]=ch;
      box_num[stage_num][0]++;
    }
    map[stage_num][y][x] = ch;
    x++;
  }
  fclose(fp);
}
/*****************맵출력(print_stage)******************/

void print_stage(int stage_num){
  system("clear");
  printf("Hello %s", name);

  printf("\n\n"); //HELLO NAME 출력


  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",map[stage_num][a][b]);
    }
    printf("\n");
  }
}

/***************whereisplayer*************************/

void whereisplayer(){  //@ 위치 찾기
  char ch;
  int a,b;
  for(b=0;b<30;b++){
    for(a=0;a<30;a++){
      if(map[stage_num][b][a]=='@'){
        playerx=a;
        playery=b;
        break;
      }
    }
  }
}

/*******************move*******************************/
void move(){
  int dx=0, dy=0;
  switch (input_char){
    case 'h':
      dx = -1;
      dy = 0;
      break;
    case 'j':
      dx = 0;
      dy = 1;
      break;
    case 'k':
      dx = 0;
      dy = -1;
      break;
    case 'l':
      dx = 1;
      dy = 0;
      break;
  }
  if (map[stage_num][playery+dy][playerx+dx]!='#'){
    map[stage_num][playery][playerx]=' ';
    if (map[stage_num][playery+dy][playerx+dx]=='$'){
      if (map[stage_num][playery+2*dy][playerx+2*dx]=='#'||map[stage_num][playery+2*dy][playerx+2*dx]=='$'){ //2-1
        return;
      }
      map[stage_num][playery+2*dy][playerx+2*dx]='$';
    }
    if (house[stage_num][playery][playerx]=='O')
      map[stage_num][playery][playerx]='O';
    map[stage_num][playery+dy][playerx+dx]='@';

    playery+=dy;
    playerx+=dx;
    print_stage(stage_num);
  }
}

/******************nstage_check*********************/

int nstage_check(){
  int ok=0;

  for(int i=0;i<30;i++){
    for(int j=0;j<30;j++){
      if (house[stage_num][i][j]=='O'){
        if(map[stage_num][i][j]=='$'){
          ok++;
        }
      }
    }
  }
  if (ok==house_num[stage_num][0]){
    endgame=clock();
    how_long_you_play();
    startgame=0;
    endgame=0;
    system("clear");
    printf("클리어 (짝짝짝) \n");
    sleep(2);
    stage_num++;
    whereisplayer();
    clean(1);
    movesave();
    if (stage_num<5)
      {print_stage(stage_num);
      startgame=clock();}
    else
      return 0;
  }
  return 1;
}

/*****************movesave*****************************/

void movesave()
{
            for(int a=3; a>=0; a--){
                for(int b=0; b<30; b++){
                    for(int c=0; c<30; c++){
                        mvsave[a+1][b][c]=mvsave[a][b][c]; //undo를 사용하기위해 움직임저장
                    }
                }
            }
            for(int b=0; b<30; b++){
                for(int c=0; c<30; c++){
                    mvsave[0][b][c]=map[stage_num][b][c];
                }
            }
}

/********************undo*************************/
void undo()
{
    if(undocount <5)
    {
        system("clear");
        printf("Hello %s", name);

        printf("\n\n"); //HELLO NAME 출력

  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",mvsave[0][a][b]);
    }
    printf("\n");
    }

    for(int a=0; a<30; a++){
        for(int b=0; b<30; b++){
            map[stage_num][a][b]=mvsave[0][a][b]; //undo누르기 전으로 맵을 초기화
        }
    }

    whereisplayer();

    for(int a=0; a<4; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                mvsave[a][b][c]=mvsave[a+1][b][c]; //undo를 사용하고 배열을 한칸씩 옮긴다
            }
        }
    }
undocount++;
}
}

/********************배열초기화***********************/
void clean(int num)
{
    switch(num){

    case 1:
    for(int a=0; a<5; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                mvsave[a][b][c]='\0'; //배열에 움직임을 저장하기 위해 초기화
            }
        }
    }
    break;

    case 2:
    for(int a=0; a<5; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){ //맵 배열에 새로 불러오기 위해 초기화
                mvsave[a][b][c]='\0';
                map[a][b][c]='\0';
                box[a][b][c]='\0';
                house[a][b][c]='\0';
                undocount=0;
            }
        }
    }
    break;

}
}
/*******************초기 맵상태저장(save)**************************/
void save(int num)
{
    switch (num) {

        case 1:
    for(int a=0; a<5; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                base_map[a][b][c]=map[a][b][c]; //맵을 다시시작할때 초기맵 저장
                base_box[a][b][c]=box[a][b][c];
                base_house[a][b][c]=house[a][b][c];
            }
        }
    }

        case 2:
    for(int a=0; a<5; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                map[a][b][c]=base_map[a][b][c]; //다시하기시 초기맵상태를 불러옴
                house[a][b][c]=base_house[a][b][c];
                box[a][b][c]=base_box[a][b][c];
            }
        }
    }
}
}

/****************현재 맵상태저장(save_stage)******************/

void save_stage(int stage_num) // 현재 맵 상태를 sokoban.txt에 저장
{
  int a, b;
  FILE *save;

  save=fopen("sokoban.txt","wt");

  for (a=0; a<30; a++)
  {
    fprintf(save,"\n");
    for (b=0; b<30; b++)
    {
      fprintf(save,"%c",map[stage_num][a][b]);
    }
  }

  fclose(save); // 메모리 낭비를 방지하기위해 파일을 닫아줌
}

/***************맵 불러오기*******************/
void load_stage() // sokoban.txt파일에 담긴 내용을 "r(read)"모드로 열어 읽음
{
  int x = 0, y = 0;
  char ch;

  FILE *load;

  load = fopen("sokoban.txt","r");
  while(fscanf(load,"%c",&ch) != EOF)
  {
    if (ch=='\n')
    {
      y++;
      x=0;
      continue;
    }

    if(ch=='O')
    {
      house[stage_num][y][x]=ch;
      house_num[stage_num][0]++;
    }

    if(ch=='$')
    {
      box[stage_num][y][x]=ch;
      box_num[stage_num][0]++;
    }

    map[stage_num][y][x] = ch;
    x++;
  }

  fclose(load);
}

/****************로드 맵 그리기***************/
void print_load(int stage_num) // load_stage 함수에서 읽어온 정보를 그림
{
  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",map[stage_num][b][a]);
    }
    printf("\n");
  }
}

/******************입력(getch)********************/
int getch(void){
  char ch;
  struct termios buf;
  struct termios save;
  tcgetattr(0,&save);
  buf=save;
  buf.c_lflag&=~(ICANON|ECHO);
  buf.c_cc[VMIN]=1;
  buf.c_cc[VTIME]=0;
  tcsetattr(0,TCSAFLUSH,&buf);
  ch=getchar();
  tcsetattr(0,TCSAFLUSH,&save);
  return ch;
}
/***************시간 측정하기***************/
void how_long_you_play()
{
  gap=((float)(endgame-startgame)-(float)(stopend-stop))/CLK_TCK;
}

/*****************디스플레이****************/
void show_me_display()
{
  system("clear");
  printf("Hello %s\n", name);
  printf("\n\n");
  printf("h(왼쪽),j(아래),k(위),l(오른쪽)\n");
  printf("u(undo)\n");
  printf("r(replay)\n");
  printf("n(new)\n");
  printf("e(exit)\n");
  printf("s(save)\n");
  printf("f(file load)\n");
  printf("d(display help)\n");
  printf("t(top)\n");

  if (getch()) return;

}
/***************파일 열어서 top옵션(랭킹) 불러오기********************/
void check_time()
{
 system("clear");
  char ch[11];
  double ranking[5][5]={};
  int num_map;
  double sec;
  double tmp=0;
  FILE *fp;
  fp=fopen("ranking.txt","w+t");
  while(fscanf(fp,"%s", &ch)!=EOF)
  {
      if(ch[0]=='m'&&ch[1]=='a'&&ch[2]=='p')
      num_map=ch[3]-49;

      if(ch[4]=='s'&&ch[5]=='e'&&ch[6]=='c'){
          sec=ch[0]*10 + ch[1]*1 + ch[3]*0.1 - 532.8;
          for(int a=0; a<5; a++){
              if(ranking[num_map][a]=='\0'){
              ranking[num_map][a]=sec;
              break;
          }

          }
      }

      for(int b=0; b<11; b++)
      ch[b]='\0';
  }

//버블 정렬
ranking[5][5]=0;//k=n l=i y=j i=h j=k
int i,j,k,l,y;
for(k=0;k<5;k++)
  for(l=0; l<5; l++){
      y=l;
      for(i=k; i<5; i++){
          for (j=y;j<5;j++)
          if(ranking[k][l]>ranking[i][j]){
              tmp=ranking[k][l];
              ranking[k][l]=ranking[i][j];
              ranking[i][j]=tmp;
             }
             y=0;
          }
      }




  for(int c=0; c<5; c++){
      printf("map%d\n", c+1);
      for(int d=0; d<5; d++)
          printf("%.1lf\n", (double)ranking[c][d]);
  }

  if (getch()) return;
}
