/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:25:51 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/30 15:25:20 by ellanglo         ###   ########.fr       */
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
#include <jwt.h>

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

static int do_curl_get(char *url, char *response)
{
	curl_easy_reset(App.curl);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_USERAGENT, CURL_USER_AGENT);
	CURL_OPT(CURLOPT_WRITEFUNCTION, write_callback);
	CURL_OPT(CURLOPT_WRITEDATA, response);
	CURL_OPT(CURLOPT_SSL_VERIFYPEER, 0L);
	CURL_OPT(CURLOPT_SSL_VERIFYHOST, 0L);

	CURLcode res = curl_easy_perform(App.curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "%s\n", curl_easy_strerror(res));
		return 2;
	}
	return 0;
}

static int do_curl_post(char *url, char *data, char *response)
{
	curl_easy_reset(App.curl);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_POST, 1L);
	CURL_OPT(CURLOPT_POSTFIELDS, data);
	CURL_OPT(CURLOPT_WRITEFUNCTION, write_callback);
	CURL_OPT(CURLOPT_WRITEDATA, response);
	CURL_OPT(CURLOPT_SSL_VERIFYPEER, 0L);
	CURL_OPT(CURLOPT_SSL_VERIFYHOST, 0L);

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
	printf("%s\n", body);
	int res = do_curl_get(body, response);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	App.GameState.score = JSON_GET(data, "score", JSON_INT);
	App.GameState.lpos = JSON_GET(data, "lpos", JSON_DOUBLE);
	App.GameState.rpos = JSON_GET(data, "rpos", JSON_DOUBLE);
	App.GameState.bposx = JSON_GET(data, "bposx", JSON_DOUBLE);
	App.GameState.bposy = JSON_GET(data, "bposy", JSON_DOUBLE);
	return 0;
}

int post_input(int input)
{
	char response[RESPONSE_SIZE];
	char body[50];
	snprintf(body, sizeof(body), "{\"game_id\":%d,\"input\":%d,\"side\":%d}", App.gameId, input, App.side);
	int res = do_curl_post(PONG_POST, body, response);
	if (res)
		return 1;
	return 0;
}

int set_ball(double x, double y, int mode)
{
	char response[RESPONSE_SIZE];

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
	int res = do_curl_post(PONG_SET_BALL, body, response);
	if (res)
		return 1;
	return 0;
}

int register_user(char *username, char *email, char *password)
{
	char response[RESPONSE_SIZE];
	size_t size = 41 + strlen(username) + strlen(email) + strlen(password);
	char *body = malloc(size * sizeof(char));
	snprintf(body, size, "{\"username\":\"%s\",\"email\":\"%s\",\"password\":\"%s\"}", username, email, password);
	int res = do_curl_post(REGISTER, body, response);
	free(body);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const cJSON *error = JSON_GET(data, "error");
	if (cJSON_IsString(error))
		goto json_error;

	printf("%s\n", JSON_GET(data, "message", JSON_STR));
	return 0;

json_error:
	printf("%s\n", error->JSON_STR);
	return 2;
}

int login_user(char *username, char *password)
{
	char response[RESPONSE_SIZE];
	size_t size = 30 + strlen(username) + strlen(password);
	char *body = malloc(size * sizeof(char));
	snprintf(body, size, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
	int res = do_curl_post(LOGIN, body, response);
	free(body);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const cJSON *error = JSON_GET(data, "error");
	if (cJSON_IsString(error))
		goto json_error;

	const char *message = JSON_GET(data, "message", JSON_STR);
	const char *jwt = JSON_GET(data, "token", JSON_STR);

	size = strlen(jwt);
	App.UserInfo.jwt = malloc((size + 1) * sizeof(char));
	memcpy(App.UserInfo.jwt, jwt, size);

	App.UserInfo.jwt[size-1] = '\0';
	printf("%s\n", message);

	jwt_t *token = NULL;
	jwt_decode(&token, jwt, NULL, 0);
	App.UserInfo.id = jwt_get_grant_int(token, "id");

	return 0;

json_error:
	printf("%s\n", error->JSON_STR);
	return 2;
}

int create_game()
{
	char response[RESPONSE_SIZE];
	char body[18];
	snprintf(body, sizeof(body), "{\"player_id\":%d}", App.UserInfo.id);
	int res = do_curl_post(PONG_CREATE, body, response);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const int id = JSON_GET(data, "id", JSON_INT);
	printf("New game created with id %d\n", id);
	App.gameId = id;
	return 0;
}

int join_game(int gameId)
{
	char response[RESPONSE_SIZE];
	char body[32];
	snprintf(body, sizeof(body), "{\"game_id\":%d,\"player_id\":%d}", gameId, App.UserInfo.id);
	int res = do_curl_post(PONG_JOIN, body, response);
	if (res)
		return 1;

	printf("Game successfully joined\n");
	App.gameId = gameId;
	return 0;
}

int start_game()
{
	char response[RESPONSE_SIZE];
	char body[17];
	snprintf(body, sizeof(body), "{\"game_id\":%d}", App.gameId);
	int res = do_curl_post(PONG_START, body, response);
	if (res)
		return 1;
	return 0;
}
