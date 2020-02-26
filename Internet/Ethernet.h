#ifndef ETHERNET_H__
#define ETHERNET_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Size of chunks to read from the host
#define CHUNK_SIZE 8192

// Size of the header segments sent to indicate size of the bmp and text files
#define HEADER_LENGTH 10

/**
 * Given a hostname and port number, listen on that socket and write the contents
 * of the file received from the host into bmp_filename for the image, and palette_filename
 * for the palette text file.
 * 
 * @param hostname string indicating the IP address of the host
 * @param port integer indicating the port to connect to on the host
 * @param bmp_filename string to serve as output bmp filename
 * @param palette_filename string to serve as filename for output palette file
 */
void get_file(char* hostname, int portno, char* bmp_filename, char* palette_filename);

#endif /* ETHERNET_H__ */
