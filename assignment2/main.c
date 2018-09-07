#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "main.h"

#define COUNT 10
#define ROW 21
#define COL 80

struct room rooms[COUNT];
struct space dungeon[ROW][COL];

int genRand(int max, int min){
    return rand() % max + min;
}
int checkRoom(int x, int y, int w, int h){
    int row = 0;
    int col = 0;
    if(y + h >= 20 || x + w >= 79){
        return 0;
    }else{
        for(row = y; row < y + h; row++){
            for(col = x; col < x + w; col++){
                char center = dungeon[row][col].material;
                char right = dungeon[row+1][col].material;
                char left = dungeon[row-1][col].material;
                char down = dungeon[row][col+1].material;
                char up = dungeon[row][col-1].material;
                if(center == '.' || right == '.' || left == '.' || down == '.' || up == '.'){
                    return 0;
                }
            }
        }
    }
    return 1;
}
void addRoom(int x, int y, int w, int h, int idx){
    struct room rect;
    int row;
    int col;
    for(row = y; row < y + h; row++){
        for(col = x; col < x + w; col++){
            dungeon[row][col].material = '.';
            dungeon[row][col].hardness = 0;
        }
    }
    rect.sizeX = w;
    rect.sizeY = h;
    rect.x = x +( w / 2);
    rect.y = y + (h / 2);
    rooms[idx] = rect;
}
void insert(int *x, int *y, char move){
    if (move == 'd'){//Down
        *y = *y + 1;
    }else if (move == 'u'){//Up
        *y = *y - 1;
    }else if (move == 'l'){//Left
        *x = *x - 1;
    }else if (move == 'r'){//Right
        *x = *x + 1;
    }
  if(dungeon[*y][*x].material != '.'){
    dungeon[*y][*x].material = '#';
    dungeon[*y][*x].hardness = 0;
  }
}

void calcDist(struct room *rm, double *dis, int idx){
  int cnt;
  rm->x = rooms[idx].x;
  rm->y = rooms[idx].y;
  for(cnt = 0; cnt < idx; cnt++){
    int distY = abs(rooms[idx].x - rooms[cnt].y);
    int distX = abs(rooms[idx].x - rooms[cnt].x);
    int formResult = sqrt(distX * distX + distY * distY);
    if(formResult  < *dis){
      *dis = formResult;
      rm->x = rooms[cnt].x;
      rm->y = rooms[cnt].y;
    }
  }
}
void corridor(int idx){
  double dist = 999.0;
  struct room cRoom;
  calcDist(&cRoom, &dist, idx);
  int x = rooms[idx].x;
  int y = rooms[idx].y;

  while(x != cRoom.x || y != cRoom.y){//Check all the relative points and move left, right, down, up
    if(x < cRoom.x && y < cRoom.y){
      if(dungeon[y][x+1].hardness < dungeon[y+1][x].hardness){
        insert(&x, &y, 'r');
      }else{
        insert(&x, &y, 'd');
      }
    }else if(x < cRoom.x && y > cRoom.y){
      if(dungeon[y][x+1].hardness < dungeon[y-1][x].hardness){
        insert(&x, &y, 'r');
      }else{
        insert(&x, &y, 'u');
      }
    }else if(x > cRoom.x && y < cRoom.y){
      if(dungeon[y][x-1].hardness < dungeon[y+1][x].hardness){
        insert(&x, &y, 'l');
      }else{
        insert(&x, &y, 'd');
      }
    }else if(x > cRoom.x && y > cRoom.y){
      if(dungeon[y][x-1].hardness < dungeon[y-1][x].hardness){
        insert(&x, &y, 'l');
      }else{
        insert(&x, &y, 'u');
      }
    }else if(y < cRoom.y){
      insert(&x, &y, 'd');
    }else if(x > cRoom.x){
      insert(&x, &y, 'l');
    }else if(x < cRoom.x){
      insert(&x, &y, 'r');
    }else{
      insert(&x, &y, 'u');
    }
  }
}
void genRooms(){
  int row = 0;
  int col = 0;
  int cntr = 0;
  for(row = 0; row < ROW; row++){
    for(col = 0; col < COL; col++){

      struct space cell;
      cell.material = ' ';

      dungeon[row][col] = cell;

      if(row == 0 || col == 0 || row == 20 || col == 79){
        dungeon[row][col].hardness = 255;
      }else{
        dungeon[row][col].hardness = genRand(254,1);
      }
    }
  }
  for(cntr = 0; cntr < COUNT; cntr++){
    int x = genRand(78,1);
    int y = genRand(19,1);
    int w = genRand(10,3);
    int h = genRand(10,2);
    int check = 1;

    check = checkRoom(x,y, w, h);
    if(check == 1){
        addRoom(x, y, w, h, cntr);
    }else{
      cntr--;
    }
  }
  for(cntr = 1; cntr < COUNT; cntr++){
    corridor(cntr);
  }
}
void printDungeon(){
  int row = 0;
  int col = 0;
  printf("Upper Message\n");
  for(row = 0; row < ROW; row++){
      printf("|");
    for(col = 0; col < COL; col++){
        if (row == 0 || row == 20){
            printf("-");
        }else{
            printf("%c", dungeon[row][col].material);
        }
    }
    printf("|\n");
  }
  printf("\nLower Message 1\n");
  printf("Lower Message 2\n");
}
int main(int argc, char* argv[]){
    if(argv[1] != NULL){
        int result = atoi(argv[1]);
        srand(result);
    }else{
        srand(time(0));
    }

    genRooms();
    printDungeon();

    return 0;

}
