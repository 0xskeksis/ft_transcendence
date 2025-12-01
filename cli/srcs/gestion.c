/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 18:35:04 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/30 16:44:14 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <http.h>
#include <defines.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include <gestion.h>
#include <jwt.h>
#include <Application.h>
#include <graphic.h>

static ssize_t read_pass (char **lineptr, size_t *n)
{
    struct termios old, new;
    int nread;
    tcgetattr (fileno(stdin), &old);
    new = old;
    new.c_lflag &= ~ECHO;
    tcsetattr(fileno(stdin), TCSAFLUSH, &new);
    nread = getline(lineptr, n, stdin);
    tcsetattr(fileno(stdin), TCSAFLUSH, &old);
    return nread;
}

static char *read_input(const char *prompt, bool mask)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    while (1)
	{
        printf("%s", prompt);
        fflush(stdout);

        if (mask)
            read = read_pass(&line, &len);
        else
            read = getline(&line, &len, stdin);

        if (read == -1) 
		{
            printf("\nInvalid input, please try again.\n");
            free(line);
			clearerr(stdin);
            line = NULL;
            len = 0;
            continue;
        }

        if (read <= 1) 
		{
            printf("Your input can't be empty\n");
            continue;
        }

        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        return line;
    }
}

static int max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}
static int user_register_sequence(UNUSED void* _)
{
	char *data[4];
	int pass = 0;
	int len;

	if (App.UserInfo.jwt != NULL)
		return printf("User already loged in\n"), 1;
	do 
	{
		if (pass)
		{
			printf("Error: difference in password!\n");
			free(data[0]);
			free(data[1]);
			free(data[2]);
			free(data[3]);
		}
		data[0] = read_input("Username:\n", 0);
		data[1] = read_input("Email:\n", 0);
		data[2] = read_input("Password:\n", 1);
		data[3] = read_input("Password confirmation:\n", 1);
		len = max(strlen(data[2]), strlen(data[3]));
		pass = 1;
	} while (strncmp(data[2], data[3], len) != 0);
	int ret = register_user(data[0], data[1], data[2]);
	free(data[0]);
	free(data[1]);
	free(data[2]);
	free(data[3]);
	return ret;
}

static int user_login_sequence(UNUSED void* _)
{
	char *data[2];

	if (App.UserInfo.jwt != NULL)
		return printf("User already loged in\n"), 1;
	data[0] = read_input("Username:\n", 0);
	data[1] = read_input("Password:\n", 1);
	int ret = login_user(data[0], data[1]);
	free(data[0]);
	free(data[1]);
	return ret;
}

static int create_game_sequence(UNUSED void* _)
{
	if (App.UserInfo.jwt == NULL)
		return printf("User has to log in in order to create a game\n"), 1;
	
	int ret = create_game();
	if (ret)
		return ret;
	App.side = 0;
	init_gtk(0, NULL);
	return 0;
}

static int join_game_sequence(UNUSED void* _)
{
	if (App.UserInfo.jwt == NULL)
		return printf("User has to log in in order to join a game\n"), 1;

	char *id = read_input("Game id:\n", 0);
	int ret = join_game(atoi(id));
	if (ret)
		return ret;
	App.side = 1;
	init_gtk(0, NULL);
	return 0;
}

static int free_and_quit(void *to_free)
{
	free(to_free);
	delete_app();
	_Exit(0);
}

static int help_menu(UNUSED void* _)
{
	printf("Here is the list of command you can do:\n");
	printf("-'register' to start the register process\n");
	printf("-'login' to start the login process\n");
	printf("-'create' to create and join a game\n");
	printf("-'join' to join a game\n");
	printf("-'quit' to quit the cli\n");
	return 0;
}

#define OPTION_NB 6
void user_menu()
{
	char *response;
	static const MenuOption Options[OPTION_NB] =
		{
			[0] = {user_register_sequence, "register", 8},
			[1] = {user_login_sequence, "login", 5},
			[2] = {create_game_sequence, "create", 6},
			[3] = {join_game_sequence, "join", 4},
			[4] = {free_and_quit, "quit", 4},
			[5] = {help_menu, "help", 4}
		};

	while (1)
	{
		response = read_input("Option:\n", 0);
		bool matched = false;
		for (int i = 0; i < OPTION_NB; i++)
		{
			MenuOption current = Options[i];

			if (strncmp(response, current.str, current.len) == 0)
			{
				matched = true;
				current.func(response);
				break;
			}
		}
		if (!matched)
			printf("Invalid Option\n");
		free(response);
	}
}
