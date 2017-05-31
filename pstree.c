# include <stdio.h>
# include <malloc.h>
# include <dirent.h>
# include <string.h>
struct PSTREE
{
	int pid,ppid;
	struct PSTREE *link;
};

typedef struct PSTREE PT;
PT *ptree=NULL;

void addToPT(int pid,int ppid)
{
	PT *newnode,*temp;
	newnode=(PT*)malloc(sizeof(PT));
	newnode->pid=pid;
	newnode->ppid=ppid;
	newnode->link=NULL;
	if(ptree==NULL)
		ptree=newnode;
	else
	{
		for(temp=ptree;temp->link!=NULL;temp=temp->link);
		temp->link=newnode;
	}
}

void checkForProcess(char *pid)
{
	//printf("%s recieved\n",pid);
	int pids,i,j,ppids;
	FILE *fp;
	pids=atoi(pid);
//	printf("%d converted\n",pids);	
	char filepath[20],ch,temp[20],newpid[20];
	strcpy(filepath,"/proc/");
	//printf("%s ",filepath);
	if (pids>0)
	{	
		for(i=0;pid[i]!='\0';i++)
		{
			if(pid[i]=='\n')
				pid[i]=pid[i+1];
		}
		strcat(filepath,pid);
		strcat(filepath,"/status");
//		printf("%s generated\n",filepath);
		fp=fopen(filepath,"r");
		while(!feof(fp))
		{	
//			ch=fgetc(fp);
//			printf("%c",ch);
			fgets(temp,20,fp);
			if(strstr(temp,"PPid:\t")-temp==0)
			{
//				printf("%s Match Found\n",temp);
				for(i=0,j=6;temp[j]!='\0';i++,j++)
				{
					newpid[i]=temp[j];
				}
				newpid[i]='\0';
				ppids=atoi(newpid);
//				printf("PID:%d PPID:%d\n",pids,ppids);
				addToPT(pids,ppids);
				if(ppids==0)
				{
					fclose(fp);
					break;
				}
				else
				{
					fclose(fp);
					checkForProcess(newpid);	
				}
			}			
		}
		
//		if(!feof(fp))
//			printf("Success ");
//		fclose(fp);
	}
}

void main (int a,char **args)
{
	FILE *fp,*fptr,*dot;
	DIR *dir;
	struct dirent *ent;
	char path[20];
	PT *temp;
	char folderpath[20];
	strcpy(folderpath,"/proc");
	strcpy(path,folderpath);
	//printf("%s\n%s",path,args[1]);
	int count=0;
/*	if ((dir=opendir(path))!=NULL)
        {
                while((ent=readdir(dir))!=NULL)
                {       if(ent->d_type==DT_DIR)
                        {
//                              printf ("%s  ",ent->d_name);
                                count++;
                        }
                }
        closedir (dir);
        }*/
	fptr=fopen("output.txt","w");
	dot=fopen("graph.dot","w");
	fprintf(dot,"digraph pstree{\n");
	if ((dir=opendir(path))!=NULL) 
	{
		while((ent=readdir(dir))!=NULL)
		{	if(ent->d_type==DT_DIR)
			{	ptree=NULL;
//  				printf ("%s  ",ent->d_name);
				checkForProcess(ent->d_name);
			}	
			for(temp=ptree;temp!=NULL;temp=temp->link)
			{
				if(temp==ptree)
				{
					printf("%d->%d",temp->pid,temp->ppid);
					fprintf(fptr,"%d->%d",temp->pid,temp->ppid);
				}
				else
				{
					printf("->%d",temp->ppid);
					fprintf(fptr,"->%d",temp->ppid);
				}
				fprintf(dot,"\t\t%d->%d;\n",temp->pid,temp->ppid);
			}
			printf("\n\n");
			if(ptree!=NULL)
				fprintf(fptr,"\n");
  		}
  	closedir (dir);
	}
	else 
	{
  		printf("\nError...");
	}
	fprintf(dot,"\t}");
//	fclose(fptr);
//	fclose(fp);
//	printf("Count=%d\n",count);
}	
