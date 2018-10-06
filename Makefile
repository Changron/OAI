all:
	gcc -o server server.c
	gcc -o client client.c
	gcc -o udp_server udp_server.c
	gcc -o udp_client udp_client.c
clean:
	rm server
	rm client
	rm udp_server
	rm udp_client
