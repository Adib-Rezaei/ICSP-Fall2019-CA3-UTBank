#pragma once
#define MAX 60
typedef struct Client client;
typedef struct Transaction transaction;

struct Transaction {
	char data[MAX];
	transaction* next;
};

struct Client {
	char name[MAX];
	char password[MAX];
	char telephone_number[MAX];
	int id;
	int deposit;
	int transaction_count;
	transaction* trans;
	client* next;
};

int login(char* command, client* head, char* commander);
int add_user(char* command, client* head, char* commander);
int deposit(char* command, client* head, char* commander);
int withdraw(char* command, client* head, char* commander);
int view_person(char* person, client* head, char* commander);
int delete_person(char* command, client* head, char* commander);
int view(client* head, char* commander);
int transfer(char* command, client* head, char* commander);
client* find(client* head, char* name);
void print_trans(client* check);
void savefile(client* head);
void loadfile(client* head);