#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// 0 is reading, 1 is writing
int pipefd[2];
const size_t BUFFER_SIZE = 255;

void *reader_thread(void* data) {
    char buffer[BUFFER_SIZE + 1];
    ssize_t read_bytes = 1;

    while (read_bytes > 0) {
        read_bytes = read(0, buffer, BUFFER_SIZE);

        if (read_bytes > 1) {
            ssize_t written_bytes = 0, newly_written = 0;
            while (written_bytes < read_bytes) {
                newly_written = write(pipefd[1], buffer, read_bytes);
                if (newly_written > 1) {
                    written_bytes += newly_written;
                }
            }
        }
    }

    close(pipefd[1]);

    return 0;
}

void *writer_thread(void* data) {
    char buffer[BUFFER_SIZE + 1];
    ssize_t read_bytes = 1;

    while (read_bytes > 0) {
        read_bytes = read(pipefd[0], buffer, BUFFER_SIZE);

        if (read_bytes > 1) {
            ssize_t written_bytes = 0, newly_written = 0;
            while (written_bytes < read_bytes) {
                newly_written = write(1, buffer, read_bytes);
                if (newly_written > 1) {
                    written_bytes += newly_written;
                }
            }
        }
    }

    close(pipefd[0]);

    return 0;
}

int main() {
    pthread_t reader, writer;

    // TODO return val
    pipe(pipefd);

    // TODO 0 is correct, check other return values
    pthread_create(&reader, NULL, reader_thread, NULL);
    pthread_create(&writer, NULL, writer_thread, NULL);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    return 0;
}
