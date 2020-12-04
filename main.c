#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "lib.h"

int main () {
  char campo[ROW][COL];
  srand(time(NULL));
  azzeramento(campo); //Azzeramento e visualizzazione del primo campo di gioco
  vittoria(campo);  //Controllo della condizione di vittoria del game
  return 0;
}
