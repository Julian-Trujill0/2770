#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8080

std::mutex log_mutex;

// Initialize OpenSSL
void initialize_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

// Create an SSL context for the server
SSL_CTX* create_context() {
    const SSL_METHOD* method = TLS_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

// Configure SSL context
void configure_context(SSL_CTX* ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);

    // Load certificate and private key (self-signed for testing)
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

// Function to log client messages to a file
void log_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::ofstream log_file("server.log", std::ios::app);
    if (log_file.is_open()) {
        log_file << message << std::endl;
    }
}

// Handle client connection
void handle_client(SSL_CTX* ctx, int client_sock) {
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_sock);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        char buffer[1024] = {0};
        int bytes = SSL_read(ssl, buffer, sizeof(buffer));

        if (bytes > 0) {
            std::string message = "Received: " + std::string(buffer);
            std::cout << message << std::endl;
            log_message(message);  // Log the message to a file

            const char* response = "Message received!";
            SSL_write(ssl, response, strlen(response));
        }
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client_sock);
}

int main() {
    initialize_openssl();
    SSL_CTX* ctx = create_context();
    configure_context(ctx);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    if (listen(server_sock, 5) == -1) {
        perror("Listen failed");
        return EXIT_FAILURE;
    }

    std::vector<std::thread> threads;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock == -1) {
            perror("Accept failed");
            continue;
        }

        threads.emplace_back(handle_client, ctx, client_sock);
    }

    close(server_sock);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    return 0;
}
