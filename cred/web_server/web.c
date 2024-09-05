#include "proxy_parse.h";
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <sys/types.h> //Defines various data types used in system calls.
#include <sys/socket.h>
#include <netinet/in.h> //Defines Internet address family and structures for sockets.
#include <arpa/inet.h> //Provides functions for manipulating IP addresses (like inet_addr())
#include <unistd.h> //functions/api s from POSIX os like read(), write, close()
#include <fcntl.h> //file_Control
#include <time.h>
#include <sys/wait.h> // declarations for waiting ( for process state changes )
#include <errno.h>
#include<pthread.h>
#include<semaphore.h> //semaphores for process sync, advanced locking features

#define MAX_CLIENT 10 
#define MAX_BYTES 4096

typedef struct cache_elements cache_elements;



struct cache_elements{
    char* data;
    int length;
    char* url;

    //time based LRU cache(least recently used)
    time_t lru_time_track;
    
    //lru element ka ek element ( of the whole linked list)
    cache_elements* next;

};

	//chand functions 
	cache_elements* find(char* url);
	int add_cache_element(char* data, int size, char* url);
	void remove_cache_element();

	int port_number = 8080;
	int proxy_socketID;
	pthread_t tid[MAX_CLIENT];
	sem_t semaphore;
	pthread_mutex_t lock; //mutual exclusion

	//cache ke functions already defined, now time to implement cache ka head
	cache_elements* head;
	int cache_size;



//------------functions code---------------
//anything can be passed in void*-- int, char, anything
void *thread(void *socketNew){
	sem_wait(&semaphore);
	int p;
	sem_getvalue(&semaphore, p);
	printf('semaphore value is: %d\n', p);

	//jo socket iss function me aaya hai, usko store karega
	int *t = (int*) socketNew;
	int socket = *t; // deferencing t pointer == value nikalna

	int bytes_send_client, len;

	char *buffer = (char*)calloc(MAX_BYTES, sizeof(char));
	bzero(buffer, MAX_BYTES);
	bytes_send_client = recv(socket, buffer, MAX_BYTES, 0);

	while(bytes_send_client > 0){
		len = strlen(buffer);
		if(strstr(buffer, "\r\n\r\n") == NULL){
			bytes_send_client = recv(socket, buffer + len, MAX_BYTES-len, 0);
		}else{
			break;
		}
	} 

}	

















int main (int argc, char* argv[]){
	//jobhi client socket open karna chahega uska, socket id, and address ki length
	int client_socketId, client_sock_len;

	/*	
		idhar agar sockaddr_in aajaaye(which is originally in code, then it stands for internet socket)
		struct sockaddr  {
	__uint8_t       sa_len;          total length 
	sa_family_t     sa_family;       [XSI] address family 
	char            sa_data[14];     [XSI] addr value (actually larger) 

	*/

	struct sockaddr_in server_addr, client_addr;

	//here 2nd argument is pshared value, can be(0 or 1)
	sem_init(&semaphore,0,  MAX_CLIENT);
	pthread_mutex_init(&lock, NULL);

	//condition for ... when starting the server, to ask for port numbers
	if(argv == 2){
		//atoi changes strings to integer, toh jo value cmd terminal me strings me hogi... wo number me aa jaaegi
		port_number = atoi(argv[1]);
	}else{
		printf("give the port number as argument");
		exit(1);
	}

	printf("starting proxy server @ port: %d", port_number);







	/*
		just to make things clear, next line is proxy_socketID, every request will come
		from one one socket only into the server, as it comes, it will be alloted a thread,
		i.e. a thread will respawn to handle that req, at last that thread will directly respond to the requesting server.
	*/


	//address family- ipv4 protocol, sockstream indicates tcp socket-- specifies two way connection -- handshake in layman
	//this will store file descriptors for socket file
	
	 proxy_socketID = socket(AF_INET, SOCK_STREAM, 0);
	 if(proxy_socketID < 0){
		perror("failed in creating socket.");
		exit(1);
	 }else{
		printf('socket created...\n');
	 }

	 //if socket created successfully, we will reuse socket.
		int reuse = 1;
		if(setsockopt(proxy_socketID, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse))){
			perror('setSockOpt failed miserably\n');
		}
		
	//cleaning the variables and structures, as in C, by default compiler gives garbage value
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind((proxy_socketID), (struct sockaddr*)&server_addr, sizeof(server_addr))< 0){
		perror("port binding failed\n");
	}else{
		printf("port binded successfully...\n port available on %d", port_number);
	}

	int listen_status = listen(proxy_socketID, MAX_CLIENT);

	if(listen_status < 0){
		perror("error in listening\n");
		exit(1);
	}
	
	//keeping count of how many clients got connected 
	int i = 0;
	int Connected_socketId[MAX_CLIENT];


	//running infinite loop.
	while(1){
		//removing garbage value
		bzero((char*)&client_addr, sizeof(client_addr));
		client_sock_len = sizeof(client_addr);

		//opening socket, client ke socket se accept karenge, to proxy ke socket tak
		//doing typecasting here, as function would only accept struct* in this format ( c thingy)
		client_socketId = accept(proxy_socketID, (struct sockaddr *)&client_addr, (socklen_t*)&client_sock_len	);

		if(client_socketId< 0){
			printf("not able to connect...\n");
			exit(1);
		}else{
			Connected_socketId[i] = client_socketId;
		}



// all this to print pc's ip in text format----
/*
	notes- 
	 ntohs() function converts the unsigned short integer netshort from
       network byte order to host byte order.

	inet_ntop()- onverts the network address structure src in the 
					af address family into a character string.

*/
		//making copy of client address
		struct sockaddr_in* client_pt = (struct sockaddr_in*)&client_addr;
		struct in_addr ip_addr = client_pt->sin_addr;
		//ques- why not direct ip_addr = client_addr->sin_addr;

		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ip_addr, str, INET_ADDRSTRLEN);
		printf("client is connected to port no %d and ip adress is: %s", ntohs(client_addr.sin_port), str);



		pthread_create(&tid[i], NULL, thread_fn, (void *)&Connected_socketId[i]);
		i++;

	}

	close(proxy_socketID);
	return 0;

}