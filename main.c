#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ram.h"

int main(int args, char *argv[])
{

    printf("---WELCOME TO SHANSYS---\n\n");

    
   long int *stats = get_ram_stats();
if (stats != NULL) {
    printf("Percentage: %ld%%\n", stats[3]);
    free(stats); // Prevent memory leak
}
    return 0;

}
