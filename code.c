/****

  Programming Concepts - Assignment Two, sp2 2023

  Provided File:  Please add your solution to this file (code and comments).

  : )

****/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "blackJack.c"
#include "blackJack.h"

#define STRING_LENGTH 21 /* Maximum length of strings (20 characters plus one for the null character */
#define MAX_PLAYERS 10	 /* Maximum players stored in array of players */

/* All the functions in the assignment handout are
   to be implemented in order to complete this assignment.
   All functions must be implemented according to the specs provided.
   Function specifications are provided in the assignment handout.
*/

// Functions that are being used in my code

int read_player_file(FILE *, char player_array[][STRING_LENGTH], int score_array[]);

void display_players(char player_array[][STRING_LENGTH], int score_array[], int);

void write_to_file(FILE *, char player_array[][STRING_LENGTH], int score_array[], int num_players);

int find_player(char player_name[], char player_array[][STRING_LENGTH], int num_players);

int add_player(char player_array[][STRING_LENGTH], int score_array[], char player_name[], int score, int *num_players);

int remove_player(char player_array[][STRING_LENGTH], int score_array[], char player_name[], int *num_players);

void display_highest_score(char player_array[][STRING_LENGTH], int score_array[], int num_players);

void reset_player_score(char player_array[][STRING_LENGTH], int score_array[], int num_players, char player_name[]);

void play_blackjack_games(Card deck[], char player_array[][STRING_LENGTH], int score_array[], int player_index);

int main()
{

	printf("File : waybt001_assign2.c\nAuthor : Batman\nStud ID : 0123456X\nEmail ID : waybt001\nThis is my own work as defined by the University's Academic Misconduct Policy.\n");

	Card deck[MAX_CARDS]; /* Deck of cards of size MAX_CARDS (52) */

	char player_array[MAX_PLAYERS][STRING_LENGTH] = {'\0'};
	int score_array[MAX_PLAYERS] = {0};

	int no_players = 0;
	int result = 0;

	FILE *inFile;
	FILE *outFile;
	srand(time(NULL));

	inFile = fopen("players.txt", "r");
	outFile = fopen("new_players.txt", "w");
	char game[10] = "start";
	no_players = read_player_file(inFile, player_array, score_array);
	while (strcmp(game, "start") == 0)
	{
		const char *commands[8] = {"list", "search", "reset", "add", "remove", "high", "play", "quit"};
		printf("Please enter choice\n");
		printf("[ list , search , reset , add , remove , high , play , quit ]:\n");
		char command[50];
		fgets(command, sizeof(command), stdin);
		command[strcspn(command, "\n")] = '\0'; // Remove trailing newline character

		int flag = -1;
		while (flag == -1)
		{
			for (int i = 0; i < 8; i++)
			{
				if (strcmp(command, commands[i]) == 0)
				{
					flag = i;
				}
			}
			if (flag == -1)
			{
				printf("Not a valid command\n");

				// Flush the input buffer
				int c;
				while ((c = getchar()) != '\n' && c != EOF)
				{
					// discard characters
				}

				fgets(command, sizeof(command), stdin);
				command[strcspn(command, "\n")] = '\0'; // Remove trailing newline character
			}
		}
		printf("In %s command\n", commands[flag]);
		if (strcmp(command, "quit") == 0)
		{
			strcpy(game, "quit");
		}
		else if (strcmp(command, "list") == 0)
		{
			display_players(player_array, score_array, no_players);
		}
		else if (strcmp(command, "search") == 0)
		{
			char playerName[50];
			printf("Please enter the player's name:");
			fgets(playerName, sizeof(playerName), stdin);
			playerName[strcspn(playerName, "\n")] = '\0'; // Remove trailing newline character

			int index = find_player(playerName, player_array, no_players);
			if (index == -1)
			{
				printf("There is no player with the name %s\n", playerName);
			}
			else
			{
				int score = score_array[index];
				printf("%s's current score: %d\n", playerName, score);
			}
		}
		else if (strcmp(command, "add") == 0)
		{
			char playerName[STRING_LENGTH];
			int score;

			printf("Please enter the player's name: ");
			fgets(playerName, sizeof(playerName), stdin);
			playerName[strcspn(playerName, "\n")] = '\0'; // Remove trailing newline character

			printf("Please enter the player's score: ");
			scanf("%d", &score);
			getchar(); // Consume the newline character left in the input buffer

			int addResult = add_player(player_array, score_array, playerName, score, &no_players);

			if (addResult == -1)
			{
				printf("Error: Maximum number of players reached. Cannot add a new player.\n");
			}
			else if (addResult == 0)
			{
				printf("Error: Player '%s' already exists.\n", playerName);
			}
			else
			{
				printf("Player '%s' added successfully.\n", playerName);
			}
		}
		else if (strcmp(command, "remove") == 0)
		{
			char playerName[STRING_LENGTH];

			printf("Please enter the player's name: ");
			fgets(playerName, sizeof(playerName), stdin);
			playerName[strcspn(playerName, "\n")] = '\0'; // Remove trailing newline character

			int removeResult = remove_player(player_array, score_array, playerName, &no_players);

			if (removeResult == 0)
			{
				printf("Error: Player '%s' does not exist.\n", playerName);
			}
			else
			{
				printf("Player '%s' removed successfully.\n", playerName);
			}
		}
		else if (strcmp(command, "high") == 0)
		{
			display_highest_score(player_array, score_array, no_players);
		}
		else if (strcmp(command, "play") == 0)
		{
			char playerName[STRING_LENGTH];
			printf("Please enter the player's name: ");
			fgets(playerName, sizeof(playerName), stdin);
			playerName[strcspn(playerName, "\n")] = '\0'; // Remove trailing newline character
			int playerIndex = find_player(playerName, player_array, no_players);
			if (playerIndex == -1)
			{
				printf("There is no player with the name %s\n", playerName);
			}
			else
			{
				play_blackjack_games(deck, player_array, score_array, playerIndex);
			}
		}
		else if (strcmp(command, "reset") == 0)
		{
			char playerName[STRING_LENGTH];

			printf("Please enter the player's name: ");
			fgets(playerName, sizeof(playerName), stdin);
			playerName[strcspn(playerName, "\n")] = '\0'; // Remove trailing newline character

			reset_player_score(player_array, score_array, no_players, playerName);
		}
	}
	write_to_file(outFile, player_array, score_array, no_players);

	fclose(inFile);
	fclose(outFile);

	return 0;
}

int read_player_file(FILE *infile, char player_array[][STRING_LENGTH], int score_array[])
{

	char name[STRING_LENGTH];
	int i = 0;

	fgets(name, STRING_LENGTH, infile);

	while (i < MAX_PLAYERS && !feof(infile))
	{
		name[strlen(name) - 1] = '\0';
		strcpy(player_array[i], name);

		fscanf(infile, "%d ", &score_array[i]);

		i++;

		fgets(name, STRING_LENGTH, infile);
	}

	return i;
}

void display_players(char player_array[][STRING_LENGTH], int score_array[], int num_players)
{
	// Updated the given display func ...
	printf("=======================================\n");
	printf("| %-20s | %-6s |\n", "Name", "Score");
	printf("=======================================\n");

	for (int index = 0; index < num_players; index++)
	{
		printf("| %-20s | %-6d |\n", player_array[index], score_array[index]);
		printf("---------------------------------------\n");
	}

	printf("=======================================\n");
}

void write_to_file(FILE *outfile, char player_array[][STRING_LENGTH], int score_array[], int num_players)
{
	int index;
	for (index = 0; index < num_players; index++)
	{
		fprintf(outfile, "%s %d\n", player_array[index], score_array[index]);
	}
}

int find_player(char player_name[], char player_array[][STRING_LENGTH], int num_players)
{
	for (int i = 0; i < num_players; i++)
	{
		if (strcmp(player_array[i], player_name) == 0)
		{
			return i; // Return the index of the player if found
		}
	}
	return -1; // Player not found
}

int add_player(char player_array[][STRING_LENGTH], int score_array[], char player_name[], int score, int *num_players)
{
	int index = find_player(player_name, player_array, *num_players);

	if (index != -1)
	{
		// Player already exists, return 0
		return 0;
	}

	if (*num_players >= MAX_PLAYERS)
	{
		// Array is full, cannot add new player, return -1
		return -1;
	}

	// Add the new player at the end of the array
	strcpy(player_array[*num_players], player_name);
	score_array[*num_players] = score;

	// Increment the number of players
	(*num_players)++;

	// Player added successfully, return 1
	return 1;
}

int remove_player(char player_array[][STRING_LENGTH], int score_array[], char player_name[], int *num_players)
{
	int index = find_player(player_name, player_array, *num_players);

	if (index == -1)
	{
		// Player not found, return 0
		return 0;
	}

	// Shift all elements one position down the array to remove the player
	for (int i = index; i < (*num_players) - 1; i++)
	{
		strcpy(player_array[i], player_array[i + 1]);
		score_array[i] = score_array[i + 1];
	}

	// Clear the last element in the arrays
	player_array[*num_players - 1][0] = '\0';
	score_array[*num_players - 1] = 0;

	// Decrement the number of players
	(*num_players)--;

	// Player removed successfully, return 1
	return 1;
}

void display_highest_score(char player_array[][STRING_LENGTH], int score_array[], int num_players)
{
	int highest_score = 0;

	// Find the highest score
	for (int i = 0; i < num_players; i++)
	{
		if (score_array[i] > highest_score)
		{
			highest_score = score_array[i];
		}
	}

	// Display players with the highest score
	printf("Players with the highest score of %d are:\n", highest_score);

	for (int i = 0; i < num_players; i++)
	{
		if (score_array[i] == highest_score)
		{
			printf("--> %s\n", player_array[i]);
		}
	}
}

void reset_player_score(char player_array[][STRING_LENGTH], int score_array[], int num_players, char player_name[])
{
	int index = find_player(player_name, player_array, num_players);

	if (index == -1)
	{
		printf("Player '%s' has not been found.\n", player_name);
	}
	else
	{
		score_array[index] = 0;
		printf("Player '%s' score has been reset to 0.\n", player_name);
	}
}

void play_blackjack_games(Card deck[], char player_array[][STRING_LENGTH], int score_array[], int player_index)
{
	char play_again = 'y';

	while (play_again == 'y' || play_again == 'Y')
	{
		int game_result = play_one_game(deck); // Call the play_one_game function from the blackjack library

		// Update player's score based on the game result
		if (game_result == PLAYER)
		{
			score_array[player_index] += 3; // Add 3 points for winning
		}
		else if (game_result == DRAW)
		{
			score_array[player_index] += 1; // Add 1 point for a draw
		}
		else
		{
			score_array[player_index] += 0; // Add 0 points for losing
		}

		printf("%s's score: %d -- Play again [y|n]? ", player_array[player_index], score_array[player_index]);
		scanf(" %c", &play_again);
		getchar(); // Consume the newline character from scanf
	}
}
