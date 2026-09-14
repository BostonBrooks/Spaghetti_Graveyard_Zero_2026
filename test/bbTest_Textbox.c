#include <stddef.h>
#include <unistd.h>

#include "engine/textbox/bbTextbox.h"
#include "engine/threadsafe/bbThreadedPool.h"
#include "engine/threadsafe/bbThreadedQueue.h"

pthread_barrier_t barrier1;

thread_local char* thread;
thread_local bool debug_off;

U64 test_time = 0;
bbHandle null_handle;
char test_string[KEY_LENGTH];


typedef struct {
  I32 i;
} thread_data;

typedef struct {
    bbListElement_Handle element;
    bbHandle message_handle;
    char key[KEY_LENGTH];
} test_message;


bbTextbox_system textbox_systems[2];
//bbVPool* message_pool[2];
bbThreadedQueue message_queue[2];

void* message_passing(void* arg);

int main(void)
{
    bbTextbox_systemInit(&textbox_systems[0]);
    bbTextbox_systemInit(&textbox_systems[1]);
 //   bbVPool_newThreaded(&message_pool[0], sizeof(test_message), 512);
 //   bbVPool_newThreaded(&message_pool[1], sizeof(test_message), 512);

    bbThreadedQueue_init(&message_queue[0], NULL,sizeof(test_message),512,offsetof(test_message,element));
    bbThreadedQueue_init(&message_queue[1], NULL,sizeof(test_message),512,offsetof(test_message,element));


    thread_data data;
    data.i = 1;
    pthread_t second_pthread;
    pthread_create(&second_pthread, NULL, message_passing, &data);
    thread_data data2; data2.i = 0;

    message_passing(&data2);
}

void* message_passing(void* arg) {
    thread_data* data = (thread_data*)arg;
    bbTextbox_system *in_system, *out_system;
    bbThreadedQueue *in_queue, *out_queue;

    if (data->i == 0) {
        in_system = &textbox_systems[0];
        out_system = &textbox_systems[1];
        in_queue = &message_queue[0];
        out_queue = &message_queue[1];
        thread = "0";
    } else {
        in_system = &textbox_systems[1];
        out_system = &textbox_systems[0];
        in_queue = &message_queue[1];
        out_queue = &message_queue[0];
        thread = "1";
    }

    printf("hello thread %s\n", thread);

    test_message* message;
    bbHandle message_handle;
    char* message_text;
    I32 i = 0;
    while (1) {
        i++;
        bbThreadedQueue_alloc(out_queue,(void**)&message);
        snprintf(message->key, KEY_LENGTH, "send thread %s, i = %d",thread, i);
        bbThreadedQueue_pushL(out_queue,message);

        bbFlag flag = bbThreadedQueue_popR(in_queue,(void**)&message);
        if (flag != bbSuccess) continue;
        printf("message: %s\n", message->key);
        fflush(stdout);
        bbThreadedQueue_free(in_queue,(void**)&message);

        usleep(100000);
    }

}