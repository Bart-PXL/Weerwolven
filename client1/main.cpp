#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <string>
#include <string.h>
#include <QList>


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
        zmq_msg_close (&msg);
        sscanf(string, "%s >%s", subscr, play);
    }

    printf("Go\n");

    bool game = false;
    if (role == 3)
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
        push = "weerwolven? >";
        game = true;
    }

    QList<uint> deadlist;
    uint dead;
    uint dead2;
    int kill = 0;
    int killHeks = 0;
    char command[100];
    std::string com;
    int vote;
    int voteResult;
    char votes[500];
    bool juist = false;
    while (game == true)
    {
        switch (role)
        {
        case 0:                                                                     //code voor de burger
            printf("Iedereen gaat slapen\n");
            printf("De weerwolf wordt wakker.\n");

            while (strcmp(command, "next") != 0)                                    //wachten op Weerwolf
            {
                rc = zmq_msg_init (&msg);
                rc = zmq_recvmsg (subscriber, &msg, 0);
                size = zmq_msg_size (&msg);
                memcpy(string, zmq_msg_data(&msg), size);
                zmq_msg_close(&msg);
                string[size] = 0;
                zmq_msg_close (&msg);
                sscanf(string, "%s > %s", subscr, command);
            }
            com = "";
            strcpy(command, com.c_str());

            printf("De weerwolf gaat terug slapen.\n De heks wordt nu wakker.\n");

            while (strcmp(command, "next") != 0)                                    //wachten op heks
            {
                rc = zmq_msg_init (&msg);
                rc = zmq_recvmsg (subscriber, &msg, 0);
                size = zmq_msg_size (&msg);
                memcpy(string, zmq_msg_data(&msg), size);
                zmq_msg_close(&msg);
                string[size] = 0;
                zmq_msg_close (&msg);
                sscanf(string, "%s > %s", subscr, command);
            }
            com = "";
            strcpy(command, com.c_str());

            printf("De heks gaat terug slapen.\n Iedereen mag terug wakker worden.\n");



            rc = zmq_msg_init (&msg);                                               //inlezen wie er vermoord wordt
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%d", subscr, command, &dead, &dead2);
            if (dead > 0)
            {
                deadlist.append(dead);                                              //vermoorde player aan dodenlijst toevoegen
                if (dead2 > 0)
                {
                    printf("Player%d en player%d zijn deze nacht vermoord \n Wie denk je dan de weerwolf is(type de nummer van de player) \n", dead, dead2);
                    deadlist.append(dead2);                                         //vermoorde player aan dodenlijst toevoegen
                }
                else
                {
                    printf("Player%d is deze nacht vermoord\n Wie denk je dan de weerwolf is?(type de nummer van de player) \n", dead);
                }
            }
            else
            {
                printf("Niemamd werd dezenacht vermoord. \n Wie denk je dan de weerwolf is?(type de nummer van de player) \n");
            }
            dead = 0;
            dead2 = 0;

            scanf("%d", &vote);                                                     //stemmen weerwolf
            while (kill < 1 || kill > 5 || deadlist.contains(vote))
            {
                printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                scanf("%d", &vote);
            }
            push.append("vote >");
            push.append("%d >", id);
            push.append("%d", vote);

            zmq_send( pusher, "weerwolven >%", strlen(send), 0 );                   //stem verzenden
            while (strcmp(command, "voteResult") != 0)                              //wachten op result
            {
            rc = zmq_msg_init (&msg);                                               //inlezen result
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%s", subscr, command, &voteResult, votes);
            }
            printf("%s \n", votes);                                            //print result
            printf("Het resultaat van de stemming is player%d. \n", voteResult);

            deadlist.append(voteResult);


            break;

















        case 1:                                                                     //code voor de weerwolf
            printf("Iedereen gaat slapen\n");
            printf("De weerwolf wordt wakker.\n");

            printf("welke speler wilt u vermoorden?\n");
            scanf("%d", &kill);                                                     //player vermoorden
            while (kill < 1 || kill > 5 || deadlist.contains(kill))
            {
                printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n");
                scanf("%d", &kill);
            }
            push.append("kill >%d", kill);
            strcpy(send, push.c_str());
            zmq_send( pusher, send, strlen(send), 0 );
            push = "weerwolven >";

            printf("De weerwolf gaat terug slapen.\n De heks wordt nu wakker.\n");

            while (strcmp(command, "next") != 0)                                    //wachten op heks
            {
                rc = zmq_msg_init (&msg);
                rc = zmq_recvmsg (subscriber, &msg, 0);
                size = zmq_msg_size (&msg);
                memcpy(string, zmq_msg_data(&msg), size);
                zmq_msg_close(&msg);
                string[size] = 0;
                zmq_msg_close (&msg);
                sscanf(string, "%s > %s", subscr, command);
            }

            com = "";
            strcpy(command, com.c_str());
            printf("De heks gaat terug slapen.\n Iedereen mag terug wakker worden.\n");

            rc = zmq_msg_init (&msg);                                               //inlezen wie er vermoord wordt
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%d", subscr, command, &dead, &dead2);
            if (dead > 0)
            {
                deadlist.append(dead);                                              //vermoorde player aan dodenlijst toevoegen
                if (dead2 > 0)
                {
                    printf("Player%d en player%d zijn deze nacht vermoord \n Wie denk je dan de weerwolf is(type de nummer van de player) \n", dead, dead2);
                    deadlist.append(dead2);                                         //vermoorde player aan dodenlijst toevoegen
                }
                else
                {
                    printf("Player%d is deze nacht vermoord\n Wie denk je dan de weerwolf is?(type de nummer van de player) \n", dead);
                }
            }
            else
            {
                printf("Niemamd werd dezenacht vermoord. \n Wie denk je dan de weerwolf is?(type de nummer van de player) \n");
            }
            dead = 0;
            dead2 = 0;

            scanf("%d", &vote);                                                     //stemmen weerwolf
            while (kill < 1 || kill > 5 || deadlist.contains(vote))
            {
                printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                scanf("%d", &vote);
            }
            push.append("vote >");
            push.append("%d >", id);
            push.append("%d", vote);

            zmq_send( pusher, "weerwolven >%", strlen(send), 0 );                   //stem verzenden
            while (strcmp(command, "voteResult") != 0)                              //wachten op result
            {
            rc = zmq_msg_init (&msg);                                               //inlezen result
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%s", subscr, command, &voteResult, votes);
            }
            printf("%s \n", votes);                                            //print result
            printf("Het resultaat van de stemming is player%d. \n", voteResult);

            deadlist.append(voteResult);

            break;










        case 2:
            printf("Iedereen gaat slapen\n");
            printf("De weerwolf wordt wakker.\n");

            while (strcmp(command, "next") != 0)                                    //wachten op Weerwolf
            {
                rc = zmq_msg_init (&msg);
                rc = zmq_recvmsg (subscriber, &msg, 0);
                size = zmq_msg_size (&msg);
                memcpy(string, zmq_msg_data(&msg), size);
                zmq_msg_close(&msg);
                string[size] = 0;
                zmq_msg_close (&msg);
                sscanf(string, "%s > %s", subscr, command);
            }
            com = "";
            strcpy(command, com.c_str());

            printf("De weerwolf gaat terug slapen.\n De heks wordt nu wakker.\n");

            while (strcmp(command, "kill") != 0)                                    //wachten op Weerwolf
            {
                rc = zmq_msg_init (&msg);
                rc = zmq_recvmsg (subscriber, &msg, 0);
                size = zmq_msg_size (&msg);
                memcpy(string, zmq_msg_data(&msg), size);
                zmq_msg_close(&msg);
                string[size] = 0;
                zmq_msg_close (&msg);
                sscanf(string, "%s > %s >", subscr, command);
            }


            printf("player%d is dood.\n Wilt u deze player terug levend maken?(type 'heal')\n Wilt u niks doen?(type 'niks')\n Of wilt u iemand bijvermoorden?(type 'kill')\n ", kill);
            while (juist)
            {
                scanf("%s", command);
                if (strcmp(command, "heal") == 0)
                {
                    printf("Player%d zal genezen.\n", kill);
                    kill = 0;
                    push.append("heal");
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven >";
                    juist = true;
                }
                else if (strcmp(command, "kill") == 0)
                {
                    printf("welke speler wilt u vermoorden?\n");
                    scanf("%d", &killHeks);                                                     //player vermoorden
                    while (killHeks < 1 || killHeks > 5 || deadlist.contains(killHeks))
                    {
                        printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n");
                        scanf("%d", &kill);
                    }
                    push.append("killHeks >%d", kill);
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven >";
                    juist = true;
                }
                else if (strcmp(command, "niks") == 0)
                {
                    push.append("niks");
                    strcpy(send, push.c_str());
                    zmq_send( pusher, send, strlen(send), 0 );
                    push = "weerwolven >";
                    juist = true;
                }
                else
                {
                    printf("Uw commando is niet herkent. Gelieve uw commando correct te typen.\n");
                    juist = false;
                }
            }

            printf("De heks gaatr terug slapen. \n");

            rc = zmq_msg_init (&msg);                                               //inlezen wie er vermoord wordt
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%d", subscr, command, &dead, &dead2);
            if (dead > 0)
            {
                deadlist.append(dead);                                              //vermoorde player aan dodenlijst toevoegen
                if (dead2 > 0)
                {
                    printf("Player%d en player%d zijn deze nacht vermoord \n Wie denk je dan de weerwolf is(type de nummer van de player) \n", dead, dead2);
                    deadlist.append(dead2);                                         //vermoorde player aan dodenlijst toevoegen
                }
                else
                {
                    printf("Player%d is deze nacht vermoord\n Wie denk je dan de weerwolf is?(type de nummer van de player) \n", dead);
                }
            }
            else
            {
                printf("Niemamd werd dezenacht vermoord. \n Wie denk je dan de weerwolf is?(type de nummer van de player) \n");
            }
            dead = 0;
            dead2 = 0;

            scanf("%d", &vote);                                                     //stemmen weerwolf
            while (kill < 1 || kill > 5 || deadlist.contains(vote))
            {
                printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                scanf("%d", &vote);
            }
            push.append("vote >");
            push.append("%d >", id);
            push.append("%d", vote);

            zmq_send( pusher, "weerwolven >%", strlen(send), 0 );                   //stem verzenden
            while (strcmp(command, "voteResult") != 0)                              //wachten op result
            {
            rc = zmq_msg_init (&msg);                                               //inlezen result
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%s", subscr, command, &voteResult, votes);
            }
            printf("%s \n", votes);                                            //print result
            printf("Het resultaat van de stemming is player%d. \n", voteResult);

            deadlist.append(voteResult);

            break;









        case 3:
            printf("Iedereen gaat slapen\n");
            printf("De weerwolf wordt wakker.\n");

            while (strcmp(command, "next") != 0)                                    //wachten op Weerwolf
            {
                rc = zmq_msg_init (&msg);
                rc = zmq_recvmsg (subscriber, &msg, 0);
                size = zmq_msg_size (&msg);
                memcpy(string, zmq_msg_data(&msg), size);
                zmq_msg_close(&msg);
                string[size] = 0;
                zmq_msg_close (&msg);
                sscanf(string, "%s > %s", subscr, command);
            }
            com = "";
            strcpy(command, com.c_str());

            printf("De weerwolf gaat terug slapen.\n De heks wordt nu wakker.\n");

            while (strcmp(command, "next") != 0)                                    //wachten op heks
            {
                rc = zmq_msg_init (&msg);
                rc = zmq_recvmsg (subscriber, &msg, 0);
                size = zmq_msg_size (&msg);
                memcpy(string, zmq_msg_data(&msg), size);
                zmq_msg_close(&msg);
                string[size] = 0;
                zmq_msg_close (&msg);
                sscanf(string, "%s > %s", subscr, command);
            }
            com = "";
            strcpy(command, com.c_str());

            printf("De heks gaat terug slapen.\n Iedereen mag terug wakker worden.\n");



            rc = zmq_msg_init (&msg);                                               //inlezen wie er vermoord wordt
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%d", subscr, command, &dead, &dead2);
            if (dead > 0)
            {
                deadlist.append(dead);                                              //vermoorde player aan dodenlijst toevoegen
                if (dead2 > 0)
                {
                    printf("Player%d en player%d zijn deze nacht vermoord \n Wie denk je dan de weerwolf is(type de nummer van de player) \n", dead, dead2);
                    deadlist.append(dead2);                                         //vermoorde player aan dodenlijst toevoegen
                }
                else
                {
                    printf("Player%d is deze nacht vermoord\n Wie denk je dan de weerwolf is?(type de nummer van de player) \n", dead);
                }
            }
            else
            {
                printf("Niemamd werd dezenacht vermoord. \n Wie denk je dan de weerwolf is?(type de nummer van de player) \n");
            }
            dead = 0;
            dead2 = 0;

            scanf("%d", &vote);                                                     //stemmen weerwolf
            while (kill < 1 || kill > 5 || deadlist.contains(vote))
            {
                printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                scanf("%d", &vote);
            }
            push.append("vote >");
            push.append("%d >", id);
            push.append("%d", vote);

            zmq_send( pusher, "weerwolven >%", strlen(send), 0 );                   //stem verzenden
            while (strcmp(command, "voteResult") != 0)                              //wachten op result
            {
            rc = zmq_msg_init (&msg);                                               //inlezen result
            rc = zmq_recvmsg (subscriber, &msg, 0);
            size = zmq_msg_size (&msg);
            memcpy(string, zmq_msg_data(&msg), size);
            zmq_msg_close(&msg);
            string[size] = 0;
            zmq_msg_close (&msg);
            sscanf(string, "%s > %s >%d >%s", subscr, command, &voteResult, votes);
            }
            printf("%s \n", votes);                                            //print result
            printf("Het resultaat van de stemming is player%d. \n", voteResult);

            deadlist.append(voteResult);

            break;
        }

        rc = zmq_msg_init (&msg);                                               //inlezen game end
        rc = zmq_recvmsg (subscriber, &msg, 0);
        size = zmq_msg_size (&msg);
        memcpy(string, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        string[size] = 0;
        zmq_msg_close (&msg);
        sscanf(string, "%s > %s", subscr, command);

        if (strcmp(command, "end") == 0)
        {
            game = false;
        }
    }



    zmq_close( pusher );
    zmq_close( subscriber );
    zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
    zmq_ctx_term( context ); //cleaning lady goes to work
    printf("KTNXBYE!\n");
    return 0;
}




