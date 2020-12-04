#define PL1 1
#define PL2 2
#define ROW 6
#define COL 7
#define T 1

int modalita();
void azzeramento(char[][COL]);
void stampa(char[][COL]);
int input_us(char[][COL],int*,int*,int);
int input_pc(char[][COL],int*,int*,int);
int inserimento(char[][COL],int,int*,int*,int);
int control_oriz(char[][COL],int*,int*);
int control_vert(char[][COL],int*,int*);
int obliq_cr(char[][COL],int*,int*);
int obliq_decr(char[][COL],int*,int*);
int control_obliq(char[][COL],int*,int*);
void vittoria(char[][COL]);

//Fase di azzeramento della matrice
void azzeramento (char campo[][COL]) {
  int c,r;
  for (r=0;r<ROW;r++) for (c=0;c<COL;c++) campo[r][c]=0;
}

//Scelta della modalit� di gioco
int modalita () {
  int a;
  printf("Benvenuto nel gioco Forza 4\n\nScegli la modalita di gioco: \n");
  printf("\t1) User vs User\n\t2) User vs PC\n\t3) User vs PC intelligente\n");
  do {
    printf("\nScelta: ");
    scanf("%d",&a);
  } while (a!=1 && a!=2 && a!=3);
  printf("\n\tPlayer 1: X\n");
  if (a==1) printf("\tPlayer 2: O\n");
  else printf("\tPC: O\n");
  return a;
}

//Stampa della matrice
void stampa (char campo[][COL]) {
  int c,r;
  //Stampa dei numeri soprastanti, che rappresentano i numeri delle righe
  printf("\n");
  for (r=0;r<=ROW;r++) printf("  %d ",r+1);
  printf("\n");
  //Stampa della griglia
  for (c=0;c<COL;c++) printf(" ___");
  printf("\n");
  for (r=0;r<ROW;r++) {
    for (c=0;c<COL;c++) {
      if (campo[r][c]==0) printf("|___");
      if (campo[r][c]==1) printf("|_X_");
      if (campo[r][c]==2) printf("|_O_");
    }
    printf("|\n");
  }
  printf("\n");
}

//Inserimento riga, con gli eventuali controlli
int input_us (char campo[][COL],int *r,int *c,int turno) {
  int err;
  //Esegui il ciclo fino all'inserimento di una riga accettabile (non completamente piena)
  do {
    err=0;
    printf("\n\tInserisci la colonna (1...7): ");
    scanf("%d",c);
    if (campo[0][*c-1]!=0) {
      err=1;
      printf("\n\tRiga terminata. ");
    }
  } while (*c<1 || *c>7 || err==1);
  *c=*c-1;
  //Scorrimento della matrice, fino ad inviduazione della prima cella vuota,
  //dal momento che parte a controllare dal fondo della matrice
  *r=ROW-1;
  while (campo[*r][*c]!=0) (*r)--;
  //Riempimento del campo di gioco, in base al turno
  if (turno==1) campo[*r][*c]=PL1;
  else campo[*r][*c]=PL2;
}

//Inserimento riga del pc, con l'unico controllo della colonna piena
int input_pc (char campo[][COL],int *r,int *c,int mod) {
  //Tempo di attesa, per sembrare che il pc stia pensando alla mossa
  sleep(T);
  //Al momento dell'ultima mossa dell'utente, il pc inserisce il gettone per
  //evitare la vittoria dell'utente
  if (mod==3 && control_oriz(campo,r,c)==3) {
    while (campo[*r][*c]==PL1 && *c<6) (*c)++;
    //Se la posizione di destra � occupata, si va alla ricerca della posizione di
    //sinistra, per evitare la vittoria del player 1
    if (campo[*r][*c]!=0) {
      while (campo[*r][(*c)-1]==PL1 && *c>1) (*c)--;
      (*c)--;
    }
    //Se anche la posizione di sinistra � gia occupata, si calcola un nuovo numero
    //stavolta in maniera casuale
    if (campo[*r][*c]!=0) {
      do {
        *c=rand()%6+1;
      } while (campo[0][*c]!=0);
      *r=ROW-1;
      while (campo[*r][*c]!=0) (*r)--;
    }
    //Riempimento del campo di gioco
    campo[*r][*c]=PL2;
  } else {
      //Viene controllato se il controllo verticale ritorna 3 --> se vero, viene inserito il gettone per evitare la vittoria del pc
      if (mod==3 && control_vert(campo,r,c)==3) {
          campo[*r-1][*c]=PL2;
          *r=*r-1;
      }
      //Condizione che si verifica anche quando non ci sono 3 gettoni di fila, bensi
      //con delle celle vuote di mezzo
      else {
          int n=0,celle_vuote=0,r_vuoto,c_vuoto;
          //Viene fatta scorrere la matrice fino a quando viene trovato il gettone del pc
          //o quando sono state esaminate pi� di 1 cella vuota
          if (campo[*r][(*c)+1]==0 && campo[*r][(*c)+2]!=1);
          else {
              while (campo[*r][(*c)+1]!=PL2 && celle_vuote<2 && *c<6) {
              if (campo[*r][(*c)+1]==0) celle_vuote++;
              (*c)++;
          }
          if (celle_vuote==2) (*c)--;
       }
       celle_vuote=0;
       //A partire dalla posizione, si scorre la matrice nel verso opposto
       while (campo[*r][*c]!=PL2 && celle_vuote<2 && *c>=0) {
           if (campo[*r][*c]==1) n++;
           //Memorizzo quale � la cella vuota e nella quale andr� inserito il gettone
           if (campo[*r][*c]==0) {
               //Solamente le coordinate della prima cella vuota vengono memorizzate, per poi
               //inserirci il gettone del pc
               if (celle_vuote==0) {
                   r_vuoto=*r;
                   c_vuoto=*c;
               }
               celle_vuote++;
            }
            (*c)--;
        }
        if (n>=3) {
            //Riempimento della matrice
            campo[r_vuoto][c_vuoto]=PL2;
            *r=r_vuoto;
            *c=c_vuoto;
       }
         else {
             do {
                 *c=rand()%6+1;
             } while (campo[0][*c]!=0);
             //Scorrimento della matrice, fino ad inviduazione della prima cella vuota,
             //dal momento che parte a controllare dal fondo della matrice
             *r=ROW-1;
             while (campo[*r][*c]!=0) (*r)--;
             //Riempimento del campo di gioco
             campo[*r][*c]=PL2;
        }
     }
  }
}

//Inserimento del mark nel campo di gioco, con controllo ed eventuale
//reinserimento della riga, nel caso di riga completamente piena --
//in questo caso, non viene cambiata il player a cui spetta la mossa
int inserimento (char campo[][COL],int turno,int *r,int *c,int mod) {
  if (mod==2 || mod==3) {
    if (turno==2) input_pc(campo,r,c,mod);
    else input_us(campo,r,c,turno);
  } else input_us(campo,r,c,turno);
}

//Controllo del quartetto, di ugual simboli, orizzontale
int control_oriz (char campo[][COL],int *r,int *c) {
  int i=1,n=1;
  while (i<4 && *c-i>=0) {
    if (campo[*r][*c]==campo[*r][*c-i]) n++;
    else break;
    i++;
  }
  i=1;
  while (i<4 && *c+i<7) {
    if (campo[*r][*c]==campo[*r][*c+i]) n++;
    else break;
    i++;
  }
  return n;
}

//Controllo del quartetto, di ugual simboli, verticale
int control_vert (char campo[][COL],int *r,int *c) {
  int i=1,n=1;
  while (i<4 && *r+i<6) {
    if (campo[*r][*c]==campo[*r+i][*c]) n++;
    else break;
    i++;
  }
  i=1;
  while (i<4 && *r-i>=0) {
    if (campo[*r][*c]==campo[*r-i][*c]) n++;
    else break;
    i++;
  }
  return n;
}

//Controllo quartetto obliquo decrescente --> ritorna 1 se viene trovato il quartetto
int obliq_cr (char campo[][COL],int *r,int *c) {
  int i,n=1;
  for (i=1;i<4;i++) {
    if (campo[*r][*c]==campo[*r-i][*c+i]
      || campo[*r][*c]==campo[*r+i][*c-i]) n++;
    else break;
  }
  return n;
}

//Controllo quartetto obliquo decrescente --> ritorna 1 se viene trovato il quartetto
int obliq_decr (char campo[][COL],int *r,int *c) {
  int i,n=1;
  for (i=1;i<4;i++) {
    if (campo[*r][*c]==campo[*r+i][*c+i]
      || campo[*r][*c]==campo[*r-i][*c-i]) n++;
    else break;
  }
  return n;
}

//Controllo sia obliquo decrescente che crescente del quartetto
int control_obliq (char campo[][COL],int *r,int *c) {
  if (obliq_cr(campo,r,c)==4 || obliq_decr(campo,r,c)==4) return 1;
  return 0;
}

void vittoria (char campo[][COL]) {
  int turno=1;
  int c,r;
  int mod=modalita();
  do {
    //Stampa del campo di gioco, aggiornato ad ogni mossa
    stampa(campo);
    if (turno==1) printf("\n\tTurno: Player 1\n\t");
    else {
      if (mod==2 || mod==3) printf("\n\tTurno: PC\n\t");
      else printf("\n\tTurno: Player 2\n\t");
    }
    //Inserimento della riga
    inserimento(campo,turno,&r,&c,mod);
    //Cambio di mossa
    if (turno==1) turno=2;
    else turno=1;
  } while (control_oriz(campo,&r,&c)<4 && control_vert(campo,&r,&c)<4 && control_obliq(campo,&r,&c)==0);
  //Una volta uscito dal ciclo, viene identificato il vincitore
  stampa(campo);
  //Decretazione del vincitore della partita
  if (mod==2 || mod==3 && turno==1) printf("\n\tIl vincitore e' il PC\n");
  else if (mod==1 && turno==1) printf("\n\tIl vincitore e' il Player 2\n");
  else printf("\n\tIl vincitore e' Player 1\n");
  // system("pause");
}
