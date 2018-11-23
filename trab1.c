#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


//Codigo certo, cria 9 processos mais o pai

int main(){
int i , ret;

for(i = 0 ; i < 9 ; i++)
{
  ret = fork();
  if (ret == -1) { printf("erro no fork\n"), exit(1);}
  if(ret == 0)
  {
     printf("FILHO:%d PID:%d ,PPID:%d\n", ++i, getpid(), getppid());
     break;
  }
}
if(ret != 0)
printf("PAI: PID: %d PPID: %d\n", getpid(), getppid());

sleep(5);

}


