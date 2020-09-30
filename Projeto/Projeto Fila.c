#include<stdio.h>
#include<stdlib.h>
#include<string.h> //para usar stricmp -  para compara STRINGS com letras maiusculas e minusculas
#include<ctype.h> // para usar toupper - para comparar CARACTERES maisculos e minusculos
#include<locale.h>
#include <time.h>
#include "tela.h"


int num_serv; // definindo globalmente uma variavel para o numero de servidores


typedef struct tno fila;

struct tno
{
    int tempo_entrada;

    struct tno *prox;

};

typedef struct tempos_de_espera no_tempos; //guarda os tempos de execução

struct tempos_de_espera
{
    int tempo_esp;

    struct tempos_de_espera *prox_esp;

};

void inserir(int menor_fila, fila**inicio, fila**fim, int count_tempo);

void remover(int maior_fila, fila**inicio, no_tempos**inicio_t, int count_tempo);

int tamanho_fila(int i, fila*inicio);

void imprimir_fila (int i, fila*inicio);

void analise_result (no_tempos**inicio_tempos,int tempo_simul, int tam_filas[][num_serv],int total_clientes);

void simulacao(no_tempos**inicio_t, int aceleracao_simul);



int main ()
{
    setlocale(LC_ALL,"portuguese");

    int menu;

    no_tempos *inicio_t = NULL;

    int aceleracao_simul; //variavel que difere a simulação acelerada(sem print das filas) da normal (com print)


    do
    {
        textcolor(CIANO);
        printf("\n\n\t---------");
        printf("\n\t| Filas |");
        printf("\n\t---------");
        textcolor(BRANCO);

        printf("\n\n\tMENU\n\n");

        printf("\n\t1.Iniciar simulação");
        printf("\n\n\t2.Iniciar simulação acelerada");
        printf("\n\n\t0.Sair\n");

        printf("\n\n\tDigite a opcão: ");
        scanf("%d",&menu);

        switch (menu)
        {
            case 1:
            clrscr();
            aceleracao_simul = 0;
            simulacao(&inicio_t, aceleracao_simul);
            break;

            case 2:
            clrscr();
            aceleracao_simul = 1;
            simulacao(&inicio_t,aceleracao_simul);
            break;


            case 0:
            printf("\n\n\n\t");
            exit(1);

            default:
            textcolor(VERMELHO);
            printf("\n\n\tNúmero Inválido");
            textcolor(BRANCO);
            printf("\n\n\n\t");
            system("PAUSE");
            clrscr();
            break;
        }

    }while(menu!=0);

    printf("\n\n\n\n\n\t");
    system("pause");
    return 0;

}


void simulacao(no_tempos**inicio_t, int aceleracao_simul)
{
    int tempo_simul;
    printf("\n\n\tDigite o tempo de duração da simulação (em segundos): ");
    scanf("%d", &tempo_simul);

    printf("\n\n\tDigite o número de servidores: ");
    scanf("%d", &num_serv);

    if(num_serv == 0)
    {
        textcolor(VERMELHO);
        printf("\n\n\tNúmero de servidores inválido! Deve ser no mínimo 1.");
        textcolor(BRANCO);
        printf("\n\n\n\n\t");
        system("pause");
        clrscr();
        return;

    }


    int tam_filas [tempo_simul+1][num_serv]; //matriz para guardar o tempo de cada fila

    fila *inicio[num_serv];
    fila *fim[num_serv];


    for(int i=0; i<num_serv; i++)
    {
        inicio[i] = NULL;
        fim[i] = NULL;
    }

    int menor_fila = 9999;
    int id_menor_fila = 0;
    int id_maior_fila = 0;
    int maior_fila = 0;

    int count_tempo=0; //contador de tempo 'manual'

    int total_clientes=0;

    textcolor(VERDE);
    printf("\n\n\n\n\n\n\n\t\t\tIniciando Simulação...");
    textcolor(BRANCO);


    Sleep(1500);
    clrscr();

    if (aceleracao_simul == 0) //verificando o tipo de aceleração
    {
            do
            {


            for(int i=0; i<num_serv; i++) // pega o tamanho inicial da cada fila
            {
                tam_filas[count_tempo][i] = tamanho_fila(i,inicio[i]);
            }


            if ((count_tempo != 0) && (count_tempo %(1+ rand()%2) == 0)) // INSERE NAS FILAS APÓS UM TEMPO
            {

                menor_fila = 9999;

                for(int i=0; i<num_serv; i++)
                {
                     if(tam_filas[count_tempo][i] <= menor_fila) //verifica qual a menor fila e salva seu numero
                     {
                         menor_fila = tam_filas[count_tempo][i];
                         id_menor_fila = i;

                     }
                }

                inserir(id_menor_fila, &inicio[id_menor_fila], &fim[id_menor_fila], count_tempo); //chama o insere da menor_fila;
                total_clientes++;
            }


            if ((count_tempo != 0) && (count_tempo % 3 == 0)) // REMOVE NAS FILAS APÓS UM TEMPO
            {
                maior_fila = 0;

                for(int i=0; i<num_serv; i++)
                {
                     if(tam_filas[count_tempo][i] >= maior_fila) //verifica qual a maior fila e salva seu numero
                     {
                         maior_fila = tam_filas[count_tempo][i];
                         id_maior_fila = i;
                     }
                }

                remover(id_maior_fila, &inicio[id_maior_fila], inicio_t, count_tempo); // &*inicio- se anulam
            }

            //tamanho final da fila - após inserir e remover
            for(int i=0; i<num_serv; i++)
            {
                tam_filas[count_tempo][i] = tamanho_fila(i,inicio[i]);
            }


            //PRINT DA TELA
            printf("\n\n\t\tATENDIMENTO - FILAS");

            printf("\n\n\n");


            for(int i=0; i<num_serv; i++)
            {
                printf("\tServidor %d ",i+1);
                imprimir_fila (i,inicio[i]);
                printf("\n\n");

            }

            printf("\n\n\t%d segundos",count_tempo);


            Sleep(1000);
            clrscr();

            count_tempo++;


        } while(count_tempo <= tempo_simul);
    }

    else //função acelerada
    {
        textcolor(VERDE);
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\tRealizando Simulação...");
        textcolor(BRANCO);

            do
            {


            for(int i=0; i<num_serv; i++) // pega o tamanho inicial da cada fila
            {
                tam_filas[count_tempo][i] = tamanho_fila(i,inicio[i]);
            }


            if ((count_tempo != 0) && (count_tempo %(1+ rand()%2) == 0)) // INSERE NAS FILAS APÓS UM TEMPO
            {

                menor_fila = 9999;

                for(int i=0; i<num_serv; i++)
                {
                     if(tam_filas[count_tempo][i] <= menor_fila) //verifica qual a menor fila e salva seu numero
                     {
                         menor_fila = tam_filas[count_tempo][i];
                         id_menor_fila = i;

                     }
                }

                inserir(id_menor_fila, &inicio[id_menor_fila], &fim[id_menor_fila], count_tempo); //chama o insere da menor_fila;
                total_clientes++;
            }


            if ((count_tempo != 0) && (count_tempo % 3 == 0)) // REMOVE NAS FILAS APÓS UM TEMPO
            {
                maior_fila = 0;

                for(int i=0; i<num_serv; i++)
                {
                     if(tam_filas[count_tempo][i] >= maior_fila) //verifica qual a maior fila e salva seu numero
                     {
                         maior_fila = tam_filas[count_tempo][i];
                         id_maior_fila = i;
                     }
                }

                remover(id_maior_fila, &inicio[id_maior_fila], inicio_t, count_tempo); // &*inicio- se anulam
            }

            //tamanho final da fila - após inserir e remover
            for(int i=0; i<num_serv; i++)
            {
                tam_filas[count_tempo][i] = tamanho_fila(i,inicio[i]);
            }


            count_tempo++;


        } while(count_tempo <= tempo_simul);
    }


    //SIMULAÇÃO FINALIZADA

    Sleep(2000);
    clrscr();
    textcolor(VERDE);
    printf("\n\n\n\n\n\n\n\n\n\t\t\t-------------------------");
    printf("\n\t\t\t| Simulação Finaliazada |");
    printf("\n\t\t\t-------------------------");
    textcolor(BRANCO);

    printf("\n\n\n\n\n\n\n\n\t");
    system("pause");
    clrscr();

    analise_result(inicio_t, tempo_simul, tam_filas, total_clientes);


}

int tamanho_fila(int i, fila *inicio)
{
    int count_tamanho_fila = 0; //contador do tamanho da fila

    if(inicio == NULL) //não tem ninguem na fila, retorna zero
    {
        return (count_tamanho_fila);
    }

    if(inicio->prox == NULL) //só tem uma pessoa na fila
    {
        count_tamanho_fila = 1;
    }
    else //faz a contagem
    {
        count_tamanho_fila = 1;

        do
        {
            count_tamanho_fila++;
            inicio=inicio->prox;

        }while(inicio->prox != NULL);
    }


    return (count_tamanho_fila);

}


void inserir(int menor_fila, fila**inicio, fila**fim, int count_tempo)
{

   if(*inicio==NULL)
   {
       (*inicio) = (*fim) = (fila*)malloc(sizeof(fila));


           if(*inicio==NULL)
           {
                printf("HEAP OVERFLOW");
                exit(1);
           }


            (*inicio)->tempo_entrada=count_tempo;
            (*inicio)->prox=NULL;

            return;
    }

           fila* novo;

           novo = (fila*)malloc(sizeof(fila));

           if(novo==NULL)
           {
                printf("HEAP OVERFLOW");
                exit(1);
           }


           novo->tempo_entrada=count_tempo;
           novo->prox=NULL;

           (*fim)->prox = novo;
           (*fim) = novo;

}


void remover(int maior_fila, fila**inicio, no_tempos**inicio_t, int count_tempo)
{
    if(*inicio==NULL)
    {
        return;
    }


    no_tempos *auxt;

    auxt = (no_tempos*)malloc(sizeof(no_tempos));

    if(auxt==NULL)
        {
            printf("HEAP OVERFLOW");
            exit(1);
        }

    auxt->tempo_esp = ((count_tempo) - (*inicio)->tempo_entrada); //tempo de atendimento

    auxt->prox_esp = (*inicio_t);

    (*inicio_t)= auxt;

    fila *aux = (*inicio);
    (*inicio)=(*inicio)->prox;
    free(aux);

}


void imprimir_fila (int i, fila*inicio)
{
    int tam_fila;

    tam_fila = tamanho_fila(i,inicio);

    if(tam_fila == 0)
    {
        printf("\n\t----------------------------------------------------------");
        printf("\n\tFILA VAZIA");
        printf("\n\t----------------------------------------------------------");

        return;
    }


    printf("\n\t----------------------------------------------------------");
    printf("\n\tFILA: ");

    int a = 164;

    for(int i=1; i<=tam_fila; i++)
    {
        textcolor(CIANO);
        printf("%c ",a);
    }

    textcolor(BRANCO);

    printf("\n\t----------------------------------------------------------");
}


void analise_result (no_tempos**inicio_tempos,int tempo_simul, int tam_filas[][num_serv], int total_clientes)
{
    textcolor(VERDE);
    printf("\n\n\t\t\tRELATÓRIO");
    textcolor(BRANCO);

    no_tempos *inicio_t;

    inicio_t = (*inicio_tempos);

    int count_clientes_atendidos = 1; // para usar na divisão do tempo médio - count dos clientes atendidos

    //primeiro, vemos o tempo médio, min e max de espera

    if(inicio_t == NULL)
    {
        printf("\n\n\t----------------------------------------------------------");
        printf("\n\n\tNenhum cliente foi atendido");
    }
    else
    {
        int maior_t = 0, menor_t = 9999;
        float tempo_medio = 0;

        maior_t = inicio_t->tempo_esp;
        menor_t = inicio_t->tempo_esp;

        tempo_medio = tempo_medio + inicio_t->tempo_esp;

        if(inicio_t->prox_esp == NULL)
        {

            printf("\n\n\t----------------------------------------------------------");

            textcolor(VERDE);
            printf("\n\n\tTempos de Atendimento");
            textcolor(BRANCO);

            printf("\n\n\tMenor Tempo: %d segundos",menor_t);
            printf("\n\n\tMaior Tempo: %d segundos",maior_t);
            printf("\n\n\tTempo Médio: %.2f segundos",tempo_medio);

        }
        else
        {

            while(inicio_t->prox_esp != NULL)
            {
                inicio_t = inicio_t->prox_esp; // vai para o prox da lista
                count_clientes_atendidos++;

                if(inicio_t->tempo_esp>maior_t)
                {
                    maior_t=inicio_t->tempo_esp;
                }


                if(inicio_t->tempo_esp<menor_t)
                {
                    menor_t=inicio_t->tempo_esp;
                }

                tempo_medio = tempo_medio + inicio_t->tempo_esp;

            }

            tempo_medio = tempo_medio/count_clientes_atendidos;

            printf("\n\n\t----------------------------------------------------------");

            textcolor(VERDE);
            printf("\n\n\tTempos de Atendimento");
            textcolor(BRANCO);

            printf("\n\n\tMenor Tempo: %d segundos",menor_t);
            printf("\n\n\tMaior Tempo: %d segundos",maior_t);
            printf("\n\n\tTempo Médio: %.2f segundos",tempo_medio);
        }
    }


        //depois, vemos o tamanho médio das filas


    int soma_tam_filas = 0;

    for(int i=0; i<num_serv; i++)
    {
        for(int j=0; j<tempo_simul+1; j++)
        {
            soma_tam_filas = soma_tam_filas + tam_filas[j][i];
        }
    }


    if(soma_tam_filas == 0)
    {
        printf("\n\n\t----------------------------------------------------------");
        printf("\n\n\tSem clientes");
        printf("\n\n\t----------------------------------------------------------\n");
    }
    else
    {
        int maior_tam = 0, menor_tam = 9999;
        float tamanho_medio = 0;

        for(int i=0; i<num_serv; i++)
        {
            for(int j=0; j<tempo_simul+1; j++)
            {
                if(tam_filas[j][i] >= maior_tam)
                {
                    maior_tam = tam_filas[j][i];
                }

                if(tam_filas[j][i] <= menor_tam)
                {
                    menor_tam = tam_filas[j][i];
                }
            }
        }


        tamanho_medio = (float) (soma_tam_filas  / (num_serv * (tempo_simul+1)));


        printf("\n\n\t----------------------------------------------------------");

        textcolor(VERDE);
        printf("\n\n\tTamanhos das Filas");
        textcolor(BRANCO);

        printf("\n\n\tMenor Tamanho: %d",menor_tam);
        printf("\n\n\tMaior Tamanho: %d",maior_tam);
        printf("\n\n\tTamanho Médio: %.2f",tamanho_medio);

        printf("\n\n\t----------------------------------------------------------");



    }

    textcolor(VERDE);
    printf("\n\n\tTotal de clientes: %d",total_clientes);
    printf("\n\n\tTotal de clientes atendidos: %d",count_clientes_atendidos);
    textcolor(BRANCO);

    free(*inicio_tempos);

    printf("\n\n\n\t");
    system("pause");
    clrscr();

}










