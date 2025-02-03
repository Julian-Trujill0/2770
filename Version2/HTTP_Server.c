#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    free(socket_desc);  // Free allocated memory
    char buffer[1024] = {0};
    char method[16], url[1024], protocol[16];

    // Read client request
    int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("Read failed");
        close(sock);
        return NULL;
    }
    buffer[bytes_read] = '\0'; // Null-terminate received data

    // Parse request line
    sscanf(buffer, "%15s %1023s %15s", method, url, protocol);

    // Prepare HTTP response
    const char *response_template =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s";

    const char *body = "<html><body><h1>Hello, world!</h1></body></html>";
    char response[2048];
    int response_length = snprintf(response, sizeof(response), response_template, (int)strlen(body), body);

    // Send response
    if (write(sock, response, response_length) < 0) {
        perror("Write failed");
    }

    printf("Response sent to client\n");

    close(sock);
    return NULL;
}

int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, 5) < 0) { // Increased backlog queue size
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Allocate memory for socket descriptor
        new_sock = malloc(sizeof(int));
        if (!new_sock) {
            perror("Memory allocation failed");
            close(new_socket);
            continue;
        }
        *new_sock = new_socket;

        // Create a new thread for each connection
        pthread_t thread;
        if (pthread_create(&thread, NULL, connection_handler, (void*)new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
            close(new_socket);
        }

        // Detach the thread so it cleans up after itself
        pthread_detach(thread);
    }

    return 0;
}
