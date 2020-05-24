#include <QList>
#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <string>
#include <string.h>
#include "player.h"
#include <time.h>

using namespace std;

void * context;
int getRole();

QList<uint> numList;
uint num;
bool duplicate;
bool burger;

int main()
{
    string push = "weerwolven! >";
    char send[1000];

    player player1;
    player player2;
    player player3;
    player player4;
    player player5;
    player player6;

    printf("Hello World!\n");
    context = zmq_ctx_new();

    void * pusher = zmq_socket( context, ZMQ_PUSH );
    void * subscriber = zmq_socket(context, ZMQ_SUB);

    zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

    zmq_setsockopt(pusher,ZMQ_SUBSCRIBE,"weerwolven! >", 13);        //waarop verzenden
    zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,"weerwolven? >", 13);    //waarop lezen

    char string[1000] = "";
    zmq_msg_t msg;
    int rc;
    int size;
    int weerwolf;
    int cupido;
    int heks;
    int i = 0;
    char sub[11];
    while (i <= 4)
    {
        rc = zmq_msg_init (&msg);
        rc = zmq_recvmsg (subscriber, &msg, 0);
        size = zmq_msg_size (&msg);
        memcpy(string, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        string[size] = 0;
        printf("%s\n", string);
        zmq_msg_close (&msg);


        char name[25];
        sscanf(string, "%s > %s >", sub, name);

        switch (i)
        {
        case 0:

            player1.name = name;
            player1.role = getRole();
            player1.alive = true;

            push.append(name);
            push.append(" >");
            push.append("1");
            push.append(" >");
            push.append(to_string(player1.role));
            strcpy(send, push.c_str());
            zmq_send( pusher, send, strlen(send), 0 );
            i++;
            push = "weerwolven! >";
            strcpy(name, "");
            break;
        case 1:

            player2.name = name;
            player2.role = getRole();
            player2.alive = true;

            push.append(name);
            push.append(" >");
            push.append("2");
            push.append(" >");
            push.append(to_string(player2.role));
            strcpy(send, push.c_str());
            zmq_send( pusher, send, strlen(send), 0 );
            i++;
            strcpy(name, "");
            push = "weerwolven! >";
            break;
        case 2:

            player3.name = name;
            player3.role = getRole();
            player3.alive = true;

            push.append(name);
            push.append(" >");
            push.append("3");
            push.append(" >");
            push.append(to_string(player3.role));
            strcpy(send, push.c_str());
            zmq_send( pusher, send, strlen(send), 0 );
            i++;
            push = "weerwolven! >";
            strcpy(name, "");
            break;
        case 3:

            player4.name = name;
            player4.role = getRole();
            player4.alive = true;

            push.append(name);
            push.append(" >");
            push.append("4");
            push.append(" >");
            push.append(to_string(player4.role));
            strcpy(send, push.c_str());
            zmq_send( pusher, send, strlen(send), 0 );
            i++;
            push = "weerwolven! >";
            strcpy(name, "");
            break;
        case 4:

            player5.name = name;
            player5.role = getRole();
            player5.alive = true;

            push.append(name);
            push.append(" >");
            push.append("5");
            push.append(" >");
            push.append(to_string(player5.role));
            strcpy(send, push.c_str());
            zmq_send( pusher, send, strlen(send), 0 );
            i++;
            push = "weerwolven! >";
            strcpy(name, "");
            break;
        }
    }

    if (player1.role == 1)
    {
        weerwolf = 1;
    }
    else if (player2.role == 1)
    {
        weerwolf = 2;
    }
    else if (player3.role == 1)
    {
        weerwolf = 3;
    }
    else if (player4.role == 1)
    {
        weerwolf = 4;
    }
    else if (player5.role == 1)
    {
        weerwolf = 5;
    }

    if (player1.role == 2)
    {
        heks = 1;
    }
    else if (player2.role == 2)
    {
        heks = 2;
    }
    else if (player3.role == 2)
    {
        heks = 3;
    }
    else if (player4.role == 2)
    {
        heks = 4;
    }
    else if (player5.role == 2)
    {
        heks = 5;
    }

    if (player1.role == 3)
    {
        cupido = 1;
    }
    else if (player2.role == 3)
    {
        cupido = 2;
    }
    else if (player3.role == 3)
    {
        cupido = 3;
    }
    else if (player4.role == 3)
    {
        cupido = 4;
    }
    else if (player5.role == 3)
    {
        cupido = 5;
    }

    rc = zmq_msg_init (&msg);
    rc = zmq_recvmsg (subscriber, &msg, 0);
    size = zmq_msg_size (&msg);
    memcpy(string, zmq_msg_data(&msg), size);
    zmq_msg_close(&msg);
    string[size] = 0;
    printf("%s\n", string);
    zmq_msg_close (&msg);
    char command[20];
    int koppel1;
    int koppel2;
    sscanf(string, "%s >%s >%d >%d", sub, command, &koppel1, &koppel2);

    int kill;
    int killHeks;
    bool game = true;
    int id;
    while (game)
    {
        printf("Iedereen mag gaan slapen.\n");
        printf("De weerwolf mag wakker worden.\n");
        push.append("weerwolf >");
        strcpy(send, push.c_str());
        zmq_send( pusher, send, strlen(send), 0 );
        push = "weerwolven >";

        rc = zmq_msg_init (&msg);
        rc = zmq_recvmsg (subscriber, &msg, 0);
        size = zmq_msg_size (&msg);
        memcpy(string, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        string[size] = 0;
        printf("%s\n", string);
        zmq_msg_close (&msg);

        sscanf(string, "%s > %s", sub, &kill);

        printf("De weerwolf mag terug gaan slapen.\nDe heks mag wakker worden.\n");
        push.append("heks >");
        push.append("%d", kill);
        strcpy(send, push.c_str());
        zmq_send( pusher, send, strlen(send), 0 );
        push = "weerwolven >";

        rc = zmq_msg_init (&msg);
        rc = zmq_recvmsg (subscriber, &msg, 0);
        size = zmq_msg_size (&msg);
        memcpy(string, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        string[size] = 0;
        printf("%s\n", string);
        zmq_msg_close (&msg);

        sscanf(string, "%s >%s >%d", sub, command, &id);
        if (strcmp(command, "kill") == 0)
        {
            killHeks = id;
        }
        else if (strcmp(command, "heal") == 0)
        {
            kill = 0;
        }


        if (kill == 0)
        {
            printf ("Er is niemand vermoord deze nacht.\n");
        }
        else if (kill > 0 && killHeks > 0)
        {
            printf("player%d en player%d zijn deze nacht vermoord.\n", kill, killHeks);
            if (kill || killHeks == koppel1 || koppel2)
            {
                //kill koppel
            }
        }
        else if (killHeks > 0)
        {
            printf("player%d is deze nacht vermoord.\n", killHeks);
            if (kill || killHeks == koppel1 || koppel2)
            {
                //kill koppel
            }
        }
        else
        {
            printf("player%d is deze nacht vermoord.\n");
            if (kill || killHeks == koppel1 || koppel2)
            {
                //kill koppel
            }
        }
    }





    return 0;
}


int getRole()
{
    do {
        srand(time(NULL));
        num =  rand()% 4;
        if (numList.contains(num) && num != 0)
        {
            duplicate = true; // skip duplicates
        }
        else
        {
            if (num == 0 && burger != true)
            {
                duplicate = true;
                burger = true;
            }
            else
            {
                numList.append(num);
                duplicate = false;
            }
        }
    } while (duplicate);
    return num;
}
