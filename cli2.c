
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sockfd;//客户端socket
int filefd;
char* IP = "192.168.10.143";//服务器的IP
short PORT = 10222;//服务器服务端口
typedef struct sockaddr SA;
char name[30];




char square[10] = { 'o', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

int checkwin();
void board();

int checkwin()
{
    if (square[1] == square[2] && square[2] == square[3])
        return 1;
        
    else if (square[4] == square[5] && square[5] == square[6])
        return 1;
        
    else if (square[7] == square[8] && square[8] == square[9])
        return 1;
        
    else if (square[1] == square[4] && square[4] == square[7])
        return 1;
        
    else if (square[2] == square[5] && square[5] == square[8])
        return 1;
        
    else if (square[3] == square[6] && square[6] == square[9])
        return 1;
        
    else if (square[1] == square[5] && square[5] == square[9])
        return 1;
        
    else if (square[3] == square[5] && square[5] == square[7])
        return 1;
        
    else if (square[1] != '1' && square[2] != '2' && square[3] != '3' &&
        square[4] != '4' && square[5] != '5' && square[6] != '6' && square[7] 
        != '7' && square[8] != '8' && square[9] != '9')

        return 0;
    else
        return  - 1;
}

void board()
{
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");

    printf("Player 1 (X)  -  Player 2 (O)\n\n\n");


    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", square[1], square[2], square[3]);

    printf("_____|_____|_____\n");
    printf("     |     |     \n");

    printf("  %c  |  %c  |  %c \n", square[4], square[5], square[6]);

    printf("_____|_____|_____\n");
    printf("     |     |     \n");

    printf("  %c  |  %c  |  %c \n", square[7], square[8], square[9]);

    printf("     |     |     \n\n");
}



void init(){
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(PORT);
    //addr.sin_addr.s_addr = inet_addr(IP);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    
    if (connect(sockfd,(SA*)&addr,sizeof(addr)) == -1){
        perror("error");
        exit(-1);
    }
    printf("<< Client start >> \n");
}


void start(){

    char buf2[100] = {};

    while(1){
        recv(sockfd,buf2,sizeof(buf2),0);
        if (strcmp(buf2,"server-req-name?") == 0){
            send(sockfd,name,strlen(name),0);
            break;
        }
    }


    pthread_t id;
    void* recv_thread(void*);
    pthread_create(&id,0,recv_thread,0);
    
    while(1){
        char buf[100] = {};
        fgets(buf, sizeof(buf), stdin);
        char *ptr = strstr(buf, "\n");
        *ptr = '\0';
        char msg[131] = {};
        if (strcmp(buf,"endgame") == 0){
            memset(buf2,0,sizeof(buf2));
            sprintf(buf2,"[INFO] %s quit game",name);
            send(sockfd,buf2,strlen(buf2),0);
            break;
        }
        if (strcmp(buf,"ls") == 0){
            memset(buf2,0,sizeof(buf2));
            sprintf(buf2,"ls");
            send(sockfd,buf2,strlen(buf2),0);
        }
        else{
            sprintf(msg,"%s:%s",name,buf);
            send(sockfd,msg,strlen(msg),0);

			
	int player = 1, i, choice;

	    char mark;
	    do
	    {
		board();
		player = (player % 2) ? 1 : 2;

		printf("Player %d, enter a number:  ", player);
		scanf("%d", &choice);

		mark = (player == 1) ? 'X' : 'O';

		if (choice == 1 && square[1] == '1')
		    square[1] = mark;
		    
		else if (choice == 2 && square[2] == '2')
		    square[2] = mark;
		    
		else if (choice == 3 && square[3] == '3')
		    square[3] = mark;
		    
		else if (choice == 4 && square[4] == '4')
		    square[4] = mark;
		    
		else if (choice == 5 && square[5] == '5')
		    square[5] = mark;
		    
		else if (choice == 6 && square[6] == '6')
		    square[6] = mark;
		    
		else if (choice == 7 && square[7] == '7')
		    square[7] = mark;
		    
		else if (choice == 8 && square[8] == '8')
		    square[8] = mark;
		    
		else if (choice == 9 && square[9] == '9')
		    square[9] = mark;
		    
		else
		{
		    printf("Invalid move ");

		    player--;
		    //getch();
		}
		i = checkwin();

		player++;
	    }while (i ==  - 1);
	    
	    board();
	    
	    if (i == 1)
		printf("==>\aPlayer %d win ", --player);
	    else
		printf("==>\aGame draw");

	    //getch();


        }
    }
    close(sockfd);
}

void* recv_thread(void* p){
    while(1){
        char buf[100] = {};
        if (recv(sockfd,buf,sizeof(buf),0) <= 0){
            return;
        }
        if (strcmp(buf,"server-req-name ?") == 0){
            send(sockfd,name,strlen(name),0);
        }
        else{
            printf("%s\n",buf);
        }
    }
}

int main(){
    init();
    printf("ENTER account：");
    scanf("%s",name);
    start();
    return 0;
}
