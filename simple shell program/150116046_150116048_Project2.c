#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#define CREATE_FLAGS ( O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */

/* The setup function below will not return any value, but it will just: read
in the next command line; separate it into distinct arguments (using blanks as
delimiters), and set the args array entries to point to the beginning of what
will become null-terminated, C-style strings. */

void setup(char inputBuffer[], char *args[],int *background , int *ctk)
{
    int length, /* # of characters in the command line */
        i,      /* loop index for accessing inputBuffer array */
        start,  /* index where beginning of next command parameter is */
        ct;     /* index of where to place the next parameter into args[] */

    ct = 0;

    /* read what the user enters on the command line */
    length = read(STDIN_FILENO,inputBuffer,MAX_LINE);

    /* 0 is the system predefined file descriptor for stdin (standard input),
       which is the user's screen in this case. inputBuffer by itself is the
       same as &inputBuffer[0], i.e. the starting address of where to store
       the command that is read, and length holds the number of characters
       read in. inputBuffer is not a null terminated C-string. */

    start = -1;
    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */

    /* the signal interrupted the read system call */
    /* if the process is in the read() system call, read returns -1
      However, if this occurs, errno is set to EINTR. We can check this  value
      and disregard the -1 value */
    if ( (length < 0) && (errno != EINTR) )
    {
        perror("error reading the command");
        exit(-1);           /* terminate with error code of -1 */
    }

    printf(">>%s<<",inputBuffer);
    for (i=0; i<length; i++) /* examine every character in the inputBuffer */
    {

        switch (inputBuffer[i])
        {
        case ' ':
        case '\t' :               /* argument separators */
            if(start != -1)
            {
                args[ct] = &inputBuffer[start];    /* set up pointer */
                ct++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

        case '\n':                 /* should be the final char examined */
            if (start != -1)
            {
                args[ct] = &inputBuffer[start];
                ct++;
            }
            inputBuffer[i] = '\0';
            args[ct] = NULL; /* no more arguments to this command */
            break;

        default :             /* some other character */
            if (start == -1)
                start = i;
            if (inputBuffer[i] == '&')
            {
                *background  = 1;
                inputBuffer[i-1] = '\0';
            }
        } /* end of switch */
    }    /* end of for */
    args[ct] = NULL; /* just in case the input line was > 80 */
	*ctk=ct;
    for (i = 0; i <= ct; i++)
        printf("args %d = %s\n",i,args[i]);
} /* end of setup routine */


//Global variables to check foreground processes 
int isThereFg;
int fgid;
int fgid2;
//The handler function for ctrl-z signal
void handleCtrlZ(int num)
{
    int st; //status (for waitpid parameter)
    if(isThereFg) //if there is foreground process
    {
        kill(fgid, 0);//first kill the process and then check error condition; if it is not running it is ESRCH
        if(errno == ESRCH)
        {
            printf("Cannot find process %d\n", fgid);//Print error message
            isThereFg = 0;//there is no foreground processes anymore
            fflush(stdout);
        }
        else
        {
            kill(fgid, SIGKILL);//kill the process
            waitpid(-fgid, &st, WNOHANG); //it is for waiting all processes that are created in that foreground process
            isThereFg = 0;//no foreground processes anymore
        }
    }
    fflush(stdout);

}

int main(void)
{
    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/2 + 1]; /*command line arguments */
	int ctk=0;
    int h;
    char hist[10][25];//it is to hold last 10 commands 
    for(h=0; h<10; h++)//firstinitialize them (just random valur like "aa")
    {
        strcpy(hist[h],"aa");
    }

   char* s = getenv("PATH");//it simply take path environment variable and assign it to s
    int len=strlen(s),p=1,i=0,j,k,y,z,t,o=1,l;//some assignments
    char delim[] = ":";//to split all paths by ':' cahracter
    char *ptr[30];//it is to keep different paths
    ptr[0] = strtok(s, delim);//split and assign paths
	int fd;//open file
    while((ptr[p] = strtok(NULL, delim))!=NULL)
    {
        p++;
    }
    z=p;//z indicates the last index of paths
    int a=0;
    char temp[20];
	char file[30] = "";
    struct dirent *de;  // Pointer for directory entry
    DIR *dr;

	//for signal handling
    int er0,er1;
    struct sigaction act; //declare a sigaction struct
    act.sa_handler = handleCtrlZ;//assign handler as function written above
    act.sa_flags = SA_RESTART;
    //simply for error conditions
    er0=sigemptyset(&act.sa_mask);
    er1=sigaction(SIGTSTP, &act, NULL);
    if (( er0==-1) || (er1==-1))
    {
        printf("Cannot handle ctrl-z signal!\n");//if error inform user and terminate
        exit(-1);
    }


    while (1)
    {
        background = 0;
        printf("myshell: ");
        /*setup() calls exit() when Control-D is entered */
        setup(inputBuffer, args, &background, &ctk);
		int wr=0;
		int app=0;
		int read=0;
		int err=0;
		int sayi2=0;
		int dup=0;
		char sign[] =" ";
		// C) redirection part
		for(int sayi=0; sayi<ctk; sayi++){
			//put value of integer based  on operation
			wr=!(strcmp(args[sayi], ">"));
			app=!(strcmp(args[sayi], ">>"));
			err=!(strcmp(args[sayi], "2>"));
			read=!(strcmp(args[sayi], "<"));
			if(app | wr | read ){
				strcpy(sign, args[sayi]);
				strcpy(file, args[sayi+1]);
				printf("file: %s \n", file);
				dup=1;
				printf("read %d   ", read);
				printf("write %d   ", wr);
				if(read);
					sayi2=sayi;
				args[sayi]=NULL;
				break;
			}
			//error check
			if(err) {
				strcpy(sign, args[sayi]);
				strcpy(file, args[sayi+1]);
				printf("file: %s \n", file);
				dup=1;
				args[sayi+1]=NULL;
				break;
			}
			
        //firstly check if argument is history, that is, one of built-in commands
		}
        if(strcmp(args[0],"history")==0)
        {
            if(args[1]!=NULL && strcmp(args[1],"-i")==0) //if so check if it is to print history or to run one of command at specific index
            {
               
                char temp2[25];
                strcpy(temp2,hist[atoi(args[2])]);
                strcpy(delim," ");//to split command into arguments like if it is "ls -l" take ls and l seperately

                args[0] = strtok(temp2, delim);


                while((args[o] = strtok(NULL, delim))!=NULL)
                {

                    o++;
                }

                args[o]=NULL;//make next one NULL so that it cannot be reached

                o=1;

                goto cnt;//and go to execv part which is in A part of project
            }

            for(k=0; k<10; k++)
            {
                if(strcmp(hist[k],"aa")==0)
                    break;
                printf("%d: %s\n",k,hist[k]);
            }

        }
		//if it is path
        else if(strcmp(args[0],"path")==0)
        {
            if(args[1]==NULL)//simply prints all paths with : seperator
            {
                printf("\n");
                printf("%s",ptr[0]);
                for(y=1; y<z; y++)
                {
                    printf(":%s",ptr[y]);
                }
                printf("\n");
                continue;
            }
            else if(strcmp(args[1],"+")==0)//to add a path to our path variable
            {
                ptr[z]=(char *)malloc(25);
                strcpy(ptr[z],args[2]);
                //  printf("%s\n",ptr[z]);
                z++;
                continue;
            }
            else if(strcmp(args[1],"-")==0)//to delete a path - it also deletes all duplicates of it -
            {
                // printf("%s\n",ptr[z-1]);
                for(y=0; y<z; y++)
                {
                    for(l=y; l<z; l++)
                    {
                        if(strcmp(ptr[l],args[2])==0)
                        {
                            for(t=l; l<z; l++)
                            {
                                ptr[l]=ptr[l+1];
                            }
                            ptr[--z]=NULL;
                        }
                    }
                }
                continue;
            }
        }
		//if it is exit
        else if(strcmp(args[0],"exit")==0)
        {
            if(waitpid(-1,NULL,WNOHANG)==-1)//then check if there is any processes or not
            {
                exit(0);
            }
            else//inform user
            {
                printf("There are still backround processes executing. First terminate all of them and then try again!\n");
                continue;
            }
        }

        //if it is fg command
        else if(strcmp(args[0], "fg")==0)
        {
            char *pt;
            strcpy(pt,args[1]);
            strcpy(delim,"%");//to delete % from id and turn it to integer
            int b_pid=atoi(strtok(pt,delim));

            if(waitpid(b_pid,NULL,WUNTRACED)==-1)//if cannot wait for that process inform user
            {
                printf("The process with id %d is either terminated or cannot be waited!\n",b_pid);
            }
            else
            {
                isThereFg=1;//otherwise there is a foreground process now
                fgid=b_pid;
            }
            continue;
        }

        //if it is not build-in commands
        else
        {

cnt:
            for(p=0; p<30; p++)//it goes through all paths
            {

                if(a)//if arguments is found one of paths it breaks and donot continue to search
                {
					
                    a=0;
                    break;
                }
                dr = opendir(ptr[p]);//open that path directory
                if (dr == NULL)  //if cannot open directory
                {
                    return 0;
                }
                int ll;
                // for readdir()
                while ((de = readdir(dr)) != NULL)//if the file is the argument given
                {
                    if(strcmp(de->d_name,args[0])==0)
                    {
						//this part is simply to add that command to history
                        if(i==0)
                        {
                            strcpy(hist[i],args[0]);
                            for(k=1; args[k]!=NULL; k++)
                            {
                                strcat(hist[i]," ");
                                strcat(hist[i],args[k]);

                            }
                        }
                        else
                        {

                            for(k=i; k>=0; k--)
                            {

                                strcpy(hist[k],hist[k-1]);

                            }
                            strcpy(hist[0],args[0]);

                            for(k=1; args[k]!=NULL; k++)
                            {
                                strcat(hist[0]," ");
                                strcat(hist[0],args[k]);

                            }
                        }
                        //if the argument is found than to execute it modify path accordingly
                        strcpy(temp,ptr[p]);
                        strcat(temp,"/");
                        strcat(temp,args[0]);

                        int childpid;
						int childpid2;

      					//do c part if it is in the command
						if(app | err | wr | read){


							char file_arg[30];
							file_arg[0]=' ';
							
							int y_args=1;
							pid_t kidpid;
							if((childpid2=fork())==-1)
                        	{
                        	    printf("Could not create\n");
                        	    return 0;
                        	}
							//child process
							if(childpid2==0){
								
								int fd=0; 
								if(wr  | err)
									fd = open(file, O_RDWR |  O_TRUNC | O_CREAT , S_IRUSR | S_IWUSR);
								if(app)
									fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
								/*if(read)
									fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRUSR |S_IWUSR);*/
								if(fd==-1){
									perror("failed to open file");
									return 1;
								}

								if(err){
    								dup2(fd, 1); // make stdout go to file
								
    								dup2(fd, 2); // make stderr go to file -					}
                			    	//error check
									if (close(fd)==-1){
										perror("failed to close");
										return 1;
									}
								}
    							else if(wr | app) {
									dup2(fd, 1);
									if (close(fd)==-1){
										perror("failed to close");
										return 1;
									}
								}
								else if (read){
								    //char * buffer = 0;
									//long length;
									
									FILE * fPtr = fopen (file, "r");
									char r_ch;
									if(fPtr == NULL)
    								{
        								printf("Unable to open file.\n");
    								    printf("Please check whether file exists.n");
        								exit(EXIT_FAILURE);
    								}
									//reads the file
									while (r_ch!=EOF)
									{
										r_ch = fgetc(fPtr);
										file_arg[y_args]=r_ch;
										y_args++;
										
									}
									fclose (fPtr);
									//execv(temp,&args[0]);
								}

							}
							//parent process(firstly parent exec the command then child do file operation)
                        	if(childpid2>=0){

									if(read){
										wait(&childpid2);
										printf("parent temp: %s ", temp);
										printf("parent file_arg: %s ", file_arg);
										args[sayi2]=file_arg;
										execv(temp,&args[0]);
									}
									else{
										
										execv(temp,&args[0]);
										wait(&childpid2);  
									}
									
							}
							if(background==0)
                        	{
                            	wait(&childpid2);
                        	}
							a=1;
							/*if(background==0)//background checking - if foreground wait otherwise donot -
                        	{
                            	isThereFg2=1;
                        	    fgid2=childpid2;
                            	wait(&childpid2);
                        	}*/
						}
						else{
							if((childpid=fork())==-1)//cretae child process
                       		{
                       		    printf("Could not create\n");
                            	return 0;
                        	}
                        	if(childpid==0)
                            	execv(temp,&args[0]);//child is executing the command
							a=1;
							if(background==0)//background checking - if foreground wait otherwise donot -
                        	{
                            	isThereFg=1;
                        	    fgid=childpid;
                            	wait(&childpid);
                        	}
						}
                        
                        if(i<9)//for history part
                            i++;
                        break;
                    }

                }

                closedir(dr);
            }
        }

    }

    /** the steps are:
    (1) fork a child process using fork()
    (2) the child process will invoke execv()
    (3) if background == 0, the parent will wait,
    otherwise it will invoke the setup() function again. */
}


