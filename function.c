#define _CRT_SECURE_NO_WARNINGS
#include "Bank.h"
#include <stdio.h>

int login(char* command, client* head, char* commander) {			//login into the account only if the commander is anonymous
	char name[MAX];
	char password[MAX];
	sscanf(command, "%*s %s %s", name, password);
	

	client* person = find(head, name);
	if (person == NULL) {
		printf("No client with such a name!, try again\n");
		return 0;
	}
	if (strcmp(password, person->password) == 0) {
		printf("client %s successfully loged in!\n", name);
		strcpy(commander, name);
		return 1;
	}
	else {
		printf("Wrong password , try again!\n");
		return 0;
	}

}

int add_user(char* command, client* head, char* commander) {		//adding a new node to the client linked list with given data
	if (strcmp(commander, "admin") != 0) {
		printf("You have no permission to do that!\n");
		return 0;
	}
	client* last = head;
	client* new_user = (client*)malloc(sizeof(client));

	char name[MAX];
	char password[MAX];
	char telephone[MAX];
	int id = 0;
	sscanf(command, "%*s %s %s %s", name, password, telephone);
	if (find(head, name) != NULL) {
		printf("Existing client!, try again\n");
		return 0;
	}
	strcpy(new_user->name, name);				//setting values
	strcpy(new_user->password, password);
	strcpy(new_user->telephone_number, telephone);
	for (int i = 0;; i++) {
		if (name[i] == '\0')
			break;
		id += name[i];
	}
	new_user->id = id;
	new_user->deposit = 0;
	new_user->transaction_count = 0;
	new_user->trans = NULL;
	new_user->next = NULL;

	/*  If the Linked List is empty, then make the new node as head */
	if (head == NULL) {
		head = new_user;
		return 1;
	}
	while (last->next != NULL)
		last = last->next;

	last->next = new_user;
	printf("User %s successfully added!\n", name);
	return 1;
}

int deposit(char* command, client* head, char* commander) {			//rasie deposit from administator
	if (strcmp(commander, "admin") != 0) {
		printf("You have no permission to do that!\n");
		return 0;
	}
	char name[MAX];
	int income;
	sscanf(command, "%*s %s %d", name, &income);

	client* person = find(head, name);
	if (person == NULL) {
		printf("No client with such a name!, try again\n");
		return 0;
	}
	char temp[MAX];
	sprintf(temp, "+%d deposit to account", income);

	transaction* new_trans = (transaction*)malloc(sizeof(transaction));		//making new transaction log node for this action

	person->deposit += income;
	person->transaction_count++;
	strcpy(new_trans->data, temp);

	new_trans->next = NULL;

	/*  If the Linked List is empty, then make the new node as head */
	if (person->trans == NULL)
		person->trans = new_trans;

	else {
		transaction* last = person->trans;
		while (last->next != NULL)
			last = last->next;
		last->next = new_trans;
	}
	printf("client %s successfully raised deposit!\n", name);
	return 1;
}

int withdraw(char* command, client* head, char* commander) {			//withdraw the money from accout with admin command
	if (strcmp(commander, "admin") != 0) {
		printf("You have no permission to do that!\n");
		return 0;
	}
	char name[MAX];
	int outcome;
	sscanf(command, "%*s %s %d", name, &outcome);

	client* person = find(head, name);
	if (person == NULL) {
		printf("No client with such a name!, try again\n");
		return 0;
	}

	char temp[MAX];
	if (person->deposit - outcome < 0) {
		printf("Not enough deposit to withdraw this amount of money!\n");
		return 0;
	}
	sprintf(temp, "-%d withdraw from account", outcome);
	person->deposit -= outcome;
	person->transaction_count++;

	transaction* new_trans = (transaction*)malloc(sizeof(transaction));			//making new transaction log node for this action
	strcpy(new_trans->data, temp);
	new_trans->next = NULL;

	if (person->trans == NULL)
		person->trans = new_trans;

	else {
		transaction* last = person->trans;
		while (last->next != NULL)
			last = last->next;
		last->next = new_trans;
	}

	printf("client %s successfully reduced deposit!\n", name);
	return 1;
}

int view_person(char* name, client* head, char* commander) {		//view person information by admin
	if (strcmp(commander, "admin") != 0) {
		printf("You have no permission to do that!\n");
		return 0;
	}
	client* person = find(head, name);
	if (person == NULL) {
		printf("No client with such a name!, try again\n");
		return 0;
	}
	printf("[User information]\n");
	printf("name : %s\npassword : %s\ntelephone : %s\nid : %d\ndeposit : %d\n", person->name, person->password, person->telephone_number, person->id, person->deposit);
	printf("[User transactions]\n");
	print_trans(person);			//print transaction linked list
	return 1;
}

int view(client* head, char* commander) {					//view the current loged in client information
	if (strcmp(commander, "admin") == 0) {
		printf("Can't acccess to the admin information\n");
		return 0;
	}
	client* person = find(head, commander);
	if (person == NULL) {
		printf("No client with such a name!, try again\n");
		return 0;
	}
	printf("[User information]\n");
	printf("name : %s\npassword : %s\ntelephone : %s\nid : %d\ndeposit : %d\n", person->name, person->password, person->telephone_number, person->id, person->deposit);
	printf("[User transactions]\n");
	print_trans(person);
	return 1;
}

int delete_person(char* command, client* head, char* commander) {		//delete a client node from linked list
	if (strcmp(commander, "admin") != 0) {
		printf("You have no permission to do that!\n");
		return 0;
	}

	char name[MAX];
	sscanf(command, "%*s %s", name);
	client* person = find(head, name);
	if (person == NULL) {
		printf("No client with such a name!, try again\n");
		return 0;
	}
	// Store head node 
	client* temp = person;
	client* prev = (client*)malloc(sizeof(client));

	// If head node itself holds the person to be deleted 
	if (person == head)
	{
		head = person->next;   // Changed head 
		free(person);               // free old head 
		printf("client %s successfully deleted\n", name);
		return 1;
	}

	// Search for the person to be deleted, keep track of the 
	// previous node as we need to change 'prev->next' 
	client* check = head;
	while (check != NULL) {
		if (strcmp(name, check->name) == 0) {
			prev->next = check->next;
			free(check);
			printf("client %s successfully deleted\n", name);
			return 1;
		}
		else {
			prev = check;
			check = check->next;
		}
	}
	return 0;
}

int transfer(char* command, client* head, char* commander) {		//transfer money from current account to the given person
	if (strcmp(commander, "admin") == 0) {
		printf("Admins can't do that!\n");
		return 0;
	}
	char name[MAX];
	int money;
	sscanf(command, "%*s %s %d", name, &money);

	client* from_person = find(head, commander);
	client* to_person = find(head, name);
	if (to_person == NULL) {
		printf("No client with such a name to transfer money to!\n");
		return 0;
	}

	if (from_person->deposit - money < 0) {
		printf("Not enough deposit to transfer this amount of money!\n");
		return 0;
	}
	char temp[MAX];
	char temp2[MAX];
	sprintf(temp, "Transfer -%d Dollars To %s", money, name);
	sprintf(temp2, "Transfer +%d Dollars from %s", money, commander);
	//making transfer log for who transfers money
	from_person->deposit -= money;
	from_person->transaction_count++;
	transaction* new_trans = (transaction*)malloc(sizeof(transaction));
	strcpy(new_trans->data, temp);

	new_trans->next = NULL;

	if (from_person->trans == NULL)
		from_person->trans = new_trans;
	else {
		transaction* last = from_person->trans;
		while (last->next != NULL)
			last = last->next;
		last->next = new_trans;
	}
	//making transfer log for who has been transfered some money
	to_person->deposit += money;
	to_person->transaction_count++;
	transaction* new_trans_2 = (transaction*)malloc(sizeof(transaction));
	strcpy(new_trans_2->data, temp2);
	new_trans_2->next = NULL;

	if (to_person->trans == NULL)
		to_person->trans = new_trans_2;
	else {
		transaction* last = to_person->trans;
		while (last->next != NULL)
			last = last->next;
		last->next = new_trans_2;
	}
	printf("Successfully transfered!\n");
	return 1;
}

void loadfile(client* head) {							//load file from database file into linked list
	FILE* file_temp = fopen("database.txt", "a+");
	if (file_temp == NULL) {
		printf("Can't access to database file!\n");
		return;
	}
	if (fclose(file_temp) == EOF)
		printf("closing file operation failed\n");

	FILE* file = fopen("database.txt", "r");
	if (file == NULL) {
		printf("Can't access to database file!\n");
		return;
	}
	while (1) {
		char temp[MAX];
		client* person = (client*)malloc(sizeof(client));
		person->next = NULL;
		person->trans = NULL;
		if (fscanf(file, "%s %s %s %d %d %d", person->name, person->password, person->telephone_number, &person->id, &person->deposit, &person->transaction_count) != 6) {
			free(person);
			break;
		}
		for (int i = 0; i < person->transaction_count; i++) {
			transaction* new_trans = (transaction*)malloc(sizeof(transaction));
			new_trans->next = NULL;
			fscanf(file, "%c", &temp);			//clearing \n buffer from previous fscanf function in order to read correctly the whole line
			fscanf(file, "%[^\n]", new_trans->data);		//read until \n is reached
			if (person->trans == NULL)
				person->trans = new_trans;
			else {
				transaction* last = person->trans;
				while (last->next != NULL)
					last = last->next;
				last->next = new_trans;
			}
		}
		if (head->next == NULL)
			head->next = person;
		else {
			client* last = head;
			while (last->next != NULL)
				last = last->next;
			last->next = person;
		}
	}
	if (fclose(file) == EOF)
		printf("closing file operation failed\n");
}

void savefile(client* head) {						//save file into database file
	FILE* file = fopen("database.txt", "w");
	if (file == NULL) {
		printf("Can't access to database file!\n");
		return;
	}
	client* person = head;
	transaction* trans = (transaction*)malloc(sizeof(transaction));
	person = person->next;
	while (head->next != NULL && person != NULL) {
		trans = person->trans;
		fprintf(file, "%s\n%s\n%s\n%d\n%d\n%d\n", person->name, person->password, person->telephone_number, person->id, person->deposit, person->transaction_count);
		for (int i = 0; i < person->transaction_count; i++) {
			fprintf(file, "%s\n", trans->data);
			trans = trans->next;
		}
		person = person->next;
	}
	if (fclose(file) == EOF)
		printf("closing file operation failed\n");
}


client* find(client* head, char* name) {		//finding person in linked list
	client* check = head;
	while (check != NULL) {
		if (strcmp(name, check->name) == 0) {
			return check;
		}
		else {
			check = check->next;
		}
	}
	return NULL;
}

void print_trans(client* check) {			//print transactions linked list
	transaction* trans_head = check->trans;
	while (trans_head != NULL)
	{
		puts(trans_head->data);
		trans_head = trans_head->next;
	}
}