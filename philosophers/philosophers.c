/* The dining philosopher's problem in C code
 *
 *    Abed  -- f2 --  Troy
 *      |               |
 *     f1              f3
 *      |               |
 *    Jeff  -- f4 -- Britta
 *
 *    Each philosopher picks up the fork to their left just before
 *    picking up the fork to their right. When both forks are in hand,
 *    the philosopher eats before returning the forks to the table.
 *
 * This program deadlocks fairly reliably and quickly when run. 
 * A one line change to code exists that will prevent the deadlock... but
 * it causes one of the philosophers to confuse their right and left hands.
 */

// IO functions
#include <stdio.h>
// Threading functions
#include <pthread.h>
// malloc
#include <stdlib.h>

// Model of a fork (a fork can only be held by one thread at a time)
struct fork {
    pthread_mutex_t lock;
};

// Model of a philosopher (a name and a fork for each hand)
struct philosopher {
    char*  name;
    struct fork* left;
    struct fork* right;
};

// "Fork Constructor"
struct fork* new_fork() {
    struct fork* f = (struct fork*)malloc(sizeof(struct fork));
    pthread_mutex_init(&(f->lock),NULL);
    return f;
}
    
// Logic for what to do when a philosopher joins a table
void* join_table(void* args) {
    int err;
    struct philosopher* p = (struct philosopher*)args;
    printf("%s joins the table\n",p->name);
    while(1) {
        // grab the forks
        err = pthread_mutex_lock(&(p->left->lock));
        err = pthread_mutex_lock(&(p->right->lock));
        // eat
        printf("%s eats\n",p->name);
        // put the forks back
        err = pthread_mutex_unlock(&(p->right->lock));
        err = pthread_mutex_unlock(&(p->left->lock));
    }
}

int main(int argc, char** argv) {
    int err;
    // create the forks
    struct fork* f1 = new_fork();
    struct fork* f2 = new_fork();
    struct fork* f3 = new_fork();
    struct fork* f4 = new_fork();

    // create the philosophers
    struct philosopher abed;
    struct philosopher troy;
    struct philosopher britta;
    struct philosopher jeff;

    // create the threads needed for the philosophers to act
    pthread_t abed_thread;
    pthread_t troy_thread;
    pthread_t britta_thread;
    pthread_t jeff_thread;

    // assign the philosophers their names and forks going around the table
    abed.name = "Abed"; abed.left = f1; abed.right = f2;
    troy.name = "Troy"; troy.left = f2; troy.right = f3;
    britta.name = "Britta"; britta.left = f3; britta.right = f4;
    jeff.name = "Jeff"; jeff.left = f4; jeff.right = f1;

    // seat and start each philosopher
    err = pthread_create(&abed_thread, NULL, &join_table, &abed); 
    err = pthread_create(&troy_thread, NULL, &join_table, &troy); 
    err = pthread_create(&britta_thread, NULL, &join_table, &britta); 
    err = pthread_create(&jeff_thread, NULL, &join_table, &jeff); 

    // join the threads so that execution goes on forever
    err = pthread_join(abed_thread,NULL);
    err = pthread_join(troy_thread,NULL);
    err = pthread_join(britta_thread,NULL);
    err = pthread_join(jeff_thread,NULL);

    return 0;
}
