#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void initialize_openssl() {
    OpenSSL_add_all_algorithms(); // Register all algorithms
    SSL_load_error_strings();      // Load error strings
    OpenSSL_add_ssl_algorithms();  // Register SSL/TLS algorithms
}

SSL_CTX* create_context() {
    const SSL_METHOD* method = TLS_client_method(); // Use client method
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        abort();
    }

    // Set the cipher list to include only anonymous cipher suites
    if (SSL_CTX_set_cipher_list(ctx, "aNULL") != 1) {
        ERR_print_errors_fp(stderr);
        abort();
    }

    return ctx;
}

SSL* create_ssl_connection(SSL_CTX* ctx, int sockfd) {
    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        ERR_print_errors_fp(stderr);
        abort();
    }

    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
        abort();
    }

    return ssl;
}

int main() {
    initialize_openssl();

    SSL_CTX* ctx = create_context();

    // Create a socket and connect to the server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4433); // Server port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        return -1;
    }

    // Create SSL connection
    SSL* ssl = create_ssl_connection(ctx, sockfd);

    // Receive data from the server
    char buf[1024];
    int bytes = SSL_read(ssl, buf, sizeof(buf));
    if (bytes > 0) {
        std::cout << "Received: " << std::string(buf, bytes) << std::endl;
    }

    // Clean up
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    EVP_cleanup();

    return 0;
}
