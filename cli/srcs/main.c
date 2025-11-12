/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:48:19 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/12 18:56:52 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "GameState.h"
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <defines.h>

struct Memory {
    char *response;
    size_t size;
};

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;
    char *ptr = realloc(mem->response, mem->size + real_size + 1);
    if (!ptr) return 0;
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, real_size);
    mem->size += real_size;
    mem->response[mem->size] = '\0';
    return real_size;
}

// 🔘 Cette fonction sera appelée quand on clique le bouton
static void on_button_clicked(GtkWidget *button, gpointer user_data) {
	(void)user_data;
    (void)button; // éviter warning -Wunused-parameter
    printf("➡️  Lancement du CURL !\n");

    CURL *curl = curl_easy_init();
    if (!curl) return;

    struct Memory chunk = { .response = malloc(1), .size = 0 };

    curl_easy_setopt(curl, CURLOPT_URL, PONG);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK)
        printf("✅ Réponse :\n%s\n", chunk.response);
    else
        fprintf(stderr, "Erreur: %s\n", curl_easy_strerror(res));

    free(chunk.response);
    curl_easy_cleanup(curl);
}

// 🪟 Setup GTK
static void activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Test CURL");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    GtkWidget *button = gtk_button_new_with_label("Faire une requête");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    gtk_window_set_child(GTK_WINDOW(window), button);
    gtk_window_present(GTK_WINDOW(window));
}

CURL *curl;
int main() 
{
	GameState gameState;
	get_game_data(&gameState);
	post_input(56);	
	curl_easy_cleanup(curl);
}
