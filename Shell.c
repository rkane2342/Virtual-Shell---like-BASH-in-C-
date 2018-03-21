#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#define inputB 1024

struct file_attr {
    int num;
    char * filename;
};

int comparator(const void *a, const void *b) {
    return ( *(int *)a - *(int *)b);
}

extern char **environ;
void parsecall(char **a,char *b){
    int i=0,j=0;
    char *temp,help;
    temp = (char*) malloc(sizeof(char)*inputB);
    while(*b != '\0')
    {
        help = *b++;
        if (help == ' ' || help == '\n')
        {
            a[i] = (char *) malloc(sizeof(char)*inputB);
            temp[j++] = '\0';

            a[i] = strdup(temp);
            j=0;
            //puts(a[i]);  test
            i++;
            while(*temp){
                *temp++ = '\0';
            }


        }
        else
            temp[j++] = help;
    }

}

void* thread2(void * t){
    char * myfile = (char *)t;
    FILE* fp= fopen(myfile,"r");
    unsigned int * temp = (unsigned int*)malloc(sizeof(unsigned int) * inputB);
    unsigned int num;
    int i = 0;
    while(fscanf(fp, "%u\n", &num) != -1){
        temp[i++] = num;
    }
    qsort(temp, i, sizeof(unsigned int), comparator);
    fclose(fp);

    FILE *fp2= fopen(myfile,"w");
    for (int j =0 ; j< i ;j++){
        //printf("%u", temp[j]); test case
        fprintf(fp, "%u\n", temp[j]);
    }
    fclose(fp2);
    return NULL;
}


void fsort(struct file_attr getnumf)
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread2, getnumf.filename);
    pthread_join(tid, NULL);
}

void* thread1(void * t){
    struct file_attr * myfile = (struct file_attr *)t;
    FILE* fp= fopen(myfile->filename,"w");
    unsigned int temp;
    for (int i =0 ; i< myfile->num ;i++){
        temp = rand();
        fprintf(fp, "%u\n", temp);
    }
    fclose(fp);
    return NULL;
}


void frand(struct file_attr getnumf)
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread1, &getnumf);
    pthread_join(tid, NULL);
}


void functioncall(){
    size_t size;
    char *z; //thing that has my function cmd
    getline(&z,&size,stdin);
    char **a = (char **)malloc(10 * sizeof(char *));
    parsecall(a,z);
    if (!strcmp(a[0],"clr"))
    {
        system("clear");
    }
    else if (!strcmp(a[0],"dir"))
    {
        system("ls");
    }
    else if (!strcmp(a[0],"environ"))
    {
        char ** env = environ;

        while (*env) printf("%s\n",*env++);  // step through environment
    }
    else if (!strcmp(a[0],"frand"))
    {
        struct file_attr my_struct;
        my_struct.filename = (char *)malloc(sizeof(char) * inputB);
        my_struct.filename = a[1];
        my_struct.num = atoi(a[2]);
        frand(my_struct);
    }
    else if(!strcmp(a[0],"fsort"))
    {struct file_attr my_struct;
        my_struct.filename = (char *)malloc(sizeof(char) * inputB);
        my_struct.filename = a[1];
        fsort(my_struct);

    }
	else if(!strcmp(a[0],"quit"))
	{
		exit(0);
	}
    else
        system(z);
}


void shell()
{    srand((unsigned int)time(NULL));
    system("clear");
	printf("*****************************************************************************************\n");
	printf("\n*****************************************************************************************\n");
    printf("\n*****************************  Welcome to Rohits Shell  *********************************\n");
	printf("\n*****************************************************************************************\n");
	printf("\n*****************************************************************************************\n");

    while (true){
        printf("\n@Rkurhekar :# ");
        functioncall();

    }
}




int main (){
    shell();
}
