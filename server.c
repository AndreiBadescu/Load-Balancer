#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "utils.h"

// hash function for string
unsigned int hash_function_key_(char* a) {
	char* puchar_a = a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

server_memory* init_server_memory() {
	server_memory* server = calloc(1, sizeof(server_memory));
	DIE(server == NULL, "couldn't initialize server memory");
	return server;
}

ht_item* create_new_item(const char* key, const char* value) {
	ht_item* item = malloc(sizeof(ht_item));
	DIE(item == NULL, "couldn't create new hashtable item");
	item->key = malloc(strlen(key) + 1);
	DIE(item->key == NULL, "malloc call returned NULL (item->key)");
	strcpy(item->key, key);
	item->value = malloc(strlen(value) + 1);
	DIE(item->value == NULL, "malloc call returned NULL (item->key)");
	strcpy(item->value, value);
	return item;
}

ht_node* create_new_node(ht_item* const item, ht_node* const next_node) {
	ht_node* node = malloc(sizeof(ht_node));
	DIE(node == NULL, "couldn't create new hashtable node");
	node->item = item;
	node->next = next_node;
	return node;
}

void server_store(server_memory* server, char* key, char* value) {
	int ind = hash_function_key_(key) % CAPACITY;
	ht_node* head = server->buckets[ind];
	if (head == NULL) {
		server->buckets[ind] = create_new_node(create_new_item(key, value), NULL);
	} else {
		server->buckets[ind] = create_new_node(create_new_item(key, value), head);
	}
}

void free_ht_node(ht_node* node) {
	DIE(node == NULL, "can't delete NULL node");
	free(node->item->key);
	free(node->item->value);
	free(node->item);
	free(node);
}

void server_remove(server_memory* server, char* key) {
	int ind = hash_function_key_(key) % CAPACITY;
	ht_node* prev_node = NULL;
	ht_node* curr_node = server->buckets[ind];
	while (curr_node) {
		if (strcmp(curr_node->item->key, key) == 0) {
			if (prev_node == NULL) {
				server->buckets[ind] = curr_node->next;
			} else {
				prev_node->next = curr_node->next;	
			}
			free_ht_node(curr_node);
			return;
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}
	DIE(1, "KEY TO BE REMOVED NOT FOUND IN SERVER");
}

char* server_retrieve(server_memory* server, char* key) {
	ht_node* node = server->buckets[hash_function_key_(key) % CAPACITY];
	while (node) {
		if (strcmp(node->item->key, key) == 0) {
			return node->item->value;
		}
		node = node->next;
	}
	return NULL;
}

void free_ht_list(ht_node* head) {
	while (head) {
		ht_node* tmp = head;
		head = head->next;
		free_ht_node(tmp);
	}
}

void free_server_memory(server_memory* server) {
	for (int i = 0; i < CAPACITY; ++i) {
		free_ht_list(server->buckets[i]);
	}
	free(server);
}
