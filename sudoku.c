#include <stdio.h>
int game[9][9];
int done();
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
  if(done()){
    for(int i=0;i<9;i++){
      for(int j=0;j<9;j++){
        printf("%d ",game[i][j]);
      }
      printf("\n");
    }  
  }
  return 0;
}
int done(){
  int mp=10,bx=-1,by=-1;
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      if(game[i][j]==0){
        int p=0;
        for(int u=1;u<=9;u++){
          if(!xused[i][u]&&!yused[j][u]&&!boxused[(i/3)*3+j/3][u]){
            p++;
          }
        }
        if(p==0) return 0;
        if(p<mp){
          mp=p;
          bx=i;
          by=j;
        }
      }
    }
  }
  if(bx!=-1){
    for(int u=1;u<=9;u++){
      if(xused[bx][u]==0&&yused[by][u]==0&&boxused[(bx/3)*3+by/3][u]==0){
        game[bx][by]=u; 
        change(bx,by,u,1);
        if (done()) return 1;
        change(bx,by,u,0);
        game[bx][by]=0;
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
