#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_

#include "server.h"

#define MAX 100000

typedef struct load_balancer load_balancer;
typedef struct server server;

struct server {
    server_memory* hashtable;
    unsigned int hash;
    int server_id;
};

struct load_balancer {
    server hashring[3 * MAX];
    int size; // number_of_servers = size / 3
};

load_balancer* init_load_balancer();

void free_load_balancer(load_balancer* main);

void loader_store(load_balancer* main, char* key, char* value, int* server_id);

char* loader_retrieve(load_balancer* main, char* key, int* server_id);

void loader_add_server(load_balancer* main, int server_id);

void loader_remove_server(load_balancer* main, int server_id);

#endif  /* LOAD_BALANCER_H_ */
