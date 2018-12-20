/*
 * ZeroTier SDK - Network Virtualization Everywhere
 * Copyright (C) 2011-2017  ZeroTier, Inc.  https://www.zerotier.com/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --
 *
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial closed-source software that incorporates or links
 * directly against ZeroTier software without disclosing the source code
 * of your own application.
 */

#include <stdio.h>
#include <string.h>
#include <string>
#include <inttypes.h>
#include <unistd.h>

#if defined(_WIN32)
#include <WinSock2.h>
#include <stdint.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif



// UDP Server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "libzt.h"

int mainztoudpclient(char *msg, int leng);

struct sockaddr_in g_toAddr;
int g_sockudp;

int mainudpserver()
{
	// Create UDP socket
	g_sockudp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (g_sockudp < 0)
	{
		printf("Create socket faild\r\n");
		exit(1);
	}

	// Bind socket and the client of send link request
	struct sockaddr_in fromAddr;
	memset(&fromAddr, 0, sizeof(fromAddr));
	fromAddr.sin_family = AF_INET;
	fromAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	fromAddr.sin_port = htons(4000);

	if (bind(g_sockudp, (struct sockaddr*)&fromAddr, sizeof(fromAddr)) < 0)
	{
		printf("Bind socket faild!\r\n");
		close(g_sockudp);
		exit(1);
	}

	char recvBuffer[1024*128] = {0};
	int recvLen = -1;
	unsigned int addrLen = sizeof(g_toAddr);

	while(1)
	{
		if ((recvLen = recvfrom(g_sockudp, recvBuffer, 1024*128, 0, (struct sockaddr*)&g_toAddr, &addrLen)) < 0)
		{
			printf("Receive -------------------------message faild!\r\n");
			close(g_sockudp);
			exit(1);
		}

		if (recvLen > 0)
		{
			//printf("1--udp  %i \r\n", recvLen);
			mainztoudpclient(recvBuffer,recvLen);
			//printf("2---udp\r\n");
		}else
		{
			printf("Recive ----------------------------------------message:%s\r\n", recvBuffer);
		}
	}

	close(g_sockudp);
	return 0;
}

struct sockaddr_in g_remote4;
int g_sockfd;

int mainztoudpinit()
{
	std::string path        = "./";
	std::string nwidstr     = "17d709436c911e4f";
	std::string remote_addr = "10.241.21.212";
	int remote_port         = 9999;
	int r=0, w=0, err=0;
	//char rbuf[32];
	//memset(rbuf, 0, sizeof rbuf);

	struct sockaddr_in in4;
	in4.sin_port = htons(remote_port);
	in4.sin_addr.s_addr = INADDR_ANY;
	in4.sin_family = AF_INET;


	g_remote4.sin_port = htons(remote_port);
	g_remote4.sin_addr.s_addr = inet_addr(remote_addr.c_str());
	g_remote4.sin_family = AF_INET;

	// --- BEGIN EXAMPLE CODE

	printf("Waiting for libzt to come online...\n");
	uint64_t nwid = strtoull(nwidstr.c_str(),NULL,16);
	printf("nwid=%llx\n", (unsigned long long)nwid);
	zts_startjoin(path.c_str(), "17d709436c911e4f");//nwid);

	if ((g_sockfd = zts_socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error creating ---------------ZeroTier socket\n");
	}

	if ((err = zts_bind(g_sockfd, (struct sockaddr *)&in4, sizeof(struct sockaddr_in)) < 0)) {
		printf("error binding -------------------to interface (%d)\n", err);
	}
}

int g_flags = 0;

int mainztoudpclient(char *msg, int len)
{
	int err=0;
	g_flags=0;

	if ((err = zts_sendto(g_sockfd, msg, len, g_flags, (const struct sockaddr *)&g_remote4, sizeof(g_remote4))) < 0) {
			printf("error -------------------------------sending message to remote host (%d)\n", err);
		}
	/*
	printf("reading from server...\n");
	r = zts_read(sockfd, rbuf, strlen(msg));

	printf("Sent     : %s\n", msg);
	printf("Received : %s\n", rbuf);
	*/

	return err;
}

void *mainztoudpserver(void*)
{
	char rbuf[32*1024];
	memset(rbuf, 0, sizeof rbuf);
	int flags = 0;

	struct sockaddr_in in4, acc_in4;
	printf("reading from client...\n");
	socklen_t addrlen = sizeof(acc_in4);
	memset(&acc_in4, 0, sizeof acc_in4);

	while(true) {
		printf("---zts0\r\n");
		memset(&rbuf, 0, sizeof rbuf);
		flags =0;
		int r = zts_recvfrom(g_sockfd, rbuf, sizeof(rbuf), flags, (struct sockaddr *)&acc_in4, &addrlen);
		if (r >= 0) {
			printf("-----zts\r\n");
			if (sendto(g_sockudp, rbuf, r, 0, (struct sockaddr*)&g_toAddr, sizeof(g_toAddr)) != r)
			{
				printf("sendto faild\r\n");
				close(g_sockudp);
				exit(1);
			}
			printf("--------------zts\r\n");
		}else{
			printf("-------------------------------------------\r\n");
		}
	}
}


#include <pthread.h>
int pthread_equal(pthread_t t1, pthread_t t2);

int main()
{
	mainztoudpinit();
	pthread_t thread1;
	int error = pthread_create (&thread1, NULL, mainztoudpserver, NULL);
	if (error)
	{
		perror ("pthread_create");
		exit (EXIT_FAILURE);
	}
	mainudpserver();
	return 0;
}
