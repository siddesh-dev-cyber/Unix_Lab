#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define QUEUE_NAME "/msgqueue"
#define MAX_SIZE 1024

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char message[MAX_SIZE];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq_unlink(QUEUE_NAME);

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    printf("Sender process: Enter a message to send to receiver (type 'exit' to quit):\n");

    while (1) {
        printf("Receiver process: Waiting for messages...\n");

        fgets(message, MAX_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        mq_send(mq, message, strlen(message) + 1, 0);

        char buffer[MAX_SIZE];
        mq_receive(mq, buffer, MAX_SIZE, NULL);

        printf("Received message: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
