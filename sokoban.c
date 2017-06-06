#include <stdio.h>
#include <termio.h>
#include <stdlib.h>
#include <unistd.h>

void start();
void move();
int getch();
void stage();
void print_stage(int);
void save_stage();
void load_stage();
void print_load();
void whereisplayer();
int nstage_check();

char name[10];

int stage_num = -1;
int stage_num_save = -1;

int house_num[5][1];
int house_num_save[5][1];

int box_num[5][1];
int box_num_save[5][1];

char map[5][30][30];
char mapforsave[5][30][30];

char house[5][30][30]; // 보관장소
char houseforsave[5][30][30];

char box[5][30][30]; // 상자
char boxforsave[5][30][30];

int playerx,playery;
char input_char;


//------------------------------------------------------------------------------
int main(){
  start(); //시작 및 이름 입력
  sleep(1);
  stage();//맵 불러오기

  if (box_num[stage_num][0]!=house_num[stage_num][0]){ //맵 오류 체크
  printf("맵 오류 : 박스 개수와 보관장소 개수 불일치\n프로그램을 종료합니다.");
  return 0;}

    print_stage(stage_num=0);
    whereisplayer();
    while(1)
    {
      input_char=getch();

      if(input_char == 'h', 'j', 'k', 'l')
      {
      move();
      }

      if (nstage_check()==0)
      break;

      if(input_char == 's')
      {
        save_stage();
        printf("저장완료");
        continue;
      }

      if(input_char == 'e')
      {
        save_stage();
        return 0;
      }

      if(input_char == 'f')
      {
        system("clear");
        load_stage();
        print_load();
        continue;
      }

      if(input_char == 'h', 'j', 'k', 'l'){
      move();}

      if (nstage_check()==0)
      break;
    }

  return 0;
}

//---------------------START---------------------
void start(){
  int i=0;
  printf("Start...\nInput name : ");
  scanf("%s",name);
}

//------------STAGE : 맵 불러오기 ---------------
void stage(){
  int x = 0, y = 0;
  char ch;

  FILE *fp;

  fp=fopen("map.txt","r");
  while(fscanf(fp,"%c",&ch) != EOF)
  {
    if (ch=='m'){ //m이 입력되었을 때 stage_num 증가시켜 맵 번호 할당
      stage_num++;
      x=0;
      y=-1;
      continue;}

    if((ch=='a') || (ch == 'p')){
      continue;}

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


//-------------PRINT_STAGE : 맵 출력하기 -----------------
void print_stage(int stage_num){
  system("clear");
  int i=0;

  printf("Hello ");
  while(i<=10&&name[i]!=EOF){
  printf("%c", name[i]);
  i++;}
  printf("\n\n"); //HELLO NAME 출력


  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",map[stage_num][a][b]);
    }
    printf("\n");
  }
//  whereisplayer(); // @ 위치 찾기
}

//  whereisplayer(); // @ 위치 찾기
void whereisplayer(){
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
        return;}
        map[stage_num][playery+2*dy][playerx+2*dx]='$';}

  if (house[stage_num][playery][playerx]=='O')
  map[stage_num][playery][playerx]='O';

  map[stage_num][playery+dy][playerx+dx]='@';

  playery+=dy;
  playerx+=dx;
 //system("clear");
  print_stage(stage_num);
  }
}

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
    system("clear");
    printf("클리어 (짝짝짝) \n");
    sleep(2);
    stage_num++;
    whereisplayer();
    if (stage_num<5) print_stage(stage_num);
    else return 0;

  }
  return 1;

}

void save_stage(int stage_num_save)
{
  int a, b;
  FILE *save;

  save=fopen("sokoban.txt","w");
  fprintf(save,"%c",map[stage_num][a][b]);
  fclose(save);
}

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

    map[stage_num][y][x] = ch;
    x++;
  }

  fclose(load);
}

void print_load(int stage_num)
{
  for(int a=0;a<30;a++){
    for(int b=0;b<30;b++){
      printf("%c",map[stage_num][b][a]);
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
