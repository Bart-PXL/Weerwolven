#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <string>
#include <string.h>


using namespace std;

void * context; //Global context, because you only need one !



int main(int argc, char * argv[])
{
    char send[1000];
    string sub = "weerwolven! >";
    string push = "weerwolven? >";
    printf("welkom bij Weerwolven\n");
    context = zmq_ctx_new();

    void * pusher = zmq_socket( context, ZMQ_PUSH );
    void * subscriber = zmq_socket(context, ZMQ_SUB);

    zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

    printf("Wat is uw naam? ");
    char name[25];
    scanf("%s", name);
    sub.append(name);
    char subscribe[100];
    strcpy(subscribe, sub.c_str());
    printf("%s\n", subscribe);

    zmq_setsockopt(pusher,ZMQ_SUBSCRIBE,"weerwolven? >", 28);        //waarop verzenden
    zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,subscribe, strlen(subscribe));    //waarop lezen


    push.append(name);
    strcpy(send, push.c_str());
    zmq_send( pusher, send, strlen(send), 0 );    //verzenden
    push = "weerwolven? >";
    printf("%s\n", send);

    char string[1000] = "";
    zmq_msg_t msg;
    int rc;
    int size;

    rc = zmq_msg_init (&msg);
    rc = zmq_recvmsg (subscriber, &msg, 0);
    size = zmq_msg_size (&msg);
    memcpy(string, zmq_msg_data(&msg), size);
    zmq_msg_close(&msg);
    string[size] = 0;
    printf("%s\n", string);
    zmq_msg_close (&msg);

    char subscr[11];
    int role;
    int id;
    sscanf(string, "%s >%s >%d >%d", subscr, name, &id, &role);
    printf("Uw id is player%d\n", id);
    switch (role)
    {
    case 0:
        printf("U bent een burger.\n");
        sub = "weerwolven! >";
        strcpy(subscribe, sub.c_str());
        zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,subscribe, strlen(subscribe));
        break;
    case 1:
        printf("U bent de weerwolf!\n");
        sub = "weerwolven! >";
        strcpy(subscribe, sub.c_str());
        zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,subscribe, strlen(subscribe));
        break;
    case 2:
        printf("U bent de heks.\n");
        sub = "weerwolven! >";
        strcpy(subscribe, sub.c_str());
        zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,subscribe, strlen(subscribe));
        break;
    case 3:
        printf("U bent cupido.\n");
        sub = "weerwolven! >";
        strcpy(subscribe, sub.c_str());
        zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,subscribe, strlen(subscribe));
        break;
    }

    char play[5] = "";
    printf("Waiting for other players...");
    while (strcmp(play, "play") != 0)
    {
        rc = zmq_msg_init (&msg);
        rc = zmq_recvmsg (subscriber, &msg, 0);
        size = zmq_msg_size (&msg);
        memcpy(string, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        string[size] = 0;
        printf("%s\n", string);
        zmq_msg_close (&msg);
        sscanf(string, "%s >%s", subscr, play);
    }

    if (id == 3)
    {
        printf("welke spelers wilt u koppelen?\nU kan kiezen uit player1 (1), player2 (2), player3 (3), player4 (4) en player5 (5)\nType de nummers van de spelers met een spatie er tussen.");
        int persoon1;
        int persoon2;
        scanf("%d %d", &persoon1, &persoon2);
        push.append("koppel >");
        push.append("%d", persoon1);
        push.append(" >");
        push.append("%d", persoon2);
        strcpy(send, push.c_str());
        zmq_send( pusher, send, strlen(send), 0 );
    }






    zmq_close( pusher );
    zmq_close( subscriber );
    zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
    zmq_ctx_term( context ); //cleaning lady goes to work
    printf("KTNXBYE!\n");
    return 0;
}
