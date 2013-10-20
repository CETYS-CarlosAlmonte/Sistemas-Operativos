#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

struct sockaddr_in server_socket;
int listener, conn, x;

int sendfile(char *filename);
char *contenttype(char *filename);

int main(void)
{
	x = 1;
	if ( (listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("No se pudo crear la descripcion del Socket.\n");
		return 1;
	}

	memset(&server_socket, 0, sizeof(server_socket));

	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = htonl(INADDR_ANY);
	server_socket.sin_port = htons(5000);

	bind(listener, (struct sockaddr*)&server_socket, sizeof(server_socket));
	if (listen(listener, 10))
	{
		printf("Error al preparar Socket como listener.\n");
		return 1;
	}

	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (char*)&x, sizeof(x)) < 0)
	{
		printf("Error al establecer conexion.\n");
		close(listener);
		return 1;
	}

	while (1)
	{
		char buffer[1024];
		memset(&buffer, 0, sizeof(buffer));

		printf("\nProceso Padre esta esperando peticion de un Cliente...\n\n");
		conn = accept(listener, NULL, NULL);

		int pid, n;
		pid = fork();
		if (pid == 0) // Child
			n = read(conn, buffer, sizeof(buffer)); //Leer el HTTP request
		if (n > 0 && pid == 0) // Child
		{
			printf("Proceso Hijo %d atiende peticion:\n%s\n", getpid(), buffer);
			char *trimed_request = strstr(buffer, "/")+1;

			int end = strcspn(trimed_request, "\n");
			end -= 10; // Back from '\n' passing through " HTTP/1.1" and get to end of the path.
			char filepath[end];
			strncpy(filepath, trimed_request, end);
			filepath[end] = 0;
			printf("Proceso Hijo %d: enviando datos...\n", getpid());
			if (sendfile(filepath) <= 0)
				printf("Proceso Hijo %d: No se enviaron datos al Cliente\n", getpid());
		}
		else if (pid == 0 && n <= 0) // Child
			printf("Proceso Hijo %d: No se recibieron datos por parte del Cliente.\n", getpid());

		if (pid == 0) //Child
		{
			printf("\nFin de Proceso Hijo %d\n", getpid());
			kill(getpid(), 9);
		}
		if (pid > 0)
			while (waitpid(pid) != pid) { }
		close(conn); //Parent
	}

	return 0;
}

int sendfile(char *filename)
{	static char data[8192];

	char *type = contenttype(filename);
	char *ok = "HTTP/1.1 200 OK\nContent-Type: %s\nContent-length: %lu\n\n";
	char *notfound = "HTTP/1.1 404 Not Found\nContent-length: %d\nContent-Type: text/html\n\n<html><head><title>404 Not Found</title></head><body><H1>404 Not Found</h1><h3>El archivo solicitado no existe :(</H3><img src=\"404.png\" /></body></html>";

	int f = open(filename, O_RDONLY);

	if (f > -1)
	{
		FILE *fp = fopen(filename, "rb");
		unsigned long length;
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		sprintf(data, ok, type, length);
		(void)write(conn, data, strlen(data));
		fclose(fp);
		int received;
		while ((received = read(f, data, sizeof(data))) > 0)
			(void)write(conn, data, received);

		return 1;
	}
	else
	{
		sprintf(data, notfound, strlen(notfound));
		return write(conn, data, strlen(notfound));
	}
}

char *contenttype(char *filename)
{
	char* extension = strstr(filename, ".");

	if (strcmp(extension,".html") == 0)
		return (char*)"text/html";
	else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0)
		return (char*)"image/jpg";
	else if (strcmp(extension, ".gif") == 0)
		return (char*)"image/gif";
	else if (strcmp(extension, ".png") == 0)
		return (char*)"image/png";
	else
		return (char*)"text/plain";
}
