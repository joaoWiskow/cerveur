#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "HTTP_Server.h"
#include "Routes.h"
#include "Response.h"

int main(int argc, char *argv[]) 
 {
	// initiate HTTP_Server
	HTTP_Server http_server;
	init_server(&http_server, 6969);

	int client_socket;
	
	// registering Routes
	struct Route * route = initRoute("/", "index.html"); 
	addRoute(route, "/about", "about.html");


	printf("\n====================================\n");
	printf("=========ALL VAILABLE ROUTES========\n");
	// display all available routes
	inorder(route);
  int profile_mode = 0;
int connection_limit = 3;

if (argc > 1 && strcmp(argv[1], "profile") == 0) {
    profile_mode = 1;
    printf("[INFO] Modo de profiling ativado. O servidor aceitará %d conexões e encerrará.\n", connection_limit);
}


if (profile_mode) {
    printf("[INFO] Executando chamadas para profiling...\n");

    struct Route * testRoute = initRoute("/", "index.html");

    for (int i = 0; i < 1000000; i++) {
	
        addRoute(testRoute, "/about", "about.html");
        addRoute(testRoute, "/contact", "contact.html");
        addRoute(testRoute, "/services", "services.html");
        search(testRoute, "/about");
        search(testRoute, "/contact");
        search(testRoute, "/services");
        render_static_file("templates/index.html");
        inorder(testRoute);
		
    }
return 0;

}

int connection_count = 0;
while (connection_count < connection_limit) {
    // ...

		char client_msg[4096] = "";

		client_socket = accept(http_server.socket, NULL, NULL);

		read(client_socket, client_msg, 4095);
		printf("%s\n", client_msg);

		// parsing client socket header to get HTTP method, route
		char *method = "";
		char *urlRoute = "";

		char *client_http_header = strtok(client_msg, "\n");
			
		printf("\n\n%s\n\n", client_http_header);

		char *header_token = strtok(client_http_header, " ");
		
		int header_parse_counter = 0;

		while (header_token != NULL) {

			switch (header_parse_counter) {
				case 0:
					method = header_token;
				case 1:
					urlRoute = header_token;
			}
			header_token = strtok(NULL, " ");
			header_parse_counter++;
		}

		printf("The method is %s\n", method);
		printf("The route is %s\n", urlRoute);


		char template[100] = "";
		
		if (strstr(urlRoute, "/static/") != NULL) {
			//strcat(template, urlRoute+1);
			strcat(template, "static/index.css");
		}else {
			struct Route * destination = search(route, urlRoute);
			strcat(template, "templates/");

			if (destination == NULL) {
				strcat(template, "404.html");
			}else {
				strcat(template, destination->value);
			}
		}

		char * response_data = render_static_file(template);

		char http_header[4096] = "HTTP/1.1 200 OK\r\n\r\n";

		strcat(http_header, response_data);
		strcat(http_header, "\r\n\r\n");


		send(client_socket, http_header, sizeof(http_header), 0);
		close(client_socket);
		connection_count++;
		free(response_data);
		
	}
	return 0;
}
