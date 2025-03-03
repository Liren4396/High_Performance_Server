#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080  // 定义服务器监听的端口号，可按需修改
#define BUFFER_SIZE 1024  // 定义缓冲区大小，用于接收和发送数据

// 处理客户端请求，生成并返回响应
void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    // 接收客户端发送的请求数据
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Error reading from client");
        return;
    }

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n";
    response += "<html><body><h1>Hello from HTTP Server!</h1></body></html>";

    // 发送响应数据给客户端
    if (write(client_socket, response.c_str(), response.length()) < 0) {
        perror("Error writing to client");
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // 创建服务器套接字
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation error");
        return -1;
    }

    // 设置服务器地址信息
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // 绑定套接字到指定地址和端口
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind error");
        return -1;
    }

    // 监听套接字，等待客户端连接
    if (listen(server_socket, 5) < 0) {
        perror("Listen error");
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    while (true) {
        // 接受客户端连接
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) < 0) {
            perror("Accept error");
            continue;
        }

        handle_request(client_socket);
    }

    close(server_socket);
    return 0;
}