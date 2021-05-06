#ifndef SERVER_H_
#define SERVER_H_

#define CAPACITY 101

typedef struct server_memory server_memory;
typedef struct ht_item ht_item;
typedef struct ht_node ht_node;

struct ht_item {
	char* key;
	char* value;
};

struct ht_node {
	ht_item* item;
	ht_node* next;
};

struct server_memory {
	ht_node* buckets[CAPACITY];
};

server_memory* init_server_memory();

void free_server_memory(server_memory* server);

void server_store(server_memory* server, char* key, char* value);

void server_remove(server_memory* server, char* key);

char* server_retrieve(server_memory* server, char* key);

#endif  /* SERVER_H_ */
