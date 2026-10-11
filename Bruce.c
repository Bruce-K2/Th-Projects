#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
    while (1) {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        printf("\r%02d:%02d:%02d",
               tm_info->tm_hour,
               tm_info->tm_min,
               tm_info->tm_sec);
        fflush(stdout);
        sleep(1);
    }
    return 0;
}