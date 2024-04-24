#include <stdio.h>
#include <time.h>

int main() {
    time_t current_time;
    struct tm *local_time;

    // Get current time
    current_time = time(NULL);

    // Convert to local time
    local_time = localtime(&current_time);

    // Print local time
    printf("Local time: %s", asctime(local_time));

    return 0;
}
