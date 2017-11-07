#include <stdio.h>
#include <stdlib.h>

char* phase1phrase = "They raised him with mustard and cress.";
char* phase5answer = "giants";
char* phase5puzzle = "isrveawhobpnutfg...";

typedef struct node{
	int value;
	struct node* next;
} node;

node* head;

node* func6(node* curr);
void read_six_numbers(char* input, int* array);
int string_length(char* input);
int strings_not_equal(char* input1, char* input2);
int func4(int n);

void explode()
{
	exit(EXIT_FAILURE);
}

void initialzeNodes()
{
	node* curr;

	head = (node*)malloc(sizeof(node));
	curr = head;

	int array[] = {559, 442, 90, 508, 482, 771, 231, 559, 721, 112};

	for (int k = 0; k < 10; k++) {
		curr->next = (node*)malloc(sizeof(node));
		curr = curr->next;
		curr->value = array[k];
	}

	curr->next = NULL;
}

void phase_1(char* input)
{
	if (strings_not_equal(input, phase1phrase)) {
		explode();
	} 
	return;
}

void phase_2(char* input)
{
	int array[6];
	read_six_numbers(input, array);
	for (int k = 1; k < 6; k++) {
		if (array[k - 1] != array[k] + 5) {
			explode();
		}
	}
	return;
}

void phase_3(char* input)
{
	int n1, n2;
	if (sscanf(input, "%i %i", n1, n2) < 2) {
		explode();
	}

	if (n1 > 7)
		explode();

	int x = 0;

	switch(n1) {
		case (0):
			x = 0x275;
			x -= 0x389;
		case(1):
			x += 0x81;
		case(3):
			x -= 0x1df;
		case(2):
			x += 0x34c;
		case(4):
			x -= 0x156;
		case(6):
			x += 0x156;
		case(5):
			x -= 0x377;
			break;
		default:
			explode();
			break;
	}

	if (n1 > 5)
		explode();

	if (x != n2)
		explode();

	return;
}

void phase_4(char* input)
{
	int number;
	if (sscanf(input, "%i", number) != 1) {
		explode();
	}

	if (number <= 0) {
		explode();
	}

	if (func4(number) != 0x63d) {
		explode();
	}

	return;
}

int func4(int n)
{
	if (n <= 1)
		return n;
        return (func4(n-1) + func4(n-2));
}

void phase_5(char* input)
{
	if (string_length(input) < 6) {
		explode();
	}

	char* s = "";

	for (int k = 0; k < 6; k++) {
		s += phase5puzzle[input[k] & 0xF];
	}

	if (strings_not_equal(s, phase5answer)) {
		explode();
	}
	return;
}

void phase_6(char* input)
{
	int x = strtol(input, NULL, 10);
	head->value = x;
	
	node* curr = func6(head);

	for (int k = 0; k < 6; k++) {
		curr = curr->next;
	}

	if (x != curr->value) {
		explode();
	}
	return;
}

node* func6(node* head)
{
	node* prev = head;
	node* curr = head->next;
	node* largest = head; 
	node* temp;

	while(prev != NULL) {
		while(curr != NULL) {
			temp = curr->next;
			if(curr->value > largest->value) {
				prev->next = curr->next;
				largest = curr;
				largest->next = head;
			}
			curr = temp;
		}
		prev = prev->next;
		curr = prev->next;
	}
	return largest ;
}









