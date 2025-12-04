#include<stdio.h>
#include<sys/time.h>

// différence entre deux temps en sec
long time_diff(struct timeval start, struct timeval end)
{   // tv_sec est de type long
    return end.tv_sec - start.tv_sec;
}

int main()
{
    int val;
    long duree;
    struct timeval start, end;
    gettimeofday(&start, NULL); // top chrono
    printf("saisir un enier : ");
    scanf("%d", &val);
    gettimeofday(&end, NULL); // stop chrono
    duree = time_diff(start, end);
    printf("durée : %ld secondes\n", duree);
}