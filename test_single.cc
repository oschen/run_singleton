#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<libgen.h>
#include<iostream>
#include<dirent.h>

#define MAX_PATH_LEN 1024
#define MAX_CMD_LEN  512
#define MAX_PROC_LEN 100

using namespace std;

bool is_prog_running()
{
   long pid;
   pid=getpid();
   cout<<"pid: "<<pid<<endl;
   char path[MAX_PATH_LEN]={0};
   char progname[MAX_PROC_LEN]={0};
   char cmdline[MAX_CMD_LEN]={0};
   char *name1,*name2;
   int fd;
   sprintf(path,"/proc/%ld/cmdline",pid);
   if(access(path,F_OK)==0)
      {
         fd=open(path,O_RDONLY);
         if(fd==-1)
           return false;
        else
        {
          read(fd,cmdline,MAX_CMD_LEN);
          close(fd);
          int i=0;
          char *pt=cmdline;
          while(*pt!=' ' && *pt!='\0')
          {
            progname[i++]=*pt;
            pt++;
          }
          char *current_pro=strdup(progname);
          name1=basename(current_pro);

          DIR *dir;
          struct dirent *result;
          dir=opendir("/proc");
          while((result=readdir(dir))!=NULL)
          {
            if(!strcmp(result->d_name,".") || !strcmp(result->d_name,"..") || !strcmp(result->d_name,"self") || atol(result->d_name)==pid)
               continue;
            memset(cmdline,0,MAX_CMD_LEN);
            memset(path,0,MAX_PATH_LEN);
            memset(progname,0,MAX_PROC_LEN);
            sprintf(path,"/proc/%s/cmdline",result->d_name);
            if(access(path,F_OK)==0)
            {
              fd=open(path,O_RDONLY);
              if(fd==-1)
                continue;
              read(fd,cmdline,MAX_CMD_LEN);
              close(fd);
              char *qt=cmdline;
              int j=0;
              while(*qt!=' '&&*qt!='\0')
              {
                progname[j++]=*qt;
                qt++;
              }
              char *other_pro=strdup(progname);
              name2=basename(other_pro);
              if(strcmp(name1,name2)==0)
              {
                 
                  return true;
              }

            }

          }
          return false;


        }
      }
   else
      {
        return false;
      }


}

int main(int argc,char *argv[])
{
  if(is_prog_running())
  {
    printf("the prog is already running\n");
    exit(1);
  }
  while(true)
  {
    printf("program to be started\n");
    sleep(2);
  }
  return 0;
}
