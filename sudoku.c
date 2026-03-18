#include <stdio.h>
int game[9][9];
int done(int x,int y);
int change(int x,int y,int val,int to);
int xused[9][10]={0};
int yused[9][10]={0};
int boxused[9][10]={0};
int main() {
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      scanf("%1d",&game[i][j]);
      if(game[i][j]!=0){
        xused[i][game[i][j]]=1;
        yused[j][game[i][j]]=1;
        boxused[(i/3)*3+j/3][game[i][j]]=1;
      }
    }
  }
  if(done(0,0)){
    for(int i=0;i<9;i++){
      for(int j=0;j<9;j++){
        printf("%d ",game[i][j]);
      }
      printf("\n");
    }  
  }
  return 0;
}
int done(int x,int y){
  if(x!=9){
    if(game[x][y]==0){
      for(int u=1;u<=9;u++){
        if(xused[x][u]==0&&yused[y][u]==0&&boxused[(x/3)*3+y/3][u]==0){
          game[x][y]=u; 
          change(x,y,u,1);
          if(y<8){
            if (done(x,y+1)) return 1;
          }
          else{
            if (done(x+1,0)) return 1;
          }
          change(x,y,u,0);
          game[x][y]=0;
        }
      }
    }
    else{
      if(y<8){
        if (done(x,y+1)) return 1;
      }
      else{
        if (done(x+1,0)) return 1;
      }
    }
  }
  else{
    return 1;
  }
  return 0;
}
int change(int x,int y,int val,int to){
  xused[x][val]=to;
  yused[y][val]=to;
  boxused[(x/3)*3+y/3][val]=to;
  return 0;
}
