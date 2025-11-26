/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:47:04 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/26 17:53:20 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Application.h>
#include <curl/curl.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

t_App App;

void create_app()
{
	App.curl = curl_easy_init();
	App.gtk = gtk_application_new("pong.cli", G_APPLICATION_DEFAULT_FLAGS | G_APPLICATION_HANDLES_OPEN);
	App.Inputs = malloc(sizeof(*App.Inputs));
	memset(App.Inputs, 0, sizeof(*App.Inputs));
}

__attribute__((destructor))
void delete_app()
{
	g_object_unref(App.gtk);
	curl_easy_cleanup(App.curl);
	free(App.Inputs);
}
