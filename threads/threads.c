#include <threads.h>
#include <stdio.h>

int runForFun(void* arg){
    struct timespec sleepFor;

    sleepFor.tv_sec = 3;
    sleepFor.tv_nsec = 33;

    printf("hi %s - is sleeping for %ld\n",__FUNCTION__,sleepFor.tv_sec);

    thrd_sleep(&sleepFor,NULL);

    return 0;
}

int main(int argc, char* argv[]){

    thrd_t myThread;

    int runForFunResult;

    thrd_create(&myThread, runForFun, NULL);

    thrd_join(myThread, &runForFunResult);
    
    printf("My thread returned %d\n", runForFunResult);

    return 0;
}
