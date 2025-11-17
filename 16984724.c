#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define POTENCIA 1000000
char *cores[5] = {"Azul","Verde","Amarelo","Laranja","Vermelho"};
char *preferencias[6] = {"N/A","Idoso", "PCD", "Gravida", "Pessoa Obesa","Recem Nascido"};

typedef struct {
  int cor;
  int preferencia;
  int tempo;

} ficha_Paciente;

int matchingNumber(char * s, int coluna);

int qtdLinhas(char * nomeArq);

int abrirArquivo(char * nomeArq, ficha_Paciente * fichas, double * numeros);

double escalonar(ficha_Paciente * ficha);

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(double nums[], int ids[], int inicio, int fim) {
    double pivot = nums[ids[fim]]; 
    int i = inicio - 1; 
    for (int j = inicio; j < fim; j++) {
        if (nums[ids[j]] <= pivot) { 
            i++;
            swap(&ids[i], &ids[j]);
        }
    }
    swap(&ids[i + 1], &ids[fim]);
    return i + 1; 
}

void quickSort(double nums[], int ids[], int inicio, int fim) {
    if (inicio < fim) {
        int pi = partition(nums, ids, inicio, fim);
        quickSort(nums, ids, inicio, pi - 1);
        quickSort(nums, ids, pi + 1, fim);
    }
}

int main(int argc, char *argv[]){
  int linhas = qtdLinhas(argv[1]);
  if(linhas == -1) return -1;
  ficha_Paciente * fichas = (ficha_Paciente*) malloc(sizeof(ficha_Paciente)*linhas);
  double * numeros = (double*) malloc(sizeof(double)*linhas);
  int * ids = (int*) malloc(sizeof(int)*linhas);
  for (int i = 0; i < linhas; i++) ids[i] = i;

  abrirArquivo(argv[1], fichas, numeros);
  quickSort(numeros, ids, 0, linhas-1);
  FILE *newFile = fopen("OrdemDeAtendimentos.csv", "w"); 
  if (newFile == NULL) {
      printf("Erro ao criar arquivo!\n");
      return -1;
  }
  for (int i = linhas-1; i >=0; i--)
  {
    fprintf(newFile, "%s,%s,%d\n", cores[fichas[ids[i]].cor], preferencias[fichas[ids[i]].preferencia], fichas[ids[i]].tempo);
  }
  fclose(newFile);
  return 0;
}

double escalonar(ficha_Paciente * ficha){

  switch (ficha->cor)
  {
    case 0:
      ficha->cor = (ficha->tempo > 240)?ficha->cor+1:ficha->cor;
      break;
    case 1:
      ficha->cor = (ficha->tempo > 120)?ficha->cor+1:ficha->cor;
      break;
    case 2:
      ficha->cor = (ficha->tempo > 60)?ficha->cor+1:ficha->cor;
      break;
    case 3:
      ficha->cor = (ficha->tempo > 10)?ficha->cor+1:ficha->cor;
      break;
    default:
      break;
  }

  return (double)(ficha->cor * 10 +
    ((ficha->cor >= 3 || !ficha->preferencia  ) ? 0 : 1)) +
    ((double)ficha->tempo) / POTENCIA;

}


int abrirArquivo(char * nomeArq, ficha_Paciente * fichas, double * numeros){

  FILE * arquivo = fopen(nomeArq, "r");
  int c;
  int coluna = 0, linhaAtual = 0;
  char substring[30]; 
  substring[0] = '\0';
  while((c = fgetc(arquivo))!=EOF){
    if(c >= 254) continue;
    if (c == ',' || c == '\n') {
        switch (coluna)
        {
          case 0: fichas[linhaAtual].cor = matchingNumber(substring, coluna);break;
          case 1: fichas[linhaAtual].preferencia = matchingNumber(substring, coluna);break;
          case 2:
            fichas[linhaAtual].tempo = atoi(substring);
            numeros[linhaAtual] = escalonar(&fichas[linhaAtual]);
            linhaAtual++;
            break;
        }        
        substring[0] = '\0';   
        coluna = (coluna + 1) % 3;
    } else {
        int len = strlen(substring);
        substring[len] = c;
        substring[len+1] = '\0';
    }
  }
  fichas[linhaAtual].tempo = atoi(substring);
  numeros[linhaAtual] = escalonar(&fichas[linhaAtual]);
  fclose(arquivo);
  return 1;
}

int matchingNumber(char * s, int coluna){
  switch (coluna){
    case 0: 
      for (int i = 0; i < 5; i++)
        if (strcmp(cores[i], s) == 0)
            return i;
      break;

    case 1:
      for (int i = 0; i < 6; i++)
        if (strcmp(preferencias[i], s) == 0)
            return i;
      break;
    default: break;
  }
  return -1; 
}

int qtdLinhas(char * nomeArq){
  FILE * arquivo = fopen(nomeArq, "r");
  if(arquivo == NULL){
    printf("\nErro ao abrir o arquivo: %s\n", nomeArq);
    return -1;
  }

  int qtd = 0;
  int c;
  char lastVal = '\n';
  while((c = fgetc(arquivo))!=EOF){
    if((char)c=='\n')qtd++;
    lastVal = (char) c;
  }
  if(lastVal!='\n')qtd++;
  fclose(arquivo);
  return qtd;
}
