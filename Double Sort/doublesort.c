/*
 * This is a program that reads input from a file and
 * then converts said input into a doubly linked list. 
 * This list is then traversed/sorted either first to 
 * last item, or last to first item. The user then may 
 * choose to either output the sorted list to the command 
 * line or to an output file of their choosing.
 *
 * @author Luis Pena (PID 6300130)
 * @version 03/07/2023
 *
 * I, Luis K. Pena certify that this is my own original
 * program developed by me with no internet or outside
 * help
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

//Global NODE struct declaration
typedef struct NODE
{
	char data[30];
	struct NODE *prev;
	struct NODE *next;
} NODE;


//Instantiating and Initializing doubly linked list pointers
NODE *head = NULL;
NODE *tail = NULL;

//Function to insert nodes into doubly linked list
void insert(char* word)
{
	//Instantiating newNode and curr pointers
	NODE *newNode = NULL;
	NODE *curr = NULL;

	//Allocating memory for newNode NODE 
	if(!(newNode = malloc(sizeof(NODE))))
	{
		//Informing user if no memory is available
		printf("No memory available for newNode NODE\n");
		exit(101);
	}

	//Allocating memory for curr NODE
	if(!(curr = malloc(sizeof(NODE))))
	{
		printf("No memory available for curr NODE\n");
		exit(101);
	}

	//Setting pointers and passing string argument to new NODE data
	newNode->prev = NULL;
	strcpy(newNode->data, word);
	newNode->next = NULL;

	//Checking if head node of list is empty
	if(head == NULL)
	{
		//If so, set head and tail to the new NODE
		head = newNode;
		tail = newNode;
		return;
	}

	//Setting curr NODE to head
	curr = head;

	//Checking if current string goes before head 
	if(strcmp(newNode->data, curr->data) < 0)
	{
		newNode->next = curr;
		curr->prev = newNode;
		head = newNode;
		
		//Return as insertion is complete
		return;
	}

	//Checking that string is smaller than next or end of list
	while(curr->next != NULL && (strcmp(newNode->data, curr->next->data) >=  0))
	{
		//Checking if string is already in doubly linked list 
		if(curr->next != NULL && (strcmp(newNode->data, curr->next->data) == 0))
		{
			return;
		}
		
		//Updating curr NODE to next
		curr = curr->next;
	}

	//Inserting node at end of doubly linked list
	if(curr->next == NULL)
    	{
        curr->next = newNode;
        newNode->prev = curr;
        newNode->next = NULL;
        tail = newNode;
    	}
    	
	//Else insert between curr and curr->next
	else{
        newNode->next = curr->next;
        newNode->next->prev = newNode;
        newNode->prev = curr;
        curr->next = newNode;
    }
}

//Function to reclaim doubly linked list
void reclaim_list()
{
	//Instantiating NODE temp 
	NODE *temp;

	//Setting temp to head
	temp = head;

	//While head is not NULL free NODE
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

//Function to print doubly linked list first item to last
void print_forward()
{
	//Setting current node pointer to head
	NODE *curr = head;

	//Iterating through doubly linked list first item to last
	while(curr != NULL)
	{
		//Printing data and updating curr NODE
		printf("%s\n", curr->data);
		curr = curr->next;
	}	
}

//Function to write doubly linked list first item to last on output file
void write_forward(FILE *outputFile)
{
	//Setting current node pointer to head
	NODE *curr = head;

	//Iteraring through doubly linked linst first item to last
	while(curr != NULL)
	{
		//Writing data to output file and updating curr NODE  
		fprintf(outputFile,"%s\n", curr->data);
		curr = curr->next;
	}
}
		
//Function to print doubly linked list last item to first
void print_backwards()
{
	//setting current node pointer to tail
	NODE *curr = tail;

	//Iterating through doubly linked list last item to first
	while(curr != NULL)
	{
		printf("%s\n", curr->data);
		curr = curr->prev;		
	}
}

//Function to write doubly linked list last item to first on output file
void write_backwards(FILE *outputFile)
{
	//Stting curr node pointer to tail
	NODE *curr = tail;

	//Iterating through doubly linked list last item to first
	while(curr != NULL)
	{
		fprintf(outputFile, "%s\n", curr->data);
	       	curr = curr->prev; 	
	}
}

//Main function
int main(int argc, char **argv)
{
	//Instantiating and initializing variables
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int dflag = 0, oflag = 0;
	FILE *inputFile, *outputFile;
	char inputFileName[20], outputFileName[20], word[20];
	static char usage[] = "usage: %s doublesort [-d] [-o output_file_name] input_file_name [name ...]\n";


	//Checking and setting flags
	while((c = getopt(argc, argv, "do:")) != -1)
	switch(c)
	{
		//Reverse doubly linked list flag
		case 'd':
			dflag = 1;
			break;

		//Print to output file flag	
		case 'o':
			oflag = 1;
			strcpy(outputFileName, optarg);
	}	
	
	//Checking that an input file is given  
	if((optind+1) > argc)
    	{	
		//Informing user an input file is required
		fprintf(stderr, "%s: missing input file name\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} 
	
	//Copying input file content into doubly linked list
	if(optind < argc)
	{
		//Setting input file name from commandline
		strcpy(inputFileName, argv[optind]);

		//Opening input file in Read mode
		inputFile  = fopen(inputFileName,"r");
		
		//Informing user if file cannot be found/opened
		if(!inputFile)
		{
			printf("Could not open input file %s\n", inputFileName);
			exit (101);
		}
		
				
		//Reading information from input file
		while((fscanf(inputFile, "%s", word)) != EOF)
		{										
			//Iterating over each character in string	
			for(int i = 0; i < strlen(word); i++)
			{
				//Converting all characters in string to lowercase
				word[i] = tolower(word[i]);
							
				//Removing all non-alpha characters from string
 				if(!isalpha(word[i]))
				{
					word[i] = ' ';
				}
			}

			//Checking that string is not empty before adding to list
			if(word[0] != ' ')
			{
				//Caling insert function to add work to list
				insert(word);
			}
		}
		
		//Closing input file
		fclose(inputFile);	
	}

	else
	{
		printf("no arguments left to process\n");
	}

	//Traversing doubly linked list first to last item
	if(dflag == 0)
	{
		//If not oflag, simply print doubly linked list to screen
		if(oflag == 0)
		{
			//Calling print_forward function
			print_forward();	
		}

		//If o flag, write each item to output file using write feature 
		else if(oflag == 1)
		{
			//Open or create output file using Write mode
			outputFile  = fopen(outputFileName,"w");
			
			//Inform user if output file cannot be opened/created
			if(!outputFile)
			{
				printf("Could not open input file %s\n", outputFileName);
				exit (101);
			}
			
			//Calling write_forward function
			write_forward(outputFile);
			
						
			//Closing output file
			fclose(outputFile);	
		}
	}
	
	//Traverse doubly linked list last to first item
	if(dflag == 1)
	{
		//If not oflag, simply print doubly linked list to screen
		if(oflag == 0)
		{
			print_backwards();									
		}

		//If o flag, write name to a file using write feature 
		else if(oflag == 1)
		{
			//Open or create output file using Write mode
			outputFile  = fopen(outputFileName,"w");
			
			//Inform user if output file cannot be opened/created
			if(!outputFile)
			{
				printf("Could not open input file %s\n", outputFileName);
				exit (101);
			}
		
			//Calling write_backwards function
			write_backwards(outputFile);
						
			//Closing output file
			fclose(outputFile);	
		}
	}

	//Reclaiming all nodes from doubly linked list
	reclaim_list();
	
	//Exit with 0 message
	exit(0);
}
