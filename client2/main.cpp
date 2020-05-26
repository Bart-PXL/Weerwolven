#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <string>
#include <string.h>
#include <QList>

using namespace std;

void * context;


int main()
{
    context = zmq_ctx_new();

    void * pusher = zmq_socket( context, ZMQ_PUSH );
    void * subscriber = zmq_socket(context, ZMQ_SUB);

    zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

    string sub = "weerwolven! >";
    string push = "weerwolven? >";
    char name[25];
    char subscribe[100];
    char send[1000];
    bool game = true;
    char read[1000] = "";
    zmq_msg_t msg;
    int rc;
    int size;
    char subscr[20];
    char command[100];
    char data[100];
    char data2[100];
    int id;
    int role;
    bool init = false;
    int state = 4;
    int kill = 0;
    int killHeks = 0;
    QList<int> deadlist;
    int vote;
    int playerid;
    int playervote;
    int playerrole;
    int koppel1;
    int koppel2;
    bool koppel = false;
    int murder = 0;
    int keuzeheks = 0;
    int murderheks = 0;
    bool play = false;

    printf("Wat is uw naam? ");                                                     //speler id en role geven
    scanf("%s", name);
    strcpy(subscribe, sub.c_str());
    zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,subscribe, strlen(subscribe));

    push.append("init >");
    push.append(name);
    strcpy(send, push.c_str());
    zmq_send( pusher, send, strlen(send), 0 );    //verzenden
    push = "weerwolven? >";

    while (game)
    {
        rc = zmq_msg_init (&msg);                                                           //read benthernet
        rc = zmq_recvmsg (subscriber, &msg, 0);
        size = zmq_msg_size (&msg);
        memcpy(read, zmq_msg_data(&msg), size);
        zmq_msg_close(&msg);
        read[size] = 0;
        printf("%s\n", read);
        zmq_msg_close (&msg);

        sscanf(read, "%s > %s >%s >%s", subscr, command, data, data2);

        if (strcmp(command, "init") == 0)
        {
            if (init == false)
            {
                sscanf(data, "%d", &id);
                sscanf(data2, "%d", &role);
                printf("Jouw playerid is %d\n", id);
                switch (role)
                {
                case 0:
                    printf("jij bent een burger.\n");
                    break;
                case 1:
                    printf("Jij bent een weerwolf.\n");
                    break;
                case 2:
                    printf("Jij bent de heks.\n");
                    break;
                case 3:
                    printf("Jij bent cupido.\n");
                }
                init = true;
                printf("Wachten op andere spelers...\n");
            }
        }

        if (strcmp(command, "play") == 0)   {play = true;}

        if (play)
        {
            if (role == 0)
            {
                switch (state)
                {
                case 0:                                                                     //wachten op de weerwolf
                    printf("Iedereen gaat slapen.\nDe weerwolf wordt wakker.\n");
                    if (strcmp(command, "next") == 0)   {state++;}
                    break;
                case 1:                                                                     //wachten op de heks
                    printf("De weerwolf gaat terug slapen end de heks wordt wakker.\n");
                    if (strcmp(command, "next") == 0)   {state++;}
                    break;
                case 2:                                                                     //kijken we er vermoord is en stemmen opp wie de weerwolf is
                    printf("De heks gaat terug slapen.\n");
                    if (strcmp(command, "vermoord") == 0)
                    {
                        sscanf(data, "%d", &kill);
                        sscanf(data2, "%d", &killHeks);
                        if (kill > 0)
                        {
                            if (killHeks > 0)
                            {
                                printf("player%d en player%d zijn deze nacht vermoord\n", kill, killHeks);
                                deadlist.append(kill);
                                deadlist.append(killHeks);
                                kill = 0;
                                killHeks = 0;
                            }
                            else
                            {
                                printf("player%d is deze nacht vermoord\n", kill);
                                deadlist.append(kill);
                                kill = 0;
                            }
                        }
                        else    {printf("Er is niemand vermoord deze nacht.\n");}
                        printf("Wie denk je dat de weerwolf is? (type de nummer van de player)\n");
                        scanf("%d", &vote);
                        while (vote < 1 || vote > 5 || deadlist.contains(vote))
                        {
                            printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                            scanf("%d", &vote);
                        }
                        push.append("vote >");
                        push.append(to_string(id));
                        push.append(" >");
                        push.append(to_string(vote));
                        strcpy(send, push.c_str());
                        zmq_send( pusher, send, strlen(send), 0 );    //verzenden
                        push = "weerwolven? >";
                        state++;
                    }
                    break;
                case 3:                                                                     //kijken wie wat heeft gestemd en de uitslag van de stemming
                    if (strcmp(command, "vote") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playervote);
                        printf("player%d heeft op player%d gestemd\n", playerid, playervote);
                    }
                    if (strcmp(command, "result") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playerrole);
                        printf("player%d was een ", playerid);
                        switch (playerrole)
                        {
                        case 0:
                            printf("burger.\n");
                            break;
                        case 1:
                            printf("weerwolf.\n");
                            game = false;
                            break;
                        case 2:
                            printf("heks.\n");
                            break;
                        case 3:
                            printf("cupido.\n");
                            break;
                        }
                        state = 0;
                    }
                    break;
                case 4:
                    printf("Cupido kiest een koppel uit.\n");
                    if (strcmp(command, "koppel") == 0)
                    {
                        sscanf(data, "%d", &koppel1);
                        sscanf(data2, "%d", &koppel2);
                        if (koppel1 == id)  {printf("U bent een koppel met player%d.\n", koppel2);koppel = true;}
                        if (koppel2 == id)  {printf("U bent een koppel met player%d.\n", koppel1);koppel = true;}
                        state = 0;
                    }
                    if (strcmp(command, "next") == 0)   {state = 0;}
                    break;
                }
            }
            else if (role == 1)
            {
                switch (state)
                {
                case 0:                                                                     //wachten op de weerwolf
                    printf("Iedereen gaat slapen.\nDe weerwolf wordt wakker.\n");
                    zmq_send( pusher, "weerwolven? >", 13, 0);
                    if (strcmp(command, "weerwolf") == 0)                                   //weerwolf kiest slachtoffer
                    {
                        printf("Geef het idnummer van de persoon die je wilt vermoorden");
                        scanf("%d", &murder);
                        while (murder < 1 || murder > 5 || deadlist.contains(murder))
                        {
                            printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                            scanf("%d", &murder);
                        }
                        push.append("murder >");
                        push.append("%d >", murder);
                        strcpy(send, push.c_str());
                        zmq_send( pusher, send, strlen(send), 0 );
                        push = "weerwolven? >";
                        state++;
                    }
                    break;
                case 1:                                                                     //wachten op de heks
                    printf("De weerwolf gaat terug slapen end de heks wordt wakker.\n");
                    if (strcmp(command, "next") == 0)   {state++;}
                    break;
                case 2:                                                                     //kijken we er vermoord is en stemmen opp wie de weerwolf is
                    printf("De heks gaat terug slapen.\n");
                    if (strcmp(command, "vermoord") == 0)
                    {
                        sscanf(data, "%d", &kill);
                        sscanf(data2, "%d", &killHeks);
                        if (kill > 0)
                        {
                            if (killHeks > 0)
                            {
                                printf("player%d en player%d zijn deze nacht vermoord\n", kill, killHeks);
                                deadlist.append(kill);
                                deadlist.append(killHeks);
                                kill = 0;
                                killHeks = 0;
                            }
                            else
                            {
                                printf("player%d is deze nacht vermoord\n", kill);
                                deadlist.append(kill);
                                kill = 0;
                            }
                        }
                        else    {printf("Er is niemand vermoord deze nacht.\n");}
                        printf("Wie denk je dat de weerwolf is? (type de nummer van de player)\n");
                        scanf("%d", &vote);
                        while (vote < 1 || vote > 5 || deadlist.contains(vote))
                        {
                            printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                            scanf("%d", &vote);
                        }
                        push.append("vote >");
                        push.append("%d >", id);
                        push.append("%d >", vote);
                        strcpy(send, push.c_str());
                        zmq_send( pusher, send, strlen(send), 0 );    //verzenden
                        push = "weerwolven? >";
                        state++;
                    }
                    break;
                case 3:                                                                     //kijken we wat heeft gestemd en de uitslag van de stemming
                    if (strcmp(command, "vote") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playervote);
                        printf("player%d heeft op player%d gestemd\n", playerid, playervote);
                    }
                    if (strcmp(command, "result") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playerrole);
                        printf("player%d was een ", playerid);
                        switch (playerrole)
                        {
                        case 0:
                            printf("burger.\n");
                            break;
                        case 1:
                            printf("weerwolf.\n");
                            game = false;
                            break;
                        case 2:
                            printf("heks.\n");
                            break;
                        case 3:
                            printf("cupido.\n");
                            break;
                        }
                        state = 0;
                    }
                    break;
                case 4:
                    printf("Cupido kiest een koppel uit.\n");
                    if (strcmp(command, "koppel") == 0)
                    {
                        sscanf(data, "%d", &koppel1);
                        sscanf(data2, "%d", &koppel2);
                        if (koppel1 == id)  {printf("U bent een koppel met player%d.\n", koppel2);koppel = true;}
                        if (koppel2 == id)  {printf("U bent een koppel met player%d.\n", koppel1);koppel = true;}
                        state = 0;
                    }
                    break;
                }
            }
            else if (role == 2)
            {
                switch (state)
                {
                case 0:                                                                     //wachten op de weerwolf
                    printf("Iedereen gaat slapen.\nDe weerwolf wordt wakker.\n");
                    if (strcmp(command, "next") == 0)   {state++;}
                    break;
                case 1:                                                                     //wachten op de heks
                    printf("De weerwolf gaat terug slapen end de heks wordt wakker.\n");
                    if (strcmp(command, "heks") == 0)
                    {
                        sscanf(data, "%d", &murder);
                        printf("player%d is vermoord\nWilt u deze player genezen?(type '1')\nWilt u iemand extra vermoorden?(type '2')\nWilt u niks doen?(type '3'\n", murder);
                        scanf("%d", &keuzeheks);
                        if (keuzeheks == 1)
                        {
                            push.append("heal >");
                            push.append(to_string(murderheks));
                            strcpy(send, push.c_str());
                            zmq_send( pusher, send, strlen(send), 0 );    //verzenden
                            push = "weerwolven? >";
                        }
                        else if (keuzeheks == 2)
                        {
                            printf("Wie wilt u nog vermoorden?\n");
                            scanf("%d", &murderheks);
                            while (murderheks < 1 || murderheks > 5 || deadlist.contains(murderheks))
                            {
                                printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                                scanf("%d", &murderheks);
                            }
                            push.append("heks >");
                            push.append(to_string(murderheks));
                            strcpy(send, push.c_str());
                            zmq_send( pusher, send, strlen(send), 0 );    //verzenden
                            push = "weerwolven? >";
                        }
                        else if (keuzeheks == 3)
                        {
                            printf("U heeft gekozen om niks te doen.\n");
                        }
                        state++;
                    }
                    break;
                case 2:                                                                     //kijken we er vermoord is en stemmen opp wie de weerwolf is
                    printf("De heks gaat terug slapen.\n");
                    if (strcmp(command, "vermoord") == 0)
                    {
                        sscanf(data, "%d", &kill);
                        sscanf(data2, "%d", &killHeks);
                        if (kill > 0)
                        {
                            if (killHeks > 0)
                            {
                                printf("player%d en player%d zijn deze nacht vermoord\n", kill, killHeks);
                                deadlist.append(kill);
                                deadlist.append(killHeks);
                                kill = 0;
                                killHeks = 0;
                            }
                            else
                            {
                                printf("player%d is deze nacht vermoord\n", kill);
                                deadlist.append(kill);
                                kill = 0;
                            }
                        }
                        else    {printf("Er is niemand vermoord deze nacht.\n");}
                        printf("Wie denk je dat de weerwolf is? (type de nummer van de player)\n");
                        scanf("%d", &vote);
                        while (vote < 1 || vote > 5 || deadlist.contains(vote))
                        {
                            printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                            scanf("%d", &vote);
                        }
                        push.append("vote >");
                        push.append("%d >", id);
                        push.append("%d >", vote);
                        strcpy(send, push.c_str());
                        zmq_send( pusher, send, strlen(send), 0 );    //verzenden
                        push = "weerwolven? >";
                        state++;
                    }
                    break;
                case 3:                                                                     //kijken we wat heeft gestemd en de uitslag van de stemming
                    if (strcmp(command, "vote") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playervote);
                        printf("player%d heeft op player%d gestemd\n", playerid, playervote);
                    }
                    if (strcmp(command, "result") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playerrole);
                        printf("player%d was een ", playerid);
                        switch (playerrole)
                        {
                        case 0:
                            printf("burger.\n");
                            break;
                        case 1:
                            printf("weerwolf.\n");
                            game = false;
                            break;
                        case 2:
                            printf("heks.\n");
                            break;
                        case 3:
                            printf("cupido.\n");
                            break;
                        }
                        state = 0;
                    }
                    break;
                case 4:
                    printf("Cupido kiest een koppel uit.\n");
                    if (strcmp(command, "koppel") == 0)
                    {
                        sscanf(data, "%d", &koppel1);
                        sscanf(data2, "%d", &koppel2);
                        if (koppel1 == id)  {printf("U bent een koppel met player%d.\n", koppel2);koppel = true;}
                        if (koppel2 == id)  {printf("U bent een koppel met player%d.\n", koppel1);koppel = true;}
                        state = 0;
                    }
                    break;
                }
            }
            else if (role == 3)
            {
                switch (state)
                {
                case 0:                                                                     //wachten op de weerwolf
                    printf("Iedereen gaat slapen.\nDe weerwolf wordt wakker.\n");
                    if (strcmp(command, "next") == 0)   {state++;}
                    break;
                case 1:                                                                     //wachten op de heks
                    printf("De weerwolf gaat terug slapen end de heks wordt wakker.\n");
                    if (strcmp(command, "next") == 0)   {state++;}
                    break;
                case 2:                                                                     //kijken we er vermoord is en stemmen opp wie de weerwolf is
                    printf("De heks gaat terug slapen.\n");
                    if (strcmp(command, "vermoord") == 0)
                    {
                        sscanf(data, "%d", &kill);
                        sscanf(data2, "%d", &killHeks);
                        if (kill > 0)
                        {
                            if (killHeks > 0)
                            {
                                printf("player%d en player%d zijn deze nacht vermoord\n", kill, killHeks);
                                deadlist.append(kill);
                                deadlist.append(killHeks);
                                kill = 0;
                                killHeks = 0;
                            }
                            else
                            {
                                printf("player%d is deze nacht vermoord\n", kill);
                                deadlist.append(kill);
                                kill = 0;
                            }
                        }
                        else    {printf("Er is niemand vermoord deze nacht.\n");}
                        printf("Wie denk je dat de weerwolf is? (type de nummer van de player)\n");
                        scanf("%d", &vote);
                        while (vote < 1 || vote > 5 || deadlist.contains(vote))
                        {
                            printf("Deze speler bestaat niet of is dood. Kies opnieuw!\n ");
                            scanf("%d", &vote);
                        }
                        push.append("vote >");
                        push.append(to_string(id));
                        push.append(" >");
                        push.append(to_string(vote));
                        strcpy(send, push.c_str());
                        zmq_send( pusher, send, strlen(send), 0 );    //verzenden
                        push = "weerwolven? >";
                        state++;
                    }
                    break;
                case 3:                                                                     //kijken wie wat heeft gestemd en de uitslag van de stemming
                    if (strcmp(command, "vote") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playervote);
                        printf("player%d heeft op player%d gestemd\n", playerid, playervote);
                    }
                    if (strcmp(command, "result") == 0)
                    {
                        sscanf(data, "%d", &playerid);
                        sscanf(data2, "%d", &playerrole);
                        printf("player%d was een ", playerid);
                        switch (playerrole)
                        {
                        case 0:
                            printf("burger.\n");
                            break;
                        case 1:
                            printf("weerwolf.\n");
                            game = false;
                            break;
                        case 2:
                            printf("heks.\n");
                            break;
                        case 3:
                            printf("cupido.\n");
                            break;
                        }
                        state = 0;
                    }
                    break;
                case 4:
                    printf("Cupido kiest een koppel uit.\n");
                    if (strcmp(command, "cupido") == 0)
                    {
                        printf("U mag een koppel uitkiezen.\nType de nummer van de eerste persoon die u in het koppel wilt.");
                        scanf("%d", &koppel1);
                        printf("Type de nummer van de eerste persoon die u in het koppel wilt.");
                        scanf("%d", &koppel2);
                        while (koppel2 == koppel1)  {printf("Dit gaat niet. Type de nummer van de eerste persoon die u in het koppel wilt.");scanf("%d", &koppel2);}
                        push.append("koppel >");
                        push.append(to_string(koppel1));
                        push.append(" >");
                        push.append(to_string(koppel2));
                        strcpy(send, push.c_str());
                        zmq_send( pusher, send, strlen(send), 0 );
                        push = "weerwolven? >";
                    }
                    if (strcmp(command, "koppel") == 0)
                    {
                        sscanf(data, "%d", &koppel1);
                        sscanf(data2, "%d", &koppel2);
                        if (koppel1 == id)  {printf("U bent een koppel met player%d.\n", koppel2);koppel = true;}
                        if (koppel2 == id)  {printf("U bent een koppel met player%d.\n", koppel1);koppel = true;}
                        state = 0;
                    }
                    break;
                }
            }
        }
    }

    return 0;
}



