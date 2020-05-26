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
    context = zmq_ctx_new();

    void * pusher = zmq_socket( context, ZMQ_PUSH );
    void * subscriber = zmq_socket(context, ZMQ_SUB);

    zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

    zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,"weerwolven? >", 13);

    char read[1000] = "";
    zmq_msg_t msg;
    int rc;
    int size;
    bool init = false;
    char sub[20];
    char name[25];
    char command[30];
    char data[700];
    char data2[700];
    int i = 0;
    string push = "weerwolven! >";
    char send[1000];
    int state = 4;
    bool play = false;
    bool cupido = false;
    bool weerwolf = false;
    bool heks = false;
    int koppel1 = 0;
    int koppel2 = 0;
    int murder = 0;

    player player1;
    player player2;
    player player3;
    player player4;
    player player5;

    while (1)
    {
        rc = zmq_msg_init (&msg);                                                           //read benthernet
        rc = zmq_recvmsg (subscriber, &msg, 0);
        size = zmq_msg_size (&msg);
        memcpy(read, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        read[size] = 0;
        printf("%s\n", read);
        zmq_msg_close (&msg);

        sscanf(read, "%s > %s >%s >%s", sub, command, data, data2);

        if (strcmp(command, "init") == 0)
        {
            if (init)
            {
                //u kan de game niet joinen omdat er al genoeg spelers zijn.
            }
            else
            {
                sscanf(data, "%s", name);
                switch (i)
                {
                case 0:

                    player1.name = name;
                    player1.role = getRole();
                    player1.alive = true;

                    push.append("init >");
                    push.append("1");
                    push.append(" >");
                    push.append(to_string(player1.role));
                    push.append(" >");
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

                    push.append("init >");
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

                    push.append("init >");
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

                    push.append("init >");
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

                    push.append("init >");
                    push.append("5");
                    push.append(" >");
                    push.append(to_string(player5.role));
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven! >";
                    strcpy(name, "");

                    play = true;
                    init = true;
                    break;
                }
            }
        }

        if (play)
        {
            push.append("play >");
            strcpy(send, push.c_str());
            zmq_send( pusher, send, strlen(send), 0 );
            push = "weerwolven! >";
            play = false;
        }

        if (init)
        {
            switch (state)
            {
            case 0:
                if (weerwolf == false)
                {
                    push.append("weerwolf >");
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven! >";
                    weerwolf = true;
                }
                if (strcmp(command, "murder") == 0)
                {
                    sscanf(data, "%d", &murder);
                    push.append("heks >%d", murder);
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven! >";
                    weerwolf = false;
                }
                break;
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:
                if (cupido == false)
                {
                    push.append("cupido >");
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven! >";
                    cupido = true;
                }
                if (strcmp(command, "koppel") == 0)
                {
                    sscanf(data, "%d", &koppel1);
                    sscanf(data2, "%d", &koppel2);
                    push.append("koppel >");
                    push.append(to_string(koppel1));
                    push.append(" >");
                    push.append(to_string(koppel2));
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven! >";
                    push.append("next >");
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven! >";
                    state = 0;
                }
                break;
            }
        }
    }

    zmq_close( pusher );
    zmq_close( subscriber );
    zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
    zmq_ctx_term( context ); //cleaning lady goes to work
    printf("KTNXBYE!\n");
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
            if (num == 0 && burger == false)
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
