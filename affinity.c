 #define _GNU_SOURCE 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include<sys/syscall.h>
#include<sched.h>
#include<sched.h>
#include<limits.h>
#include<sys/sysinfo.h>
#include<string.h>
#include<errno.h>

void *thrfun(void *);
#define THNUM 4
int main(int argc,char *argv[])
{
  
  pthread_t pth[THNUM];
  int cpus[THNUM]={0,1,2,3};
  int loop;
  for(loop=0;loop<THNUM;++loop){
    pthread_create(&pth[loop],NULL,thrfun,&cpus[loop]);
  }
  for(loop=0;loop<THNUM;++loop){
    pthread_join(pth[loop],NULL);
  }
  return 0;
}

void *thrfun(void *arg)
{
  unsigned long max=ULONG_MAX-1,loop=0;
  int  cpunum=get_nprocs_conf();
  int tid=syscall(SYS_gettid);
  int cpu;
  cpu_set_t set,set2;
  int find=0;
  CPU_ZERO(&set2);
  CPU_SET(*((int *)arg),&set2);
  if(sched_setaffinity(tid,sizeof(cpu_set_t),&set2)!=0){
    fprintf(stderr,"tid %d error sched_setaffinity error :%s\n",tid,strerror(errno));
    exit(1);
  }
  for(loop=0;loop<max;++loop){
    if(loop%INT_MAX==0){
      CPU_ZERO(&set);
      if(sched_getaffinity(tid,sizeof(cpu_set_t),&set)!=0){
	fprintf(stderr,"tid %d : sched_getaffinity error:%s\n",tid,strerror(errno));
	exit(1);
      }
      for(cpu=0;cpu<cpunum;++cpu){
	if(CPU_ISSET(cpu,&set)){
	  fprintf(stdout,"%d run in core: %d\n",tid,cpu);
	  find=1;
	}
      }
      if(!find){
	fprintf(stderr,"not find cpu core\n");
	exit(1);
      }
    }
  }
  return NULL;
}

  
