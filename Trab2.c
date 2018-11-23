#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>


struct sembuf operacao[1];
int idsem;

void p_sem1();
void v_sem1();
void p_sem2();
void v_sem2();


void p_sem2 ()
{
  operacao[0].sem_num = 0;
  operacao[0].sem_op = -1;
  operacao[0].sem_flg = 0;
  if (semop (idsem, operacao, 1) < 0)
    printf ("erro P2 no p=%d\n", errno);
}

void v_sem2 ()
{
  operacao[0].sem_num = 0;
  operacao[0].sem_op = 1;
  operacao[0].sem_flg = 0;
  if (semop (idsem, operacao, 1) < 0)
    printf ("erro V2 no p=%d\n", errno);
}

void p_sem1 ()
{
  operacao[0].sem_num = 1;
  operacao[0].sem_op = -1;
  operacao[0].sem_flg = 0;
  if (semop (idsem, operacao, 1) < 0)
    printf ("erro P1 no p=%d\n", errno);
}

void v_sem1 ()
{
  operacao[0].sem_num = 1;
  operacao[0].sem_op = 1;
  operacao[0].sem_flg = 0;
  if (semop (idsem, operacao, 1) < 0)
    printf ("erro V1 no p=%d\n", errno);
}

int main ()
{
  int pid, idshm;
  struct shmid_ds buf;
  int *pshm;
  int i;

/* cria memória */
if ((idshm = shmget(0x1223, sizeof(int), IPC_CREAT|0x1ff)) < 0)
  { 
    printf("erro na criação da memória\n"); 
    exit(1);
  }

pshm = (int *) shmat(idshm, (char *)0, 0);
if (pshm == (int *) -1)
	{
    printf("erro no endereço da memória\n"); 
    exit(1);
  }

/* cria semáforo */
if ((idsem = semget (0x1111, 2, IPC_CREAT|0x1ff)) < 0)
  {
    printf ("erro na criação do semáforo\n");
    exit (1);
  }

/* codigo do filho */
pid = fork();
if (pid < 0)
	printf("Fork error.\n");
if (pid == 0)
  {
    for(i = 0 ; i <= 19 ; i++)
	  {
      p_sem1();
      printf("filho - obtive o semáforo\n");
      printf("filho - recebi do pai %d\n", *pshm);
      v_sem2();
    }
  }	

/* codigo do pai */
if (i < 19)
{
for(i = 0 ; i <= 19 ; i++)
{  
	*pshm = i;
  printf("PAI - registrei um novo número\n");  
  printf("PAI - escrevi %d para o filho\n", *pshm);
  v_sem1();
  p_sem2();
}   
}else
{
  printf("Fim das mensagens\n");
}

sleep(5);

shmctl (idshm, IPC_RMID, 0);
semctl (idsem, IPC_RMID, 0);
printf("Término do programa\n");


exit (0);
}