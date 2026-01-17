#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool stop = false;


void stop_loop() {
	stop = true;
	exit(0);
}


int main(int ac, char **av) {

	(void)ac;
	int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0) {
		perror("socket:");
		return 1;
	}
	
	struct sockaddr_ll addr_ll;
	memset(&addr_ll, 0, sizeof(addr_ll));
	
	addr_ll.sll_family = AF_PACKET;
	addr_ll.sll_protocol = htons(ETH_P_ARP);
	addr_ll.sll_ifindex = if_nametoindex("br-43eccdb5951e");
	
	if (addr_ll.sll_ifindex == 0) {
			perror("if_nametoindex");
			return -1;
	}
		printf("Waiting2...\n");

	if (bind(sock, (struct sockaddr *)&addr_ll, sizeof(addr_ll)) < 0) {
			perror("bind");
			return -1;
	}
		printf("Waiting1...\n");

	signal(SIGINT, stop_loop);
	while (!stop) {
		
		char buf[2048];
		printf("Waiting...\n");
		ssize_t ret = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
		if (ret == -1) {
			perror("recvfrom:");
			return 1;
		}
		printf("arp recv: %zd bytes\n", ret);

		struct ether_header *eth = (struct ether_header *)buf;
		(void)eth;

		struct ether_arp *arp = (struct ether_arp *)(buf + sizeof(struct ether_header));
		
		printf("Sender IP: %s\n", inet_ntoa(*(struct in_addr *)arp->arp_spa));
			char *sender = inet_ntoa(*(struct in_addr *)arp->arp_spa);
			
		printf("Target IP: %s\n", inet_ntoa(*(struct in_addr *)arp->arp_tpa));
		char *target = inet_ntoa(*(struct in_addr *)arp->arp_tpa);

		if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST) {
			printf("ARP REQUEST\n");
			if ((strcmp(target, av[1]) == 0)) {
				printf("Je viens de capter une arp request de type 1 interessante \n");
			} else if ((strcmp(target, av[2]) == 0)) {
				printf("Je viens de capter une arp request de type 2 interessante \n");
				
			}
		}
		else if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REPLY)
		printf("ARP REPLY\n");
		(void)sender;
		// printf("ARP REQUEST\n");
	}
	return 0;
}
	
