#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include "Bank.h"

int main() {
	client* head = (client*)malloc(sizeof(client));		//assigning admin(head) information
	strcpy(head->name, "admin");
	strcpy(head->password, "123");
	strcpy(head->telephone_number, "00000000000");
	head->next = NULL;

	char commander[MAX] = "Anonymous";
	char command[MAX] = "";
	char temp[MAX] = "";
	char temp_commander[MAX];

	loadfile(head);										//loading data from database file and put it into linked list

	while (1) {
		sprintf(temp_commander, "(%s):~$ ", commander);
		printf("%s", temp_commander);
		fgets(command, 100, stdin);
		sscanf(command, "%s", temp);
														/*	checking what commnad is entered	*/
		if (strcmp(temp, "login") == 0) {
			if (strcmp(commander, "Anonymous") != 0) {
				printf("You should first logout in order to login!\n");
				continue;
			}
			if (!login(command, head, commander))
				continue;
		}
		else if (strcmp(temp, "logout") == 0) {
			printf("%s loged out!\n", commander);
			strcpy(commander, "Anonymous");
		}
		else if (strcmp(temp, "add_user") == 0) {
			if (!add_user(command, head, commander))
				continue;
		}
		else if (strcmp(temp, "deposit") == 0) {
			if (!deposit(command, head, commander))
				continue;
		}
		else if (strcmp(temp, "withdraw") == 0) {
			if (!withdraw(command, head, commander))
				continue;
		}
		else if (strcmp(temp, "view") == 0) {
			char temp2[100] = "nothing";
			sscanf(command, "%s %s", temp, temp2);
			if (strcmp(temp2, "nothing") == 0)
				view(head, commander);
			else {
				if (!view_person(temp2, head, commander))
					continue;
			}
		}
		else if (strcmp(temp, "delete") == 0) {
			if (!delete_person(command, head, commander))
				continue;
		}
		else if (strcmp(temp, "transfer") == 0) {
			if (!transfer(command, head, commander))
				continue;
		}
		else if (strcmp(temp, "exit") == 0) {
			savefile(head);							//saving from linked list to database file
			break;
		}
		else {
			printf("Unknown command!\n");
			continue;
		}
	}
	return 0;
}

