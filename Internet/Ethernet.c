#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Ethernet.h"
#include "WiFi.h"

/*
 * Kill the program if there are issues with the socket
 */
void error(char *msg)
{
    perror(msg);
    exit(0);
}

void get_file(char* hostname, int portno, char* bmp_filename, char* palette_filename)
{
	// Tell the server to serve the image
	open_bmp_sock();

	// Connection setup
    int sockfd;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to socket
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
	}

    // Read the bmp file header
    char img_header[HEADER_LENGTH];
    int size_recv = 0;
    while(1) {
    	memset(img_header, 0, HEADER_LENGTH);
    	size_recv = read(sockfd, img_header, HEADER_LENGTH);

    	if(size_recv == HEADER_LENGTH) {
    		break;
    	} else {
    		error("DID NOT RECEIVE IMAGE HEADER BYTES CORRECTLY");
    	}
    }

    int img_filesize = atoi(img_header);

    // Read the bmp file
	char img_chunk[CHUNK_SIZE];
	size_recv = 0;
    int remaining_bytes = img_filesize;
	FILE* bmp;
	bmp = fopen(bmp_filename, "wb+");
	while(1)
	{
		if(remaining_bytes == 0) {
			break;
		}

		// Clear the image chunk buffer to read new while still streaming
		memset(img_chunk ,0 , CHUNK_SIZE);  
		int read_size = CHUNK_SIZE > remaining_bytes ? remaining_bytes : CHUNK_SIZE;
		size_recv =  read(sockfd, img_chunk, read_size);

		if(!size_recv) {
			fclose(bmp);
			break;
		} else {
			remaining_bytes -= size_recv;
			fseek(bmp, 0, SEEK_END);
			fwrite(img_chunk,  1, size_recv, bmp);
		}
	}

	fclose(bmp);

	// Read the palette header file
	char palette_header[HEADER_LENGTH];
	size_recv = 0;
	while(1)
	{
		memset(palette_header ,0 , HEADER_LENGTH);  //clear the variable
		size_recv =  read(sockfd, palette_header, HEADER_LENGTH);

		if(size_recv == HEADER_LENGTH) {
			break;
		} else {
			error("DID NOT RECEIVE PALETTE HEADER BYTES CORRECTLY");
		}
	}

	int palette_filesize = atoi(palette_header);

	// Read the palette text file
	char palette_chunk[CHUNK_SIZE];
	size_recv = 0;
	remaining_bytes = palette_filesize;
	FILE* palette;
	palette = fopen(palette_filename, "wb+");
	while(1)
	{
		if(remaining_bytes == 0) {
			break;
		}

		// Clear the palette chunk buffer to read new while still streaming
		memset(palette_chunk ,0 , CHUNK_SIZE);
		int read_size = CHUNK_SIZE > remaining_bytes ? remaining_bytes : CHUNK_SIZE;
		size_recv =  read(sockfd, palette_chunk, read_size);

		if(!size_recv) {
			fclose(palette);
			break;
		} else {
			remaining_bytes -= size_recv;
			fseek(palette, 0, SEEK_END);
			fwrite(palette_chunk,  1, size_recv, palette);
		}
	}

	fclose(palette);
}
