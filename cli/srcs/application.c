/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:47:04 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/02 15:14:59 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Application.h>
#include <stdio.h>
#include <curl/curl.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

t_App App;

void create_app()
{
	App.curl = curl_easy_init();

	int fd = open("/dev/urandom", O_RDONLY);
	char random[11];
	int ret = read(fd, random, 10);
	close(fd);
	(void)ret;

	for (int i = 0; i != 10; i++)
		random[i] = (unsigned char)random[i] % 26 + 'A';
	random[10] = '\0';
	App.name = malloc(20 * sizeof(char));
	snprintf(App.name, 20, "pong.cli.%s", random);

	App.gtk = gtk_application_new(App.name, G_APPLICATION_DEFAULT_FLAGS | G_APPLICATION_HANDLES_OPEN);
	
	App.Inputs.up = 0;
	App.Inputs.down = 0;

	App.UserInfo.jwt = NULL;
	App.UserInfo.username = NULL;
	App.UserInfo.password = NULL;
}

__attribute__((destructor))
void delete_app()
{
	g_object_unref(App.gtk);
	curl_easy_cleanup(App.curl);

	free(App.name);
	free(App.UserInfo.jwt);
	free(App.UserInfo.username);
	free(App.UserInfo.password);
}
