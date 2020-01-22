#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <ctype.h>


void *read(void *param);
void *upper(void *param);
void *replace(void *param);
void *write(void *param);

char arr[100][60];
int arr1[100];
int flags[100][4];
int arrRin,arrUin,arrRpin,arrWin,end=1,rCount,uCount,rpCount,wCount;
sem_t rw_mutex;
sem_t r_mutex;
sem_t rpu_mutex;
sem_t u_mutex;
sem_t rp_mutex;
sem_t w_mutex;


FILE *f;
FILE *f2;

int totLine,seekValue;
int main(int argc,char *argv[])
{
    sem_init(&rw_mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&r_mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&rpu_mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&u_mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&rp_mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    sem_init(&w_mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
    //argument and format check
    if(argc<8)
    {
        printf("Not enough argument!!Format is like ./a.o -d deneme.txt -n 15 5 6 3");
        exit(1);
    }
    char fName[20];
    int numR=1,numU=1,numRp=1,numW=1;
    strcpy(fName,argv[2]);
    char chr;
    //count number of lines
    f=fopen(fName,"r");
    chr = getc(f);
int g=0;
int h=1;
int sums=0;
    while (!feof(f))
    {
g++;
        //Count whenever new line is encountered
        if (chr == '\n')
        {
            totLine++;
sums+=g;
arr1[h]=sums;
g=0;
h++;

        }
        //take next character from file.
        chr = getc(f);
    }
    fclose(f);
    numR=atoi(argv[4]);
    numU=atoi(argv[5]);
    numRp=atoi(argv[6]);
    numW=atoi(argv[7]);
    f=fopen(fName,"r");
    f2=fopen(fName,"r+");
    int i;
    //definitions
    pthread_t rTid[numR],uTid[numU],rpTid[numRp],wTid[numW];
    pthread_attr_t rAttr[numR],uAttr[numU],rpAttr[numRp],wAttr[numW];
    //create threads with given numbers
    for(i=0; i<numR; i++)
    {
        pthread_attr_init(&rAttr[i]);
        pthread_create(&rTid[i],&rAttr[i],read,fName);
    }
    for(i=0; i<numU; i++)
    {
        pthread_attr_init(&uAttr[i]);
        pthread_create(&uTid[i],&uAttr[i],upper,fName);
    }
    for(i=0; i<numRp; i++)
    {
        pthread_attr_init(&rpAttr[i]);
        pthread_create(&rpTid[i],&rpAttr[i],replace,fName);
    }

    for(i=0; i<numW; i++)
    {
        pthread_attr_init(&wAttr[i]);
        pthread_create(&wTid[i],&wAttr[i],write,fName);
    }

//wait for all of threads to exit
    for(i=0; i<numR; i++)
    {
        pthread_join(rTid[i],NULL);
    }

    fclose(f);

    for(i=0; i<numU; i++)
    {
        pthread_join(uTid[i],NULL);
    }
    for(i=0; i<numRp; i++)
    {
        pthread_join(rpTid[i],NULL);
    }
    for(i=0; i<numW; i++)
    {
        pthread_join(wTid[i],NULL);
    }
    fclose(f2);
}


//function for reader thread
void *read(void *param)
{
    int in,i;
    do
    {
        //critical section of read thread rCount common variable for all readers
  if(arrRin>=totLine)//if all lines are read then exit
            pthread_exit(0);
        sem_wait(&r_mutex);
        rCount++;
        flags[arrRin][0]=1;
        in=arrRin;//index
        arrRin++;
        if(rCount==1)
            sem_wait(&rw_mutex);//if it is the first reader wait for writer to end first

        sem_post(&r_mutex);

        //arr1[in]=seekValue;
        fgets(arr[in],sizeof(arr[in]),f);//read a line from file and inform user
        printf("Read Thread id:%ld index:%d stores line %s to array\n",pthread_self(),in,arr[in]);
        sem_wait(&r_mutex);
        rCount--;
//seekValue+=strlen(arr[in]);
//arr1[in]=seekValue;
        if(rCount==0)//if last reader ends then release the lock for writers to enter
            sem_post(&rw_mutex);
        sem_post(&r_mutex);
       // if(arrRin>=totLine)//if all lines are read then exit
         //   pthread_exit(0);

    }
    while(1);

}


//function for upper thread
void *upper(void *param)
{

    int in,i=0;
    char temp[50];
    do
    {
 if(arrUin>=totLine)//if all lines are modified exit
            pthread_exit(0);
        //critical section of upper threads
        sem_wait(&u_mutex);
        uCount++;
        in=arrUin;
        arrUin++;
        if(uCount==1)
            sem_wait(&rpu_mutex);

        sem_post(&u_mutex);
        if(arrRin>0 && in<arrRin && flags[in][1]==0)//if there is at least one entry in the array
        {
            strcpy(temp,arr[in]);
            i=0;
            while(arr[in][i])//make all letters upper one by one through loop and inform user
            {
                arr[in][i]=toupper(arr[in][i]);
                i++;
            }
            printf("Upper Thread id:%ld index:%d changed line %s to %s in array\n",pthread_self(),in,temp,arr[in]);

        }
        sem_wait(&u_mutex);
        uCount--;
        flags[in][1]=1;
        if(uCount==0)
            sem_post(&rpu_mutex);
        sem_post(&u_mutex);
       // if(arrUin>=totLine)//if all lines are modified exit
         //   pthread_exit(0);

    }
    while(1);

}


//function for replace thread
void *replace(void *param)
{

    int in,i=0;
    char temp[50];
    do
    {
  if(arrRpin>=totLine)//exit if all modifications made
            pthread_exit(0);
        //critical section for replace
        sem_wait(&rp_mutex);
        rpCount++;
        in=arrRpin;
        arrRpin++;
        if(rpCount==1)
            sem_wait(&rpu_mutex);
        sem_post(&rp_mutex);

        if(arrRin>0 && in<arrRin && flags[in][2]==0)//if at least one entry
        {
            strcpy(temp,arr[in]);
            i=0;
            while(arr[in][i])//loop through line if there is a space change it to underscore and informs user
            {
                if(arr[in][i]==' ')
                {
                    arr[in][i]='_';
                }
                i++;
            }
            printf("Replace Thread id:%ld index:%d changed line %s to %s in array\n",pthread_self(),in,temp,arr[in]);
        }
        sem_wait(&rp_mutex);
        rpCount--;
        flags[in][2]=1;
        if(rpCount==0)
            sem_post(&rpu_mutex);
        sem_post(&rp_mutex);
       // if(arrRpin>=totLine)//exit if all modifications made
         //   pthread_exit(0);

    }
    while(1);

}


//function for writer thread
void *write(void *param)
{

    int in,i;
    do
    {
 if(arrWin>=totLine)//exit if all written
                pthread_exit(0);
        if(flags[in][0] && flags[in][1] && flags[in][2])//if all modifications on at least one is made
        {
            //critical section for write
            sem_wait(&w_mutex);
            wCount++;
            flags[arrWin][3]=1;
            in=arrWin;
            arrWin++;
            if(wCount==1)
                sem_wait(&rw_mutex);

            sem_post(&w_mutex);

         //   arr1[in];

fseek(f2,arr1[in],SEEK_SET);
            fprintf(f2,"%s",arr[in]);//write modified line to the file and inform the user
fflush(f2);

            printf("Write Thread id:%ld index:%d writes line %s to file\n",pthread_self(),in,arr[in]);
            sem_wait(&w_mutex);
            wCount--;
            if(wCount==0)
                sem_post(&rw_mutex);
            sem_post(&w_mutex);

            //if(arrWin>=totLine)//exit if all written
              //  pthread_exit(0);

        }
    }
    while(1);

}
