
#include "proxy_parse.h"
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
#include<netdb.h>


#define MAX_CLIENT 10 
#define MAX_BYTES 4096
#define MAX_ELEMENT_SIZE 10*(1<<10)
#define MAX_SIZE 200*(1<<20) //cache ka total size  200 into 2 to the power 20

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

	int 	port_number = 8080;
	int proxy_socketID;
	pthread_t tid[MAX_CLIENT];
	sem_t semaphore;
	pthread_mutex_t lock; //mutual exclusion

	//cache ke functions already defined, now time to implement cache ka head
	cache_elements* head;
	int cache_size;



//------------functions code---------------
//anything can be passed in void*-- int, char, anything
//list of function-- handle_request, thread, connectRemoteServer

int connectRemoteServer(char* host_addr, int port_num){
	/**/
	int remoteSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(remoteSocket < 0){
		printf("error in creatin your socket...\n");
		return -1;
	}

	/*host ent stands for host entries,tip- from stack()verflow never look this up on internet always man on terminal*/
	struct hostent* host = gethostbyname(host_addr);
	if(host == NULL){
		perror("no such thing exists...");
		return -1;
	}

	struct sockaddr_in server_addr;
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET; // setting socket_internet family to AF_INET...
	server_addr.sin_port = htons(port_num);  //htons -> network port me convert karega integers ko

	//bitcopy
	bcopy((char *)host -> h_addr, (char*)&server_addr.sin_addr.s_addr, host-> h_length);


	//connect three arg - socketDescriptor, add of remote server, 
	if(connect(remoteSocket, (struct sockaddr *)&server_addr, (size_t)sizeof(server_addr)) < 0){
		fprintf(stderr, "error in connecting to remote server.\n");
		return -1;
	}
	return remoteSocket;

	

}

int handle_request(int ClientSocketId, struct ParsedRequest *request, char* tempReq){
	
	char *buff =  (char *)malloc(sizeof(char)*MAX_BYTES);
	strcpy(buff, "GET ");
	strcat(buff, request -> path);	
	strcat(buff, " ");
	strcat(buff, request -> version );
	strcat(buff, "\r\n");

	
	size_t len = strlen(buff);

	if(ParsedHeader_set(request, "Connection", "close") < 0){
		printf("set header is not working\n");

	}

	if(ParsedHeader_get(request, "Host") == NULL){
		if(ParsedHeader_set(request, "Host", request -> host) < 0){
			printf("set Host header key is not working in this universe...\n");
		}
	}


	if(ParsedRequest_unparse_headers(request, buff+ len, (size_t)MAX_BYTES-len) <0 ){
		printf("unparse failed\n");
	}

	//end server, where it will send it's response, i.e. example google, facebook jo bhi
	int server_port = 80;
	if(request -> port != NULL){
		//if port is 8080 overwrite it...
		server_port = atoi(request -> port );
	}
	int remoteSocketId = connectRemoteServer(request -> host, server_port);
	if(remoteSocketId < 0 ){
		return -1;
	}

	//sendin bytes to server
	int bytes_send = send(remoteSocketId, buff, strlen(buff), 0);
	bzero(buff, MAX_BYTES);

	bytes_send = recv(remoteSocketId, buff, MAX_BYTES - 1, 0); // -1: when we receive data from server, last bit is '\0', so for aesthetics
	char *temp_buffer = (char *)malloc(sizeof(char)*MAX_BYTES);
	int temp_buffer_size = MAX_BYTES;
	int temp_buffer_index = 0;


	//don't go on the name, jab tak receive karte rahenge, tab tak this is > 0
	while(bytes_send > 0){
		bytes_send = send(ClientSocketId, buff, bytes_send, 0);
		for(size_t i = 0; i < (size_t)bytes_send/sizeof(char); i++){
			temp_buffer[temp_buffer_index] = buff[i];
			temp_buffer_index++;
		}

		temp_buffer_size += MAX_BYTES;
		temp_buffer = (char*)realloc(temp_buffer, temp_buffer_size);

		if(bytes_send < 0){
			 perror("error in sending data to the client\n");
			 break;
		}

		//cleaning to send  again 
		bzero(buff, MAX_BYTES);
		bytes_send = recv(remoteSocketId, buff, MAX_BYTES -1, 0);
	}

	// this v imp line, as in if temp buffer is of 5-6 gb and response is of 
	// 1 gb(for example)so it will bring only till 1gb	
	temp_buffer[temp_buffer_index] = '\0'; 
	free(buff);
	add_cache_element(temp_buffer, strlen(temp_buffer),tempReq);

	free(temp_buffer);
	close(remoteSocketId);
	return 0;


}





//switch cases are from gpt/

int SendErrorMessage(int socket, int status_code){
	char str[1024];
	char currentTime[50];
	time_t now = time(0);

	// Get current time in GMT format
	struct tm data = *gmtime(&now);
	strftime(currentTime, sizeof(currentTime), "%a, %d, %b, %Y, %H:%M:%S GMT", &data);

	switch(status_code){
		case 400: 
			snprintf(str, sizeof(str), 
				"HTTP/1.1 400 Bad Request\r\n"
				"Date: %s\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: 52\r\n"
				"\r\n"
				"<html><body><h1>400 Bad Request</h1></body></html>\r\n", 
				currentTime);
			printf("Sending 400 Bad Request error\n");
			send(socket, str, strlen(str), 0);
			break;

		case 403: 
			snprintf(str, sizeof(str), 
				"HTTP/1.1 403 Forbidden\r\n"
				"Date: %s\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: 51\r\n"
				"\r\n"
				"<html><body><h1>403 Forbidden</h1></body></html>\r\n", 
				currentTime);
			printf("Sending 403 Forbidden error\n");
			send(socket, str, strlen(str), 0);
			break;

		case 404: 
			snprintf(str, sizeof(str), 
				"HTTP/1.1 404 Not Found\r\n"
				"Date: %s\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: 50\r\n"
				"\r\n"
				"<html><body><h1>404 Not Found</h1></body></html>\r\n", 
				currentTime);
			printf("Sending 404 Not Found error\n");
			send(socket, str, strlen(str), 0);
			break;

		case 500: 
			snprintf(str, sizeof(str), 
				"HTTP/1.1 500 Internal Server Error\r\n"
				"Date: %s\r\n"
				"Content-Type: text/html\r\n"
				"Content-Length: 58\r\n"
				"\r\n"
				"<html><body><h1>500 Internal Server Error</h1></body></html>\r\n", 
				currentTime);
			printf("Sending 500 Internal Server Error\n");
			send(socket, str, strlen(str), 0);
			break;

		default:
			printf("Unknown status code: %d\n", status_code);
			return -1;
	}

	return 1;
}


int CheckHTTPversion(char* msg){
	int version = -1;

	if(strncmp(msg, "HTTP/1.1", 8) == 0){
		version = 1;
	}else if(strncmp(msg, "HTTP/1.0", 8) == 0){
		version = 1;
	}else{
		version = -1;
	}

	return version;
}



void *thread(void *socketNew){
	sem_wait(&semaphore);
	int p;
	sem_getvalue(&semaphore, &p);
	printf("semaphore value: %d\n", p);

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

	//technically, ab ho ye raha hai ki, req, aayi hai hamari proxy tak... 
	//hum search karenge, but usse pehle we will copy it
	//not necesary to make copy, but it is good practice	
	char *tempReq = (char *)malloc(strlen(buffer)*sizeof(buffer) + 1);
	 
	for(size_t i = 0; i < strlen(buffer); i++){
		tempReq[i] = buffer[i];
	}

	//finding our req in cache memory... if we find... it then beautyfull, if not
	struct cache_elements* temp = find(tempReq);
	if(temp!= NULL){
		int size = temp->length/sizeof(char);
		int pos = 0;

		char response[MAX_BYTES];
		while(pos < size){
			bzero(response, MAX_BYTES);
			for(int i = 0; i < MAX_BYTES; i++){
				response[i] = temp->data[i];
				pos++;
			}
			send(socket, response, MAX_BYTES, 0);
		}
		printf("data retrieved from cache \n");
		printf("%s\n\n", response);

	}else if(bytes_send_client > 0){
		len = strlen(buffer);

		//ignore this error in vscode
		struct ParsedRequest *request = ParsedRequest_create();

		//buffer me request hai...
		if(ParsedRequest_parse(request, buffer, len) < 0){
			printf("parsin failed\n");
		}else{
			
			bzero(buffer, MAX_BYTES);

			if(!strcmp(request->method, "GET")){
				if( request -> host && request -> path && CheckHTTPversion(request -> version) == 1){
					bytes_send_client = handle_request(socket, request, tempReq);

					if(bytes_send_client == -1){
						SendErrorMessage(socket, 500);
					}
				}else{
					SendErrorMessage(socket, 500);
				}
			}else{
				printf("this mode doesn't support get request\n");
			}
		}

		ParsedRequest_destroy(request);

	}else if(bytes_send_client < 0){
		printf("error in recevin from client\n");
	}else if(bytes_send_client == 0){
		printf("client is disconnected\n");
	}

	shutdown(socket, SHUT_RDWR);
	close(socket);
	free(buffer);
	sem_post(&semaphore);
	sem_getvalue(&semaphore, &p);
	printf("semaphore post value is %d\n", p);
	free(tempReq);
	return NULL;
}	




/*--------------------main ----------------------------------------*/


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
	sem_init(&semaphore, 0,  MAX_CLIENT);
	pthread_mutex_init(&lock, NULL);

	
	//condition for ... when starting the server, to ask for port numbers
	//checking, for 2 arguments
	if(argc == 2){
		//atoi changes strings to integer, toh jo value cmd terminal me strings me hogi... wo number me aa jaaegi
		port_number = atoi(argv[1]);
	}else{
		printf("give the port number as argument\n");
		exit(1);
	}

	printf("starting proxy server @ port: %d \n", port_number);







	/*
		just to make things clear, next line is proxy_socketID, every request will come
		from one one socket only into the server, as it comes, it will be alloted a thread,
		i.e. a thread will respawn to handle that req, at last that thread will directly respond to the requesting server.
	*/


	//address family- ipv4 protocol, sockstream indicates tcp socket-- specifies two way connection -- handshake in layman
	//this will store file descriptors for socket file
	
	//third arg, here means default protocol, '0' for sockstream is tcp, for sockdgram is UDP.
	 proxy_socketID = socket(AF_INET, SOCK_STREAM, 0);
	 if(proxy_socketID < 0){
		perror("failed in creating socket.");
		exit(1);
	 }else{
		printf("socket created...\n");
	 }

	 //if socket created successfully, we will reuse socket.
		int reuse = 1;
		if(setsockopt(proxy_socketID, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse))){
			perror("setSockOpt failed miserably\n");
		}
		
	//cleaning the variables and structures, as in C, by default compiler gives garbage value
	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	server_addr.sin_addr.s_addr = INADDR_ANY;    // specifying to connect to any type of ip addresss/ any type of tcp addresss

	
	if(bind((proxy_socketID), (struct sockaddr*)&server_addr, sizeof(server_addr))< 0){
		perror("port binding failed\n");
	}else{
		printf("port binded successfully...\nport available on %d \n", port_number);
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
		printf("\n---client is connected to port no %d and ip adress is: %s \n", ntohs(client_addr.sin_port), str);



		pthread_create(&tid[i], NULL, thread, (void *)&Connected_socketId[i]);
		i++;

	}

	close(proxy_socketID);
	return 0;

}


//cache functions
cache_elements *find(char* url){
	cache_elements *site = NULL;
	int temp_lock_value = pthread_mutex_lock(&lock);
	printf("remove cache lock acquired: %d \n", temp_lock_value);

	if(head != NULL){
		site = head;
		while(site!=NULL){
			if(!strcmp(site -> url, url)){
				printf("LRU time track before: %ld \n", site->lru_time_track);
				printf("\n...url found...\n");

				//setting the just added url to be least used value i.e. zero:)
				site -> lru_time_track = time(NULL);
				printf("LRU time tracked after %ld \n", site -> lru_time_track);
				break;
			} 
			site = site -> next;
		}
	}else{
		printf("url not found \n");
	}

	temp_lock_value = pthread_mutex_unlock(&lock);
	printf("lock is removed %d\n", temp_lock_value);
	return(site);
	 
}

void remove_cache_element(){
	//simple LinkedL removal...
	cache_elements *p;
	cache_elements *q;
	cache_elements *temp;

	int temp_lock_value = pthread_mutex_lock(&lock);
	printf("remove cache lock is acquired %d\n", temp_lock_value);

	if(head!= NULL){
		for(q=head, p = head, temp=head; q != NULL; q = q -> next){
			if(((q -> next) -> lru_time_track) < (temp -> lru_time_track)){
				temp = q -> next;
				p = q;
				//q toh next ho hee jaega as 3rd condition of for loop.
			}

			if(temp == head){
				head = head -> next;
			}else{
				p -> next = temp->next;
			}
		}
		cache_size = cache_size - (temp ->length)-sizeof(cache_elements) - strlen(temp -> url) - 1;

		free(temp -> url);
		free(temp -> data);
		free(temp);
	}

	temp_lock_value = pthread_mutex_unlock(&lock);
	printf("remove cache Lock unlocked %d\n", temp_lock_value);
	  
}

int add_cache_element(char *data, int size, char *url){
	int temp_lock_val = pthread_mutex_lock(&lock);
	printf("add cache lock acquired %d\n", temp_lock_val);

	int element_size = size + 1 + strlen(url) + sizeof(cache_elements);

	if(element_size > MAX_ELEMENT_SIZE){
		temp_lock_val = pthread_mutex_unlock(&lock);
		printf("add cache lock is unlocked\n");
		return 0;
	}else{
		while(cache_size + element_size > MAX_SIZE){
			remove_cache_element();
		}

		cache_elements * element = (cache_elements *)malloc(sizeof(cache_elements));
		element->data = (char*)malloc(size+1);

		strcpy(element -> data, data);
		element -> url = (char*)malloc(1 + (strlen(url)* sizeof(char)));

		strcpy(element -> url, url);

		element ->lru_time_track = time(NULL);
		element -> next = head;
		element -> length = size;
		head = element;
		cache_size += element_size;
		temp_lock_val = pthread_mutex_unlock(&lock);

		printf("add cache unlocked\n");
		return 1;

	}

	return 0;
}