#include <pthread.h>
#include <stdio.h>

void *thread_function(void *arg)
{
    printf("Thread is running\n");
    return (void *)42; // Implicitly calls pthread_exit((void *)42)
}

int main()
{
    pthread_t thread;
    void *retval;
	int status;

    pthread_create(&thread, NULL, thread_function, &retval);
    printf("Thread create retval: %ld\n", (long)retval);
    status = pthread_join(thread, &retval);

    printf("Thread join retval: %ld\n", (long)retval);
    printf("Thread join status: %d\n", status);
    return 0;
}