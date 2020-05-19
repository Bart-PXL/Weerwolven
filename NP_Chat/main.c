#include <stdio.h>
#include <zmq.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void * context;

struct arg_struct {
    void *subscriber;
    void *pusher;
};


char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

void messageReader(void *subscriber)
{
    while(1)
    {
        char string[1000] = "";

        zmq_msg_t msg;

        int rc = zmq_msg_init (&msg);
        rc = zmq_recvmsg (subscriber, &msg, 0);
        int size = zmq_msg_size (&msg);
        memcpy(string, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        string[size] = 0;
        printf("%s\n", string);
        zmq_msg_close (&msg);
    }
}

void messageWriter(void *arguments)
{
    struct arg_struct *argsWriter = arguments;
    char message[100];
    while (1)
    {
        gets(message);

        if(strcmp("exit", message) == 0)
        {
            zmq_close( argsWriter -> pusher );
            zmq_close( argsWriter -> subscriber );
            zmq_ctx_shutdown( context );
            zmq_ctx_term( context );
            printf("Program terminated!\n");
            exit(0);
        }

        char* fullmessage = concat("hugohamblok>", message);
        int messageLength = strlen(fullmessage);

        zmq_send( argsWriter -> pusher, fullmessage, messageLength, 0 );

        free(fullmessage);
    }
}


int main()
{
    context = zmq_ctx_new();

    void * pusher = zmq_socket(context, ZMQ_PUSH );
    void * subscriber = zmq_socket(context, ZMQ_SUB );
    zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

    zmq_setsockopt(pusher,ZMQ_SUBSCRIBE,"hugohamblok>", 12);
    zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,"hugohamblok>", 12);

    printf("Enter <exit> to exit the program\n");
    printf("Write a message!\n");

    struct arg_struct argsWriter;

    argsWriter.pusher = pusher;
    argsWriter.subscriber = subscriber;

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, messageReader, subscriber);
    pthread_create(&thread2, NULL, messageWriter, (void *)&argsWriter);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
