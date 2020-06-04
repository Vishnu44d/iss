#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5555
#define MAX_CONNECTION 5

typedef struct node
{
    int socket_d;
    struct sockaddr_in address;
    socklen_t address_size;

} node;

node *init_q(int socket, struct sockaddr_in addr)
{
    node *Q = (node *)malloc(sizeof(node));
    Q->socket_d = -1;
    Q->address.sin_addr = addr.sin_addr;
    Q->address.sin_family = addr.sin_family;
    Q->address.sin_port = addr.sin_port;
    return Q;
}

int main()
{

    int sockfd, ret;
    struct sockaddr_in serverAddr;
    int connCount = 1;

    node *Q[25];

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("[ERR] Error in connection.\n");
        exit(1);
    }
    printf("[INFO] Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // First elelment of queue is always server
    Q[0] = init_q(sockfd, serverAddr);
    FILE *fp;
    fp = fopen("connections.csv", "w+");
    fprintf(fp, "%d,%s,%d\n", sockfd, inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
    fclose(fp);

    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        printf("[ERR] Error in binding.\n");
        exit(1);
    }
    printf("[INFO] Bind to port %d\n", PORT);

    if (listen(sockfd, 10) == 0)
    {
        printf("[INFO] Listening....\n");
    }
    else
    {
        printf("[ERR] Error in binding.\n");
    }

    while (1)
    {
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
        if (newSocket < 0)
        {
            exit(1);
        }
        // Q[connCount++] = init_q(newSocket, newAddr);
        fp = fopen("connections.csv", "a");
        fprintf(fp, "%d,%s,%d\n", newSocket, inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        fclose(fp);
        printf("[SERVER]: Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if ((childpid = fork()) == 0)
        {
            close(sockfd);

            while (1)
            {
                memset(&buffer, '\0', sizeof(buffer));
                recv(newSocket, buffer, 1024, 0);

                if (strcmp(buffer, ":exit") == 0)
                {
                    printf("[SERVER]: Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else
                {
                    printf("[Client_%d]: %s\n", newSocket, buffer);
                    send(newSocket, buffer, strlen(buffer), 0);
                    memset(&buffer, '\0', sizeof(buffer));
                }
            }
        }
    }

    close(newSocket);

    return 0;
}