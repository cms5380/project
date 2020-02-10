#include <stdio.h>
#include <string.h>
#include <pcap.h>
#include <string.h>
#define LINE_LEN 16

int pcnt;

#define MAX_PACKET  20000
int hdrlen[MAX_PACKET];
int num_get_post = 0;
struct ether_addr {
	unsigned char ether_addr_octet[6];
};
struct  ether_header {
	struct  ether_addr ether_dhost;
	struct  ether_addr ether_shost;
	unsigned short ether_type;          // 0x0800 for IP
};
struct ip_hdr {
	unsigned char ip_header_len : 4;
	unsigned char ip_version : 4;
	unsigned char ip_tos;
	unsigned short ip_total_length;
	unsigned short ip_id;
	unsigned char ip_frag_offset : 5;
	unsigned char ip_more_fragment : 1;
	unsigned char ip_dont_fragment : 1;
	unsigned char ip_reserved_zero : 1;
	unsigned char ip_frag_offset1;
	unsigned char ip_ttl;
	unsigned char ip_protocol;
	unsigned short ip_checksum;
	unsigned int ip_srcaddr;
	unsigned int ip_destaddr;
};
struct tcp_hdr {
	unsigned short source_port;
	unsigned short dest_port;
	unsigned int sequence;
	unsigned int acknowledge;
	unsigned char ns : 1;
	unsigned char reserved_part1 : 3;
	unsigned char data_offset : 4;
	unsigned char fin : 1;
	unsigned char syn : 1;
	unsigned char rst : 1;
	unsigned char psh : 1;
	unsigned char ack : 1;
	unsigned char urg : 1;
	unsigned char ecn : 1;
	unsigned char cwr : 1;
	unsigned short window;
	unsigned short checksum;
	unsigned short urgent_pointer;
};


void basic_features(const unsigned char *pkt_data);
void Time_based_features(int T, int tmp, const unsigned char *pkt_data);
void content_based_features();
void connection_based_features(const unsigned char *pkt_data);
void print_raw_packet(const unsigned char *pkt_data, int len);

unsigned char ip[10000][9];
unsigned char port[10000][5];
int count_serv_src;
int count_serv_dst;

int num_packet_src_dst;
int num_packet_dst_src;

int *pkt_T_time = new int;
int connection_iptmp;
int connection_porttmp;
int main()
{
		FILE *file = NULL;
		pcap_t *fp;
		char errbuf[PCAP_ERRBUF_SIZE];
		pcap_pkthdr *header;
		const u_char *pkt_data;
		u_int i = 0;
		int res;

		
		/* Open the capture file */
		if ((fp = pcap_open_offline("p.pcap",			// name of the device
			errbuf					// error buffer
		)) == NULL)
		{
			fprintf(stderr, "\nUnable to open the file %s.\n", errbuf);
			//return -1;
		}
		int T = 0, tmp = 0;
		double sec = 0;
		double misec = 0;
		double time = 0;
		file = fopen("p.csv", "w");
		

		///* Retrieve the packets from the file */
		while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
		{
			//printf("\n\n======================================================\n\n");
			pcnt++;
			sec = header->ts.tv_sec - sec;
			misec = header->ts.tv_usec - misec;
			misec = misec * 0.000001;
			sec = misec + sec;
			//print_raw_packet(pkt_data, header->len);
			if (pcnt != 1)
				time += sec;
			if ((header->len > 54) && pkt_data[54] == 'G' && pkt_data[55] == 'E' && pkt_data[56] == 'T') {
				num_get_post++;
			}
			if ((header->len > 54) && pkt_data[54] == 'P' && pkt_data[55] == 'O' && pkt_data[56] == 'S'&& pkt_data[57] == 'T') {
				num_get_post++;
			}
			tcp_hdr *th = (struct tcp_hdr *)&pkt_data[14 + 20];


			//printf("----------Basic features----------------------\n");


			//printf("Frame No.%d\nFrame length : %d\n", pcnt, header->len);

			//basic_features(pkt_data);
			//printf("----------------------------------------------\n\n");

			//content_based_features();
			//printf("----------------------------------------------\n\n");

			if ((int)time - T > 4) {
				pkt_T_time[tmp] = pcnt;
				tmp++;
				T = (int)time;
			}
			else {
				Time_based_features(T, tmp, pkt_data);
			}


			//printf("----------------------------------------------\n\n");

			connection_based_features(pkt_data);
			//printf("----------------------------------------------\n\n");

			sec = header->ts.tv_sec;
			misec = header->ts.tv_usec;
			//printf("======================================================\n\n");

			fprintf(file, "%d,", pcnt);
			fprintf(file, "%d,", header->len);
			fprintf(file, "%d%d%d%d,", pkt_data[26], pkt_data[27], pkt_data[28], pkt_data[29]);
			fprintf(file, "%d%d%d%d,", pkt_data[30], pkt_data[31], pkt_data[32], pkt_data[33]);
			fprintf(file, "%d,%d,", ntohs(th->source_port), ntohs(th->dest_port));
			fprintf(file, "%d,", num_get_post);
			fprintf(file, "%d,", count_serv_src);
			fprintf(file, "%d,", count_serv_dst);
			fprintf(file, "%d,", num_packet_src_dst);
			fprintf(file, "%d,1\n", num_packet_dst_src);

			
		}

		printf("\b\b\b¿Ï·á!\n");
		if (res == -1)
		{
			printf("Error reading the packets: %s\n", pcap_geterr(fp));
		}
		fclose(file);
		pcap_close(fp);

	
	return 0;
}


void print_raw_packet(const unsigned char *pkt_data, int len) {
	printf("raw packet.\n");
	for (int i = 0; i < len; i++) {
		printf("%02x ", pkt_data[i]);
		if ((i + 1) % 16 == 0) printf("\n");
	}
	printf("\n===================\n");
}

void basic_features(const unsigned char *pkt_data) {
	printf("source IP address:");
	printf("%d.%d.%d.%d\n", pkt_data[26], pkt_data[27], pkt_data[28], pkt_data[29]);
	printf("dest IP address:");
	printf("%d.%d.%d.%d\n", pkt_data[30], pkt_data[31], pkt_data[32], pkt_data[33]);
	tcp_hdr *th = (struct tcp_hdr *)&pkt_data[14 + 20];
	printf("source port:%d\ndest port:%d\n", ntohs(th->source_port), ntohs(th->dest_port));
}
void content_based_features() {
	printf("\n----------Content-based features--------------\n");
	printf("Get/post number : %d\n", num_get_post);
}
void Time_based_features(int T, int tmp, const unsigned char *pkt_data) {
	int porttmp = 0;
	for (int i = 0; i < connection_porttmp; i++) {
		if (port[i][1] == pkt_data[34] &&
			port[i][2] == pkt_data[35] &&
			port[i][3] == pkt_data[36] &&
			port[i][4] == pkt_data[37])
		{
			port[i][0]++;
			//printf("\n----------Time-based features-----------------\n");
			//printf("count-serv-src:%d\n", port[i][0]);
			count_serv_src = port[i][0];
			for (int j = 0; j < connection_porttmp; j++) {
				if (port[i][1] == port[j][3] &&
					port[i][2] == port[j][4] &&
					port[i][3] == port[j][1] &&
					port[i][4] == port[j][2])
				{
					//printf("count-serv-dst:%d\n", port[j][0]);
					count_serv_dst = port[j][0];
					break;
				}
			}
			porttmp = 1;
			break;
		}
	}

	if (porttmp == 0) {
		for (int i = 1; i < 5; i++) {
			port[connection_porttmp][i] = pkt_data[i + 33];
		}
		port[connection_porttmp][0]++;
		//printf("\n----------Time-based features-----------------\n");
		//printf("count-serv-src:%d\n", port[connection_porttmp][0]);
		count_serv_src = port[connection_porttmp][0];
		if (connection_porttmp == 0) {
			//printf("count-serv-dst:0\n");
			count_serv_dst = 0;
		}
		for (int j = 0; j < connection_porttmp; j++) {
			if (port[connection_porttmp][1] == port[j][3] &&
				port[connection_porttmp][2] == port[j][4] &&
				port[connection_porttmp][3] == port[j][1] &&
				port[connection_porttmp][4] == port[j][2])
			{
				//printf("count-serv-dst:%d\n", port[j][0]);
				count_serv_dst = port[j][0];
				break;
			}
			else if (j == connection_porttmp - 1) {
				//printf("count-serv-dst:0\n");
				count_serv_dst = 0;
				break;
			}
		}
		connection_porttmp++;
	}

}
void connection_based_features(const unsigned char *pkt_data) {
	int iptmp = 0;
	for (int i = 0; i < connection_iptmp; i++) {
		if (ip[i][1] == pkt_data[26] &&
			ip[i][2] == pkt_data[27] &&
			ip[i][3] == pkt_data[28] &&
			ip[i][4] == pkt_data[29] &&
			ip[i][5] == pkt_data[30] &&
			ip[i][6] == pkt_data[31] &&
			ip[i][7] == pkt_data[32] &&
			ip[i][8] == pkt_data[33])
		{
			ip[i][0]++;
			//printf("\n----------Connnection-based features----------\n");
			//printf("num-packets-src-dst:%d\n", ip[i][0]);
			num_packet_src_dst = ip[i][0];
			for (int j = 0; j < connection_iptmp; j++) {
				if (ip[i][1] == ip[j][5] &&
					ip[i][2] == ip[j][6] &&
					ip[i][3] == ip[j][7] &&
					ip[i][4] == ip[j][8] &&
					ip[i][5] == ip[j][1] &&
					ip[i][6] == ip[j][2] &&
					ip[i][7] == ip[j][3] &&
					ip[i][8] == ip[j][4])
				{
					//printf("num-packets-dst-src:%d\n", ip[j][0]);
					num_packet_dst_src = ip[j][0];
					break;
				}
			}
			iptmp = 1;
			break;
		}
	}

	if (iptmp == 0) {
		for (int i = 1; i < 9; i++) {
			ip[connection_iptmp][i] = pkt_data[i + 25];
		}
		ip[connection_iptmp][0]++;
		//printf("\n----------Connnection-based features----------\n");
		//printf("num-packets-src-dst:%d\n", ip[connection_iptmp][0]);
		num_packet_src_dst = ip[connection_iptmp][0];
		if (connection_iptmp == 0) {
			//printf("num-packets-dst-src:0\n");
			num_packet_dst_src = 0;
		}
		for (int j = 0; j < connection_iptmp; j++) {
			if (ip[connection_iptmp][1] == ip[j][5] &&
				ip[connection_iptmp][2] == ip[j][6] &&
				ip[connection_iptmp][3] == ip[j][7] &&
				ip[connection_iptmp][4] == ip[j][8] &&
				ip[connection_iptmp][5] == ip[j][1] &&
				ip[connection_iptmp][6] == ip[j][2] &&
				ip[connection_iptmp][7] == ip[j][3] &&
				ip[connection_iptmp][8] == ip[j][4])
			{
				//printf("num-packets-dst-src:%d\n", ip[j][0]);
				num_packet_dst_src = ip[j][0];
				break;
			}
			else if (j == connection_iptmp - 1) {
				//printf("num-packets-dst-src:0\n");
				num_packet_dst_src = 0;
				break;
			}

		}
		connection_iptmp++;
	}
}