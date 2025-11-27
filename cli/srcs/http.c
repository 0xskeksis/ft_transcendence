/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:25:51 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/27 15:28:26 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <defines.h>
#include <http.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <Application.h>
#include <cjson/cJSON.h>
#include <stdio.h>
#include <math.h>

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
    char *buffer = (char*)userp;
    size_t real_size = size * nmemb;
    if (real_size >= RESPONSE_SIZE)
        real_size = RESPONSE_SIZE - 1;
    memcpy(buffer, data, real_size);
    buffer[real_size] = '\0';
    return real_size;
}

UNUSED static int do_curl_get(char *url, char *response)
{
	curl_easy_reset(App.curl);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_USERAGENT, CURL_USER_AGENT);
	CURL_OPT(CURLOPT_WRITEFUNCTION, write_callback);
	CURL_OPT(CURLOPT_WRITEDATA, response);

	CURLcode res = curl_easy_perform(App.curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "%s\n", curl_easy_strerror(res));
		return 2;
	}
	return 0;
}

static int do_curl_post(char *url, char *data)
{
	curl_easy_reset(App.curl);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_POST, 1L);
	CURL_OPT(CURLOPT_POSTFIELDS, data);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    CURL_OPT(CURLOPT_HTTPHEADER, headers);

	CURLcode res = curl_easy_perform(App.curl);
	curl_slist_free_all(headers);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "%s\n", curl_easy_strerror(res));
		return 2;
	}
	return 0;
}

int get_game_data()
{
	char response[RESPONSE_SIZE];
	char body[64];	

	snprintf(body, sizeof(body), PONG_GET"?game_id=%d", App.gameId);
	int res = do_curl_get(body, response);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	App.GameState.score = JSON_GET(data, "score", JSON_INT);
	App.GameState.lpos = JSON_GET(data, "lpos", JSON_DOUBLE);
	App.GameState.rpos = JSON_GET(data, "rpos", JSON_DOUBLE);
	App.GameState.bposx = JSON_GET(data, "bposx", JSON_DOUBLE);
	App.GameState.bposy = JSON_GET(data, "bposy", JSON_DOUBLE);
//	printf("%d\n", App.GameState.score);
//	printf("%f\n", App.GameState.lpos);
//	printf("%f\n", App.GameState.rpos);
//	printf("%f\n", App.GameState.bposx);
//	printf("%f\n", App.GameState.bposy);
	return 0;
}

int post_input(int input)
{
	char body[50];
	snprintf(body, sizeof(body), "{\"game_id\":%d,\"input\":%d,\"side\":%d}", App.gameId, input, App.side);
	int res = do_curl_post(PONG_POST, body);
	if (res)
		return 1;
	return 0;
}

int set_ball(double x, double y, int mode)
{
	if (mode == 0)
	{
		GtkWindow *window = gtk_application_get_active_window(App.gtk);
		int w = gtk_widget_get_width(GTK_WIDGET(window));
		int h = gtk_widget_get_height(GTK_WIDGET(window));
		x /= w;
		y /= h;
	}

	char body[48];
	snprintf(body, sizeof(body), "{\"x\":%f,\"y\":%f,\"game_id\":%d}", x, y, App.gameId);
	int res = do_curl_post(PONG_SET_BALL, body);
	if (res)
		return 1;
	return 0;
}

int register_user(char *username, char *email, char *password)
{
	size_t size = 41 + strlen(username) + strlen(email) + strlen(password);
	char *body = malloc(size * sizeof(char));
	snprintf(body, size, "{\"username\":\"%s\",\"email\":\"%s\",\"password\":\"%s\"}", username, email, password);
	printf("%s\n", body);
	int res = do_curl_post(REGISTER, body);
	free(body);
	if (res)
		return 1;
	return 0;
}

int login_user(char *email, char *password)
{
	size_t size = 39 + strlen(email) + strlen(password);
	char *body = malloc(size * sizeof(char));
	snprintf(body, size, "{\"email\":\"%s\",\"password\":\"%s\",\"jwt\":false}", email, password);
	printf("%s\n", body);
	int res = do_curl_post(LOGIN, body);
	free(body);
	if (res)
		return 1;
	return 0;
}

int create_game()
{
	char body[32] = "{\"player1_id\":0,\"player2_id\":1}\0";
	int res = do_curl_post(PONG_CREATE, body);
	if (res)
		return 1;
	return 0;
}

int start_game()
{
	char body[14] = "{\"game_id\":0}\0";
	int res = do_curl_post(PONG_START, body);
	if (res)
		return 1;
	return 0;
}
