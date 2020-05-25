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

void messageReciever(void *arguments)
{
    struct arg_struct *argsWriter = arguments;
    while(1)
    {
        char string[1000] = "";
        char *parsedString;

        zmq_msg_t msg;

        zmq_msg_init (&msg);
        zmq_recvmsg (argsWriter -> subscriber, &msg, 0);
        int size = zmq_msg_size (&msg);
        memcpy(string, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);

        for (char *parse = strtok(string,">"); parse != NULL; parse = strtok(NULL, ">"))
        {
            parsedString = strdup(parse);
        }

        printf("%s\n", parsedString);
        zmq_msg_close (&msg);

        char* fullmessage = concat("hugohamblok>forwarder>", parsedString);

        zmq_send( argsWriter -> pusher, fullmessage, strlen(fullmessage), 0 );

        free(fullmessage);

    }
}

void terminal(void)
{
    char message[100];
    while (1)
    {
        gets(message);

        if(strcmp("exit", message) == 0)
        {
            printf("Program terminated!\n");
            exit(0);
        }
        else {
            printf("Unknown command\n");
        }
    }
}

int main()
{
    context = zmq_ctx_new();

    void * pusher = zmq_socket(context, ZMQ_PUSH );
    void * subscriber = zmq_socket(context, ZMQ_SUB );

    int socketp = zmq_connect( pusher, "tcp://localhost:24041" );
    int sockets = zmq_connect( subscriber, "tcp://localhost:24042" );

    if (socketp != 0 && sockets != 0)
    {
        printf("ERROR: %s\n", zmq_strerror(errno));
        return EXIT_FAILURE;
    }

    zmq_setsockopt(pusher,ZMQ_SUBSCRIBE,"hugohamblok>forwarder>", 22);
    zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,"hugohamblok>reciever>", 21);

    printf("Enter <exit> to exit the program\n");
    printf("Service running!\n");

    struct arg_struct argsWriter;

    argsWriter.pusher = pusher;
    argsWriter.subscriber = subscriber;

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, messageReciever, (void *)&argsWriter);
    pthread_create(&thread2, NULL, terminal, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
