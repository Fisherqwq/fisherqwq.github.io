#include <limits.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include <time.h>
#define MAX_DEPTH 5
#define SIZE 15
#define COVER_RANGE 3
int *game, *cov, *cov_1;
int n, m, k, i, j, l, c, p, r, s, t, u, v, w, x, y, z,
    col = SIZE, row = SIZE, min, con, space, brk = 0, pmk = 0;
long long maxx,ttime;
double cost;
clock_t stime, etime;
typedef struct {
  short mov[6];
  int size;
  int value;
  char name[12];
} movn;
typedef struct {
  int lx;
  int ly;
} bestmove;
bestmove pastmove[SIZE * SIZE];
int check();
int turn();
int screen();
int pmove();
int rmove();
long long scor();
int cover();
long long minimax(long long alpha, long long beta, int depth, int player);
void win();
void lose();
movn load[] = {
    // 必赢
    {{1, 1, 1, 1, 1}, 5, -2000000000, "五"},
    // 严重威胁
    {{0, 1, 1, 1, 1, 0}, 6, -100000000, "活四"},
    {{1, 1, 1, 1, 0}, 5, -500000000, "活四"},
    {{1, 1, 1, 0, 1}, 5, -10000000, "活四"},
    {{1, 0, 1, 1, 1}, 5, -10000000, "活四"},
    {{1, 1, 0, 1, 1}, 5, -10000000, "活四"},
    {{0, 1, 1, 1, 1}, 5, -10000000, "活四"},
    // 威胁
    {{0, 1, 1, 1, 0}, 5, -1000000, "活三"},
    {{1, 1, 1, 1}, 4, -500000, "死四"},
    {{1, 1, 0, 1}, 4, -100000, "跳四"},
    {{1, 0, 1, 1}, 4, -100000, "跳四"},
    {{0, 1, 1, 1, 2}, 5, -100000, "三"},
    {{2, 1, 1, 1, 0}, 5, -100000, "三"},
    {{1, 1, 1}, 3, -800, "死三"},
    // 一般
    {{0, 1, 1, 0}, 4, -1000, "活二"},
    {{0, 1, 0, 1, 0}, 5, -1000, "活二"},
    {{0, 1, 1, 2}, 4, -100, "活二"},
    {{2, 1, 1, 0}, 4, -100, "活二"},
    {{2, 1, 0, 1}, 4, -100, "活二"},
    {{1, 0, 1, 2}, 4, -100, "活二"},
    {{1, 1}, 2, -10, "死二"},
    {{0, 1, 0}, 3, -10, "活一"},
    {{0, 1, 2}, 3, -5, "活一"},
    {{2, 1, 0}, 3, -5, "活一"},
    {{1}, 1, -1, "死一"},
    // 必赢
    {{2, 2, 2, 2, 2}, 5, 2000000000, "五"},
    // 严重威胁
    {{0, 2, 2, 2, 2, 0}, 6, 100000000, "活四"},
    {{2, 2, 2, 2, 0}, 5, 50000000, "活四"},
    {{2, 2, 2, 0, 2}, 5, 10000000, "活四"},
    {{2, 0, 2, 2, 2}, 5, 10000000, "活四"},
    {{2, 2, 0, 2, 2}, 5, 10000000, "活四"},
    {{0, 2, 2, 2, 2}, 5, 10000000, "活四"},
    // 威胁
    {{0, 2, 2, 2, 0}, 5, 1000000, "活三"},
    {{2, 2, 2, 2}, 4, 500000, "死四"},
    {{2, 2, 0, 2}, 4, 100000, "跳四"},
    {{2, 0, 2, 2}, 4, 100000, "跳四"},
    {{0, 2, 2, 2, 1}, 5, 100000, "三"},
    {{1, 2, 2, 2, 0}, 5, 100000, "三"},
    {{2, 2, 2}, 3, 1000, "死三"},
    // 一般
    {{0, 2, 2, 0}, 4, 2000, "活二"},
    {{0, 2, 0, 2, 0}, 5, 1000, "活二"},
    {{0, 2, 2, 1}, 4, 100, "活二"},
    {{1, 2, 2, 0}, 4, 100, "活二"},
    {{1, 2, 0, 2}, 4, 100, "活二"},
    {{2, 0, 2, 1}, 4, 100, "活二"},
    {{2, 2}, 2, 10, "死二"},
    {{0, 2, 0}, 3, 10, "活一"},
    {{0, 2, 1}, 3, 5, "活一"},
    {{1, 2, 0}, 3, 5, "活一"},
    {{2}, 1, 1, "死一"},
};
int main() {
  srand(time(NULL));
  memset(pastmove, -1, sizeof(pastmove));
  game = (int *)malloc(row * col * sizeof(int)); // 棋盘
  cov = (int *)malloc(row * col * sizeof(int));  // 覆盖
  cov_1 = (int *)malloc(row * col * sizeof(int));
  space = row * col;
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
      cov[i * col + j] = 0;
      cov_1[i * col + j] = 0;
      game[i * col + j] = 0;
    }
  }
  turn();
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
      game[i * col + j] = 0;
    }
  }
  FILE *file = fopen("pastmove.txt", "w");
  for (y = 0; y < pmk; y++) {
    fprintf(file, "%d %d\n", pastmove[y].lx, pastmove[y].ly);
    if (y % 2 == 0) {
      game[pastmove[y].lx * col + pastmove[y].ly] = 1;
    } else
      game[pastmove[y].lx * col + pastmove[y].ly] = 2;
    for (i = 0; i < row; i++) {
      for (j = 0; j < col; j++) {
        if (game[i * col + j] == 0) {
          fprintf(file, "  ");
        } else if (game[i * col + j] == 1) {
          fprintf(file, "X ");
        } else
          fprintf(file, "O ");
      }
      fprintf(file, "\n");
    }
    fprintf(file,"%lld\n",scor());
  }
  fclose(file);
  free(game);
  free(cov);
  free(cov_1);
  return 0;
}
int turn() {
  screen();
  pmove();
  pmk++;
  screen();
  if (check() != 0) {
    switch (check()) {
    case 1:
      win();
      break;
    case 2:
      lose();
      break;
    case 3:
      printf("Draw\n");
      break;
    }
    return 0;
  }
  rmove();
  pmk++;
  if (check() != 0) {
    switch (check()) {
    case 1:
      win();
      break;
    case 2:
      lose();
      break;
    case 3:
      printf("Draw\n");
      break;
    }
    return 0;
  }
  return turn();
}
int check() {
  int ax[8] = {1, 0, -1, 0, 1, 1, -1, -1};
  int by[8] = {0, 1, 0, -1, 1, -1, 1, -1};
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
      if (game[i * col + j] != 0) {
        int colour = game[i * col + j];
        for (int dir = 0; dir < 8; dir++) {
          int count = 1;
          for (int step = 1; step < 5; step++) {
            int ni = i + step * ax[dir];
            int nj = j + step * by[dir];
            if (ni < 0 || ni >= row || nj < 0 || nj >= col)
              break;
            if (game[ni * col + nj] == colour) {
              count++;
            } else {
              break;
            }
          }
          if (count >= 5) {
            return colour;
          }
        }
      }
    }
  }
  if (space == 0) {
    return 3;
  }
  return 0;
}
int screen() {
  system("clear");
  printf("  ");
  for (i = 0; i < col; i++) {
    if (i < 10)
      printf("%d ", i);
    else if (i >= 10)
      printf("%d", i);
  }
  printf("\n");
  for (i = 0; i < row; i++) {
    if (i < 10)
      printf("%d ", i);
    else if (i >= 10)
      printf("%d", i);
    for (j = 0; j < col; j++) {
      if (game[i * col + j] == 0) {
        printf("  ");
      } else if (game[i * col + j] == 1) {
        printf("X ");
      } else
        printf("O ");
    }
    printf("\n");
  }
  for (i = 0; i < row; i++) {
    for (j = 0; j < col; j++) {
      if(cov_1[i * col + j]<10){
        printf("%d ", cov_1[i * col + j]);
      }
      else{
        printf("%d", cov_1[i * col + j]);
      }
    }
    printf("\n");
  }
  printf("time:%lfs",cost);
  printf("score:%lld", scor());
  printf("caltime:%lld\n",ttime);
  return 0;
}
int pmove() {
  printf("Player turn:");
  scanf("%d %d", &x, &y);
  if (game[x * col + y] == 0&&x>=0&&x<row&&y>=0&&y<col) {
    game[x * col + y] = 1;
    space--;
    pastmove[pmk].lx = x;
    pastmove[pmk].ly = y;
  } else {
    printf("Invalid move\n");
    return pmove();
  }
  return 0;
}
int rmove() {
  maxx = LLONG_MIN;
  int k = 0;
  cover();
  memcpy(cov_1, cov, row * col * sizeof(int));
  bestmove best[col * row];
  memset(best, -1, sizeof(best));
  stime=clock();
  ttime=0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (cov_1[i * col + j] != 0 && game[i * col + j] == 0) {
        game[i * col + j] = 2;
        space--;
        long long value = minimax(INT_MIN, INT_MAX, MAX_DEPTH, 1);
        game[i * col + j] = 0;
        space++;
        if (value > maxx) {
          maxx = value;
          memset(best, -1, sizeof(best));
          best[0].lx = i;
          best[0].ly = j;
          k = 1;
        }
        if (value == maxx) {
          best[k].lx = i;
          best[k].ly = j;
          k++;
        }
      }
    }
  }
  if (k > 1) {
    int idx = rand() % k; // 只调用一次rand
    game[best[idx].lx * col + best[idx].ly] = 2;
    pastmove[pmk].lx = best[idx].lx;
    pastmove[pmk].ly = best[idx].ly;
  } else {
    game[best[0].lx * col + best[0].ly] = 2;
    pastmove[pmk].lx = best[0].lx;
    pastmove[pmk].ly = best[0].ly;
  }
  etime=clock();
  cost=((double)(etime-stime))/CLOCKS_PER_SEC;
  space--;
  return 0;
}
int cover() {
  memset(cov, 0, row * col * sizeof(int));
  /*for (int i = 0; i < row; i++){
    for (int j = 0; j < col; j++){
      cov[i * col + j] = 0;
    }
  }*/
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (game[i * col + j] != 0) {
        for (int a = i - COVER_RANGE; a <= i + COVER_RANGE; a++) {
          if (a >= 0 && a < row)
            cov[a * col + j] += 1;
        }
        for (int b = j - COVER_RANGE; b <= j + COVER_RANGE; b++) {
          if (b >= 0 && b < col)
            cov[i * col + b] += 1;
        }
        for (int a = i - COVER_RANGE, b = j - COVER_RANGE; a <= i + COVER_RANGE && b <= j + COVER_RANGE; a++, b++) {
          if (a >= 0 && a < row && b >= 0 && b < col)
            cov[a * col + b] += 1;
        }
        for (int a = i - COVER_RANGE, b = j + COVER_RANGE; a <= i + COVER_RANGE && b >= j - COVER_RANGE; a++, b--)
          if (a >= 0 && a < row && b >= 0 && b < col)
            cov[a * col + b] += 1;
      }
    }
  }
  return 0;
}
long long scor() {
  // 左上到右下
  long long score = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      for (int k = 0; k < sizeof(load) / sizeof(load[0]); k++) {
        if (game[i * col + j] == load[k].mov[0]) {
          for (int l = 0; l < load[k].size; l++) {
            if (j + l >= col || i + l >= row)
              break;
            else if (game[(i + l) * col + j + l] != load[k].mov[l])
              break;
            else if (l == load[k].size - 1) {
              score += load[k].value;
            }
          }
        }
      }
    }
  }

  // 右上到左下
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      for (int k = 0; k < sizeof(load) / sizeof(load[0]); k++) {
        if (game[i * col + j] == load[k].mov[0]) {
          for (int l = 0; l < load[k].size; l++) {
            if (j - l < 0 || i + l >= row)
              break;
            else if (game[(i + l) * col + j - l] != load[k].mov[l])
              break;
            else if (l == load[k].size - 1) {
              score += load[k].value;
            }
          }
        }
      }
    }
  }

  // 水平
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      for (int k = 0; k < sizeof(load) / sizeof(load[0]); k++) {
        if (game[i * col + j] == load[k].mov[0]) {
          for (int l = 0; l < load[k].size; l++) {
            if (j + l >= col)
              break;
            else if (game[i * col + j + l] != load[k].mov[l])
              break;
            else if (l == load[k].size - 1) {
              score += load[k].value;
            }
          }
        }
      }
    }
  }

  // 垂直
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      for (int k = 0; k < sizeof(load) / sizeof(load[0]); k++) {
        if (game[i * col + j] == load[k].mov[0]) {
          for (int l = 0; l < load[k].size; l++) {
            if (i + l >= row)
              break;
            else if (game[(i + l) * col + j] != load[k].mov[l])
              break;
            else if (l == load[k].size - 1) {
              score += load[k].value;
            }
          }
        }
      }
    }
  }
  return score;
}
long long minimax(long long alpha, long long beta, int depth, int player) {
  ttime++;
  if (depth == 0 || space == 0 || check() != 0) {
    return scor();
  }
  cover();
  int cov_2[row * col];
  memcpy(cov_2, cov, row * col * sizeof(int));
  if (player == 2) {
    brk = 0;
    long long best = LLONG_MIN;
    for (int i = 0; i < row && !brk; i++) {
      for (int j = 0; j < col && !brk; j++) {
        if (cov_2[i * col + j] != 0 && game[i * col + j] == 0) {
          game[i * col + j] = 2;
          space--;
          long long value = minimax(alpha, beta, depth - 1, 1);
          best = fmax(best, value);
          alpha = fmax(alpha, best);
          game[i * col + j] = 0;
          space++;
          if (beta <= alpha) {
            brk = 1;
            break;
          }
        }
      }
    }
    return best;
  } else {
    brk = 0;
    long long best = LLONG_MAX;
    for (int i = 0; i < row && !brk; i++) {
      for (int j = 0; j < col && !brk; j++) {
        if (cov_2[i * col + j] != 0 && game[i * col + j] == 0) {
          game[i * col + j] = 1;
          space--;
          long long value = minimax(alpha, beta, depth - 1, 2);
          best = fmin(best, value);
          beta = fmin(beta, best);
          game[i * col + j] = 0;
          space++;
          if (beta <= alpha) {
            brk = 1;
            break;
          }
        }
      }
    }
    return best;
  }
}
void win() {
  screen();
  printf("You win!\n");
}
void lose() {
  screen();
  printf("You lose!\n");
}
