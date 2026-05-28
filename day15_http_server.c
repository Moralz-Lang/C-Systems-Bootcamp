#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int server_fd, client_fd;

    struct sockaddr_in address;

    socklen_t addrlen = sizeof(address);

    char buffer[4096];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd,
         (struct sockaddr*)&address,
         sizeof(address));

    listen(server_fd, 5);

    printf("HTTP server running on port 8080...\n");

    while(1)
    {
        client_fd = accept(server_fd,
                           (struct sockaddr*)&address,
                           &addrlen);

        int bytes = read(client_fd,
                         buffer,
                         sizeof(buffer) - 1);

        if(bytes > 0)
        {
            buffer[bytes] = '\0';

            char method[16];
            char path[256];

            sscanf(buffer, "%s %s", method, path);

            printf("METHOD: %s | PATH: %s\n", method, path);

            // SIMPLE ROUTING
            char filepath[512];

            if(strcmp(path, "/") == 0)
            {
                strcpy(filepath, "www/index.html");
            }
            else
            {
                snprintf(filepath,
                         sizeof(filepath),
                         "www%s",
                         path);
            }

            FILE *file = fopen(filepath, "r");

            if(file)
            {
                char file_buffer[8192];

                int file_size = fread(file_buffer,
                                      1,
                                      sizeof(file_buffer),
                                      file);

                fclose(file);

                char header[256];

                sprintf(header,
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %d\r\n"
                        "\r\n",
                        file_size);

                send(client_fd,
                     header,
                     strlen(header),
                     0);

                send(client_fd,
                     file_buffer,
                     file_size,
                     0);
            }
            else
            {
                char *not_found =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n"
                    "\r\n"
                    "404 - File Not Found";

                send(client_fd,
                     not_found,
                     strlen(not_found),
                     0);
            }
        }

        close(client_fd);
    }

    return 0;
}
