#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define NONECOLOR "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"


int directorieNum=0; 
int fileNum=0;
int softLinkNum=0;
//char *blockSize; 
long int blockSize=0;
int first=1;//TURE

int fileSort(char *dir){
	DIR *dp;
    struct dirent *entry;
	int i=0;
	dp = opendir(dir);
	
	//printf("%s\n",dir);
    while((entry = readdir(dp)) != NULL) {     
		if(strcmp(".",entry->d_name) == 0 ||  strcmp("..",entry->d_name) == 0)
                continue;	
	//printf("%s\n",entry->d_name);
        i++;
    }
	
    closedir(dp);
	return i;
}


void printSpace(int spaces,char *name,int end,char *color){
	
	char *output=(char *)malloc(1000);
	char *temp=(char *)malloc(1000);
	//strcpy(output,NONECOLOR);//reset color
	strcpy(temp,name);
	strcat(temp,NONECOLOR);//reset color
	strcpy(output,temp);
	strcpy(temp,color);//set color
	strcat(temp,output);
	strcpy(output,temp);
	
	if(end<= 1){
		strcpy(temp,"└── ");
	}
	else{
		strcpy(temp,"├── ");
	}
	strcat(temp,output);
	strcpy(output,temp);
	while(spaces>0){
		if(spaces%2!=0)
			strcpy(temp,"│    ");
		else
			strcpy(temp,"     ");
		
	strcat(temp,output);
	strcpy(output,temp);
		
		spaces/=2;
	}
	printf("%s\n",output);
}

void printdir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int spaces = depth;
	directorieNum++;
	
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
	int i=fileSort(dir);
    chdir(dir);
	
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
		/*if(first){			
			blockSize=statbuf.st_blksize;
			//strcpy(blockSize,statbuf.st_blksize);
			first=0;
		}*/
		blockSize+=statbuf.st_blksize;
		if (S_ISLNK(statbuf.st_mode))
			softLinkNum++;
		
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
           if(strcmp(".",entry->d_name) == 0 || 
                strcmp("..",entry->d_name) == 0)
                continue;       
			
			
			printSpace(depth,entry->d_name,i--,BLUE);
            /* Recurse at a new indent level */
			if(i<1)
				spaces=depth<<1;
			else
				spaces=depth<<1|1;
			
			
			printdir(entry->d_name,spaces);
        }
        else{ 
			fileNum++;
			printSpace(depth,entry->d_name,i--,NONECOLOR);
			
		};
    }
    chdir("..");
    closedir(dp);
}

/*  Now we move onto the main function.  */

int main(int argc, char* argv[])
{
    char *topdir, pwd[2]=".";
    if (argc != 2)
        topdir=pwd;
    else
        topdir=argv[1];

    printf("\033[0;32;34m%s\033[m\n",topdir);
	
    printdir(topdir,0);
   printf("directories: %d ,files: %d ,soft links: %d ,blocks: %ld\n",directorieNum,fileNum,softLinkNum,blockSize);

    return 0;
}