#include<ctype.h>
#include<cstddef>
#include<cstdio>
#include<cstdlib>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>
#include<unistd.h>
#include<iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
#define SIZE sizeof(struct sockaddr_in)
int newsockfd;

void  * receiver(void* argument){
char c[256];
while(1)
    {
    if(recv(newsockfd, &c, sizeof(c), 0)>0)
        {
        cout<<"\nКлиент: ";
        cout<<c;
        }
    }
}

void  * sender(void * argument)
{
char c[256];
while(1)
    {
    cout<<"Type message"<<endl;
    fgets(c,256,stdin);
    send(newsockfd, &c, sizeof(c), 0);
    }
}

void catcher(int sig)
{
close(newsockfd);
exit(0);
}

int main()
{
pthread_t t1, t2;
int sockfd;
char c,mess,mess1;
struct sockaddr_in server = {AF_INET, 7000, INADDR_ANY};
static struct sigaction act;
act.sa_handler = catcher;
sigfillset(&(act.sa_mask));
sigaction(SIGPIPE, &act, NULL);
/* Создает сокет */
if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
    perror("error");
    exit (1);
    }
/* Связывает адрес с сокетом */
if ( bind(sockfd, (struct sockaddr *)&server, SIZE) == -1)
    {
    perror("Ошибка вызова bind");
    exit (1);
    }
/* Включает прием соединений */
if ( listen(sockfd, 5) == -1 )
    {
    perror("Ошибка вызова listen");
    exit(1);
    }
else{
    for ( ;;)
        {
        if ( (newsockfd = accept(sockfd, NULL,NULL)) == -1)
        {
            perror("Ошибка вызова accept");
        }
        else{
            /* Принимает запрос на соединение */
            pid_t pid=fork();
            if(pid==0)
                {
                        pthread_create( &t1, NULL, receiver,NULL); // create a thread running function1
                        pthread_create( &t2, NULL, sender,NULL);
                        pthread_join(t1,NULL);
                        pthread_join(t2,NULL);
                        sleep(3);
                        exit(0);
                }
            else
                {
                close(newsockfd);
                cout<<"Error fork"<<endl;
                sleep(4);
                wait();
                }
        }
    }
}
close(newsockfd);
}

