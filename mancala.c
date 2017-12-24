
#include <windows.h>

/* Game Board */
int M[14], N;

/* Helper Functions */
void draw_board(int x) {
  system("cls");
	printf("~~~~~~ Mancala ~~~~~~\n");
  if(x==-1) printf("       Computer: hmm\n");
  else if(x) printf("       Computer: %i\n",x);
  else printf("\n");
  printf("%i\n",N);
  printf("\
 --------------------     \n\
  %2i %2i %2i %2i %2i %2i \n\
%2i                 %2i   \n\
  %2i %2i %2i %2i %2i %2i \n\
 --------------------     \n\n\
   6  5  4  3  2  1       \n\n",\
  M[8],M[9],M[10],M[11],M[12],M[13],\
  M[7],M[0],\
  M[6],M[5],M[4],M[3],M[2],M[1]);
  Sleep(100);
}

void init_board() {
  int i;
  // set all bins to 4
  for(i=0;i<14;i++) M[i]=4;
  // clear Mancalas
  M[0]=0;  M[7]=0;
  draw_board(0);
}

int move(int u, int o, int h){
  int i,n,k,s;
  // pick up stones
  n = M[u+o];
  if(n==0) return(-1);
  M[u+o] = 0;
  N=n;  
  if(h) draw_board(o>0?u:0);
  for(i=1;i<=n;i++) {
    // bin order
    k = u+o-i;
    k = k<0 ? 14+k : k;
    // skip enemies Mancala
    if((k+o)==7) {
      n+=1;
      continue;
    }
    // place a stone
    M[k] += 1;    
    N=n-i; 
    if(h) draw_board(o>0?u:0);
  }
  // Handle stealing
  if(M[k]==1) {
    for(n=1;n<=6;n++){
      if((k-o)==n) {
        s = (k+14-2*n)%14;
        M[k]+=M[s];
        M[s]=0;
        if(h) draw_board(o>0?u:0);
      }
    }
  }
  return k;
}

int cant_move(int o) {
  int n;
  // look for a place to move
  for(n=1;n<=6;n++) if(M[n+o]>0) return(0);
  // no places found
  return(1);
}

void check_win() {
  if(M[0]>M[7]) printf("PLAYER WINS!");
  else if(M[0]<M[7]) printf("COMPUTER WINS!");
  else printf("TIE!");
  getch();
}

int simulate_future(int W[7]) {
  int i,j,k, Mo[14]={0}, Wo[7]={0};
  // store original board
  for(i=0;i<14;i++) Mo[i]=M[i];
  // try each move to completion
  for(j=1;j<=6;j++) {
    if(move(j,7,0)==7) simulate_future(Wo);
    // calculate merit
    W[j]=Wo[0];
    W[j]+=(M[7]-Mo[7])*5;
    for(i=8;i<=13;i++) W[j]+=(M[i]-Mo[i]);
    // restore original board
    for(i=0;i<14;i++) M[i]=Mo[i];
  }
  return(get_max(W));
}

// TODO: add a random feature
int get_max(int W[7]){
  int i,k, m=-1000;
  for(i=1;i<=6;i++) {
    if(M[i+7]>0){
      if(W[i]>=m) {
        m=W[i];
        k=i;
      }
    }
  }
  W[0]=m;
  return(k);
}

void move_computer() {
  int i, u, k=7, W[7]={0};
  while(k==7) {
    Sleep(1000);
    // first check if computer can move
    if(cant_move(7)) {
      // player gets the remaining stones
      for(i=1;i<=13;i++) {
        if(i==7) continue;
        M[0]+=M[i];
        M[i]=0;
        draw_board(0);
      }
      check_win();
      return;
    }    
    // call the AI
    u=simulate_future(W);
    k=move(u,7,1);
  }
}

void move_player() {
  int i, u=0, k=0;
  while(k==0) {
    // first check if you can
    if(cant_move(0)) {
      // computer gets the remaining stones
      for(i=1;i<=13;i++) {
        if(i==7) continue;
        M[7]+=M[i];
        M[i]=0;
        draw_board(0);
      }
      check_win();
      return;
    }
    // user inputs where to move
    draw_board(0);
    while(u<1 || u>6 || M[u]==0) scanf("%d",&u);
    k=move(u,0,1);
  }
}


/* Main Program */
void main() {
  init_board();
  while(1) {
    move_player();
    draw_board(-1);
    Sleep(1000);
    move_computer();
  }
}