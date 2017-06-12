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

char mvsave[5][30][30];
char base[5][30][30];

double gap=0, exitgap=0; // 시간 측정 변수

clock_t startgame=0,endgame=0,stop=0,stopend=0;

/**********************MAIN*************************/
int main(){

int size=0;

int eachrank[size];
float times[5][eachrank[size]+1];


  start();
  stage();

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
    else{
    if(input_char == 's'){
      save_stage(stage_num);
      printf("저장완료");
      printf("%c\n", input_char);
      continue;
    }
    if(input_char == 'e'){
      endgame=clock();
      how_long_you_play();
      save_stage(stage_num);
      printf("%c\n", input_char);
      return 0;
    }
    if(input_char == 'f'){
      system("clear");
      load_stage();
      print_load();
      continue;
    }
    if(input_char == 'u'){
      printf("%c\n", input_char);
      undo();
    }
    if(input_char == 'n'){
        printf("%c\n", input_char);
        clean(2);
        save(2);
        whereisplayer();
        print_stage(stage_num=0);
    }
    if(input_char == 'r'){
        printf("%c\n", input_char);
        clean(2);
        save(2);
        whereisplayer();
        print_stage(stage_num);
    }
    if(input_char == 'd'){
      printf("%c\n", input_char);
      show_me_display();
      print_stage(stage_num);
      continue;
    }
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
        exit(1) ;
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
  int i=0;
  system("clear");
  printf("Hello %s", name);

  printf("\n\n"); //HELLO NAME 출력


  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",map[stage_num][a][b]);
    }
    printf("\n");
  }
  printf("(command) ");
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
                        mvsave[a+1][b][c]=mvsave[a][b][c];
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
    int undocount=0;
    int i=0;

    if(undocount <5)
    {
    system("clear");

  printf("Hello ");
  while(i<=10&&name[i]!=EOF){
  printf("%c", name[i]);
  i++;}
  printf("\n\n"); //HELLO NAME 출력

  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",mvsave[0][a][b]);
    }
    printf("\n");
    }

    for(int a=0; a<30; a++){
        for(int b=0; b<30; b++){
            map[stage_num][a][b]=mvsave[0][a][b];
        }
    }

    whereisplayer();

    for(int a=0; a<4; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                mvsave[a][b][c]=mvsave[a+1][b][c];
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
                mvsave[a][b][c]='\0';
            }
        }
    }
    break;

    case 2:
    for(int a=0; a<5; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                mvsave[a][b][c]='\0';
                map[a][b][c]='\0';
                box[a][b][c]='\0';
                house[a][b][c]='\0';
            }
        }
    }
    break;

}
}
/*******************save**************************/
void save(int num)
{
    switch (num) {

        case 1:
    for(int a=0; a<5; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                base[a][b][c]=map[a][b][c];
            }
        }
    }

        case 2:
    for(int a=0; a<5; a++){
        for(int b=0; b<30; b++){
            for(int c=0; c<30; c++){
                map[a][b][c]=base[a][b][c];
            }
        }
    }
}
}

/****************세이브*******************/

void save_stage(int stage_num)
{
  int a, b;

  FILE *save;

  save=fopen("sokoban.txt","w");

  for (a=0; a<30; a++)
  {
    fprintf(save,"\n");
    for (b=0; b<30; b++)
    {
      fprintf(save,"%c",map[stage_num][a][b]);
    }
  }

  fclose(save);
}

/***************맵 불러오기*******************/
void load_stage()
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
    if(ch=='@')
    {
      playerx=x;
      playery=y;
      continue;
    }


    map[stage_num][y][x] = ch;
    x++;
  }
  fclose(load);
}

/****************로드 맵 그리기***************/
void print_load(int stage_num)
{
  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",map[stage_num][a][b]);
    }
    printf("\n");
  }
}

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
