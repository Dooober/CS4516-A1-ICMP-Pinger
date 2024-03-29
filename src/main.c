#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define ICMP_ECHO_REQUEST 8

int checksum(char* str) {
    int len = strlen(str);
    int csum = 0;
    int countTo = (len / 2) * 2;
    int count = 0;

    while(count < countTo) {
        int thisVal = ((int) str[count + 1]) * 256 + ((int) str[count]);
        csum += thisVal;
        csum = csum & 0xffffffff;
        count += 2;
    }

    if (countTo < len) {
        csum += ((int)str[len - 1]);
        csum = csum & 0xffffffff;
    }

    //Fill in start
    //add the upper 16 bits with the lower 16bits of csum;
    //add the carry-over bit back to lower bits of csum;
    //flip everybit of csum and put it into variable "answer"
    //Fill in end

    int answer = answer & 0xffff;
    answer = answer >> 8 | (answer << 8 & 0xff00);
    return answer;

    return 0;
}

int receiveOnePing(int mySocket, int ID, int timeout, struct hostent* destAddr) {
    time_t timeLeft = timeout;
    while (1) {
        int startedSelect = time(NULL);
//        whatReady = select.select([mySocket], [], [], timeLeft)
        int howLongInSelect = (time(NULL) - startedSelect);
//        if whatReady[0] == []: // Timeout
//            return "Request timed out."
        //Fill in start
        // get the time the packet is received and store it in "timeReceived"
        // receive the packet from socket and extract information into "recPacket, addr"
        // fetch the ICMP header from the IP packet
        // get TTL, icmpType, code, checksum, packetID, and sequence
        // get data payload, and return information that can be print later, including byte_data, time used from packet sent to received, TTL
        //Fill in end
        timeLeft -= howLongInSelect;
        if (timeLeft <= 0) {
            printf("Request timed out.");
            return -1;
        }
    }
}

struct ICMP_header {
    __uint8_t type;
    __uint8_t code;
    __uint16_t checksum;
    __uint16_t id;
    __uint16_t sequence;
};

void sendOnePing(int mySocket, struct hostent* destAddr, int ID) {
    // Header is type (8), code (8), checksum (16), id (16), sequence (16)
    __uint16_t myChecksum = 0;
    // Make a dummy header with a 0 checksum
    // struct -- Interpret strings as packed binary data
    struct ICMP_header header; 
    header.type = ICMP_ECHO_REQUEST;
    header.code = 0;
    header.checksum = myChecksum;
    header.id = ID;
    header.sequence = 1;
    //Fill in start
    // Use current time as data payload and put it into "data" variable
    // Calculate the checksum on the data and the dummy header and put it into "myChecksum" variable
    //Fill in end
    // Get the right checksum, and put in the header
    // Get the right checksum, and put in the header
    myChecksum = htons(myChecksum);
    //Fill in start
    // update the header with correct checksum
    // create a variable "packet" that combines the header and the data payload
    //Fill in end
//TODO    mySocket.sendto(packet, (destAddr, 1)) // AF_INET address must be tuple, not str
    // Both LISTS and TUPLES consist of a number of objects
    // which can be referenced by their position number within the object.
}

int doOnePing(struct hostent* destAddr, int timeout) {
    struct protoent* icmp = getprotobyname("icmp");
    //Fill in start
    int mySocket = 0;
    // create a socket with SOCK_RAW as the socket type, and icmp as the protocol;
    // SOCK_RAW is a powerful socket type. For more details: http://sock-raw.org/papers/sock_raw
    //Fill in end
    int myID = getpid() & 0xFFFF; // Return the current process i
    sendOnePing(mySocket, destAddr, myID);
    int delay = receiveOnePing(mySocket, myID, timeout, destAddr);
    //mySocket.close();
    return delay;
}

int ping(char* host, int timeout) {
    // timeout=1 means: If one second goes by without a reply from the server,
    // the client assumes that either the client's ping or the server's pong is lost
    struct hostent* dest = gethostbyname(host);
    printf("Pinging %s using Python:\n\n", dest->h_name);
    //print "Pinging " + dest + " using Python:"
    while (1) {
        int delay = doOnePing(dest, timeout);
        printf("%d\n", delay);
        sleep(1);// one second
        return delay;
    }
}

int main() {
    ping("google.com", 1);
    return 0;
}