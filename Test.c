#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
int row, col, i, j, k, s, y, z, sr, sc, bom;
int *boon,*bon;
void lose();
void win();
int built();
int screen();
int open(int ix,int iy);
int main() {
    row=10,col=10;
    printf("Please input the row, col and bom:(e.g.:10 10 10)\n");
    scanf("%d %d %d", &row, &col, &bom);
    s=row*col;//全部空格和当前空格
    if (bom>=s) {
        printf("Error!\n");
        return 0;
    }
    boon = (int*)malloc(row * col * sizeof(int));//deepseek
    bon = (int*)malloc(row * col * sizeof(int));
    for (i=0;i<row;i++)//=0
        for (j=0;j<col;j++) {
          boon[i*col+j]=0;
          bon[i*col+j]=0;
        }
    screen();
    printf("Please input the row and col:(start form 0)\n");
    scanf("%d %d", &sr, &sc);
    built();
    if (boon[sr*col+sc]==0)
        open(sr,sc);
      else if (bon[sr*col+sc]!=1) {
        bon[sr*col+sc]=1;
        s--;
      }
    while (1) {
    if(s==bom) {
        win();
        return 0;
        }
      screen();
      printf("Please input the row and col:(start form 0)\n");
      scanf("%d %d", &sr, &sc);
      if (boon[sr*col+sc]<0) {
        lose();
        return 0;
      }
      else if (boon[sr*col+sc]==0)
        open(sr,sc);
      else if (bon[sr*col+sc]!=1) {
        bon[sr*col+sc]=1;
        s--;
      }
    }
    return 0;
}
int built() {
  srand(time(NULL));//全是sheet不要改
  for (i=1;i<(bom+1);i++) {
      y=rand()%row;
      z=rand()%col;
      if (boon[y*col+z]>=0&&(y!=sr||z!=sc))
      {
          boon[y*col+z]=-10;
          for (j=y-1;j<=y+1;j++) {
              for (k=z-1;k<=z+1;k++) {
                  if (j>=0&&j<row&&k>=0&&k<col)
                  boon[j*col+k]++;
              }
          }
      }
      else
      i--;//确保雷够多,qwq
  }
  return 0;
  }
int open(int ix,int iy) {
    if (ix<0||ix>=row||iy<0||iy>=col)//边界
      return 0;
    if (boon[ix*col+iy]>=0) {//打开
      for(int i=ix-1;i<=ix+1;i++)
        for(int j=iy-1;j<=iy+1;j++) {
          if (i>=0&&i<row&&j>=0&&j<col&&boon[i*col+j]==0&&bon[i*col+j]==0){//sheet
            bon[i*col+j]=1;
            s--;//当前空格减少,30min
            open(i,j);
          }
          else if(i>=0&&i<row&&j>=0&&j<col&&boon[i*col+j]>0&&bon[i*col+j]==0){//sheet
            bon[i*col+j]=1;
            s--;//当前空格减少
          }
        }
    }
    return 0;
}
int screen() {
  system("clear");
  for (i=0;i<row;i++) {
      for (j=0;j<col;j++) {
          if (boon[i*col+j]==0&&bon[i*col+j]==1)
              printf("  ");
          else if (bon[i*col+j]==1)
              printf("%d ", *(boon+i*col+j));
          else
              printf("# ");
      }
      printf("\n");
  }
  return 0;
}
void win() {
    system("clear");
    printf("You win!\n");
    for (i=0;i<row;i++) {
        for (j=0;j<col;j++) {
            if (*(boon+i*col+j)<0)
                printf("@ ");
            else if (*(boon+i*col+j)==0)
                printf("  ");
            else 
                printf("%d ", *(boon+i*col+j));
        }
        printf("\n");
    }
}
void lose() {//完成
    system("clear");
    printf("You lose!\n");
    for (i=0;i<row;i++) {
        for (j=0;j<col;j++) {
            if (*(boon+i*col+j)<0)
                printf("@ ");
            else if (*(boon+i*col+j)==0)
                printf("  ");
            else 
                printf("%d ", *(boon+i*col+j));
        }
        printf("\n");
    }
}