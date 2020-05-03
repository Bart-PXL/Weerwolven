#include <stdio.h>
#include <zmq.h>
#include <string.h>

void * context; //Global context, because you only need one !

int main(int argc, char * argv[])
{
    printf("Hello World!\n");
    context = zmq_ctx_new();

    void * pusher = zmq_socket( context, ZMQ_PUSH );
    void * subscriber = zmq_socket(context, ZMQ_SUB);

    /*zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

    zmq_setsockopt(pusher,ZMQ_SUBSCRIBE,"weerwolven?>", 28);        //waarop verzenden
    zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,"weerwolven!>", 28);    //waarop lezen

    zmq_send( pusher, "example>name>", 99, 0 );    //verzenden
    printf("Send\n");*/







    zmq_close( pusher );
    zmq_close( subscriber );
    zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
    zmq_ctx_term( context ); //cleaning lady goes to work
    printf("KTNXBYE!\n");
    return 0;
}
