#include "Drone.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <iostream>

#define BUFLEN 512
#define ADRESSEIP "192.168.1.1"
#define PORT 5556
#define PAUSE 50

using namespace std;

Drone::Drone(){
	s=100;
	x=0;
	y=0;
	alt=0;
}

void Drone::drop(char *AT){
	
	//cout<<AT<<endl;
	
    struct sockaddr_in serv_addr;
    int sockfd, slen=sizeof(serv_addr);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        cout<<"socket"<<endl;
    }
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_aton(ADRESSEIP, &serv_addr.sin_addr)==0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    if (sendto(sockfd, AT, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)==-1)
    {
        cout<<"sendto()"<<endl;
    }
    close(sockfd);
}

void Drone::rotLeft(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,0,0,0,-1082130432\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::rotRight(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,0,0,0,1065353216\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::down(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,0,0,-1082130432,0\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::up(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,0,0,1065353216,0\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::backward(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,0,1065353216,0,0\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::forward(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,0,-1082130432,0,0\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::right(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,1065353216,0,0,0\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::left(double ms){
	char AT[1024];
	int i;
	
	for(i=0; i<ms/PAUSE; i++){
		s++;
		sprintf(AT, "AT*PCMD=%d,1,-1082130432,0,0,0\r", s);
		drop(AT);
		tempo(PAUSE);
	}
}

void Drone::takeOff(){
	char AT[1024];
	
	s++;
	sprintf(AT, "AT*REF=%d,290718208\r", s);
	drop(AT);
}

void Drone::land(){
	char AT[1024];
	
	s++;
	sprintf(AT, "AT*REF=%d,290717696\r", s);
	drop(AT);
}

void Drone::tempo(double ms){
	usleep(ms*1000);
}

