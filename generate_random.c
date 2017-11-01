#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    int i;
    int r;
    FILE *fp;
    fp = fopen("random.txt", "w");

    srand(9999);

    for(i = 0; i < 2000000000; ++i){
        int r = rand();
        fprintf(fp, "%d ", r);
    }
    fclose(fp);

    return 0;
}