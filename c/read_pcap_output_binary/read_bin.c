#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ONE_BYTE 1

//unsigned char pattern_serverIP_to_clientIP[]	= {0x66, 0x01, 0x00, 0x00, 0x66, 0x01, 0x00, 0x00};
//unsigned char pattern_frame_end[8]		= {0x00, 0x00, 0x88, 0x01, 0x00, 0x00, 0x06, 0x00};
//
#define OBJECT_PAYLOAD_SIZE 8120

// LENGTH
#define IP_HEADER_LEN	20
#define TCP_HEADER_LEN	32

// OFFSET base on tail offset of pattern_serverIP_to_clientIP
#define OFFSET_HEAD_TCP_FLAG			13	// ACK, PSH, ....
#define OFFSET_HEAD_IP_PACKET_LEN		18	// IP PACKET LEN is IP_HEADER_LEN + IP_PAYLOAD_LEN
#define OFFSET_HEAD_IP_PAYLOAD			32
#define OFFSET_HEAD_IP_FLAG				13
unsigned char pattern_serverIP_to_clientIP[] = {0xAC, 0x27, 0x8D, 0x0B, 0xAC, 0x27, 0x8D, 0xA3};  //ip.src == 172.39.141.11 && ip.dst == 172.39.141.163

typedef struct {
	int payload_index = 0;
	unsigned char tcp_payload_content[OBJECT_PAYLOAD_SIZE];
	unsigned int payload_postion = 0;
	bool is_more_tcp_payload = true;
} OBJECT_PACKET;
OBJECT_PACKET object_packet;

// BOOL for TCP FLAG
bool hasACK = false;		// need more payload
bool hasPSH_ACK = false;	// no more payload


int main(int argc, char* argv[]) {

    if(argc != 2) {
        puts("指令: read <filename>");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if(!file) {
        puts("無法讀取檔案");
        return 1;
    }

    unsigned char ch;
    int count = 0;
	int checking_count = 0;
	char buffer_checking[8] = {};
	int pattern_serverIP_to_clientIP_count = 0;

	const int checking_count_ok = sizeof(pattern_serverIP_to_clientIP)/
								  sizeof(pattern_serverIP_to_clientIP[0]);
	memset(buffer_checking, 0x00, checking_count_ok);

    while(!feof(file)) {

		// read every byte and put it in buffer
        fread(&ch, sizeof(char), ONE_BYTE, file);
		printf("%2X ", ch);
		memcpy(&buffer_checking[checking_count], &ch, ONE_BYTE);

		// do memcmp every byte in buffer
		if (memcmp(&pattern_serverIP_to_clientIP[checking_count], &buffer_checking[checking_count], ONE_BYTE) == 0) {

			checking_count++;

			if(checking_count == checking_count_ok) {
				printf("\n\n======= start printf IP packet =======\n");
				pattern_serverIP_to_clientIP_count++;

				//STEP1. found the IP PACKET LEN info
				unsigned char ch_ip_packet_len_buffer[2];
				char sprintf_buffer[10];
				unsigned int int_ip_packet_len;
				unsigned long pos_pattern_serverIP_to_clientIP_position = ftell(file);

				fseek(file, -OFFSET_HEAD_IP_PACKET_LEN, SEEK_CUR);
				fread(&ch_ip_packet_len_buffer[0], sizeof(char), ONE_BYTE, file);
				fseek(file, 0, SEEK_CUR);
				fread(&ch_ip_packet_len_buffer[1], sizeof(char), ONE_BYTE, file);

				sprintf(sprintf_buffer, "0x%x%x", ch_ip_packet_len_buffer[0], ch_ip_packet_len_buffer[1]);
				printf("IP PACKET is %d\n",strtol(sprintf_buffer, NULL, 0));

				//cal the payload of tcp packet len
				int tcp_payload_len = strtol(sprintf_buffer, NULL, 0) - IP_HEADER_LEN - TCP_HEADER_LEN;
				printf("TCP PAYLOAD LEN is %d\n", tcp_payload_len);

				// Back to tail of pattern_serverIP_to_clientIP_position
				fseek(file, pos_pattern_serverIP_to_clientIP_position, SEEK_SET);

				//STEP2. tcp flag is ACK, ACK & PSH?
				fseek(file, OFFSET_HEAD_IP_FLAG, SEEK_CUR);
				fread(&ch, sizeof(char), ONE_BYTE, file);
				printf("TCP FLAG is %2x \n", ch);

				//STEP2.1 check the tcp flag
				if (ch == 0x18) {// PSH & ACK
					hasPSH_ACK = true;
				} else if (ch == 0x10) {
					hasACK = true;
				}

				//STEP2.2 decide to memcpy payload or not
				if (hasACK && hasPSH_ACK || hasACK) {

				} else {
					hasPSH_ACK = false;
					hasACK = false;
					printf("\n======= [skip this ip packet ]====\n");
					continue;
				}
				// Back to tail of pattern_serverIP_to_clientIP_position
				fseek(file, pos_pattern_serverIP_to_clientIP_position, SEEK_SET);

				int tcp_payload_position_base = object_packet.payload_postion;

//				if (object_packet.payload_postion != 0) // start from next index when payload postion is not 0
//					tcp_payload_position_base++;

				//STEP3. fseek to the HEAD position of payload of tcp packet
				fseek(file, OFFSET_HEAD_IP_PAYLOAD, SEEK_CUR);

				printf("\n======= [start] payload memcpy  & printf =======\n");
				for (int i = 0; i < tcp_payload_len; i++) {

					fread(&ch, sizeof(char), ONE_BYTE, file);
					memcpy(&object_packet.tcp_payload_content[i + tcp_payload_position_base], &ch, ONE_BYTE);
					printf("%2X ", object_packet.tcp_payload_content[i + tcp_payload_position_base]);

					if ( i > 0 && i%16 == 0) {  // 換行
						putchar('\n');
					}
					object_packet.payload_postion++;
				}

				printf("======= [end] payload memcpy  & printf =======\n");

//
				//TODO STEP4.

				FILE *fp = NULL;
				fp = fopen("./tcp_payload.bin", "w+");
				fseek(fp, 0x00, SEEK_SET);

				if (hasACK && hasPSH_ACK) {
					//decode the payload?
					printf("======= [start] printf payload==\n");
					for (int i = 0; i < object_packet.payload_postion; i++) {

						printf("%2X ", object_packet.tcp_payload_content[i]);
						fwrite(&object_packet.tcp_payload_content[i], sizeof(unsigned char), 1, fp);

//						if ( i > 0 && i%16 == 0) {  // 換行
//							printf("\n");
//						}
					}
					printf("======= [end] decode payload==\n");
				fclose(fp);

					return 0;
					//reset
					object_packet.payload_index = 0;
					memset(object_packet.tcp_payload_content, 0x00, OBJECT_PAYLOAD_SIZE);
				}

				//reset buffer_checking
				checking_count = 0;
				memset(buffer_checking, 0x00, checking_count_ok);

			}

		} else if (checking_count != 0) {

			/* prevent lose some vaild data
			example: you want to find hex string "ABCD" as factor
			Input is "AABCD...", if you use the algorithms above mentioned,
			you will get lose this one when your checking count is  > 0
			So, we need to back one step when you checking count is > 0.
			Then, you can find the one time that you want */
			fseek(file, -1, SEEK_CUR);
			checking_count = 0;

		} else {

			checking_count = 0;

		}


		// printf
		count++;
		if(count > 15) {  // 換行
			putchar('\n');
			count = 0;
		}
    }
    putchar('\n');

    fclose(file);

	printf("pattern_serverIP_to_clientIP_count is %d\n", pattern_serverIP_to_clientIP_count);
    return 0;
}
