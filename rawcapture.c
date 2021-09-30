// MIT License
// Copyright (c) 2021 viviglen <glen4linux@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>/*for uint16_t*/
#include <stdio.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
 
int main(int argc,char* argv[])
{
	char * buffer = NULL;
	int recv_size = 0;
	struct ifreq ifstruct;
	struct sockaddr_ll sll;
	int ssl_size = 0;
	int p_size = 0;
	int c_size = 0;
	memset(&sll,0,sizeof(struct sockaddr_ll));
	int sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
 
	sll.sll_family = PF_PACKET;
	sll.sll_protocol = htons(ETH_P_ALL); 

	if( argc < 3 || argc > 4){
		printf("Usage: rawcapture [interface] [size] [times]\n    [interface]: eth0,rndis0,......\n    [size]: capture and display buffer size\n    [times]: capture times, if NULL will always running\n\n    e.g.\n        rawcapture eth0 32 8\n");
		return 0;
	}

	p_size = atoi(argv[2]);
	if(p_size <= 0|| p_size > 14000){
		printf("the print data size error, 0 < size < 14000\n");
		return 0;
	}

	if( argc == 4){
		c_size = atoi(argv[3]);
		c_size--;
		if(c_size < 0){
			c_size = 0;
			printf("times need > 0\n");
			return 0;
		}
	}else{
		c_size = -1;
	}

	strcpy(ifstruct.ifr_name, argv[1]);
	ioctl(sock, SIOCGIFINDEX, &ifstruct);
	sll.sll_ifindex = ifstruct.ifr_ifindex;

	sll.sll_addr[0] = 0x12;
	sll.sll_addr[1] = 0x34;
	sll.sll_addr[2] = 0x56;
	sll.sll_addr[3] = 0x78;
	sll.sll_addr[4] = 0x9a;
	sll.sll_addr[5] = 0xbc;
 
	ioctl(sock, SIOCGIFFLAGS, &ifstruct);
	ifstruct.ifr_flags |= IFF_PROMISC;
	ioctl(sock, SIOCSIFFLAGS, &ifstruct);
	ioctl (sock, SIOCGIFHWADDR, &ifstruct);
	printf("HWaddr:%02x:%02x:%02x:%02x:%02x:%02x\n",ifstruct.ifr_ifru.ifru_hwaddr.sa_data[0],ifstruct.ifr_ifru.ifru_hwaddr.sa_data[1],ifstruct.ifr_ifru.ifru_hwaddr.sa_data[2],ifstruct.ifr_ifru.ifru_hwaddr.sa_data[3],ifstruct.ifr_ifru.ifru_hwaddr.sa_data[4],ifstruct.ifr_ifru.ifru_hwaddr.sa_data[5]);

	buffer = (char*)malloc(14000);
 
	ssl_size = sizeof(sll);
	if( bind(sock, (struct sockaddr*) &sll, ssl_size) == -1)
	{
		printf("bind:   ERROR\n");
		printf("please check the network interface[%s]\n",argv[1]);
		close(sock);
		return -1;
	}

	printf("start capture...\n");
	while(1)
	{
		memset(buffer,0,14000);
		recv_size = recvfrom(sock, buffer, p_size, 0/*MSG_WAITALL*/, (struct sockaddr*)&sll, &ssl_size);
		printf("%d[%s]:",recv_size, strerror(errno));
		for(int i = 0;i < p_size;i++){
			printf(" %02x", buffer[i]);
		}
		printf("\n");
		if( c_size == -1){
			continue;
		}else if( c_size > 0){
			c_size --;
		}else{
			break;
		}
	}
	return 0;
}
