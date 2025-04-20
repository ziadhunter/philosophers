#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    struct timeval start, end;
    long seconds, useconds;
    double total_time;

    gettimeofday(&start, NULL);

    // Code you want to measure
    usleep(500000); // Sleep for 500 milliseconds

    gettimeofday(&end, NULL);

    // Calculate elapsed time in microseconds
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    total_time = seconds + useconds / 1e6;

    printf("Elapsed time: %.6f seconds\n", total_time);

    return 0;
}
