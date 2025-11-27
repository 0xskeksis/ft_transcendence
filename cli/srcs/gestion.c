/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 18:35:04 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/27 20:02:30 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <http.h>
#include <defines.h>
#include <string.h>

char *read_input(char *prompt)
{
	char *line = NULL;
	size_t len = 0;

	printf("%s", prompt);
	ssize_t read = getline(&line, &len, stdin);
	line[read-1] = '\0';
	return line;
}

void user_register_sequence()
{
	char *data[3];

	data[0] = read_input("Username:\n");
	data[1] = read_input("Email:\n");
	data[2] = read_input("Password:\n");
	printf("%s\n%s\n%s\n", data[0], data[1], data[2]);
}

void user_login_sequence()
{
	char *data[2];

	data[]
}
