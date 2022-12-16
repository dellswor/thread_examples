// Program to use POSIX threads to walk and chew bubble gum

// IO routines
#include <stdio.h>
// pthread
#include <pthread.h>
// where the sleep function is from
#include <unistd.h>

void* walk(void* args) {
    while(1) {
        printf("walks\n");
        sleep(1);
    }
}

void* bubblegum(void* args) {
    int gum=10;
    while(gum>0) {
        printf("chews gum\n");
        sleep(1);
        gum--;
    }
    printf("all out of bubble gum...\n");
}

int main(int argc, char** argv) {
    // Make the needed variables
    int err;
    pthread_t wthread;
    pthread_t bthread;

    // Create and start some threads
    err = pthread_create(&wthread, NULL, &walk, NULL);
    err = pthread_create(&bthread, NULL, &bubblegum, NULL);

    // Join (make this thread wait until another thread finishes) to the other threads
    err = pthread_join(wthread, NULL);
    err = pthread_join(bthread, NULL);

    return 0;
}
