#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int args, char *argv[]){

    printf("---WELCOME TO SHANSYS---\n\n");
    
    char process[32];
    int process_input;

    // process_input = atoi(argv[1]); 
    
    snprintf(process, sizeof(process), "/proc/meminfo");

    printf("%s\n",process);
    
    FILE* file_ptr = fopen(process,"r");

    if(file_ptr == NULL){
        perror("Error opning file");
        return EXIT_FAILURE;
    }

    char buff[256];

    while (fgets(buff,sizeof(buff),file_ptr) != NULL)
    {
        printf("%s",buff);
    }

    
    


    fclose(file_ptr);
}

