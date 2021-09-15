#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#define DIMCOMANDO 100
#define MAXDIM 1025
#define DIM_INIZIALE 10
#define DIM_PILA_INIZIALE 500000


int max_dim_mem = DIM_INIZIALE;
int massimo_indirizzo = 0;
int dim_pila = DIM_PILA_INIZIALE;
int testa_pila = 1;//testa della pila
int pos_in_pila = 0;
int pos_in_pila_prima = 0;
int num_istruzione = 0;
int num_UR = 0;

char comando = 'a';//comando letto

typedef struct tes
{
  char testo[MAXDIM];
  struct tes *prossimo;
}Txt;
typedef Txt *txt;

txt memoria_testo[MAXDIM];

typedef struct el
{
  struct tes *punt_testo;
}Elemento;

Elemento *M;

//definire la struttura della pila
typedef struct pil
{
  int num_istruzione;
  int dim_memoria;
  int max_indirizzo;

  Elemento *D;//config dopo la delete
}Pila;

Pila *pila;

void Inizializza1()
{
  int i;
  for(i = 0; i < max_dim_mem; i++)
  {
    M[i].punt_testo = NULL;
  }
  return;
}

void Inizializza2()
{
  int j;

  pila[0].D = (Elemento*)malloc(sizeof(Elemento) * max_dim_mem);
  pila[0].max_indirizzo = massimo_indirizzo;

  for(j = 0; j <= max_dim_mem; j++)
  {
    (pila[0].D)[j].punt_testo = NULL;
  }
  return;
}

int CalcolaNumero(char ingresso[DIMCOMANDO],int i,int *x)
{
  int numero = 0;
  int array[DIMCOMANDO];
  int j = 0;
  int z = i;
  while(ingresso[i] >= 48 && ingresso[i]<= 57)
  {
    array[i] = (ingresso[i] - '0');
    i++;
  }
  *x = i;
  i--;
  while((i - z) >= 0)
  {
    if(j == 0)
      numero = array[i];
    else if(j == 1)
      numero = numero + array[i] * 10;
    else if(j == 2)
      numero = numero + array[i] * 100;
    else if(j == 3)
      numero = numero + array[i] * 1000;
    else if(j == 4)
      numero = numero + array[i] * 10000;
    else if(j == 5)
      numero = numero + array[i] * 100000;
    else if(j == 6)
      numero = numero + array[i] * 1000000;
    j++;
    i--;
  }
  return numero;
}

void StampaRiga(int ind1, int ind2)
{
  int i;
  int diff = 0;
  if(ind2 > max_dim_mem)
  {
    diff = ind2 - max_dim_mem;
    ind2 = max_dim_mem;
  }
  for(i = ind1;i <= (ind2); i++)
  {
    if(M[i].punt_testo == NULL)
      printf(".\n");
    else
    {
      fputs(M[i].punt_testo->testo,stdout);
      printf("\n");
    }
  }
  if(diff > 0)
  {
    for(i = diff; i > 0; i--)
    {
      printf(".\n");
    }
  }
  return;
}

Txt* CercaOInserisci(char testo_riga[MAXDIM])
{
  Txt *punt,*prima = NULL;
  int lunghezza_stringa = strlen(testo_riga);
  Txt *nuovo = NULL;

  punt = memoria_testo[lunghezza_stringa - 1];

  while(punt != NULL && strcmp(punt->testo,testo_riga) != 0)
  {
    prima = punt;
    punt = punt->prossimo;
  }
  if(punt == NULL)
  {
    nuovo = (Txt*) malloc(sizeof(Txt));
    strcpy(nuovo->testo, testo_riga);
    if(prima != NULL)
    {
      prima->prossimo = nuovo;
    }
    else
    {
      memoria_testo[lunghezza_stringa - 1] = nuovo;
    }
    return nuovo;
  }
  else if(strcmp(punt->testo,testo_riga) == 0)
  {
    return punt;
  }
}

void InserisciInMemoria(char stringa[MAXDIM],int riga)
{
  if(riga > max_dim_mem)
  {
    while(max_dim_mem <= riga)
    {
      M = realloc(M, 5 * max_dim_mem * sizeof(Elemento));
      max_dim_mem = 5 * max_dim_mem;
    }
  }
  M[riga].punt_testo = CercaOInserisci(stringa);
  return;
}

void Push()
{
  int j;
  
  if(testa_pila >= dim_pila)
  {
    pila = realloc(pila, 2 * dim_pila * sizeof(Pila));
    dim_pila = dim_pila * 2;
  }

  pila[testa_pila].D = (Elemento*)malloc(sizeof(Elemento) * max_dim_mem);
  pila[testa_pila].dim_memoria = max_dim_mem;
  pila[testa_pila].max_indirizzo = massimo_indirizzo;
  for(j = 1; j <= max_dim_mem; j++)
  {
    (pila[testa_pila].D)[j].punt_testo = M[j].punt_testo;
  }
  return;
}

void Delete(int ind1, int ind2)
{
  int j,i = 0;

  if(ind1 <= massimo_indirizzo && ind2 >= massimo_indirizzo)
  {
    ind2 = massimo_indirizzo;
  }
  else if(ind1 > massimo_indirizzo)
  {
    return;
  }
  if(ind2 == massimo_indirizzo)
  {
    for(j = ind1; j <= max_dim_mem; j++)
    {
      M[j].punt_testo = NULL;
    }
    return;
  }
  else
  {
    for(j = ind2 + 1; j <= massimo_indirizzo; j++)
    {
      M[ind1 + i] = M[j];
      i++;
    }

    massimo_indirizzo = massimo_indirizzo - (ind2 - ind1 + 1);

    for(j = max_dim_mem; j > massimo_indirizzo; j--)
    {
      M[j].punt_testo = NULL;
    }
    return;
  }
}

void FaiUnduRedu()
{
  int j,i;

  if(num_UR < 0)
  {

    num_UR = - num_UR;
    pos_in_pila_prima = pos_in_pila;
    pos_in_pila = pos_in_pila + num_UR;

    if(pos_in_pila >= testa_pila)
    {
      num_UR = testa_pila - 1 - pos_in_pila_prima;
      pos_in_pila = testa_pila - 1;
    }
  }
  else
  {

    pos_in_pila_prima = pos_in_pila;
    pos_in_pila = pos_in_pila - num_UR;

    if(pos_in_pila < 0)
    {
      num_UR = pos_in_pila_prima;
      pos_in_pila = 0;
    }
  }
  if(num_UR == 0)
    return;

  for(j = 0; j <= pila[pos_in_pila].dim_memoria; j++)
  {
    M[j].punt_testo = (pila[pos_in_pila].D)[j].punt_testo;
  }
  if(pila[pos_in_pila].dim_memoria < max_dim_mem)
  {
    for(j = pila[pos_in_pila].dim_memoria + 1; j <= max_dim_mem; j++)
    {
      M[j].punt_testo = NULL;
    }
  }
  massimo_indirizzo = pila[pos_in_pila].max_indirizzo;
  return;
}

bool LeggiComando()
{
  int ind1 = -1,ind2 = -1,num;//indirizzi e numero letto da c , d , u e r;
  char ingresso[DIMCOMANDO];//ingresso Comando
  char stringa[MAXDIM];//ingresso strings

  int i = 1;
  int j = 0;

  gets(ingresso);
  if(ingresso[0] == 'q')
    return true;
  else if(ingresso[0] >= 48 && ingresso[0]<= 57)
  {
    num = CalcolaNumero(ingresso,0,&i);

    if(ingresso[i] == 'u')
    {
      comando = 'u';
      num_UR = num_UR + num;
      if(pos_in_pila - num_UR < 0)
      {
        num_UR = pos_in_pila;
      }
    }
    else if(ingresso[i] == 'r')
    {
      comando = 'r';
      num_UR = num_UR - num;
      if(-num_UR + pos_in_pila >= testa_pila)
      {
        num_UR = -(testa_pila - 1 - pos_in_pila);
      }
    }
    else
    {
      if(num_UR != 0)
      {
        FaiUnduRedu();
        num_UR = 0;
      }

      ind1 = num;//primo indirizzo
      num = -1;
      i++;
      ind2  = CalcolaNumero(ingresso,i,&i);//calcola il secondo indirizzo

      if(ingresso[i] == 'c')
      {
        num_istruzione++;

        if(comando == 'u' || comando == 'r')
        {
            for(i = pos_in_pila + 1; i < testa_pila; i++)
            {
              free(pila[i].D);
              pila[i].dim_memoria = 0;
            }
            testa_pila = pos_in_pila + 1;
        }
        comando = 'c';

        if(ind2 > massimo_indirizzo)//tengo in memoria il massimo indirizzo inserito
          massimo_indirizzo = ind2;
        gets(stringa);
        i = 0;
        while((i < (ind2 - ind1 + 1)))
        {
          InserisciInMemoria(stringa,ind1+i);
          gets(stringa);
          i++;
        }
        if(massimo_indirizzo < 25000)
        {
          Push();
          pos_in_pila = testa_pila;
          testa_pila++;
        }
      }
      else if(ingresso[i] == 'd')
      {
        if(comando == 'u' || comando == 'r')
        {
            for(i = pos_in_pila + 1; i < testa_pila; i++)
            {
              free(pila[i].D);
              pila[i].dim_memoria = 0;
            }
            testa_pila = pos_in_pila + 1;
        }

        comando = 'd';
        Delete(ind1, ind2);
        Push();
        pos_in_pila = testa_pila;
        testa_pila++;
      }
      else
      {
        StampaRiga(ind1,ind2);
      }
    }
  }
  return false;
}

int main()
{
  bool EXIT = false;//true quando leggo 'q'

  M = (Elemento*)malloc(max_dim_mem * sizeof(Elemento));

  Inizializza1();

  pila = (Pila*)malloc(dim_pila * sizeof(Pila));

  Inizializza2();

  while(EXIT != true)//ripeto finchè non voglio uscire dall'editor
  {
    EXIT = LeggiComando();
  }

  return 0;
}
