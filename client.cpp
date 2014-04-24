#include<ctype.h>
#include<cstddef>
#include<cstdio>
#include<cstdlib>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include <pthread.h>
using namespace std;




#define SIZE sizeof(struct sockaddr_in)
int newsockfd;

void  * receiver(void* argument){
char c[256];

if(recv(newsockfd, &c, sizeof(c), 0)>0){

    cout<<"Сервер: ";
    cout<<c;
    }

}



void  * sender(void * argument)
{
char c[256];
cout<<"Type your message"<<endl;
    fgets(c,256,stdin);
    send(newsockfd, &c, sizeof(c), 0);

}

main()
{
int sockfd;
pthread_t t1, t2;
char c;
char rc[256];
struct sockaddr_in server = {AF_INET, 7000};
/* Преобразовывает и сохраняет IP адрес сервера */
server.sin_addr.s_addr = INADDR_ANY;
/* Создает сокет */
if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
{
perror("Ошибка вызова socket");
exit (1) ;
}
/* Соединяет сокет с сервером */
if ( connect(sockfd, (struct sockaddr *)&server, SIZE) == -1)
{
perror("Ошибка вызова connect");
exit (1);
}
/* Цикл обмена данными с сервером */
for(;;){


pthread_create( &t2, NULL, sender,NULL);
pthread_create( &t1, NULL, receiver,NULL); // create a thread running function1

pthread_join(t1,NULL);
pthread_join(t2,NULL);

//close(newsockfd);

}
}
