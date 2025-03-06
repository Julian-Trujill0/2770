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
    const SSL_METHOD* method = TLS_server_method(); // Use server method
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

void configure_context(SSL_CTX* ctx) {
    // Set the context to not require client certificates
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, nullptr);
}

int main() {
    initialize_openssl();

    SSL_CTX* ctx = create_context();
    configure_context(ctx);

    // Create a socket and bind it to a port
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4433); // Server port
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 1) == -1) {
        perror("Listen failed");
        close(sockfd);
        return -1;
    }

    // Accept incoming connections
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock == -1) {
            perror("Accept failed");
            continue;
        }

        // Create SSL connection
        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_sock);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            // Handle SSL communication here
            const char* msg = "Hello, secure client!";
            SSL_write(ssl, msg, strlen(msg));
        }

        // Clean up
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_sock);
    }

    close(sockfd);
    SSL_CTX_free(ctx);
    EVP_cleanup();

    return 0;
}
