/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:25:51 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/13 12:49:05 by ellanglo         ###   ########.fr       */
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

const char *get_jwt_header()
{
	static char *jwt_header = NULL;

	if (jwt_header == NULL && App.UserInfo.jwt)
	{
		size_t size = 23 + strlen(App.UserInfo.jwt);
		jwt_header = malloc(size * sizeof(char));
		snprintf(jwt_header, size, "Authorization: Bearer %s", App.UserInfo.jwt);
	}
	return jwt_header;
}

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

static size_t write_img(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE *)userdata;
    if (!fp)
        return 0;

    size_t items_written = fwrite(ptr, size, nmemb, fp);
    return items_written * size;
}

static int do_curl_get(char *url, char *response, bool jwt, bool img)
{
	curl_easy_reset(App.curl);
	//CURL_OPT(CURLOPT_VERBOSE, 1L);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_USERAGENT, CURL_USER_AGENT);

	FILE *fp = NULL;
	if (img)
    {
        fp = fopen(".qrcode", "wb");
        if (!fp)
		{
            perror("fopen .qrcode");
            return 2;
        }
        CURL_OPT(CURLOPT_WRITEFUNCTION, write_img);
        CURL_OPT(CURLOPT_WRITEDATA, fp);
	}
	else
	{
		CURL_OPT(CURLOPT_WRITEFUNCTION, write_callback);
		CURL_OPT(CURLOPT_WRITEDATA, response);
		CURL_OPT(CURLOPT_SSL_VERIFYPEER, 0L);
		CURL_OPT(CURLOPT_SSL_VERIFYHOST, 0L);
	}

    struct curl_slist *headers = NULL;
	if (jwt)
	{
		headers = curl_slist_append(headers, get_jwt_header());
		CURL_OPT(CURLOPT_HTTPHEADER, headers);
	}

	CURLcode res = curl_easy_perform(App.curl);
	if (jwt)
		curl_slist_free_all(headers);
	if (img)
		fclose(fp);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "%s\n", curl_easy_strerror(res));
		return 2;
	}

	return 0;
}

static int do_curl_post(char *url, char *data, char *response, bool jwt)
{
	curl_easy_reset(App.curl);
	//CURL_OPT(CURLOPT_VERBOSE, 1L);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_POST, 1L);
	CURL_OPT(CURLOPT_POSTFIELDS, data);
	CURL_OPT(CURLOPT_WRITEFUNCTION, write_callback);
	CURL_OPT(CURLOPT_WRITEDATA, response);
	CURL_OPT(CURLOPT_SSL_VERIFYPEER, 0L);
	CURL_OPT(CURLOPT_SSL_VERIFYHOST, 0L);


    struct curl_slist *headers = NULL;
	if (jwt)
		headers = curl_slist_append(headers, get_jwt_header());
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

static int generate_and_print_qr(const char *uri)
{
	char request[1024];
	snprintf(request, sizeof(request), QR_CODE_GEN QR_OPT_BASE "%s", uri);
	if (do_curl_get(request, NULL, 0, 1) == 2)
		return 1;
	(void)system("jp2a --chars=\" █\" --background=light --term-fit --invert .qrcode");
	unlink(".qrcode");
	return 0;
}

int get_game_data()
{
	char response[RESPONSE_SIZE];
	char body[64];	

	snprintf(body, sizeof(body), PONG_GET"?game_id=%d", App.gameId);
	int res = do_curl_get(body, response, 1, 0);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);	
	const int status = JSON_GET(data, "status", JSON_INT);
	if (status == 2)
		return 2;
	App.GameState.score = JSON_GET(data, "score", JSON_INT);
	App.GameState.lpos = JSON_GET(data, "lpos", JSON_DOUBLE);
	App.GameState.rpos = JSON_GET(data, "rpos", JSON_DOUBLE);
	App.GameState.bposx = JSON_GET(data, "bposx", JSON_DOUBLE);
	App.GameState.bposy = JSON_GET(data, "bposy", JSON_DOUBLE);
	return 0;
}

int get_owner()
{
	char response[RESPONSE_SIZE];
	char body[64];

	snprintf(body, sizeof(body), PONG_GET_OWNER"?game_id=%d", App.gameId);
	int res = do_curl_get(body, response, 1, 0);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	return JSON_GET(data, "owner", JSON_INT);
}

int post_input(int input)
{
	char response[RESPONSE_SIZE];
	char body[50];
	snprintf(body, sizeof(body), "{\"game_id\":%d,\"input\":%d,\"side\":%d}", App.gameId, input, App.side);
	int res = do_curl_post(PONG_POST, body, response, 1);
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
	int res = do_curl_post(REGISTER, body, response, 0);
	free(body);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const cJSON *error = JSON_GET(data, "error");
	if (cJSON_IsString(error))
		goto json_error;

	const cJSON *_2fa = JSON_GET(data, "_2fa");
	const char *secret = JSON_GET(_2fa, "secret", JSON_STR);
	const char *uriKey = JSON_GET(_2fa, "uriKey", JSON_STR);
	printf("%s\n", JSON_GET(data, "message", JSON_STR));
	if (generate_and_print_qr(uriKey))
		printf("Here is your 2fa secret: %s\n", secret);
	return 0;

json_error:
	printf("%s\n", error->JSON_STR);
	return 2;
}

int login_user(char *username, char *password, char *token)
{
	char response[RESPONSE_SIZE];
	size_t username_size = strlen(username);
	size_t password_size = strlen(password);
	size_t size = 41 + username_size + password_size + strlen(token);
	char *body = malloc(size * sizeof(char));
	snprintf(body, size, "{\"username\":\"%s\",\"password\":\"%s\",\"token\":\"%s\"}", username, password, token);
	int res = do_curl_post(LOGIN, body, response, 0);
	free(body);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const cJSON *error = JSON_GET(data, "error");
	if (cJSON_IsString(error))
		goto json_error;

	const char *message = JSON_GET(data, "message", JSON_STR);
	const char *jwt = JSON_GET(data, "jtoken", JSON_STR);

	size = strlen(jwt);
	App.UserInfo.jwt = malloc((size + 1) * sizeof(char));
	memcpy(App.UserInfo.jwt, jwt, size);
	App.UserInfo.jwt[size] = '\0';

	App.UserInfo.username = malloc((username_size + 1) * sizeof(char));
	memcpy(App.UserInfo.username, username, username_size);
	App.UserInfo.username[username_size] = '\0';

	App.UserInfo.password = malloc((password_size + 1) * sizeof(char));
	memcpy(App.UserInfo.password, password, password_size);
	App.UserInfo.password[password_size] = '\0';

	jwt_t *jtoken = NULL;
	jwt_decode(&jtoken, jwt, NULL, 0);
	App.UserInfo.id = jwt_get_grant_int(jtoken, "id");
	printf("%s\n", message);

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
	int res = do_curl_post(PONG_CREATE, body, response, 1);
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
	int res = do_curl_post(PONG_JOIN, body, response, 1);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const cJSON *error = JSON_GET(data, "error");
	if (cJSON_IsString(error))
		goto json_error;

	printf("Game successfully joined\n");
	App.gameId = gameId;
	return 0;

json_error:
	printf("%s\n", error->JSON_STR);
	return 2;
}

int start_game()
{
	char response[RESPONSE_SIZE];
	char body[17];
	snprintf(body, sizeof(body), "{\"game_id\":%d}", App.gameId);
	int res = do_curl_post(PONG_START, body, response, 1);
	if (res)
		return 1;
	return 0;
}

int get_secret(char *username, char *password)
{
	char response[RESPONSE_SIZE];
	size_t username_size = strlen(username);
	size_t password_size = strlen(password);
	size_t size = 30 + username_size + password_size;
	char *body = malloc(size * sizeof(char));
	snprintf(body, size, "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
	int res = do_curl_post(GET_SECRET, body, response, 0);
	free(body);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const cJSON *error = JSON_GET(data, "error");
	if (cJSON_IsString(error))
		goto json_error;

	const char *secret = JSON_GET(data, "secret", JSON_STR);
	const char *uriKey = JSON_GET(data, "uriKey", JSON_STR);
	if (generate_and_print_qr(uriKey))
		printf("Here is your 2fa secret: %s\n", secret);
	return 0;

json_error:
	printf("%s\n", error->JSON_STR);
	return 2;
}

int create_tournament()
{
	char response[RESPONSE_SIZE];
	int res = do_curl_post(TOURNAMENT_CREATE, "{\"a\":0}", response, 1);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const int id = JSON_GET(data, "id", JSON_INT);
	printf("New tournament created with id: %d\n", id);
	return 0;
}

int join_tournament(int tournament_id)
{
	char response[RESPONSE_SIZE];
	char body[64];
	snprintf(body, sizeof(body), "{\"tournament_id\":%d,\"player_id\":%d}", tournament_id, App.UserInfo.id);
	int res = do_curl_post(TOURNAMENT_JOIN, body, response, 1);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	const cJSON *error = JSON_GET(data, "error");
	if (cJSON_IsString(error))
		goto json_error;

	App.gameId = JSON_GET(data, "id", JSON_INT);
	App.side = JSON_GET(data, "side", JSON_INT);
	App.tournamentId = tournament_id;
	printf("Tournament successfully joined\n");
	return 0;

json_error:
	printf("%s\n", error->JSON_STR);
	return 2;
}

int start_tournament()
{
	char response[RESPONSE_SIZE];
	char body[32];
	snprintf(body, sizeof(body), "{\"tournament_id\":%d}", App.tournamentId);
	int res = do_curl_post(TOURNAMENT_START, body, response, 1);
	if (res)
		return 1;
	
	return 0;
}

int get_tournament_data()
{
	if (App.tournamentId == -1)
		return 1;
	char response[RESPONSE_SIZE];
	char body[80];
	snprintf(body, sizeof(body), TOURNAMENT_GET_DATA"?tournament_id=%d", App.tournamentId);
	int res = do_curl_get(body, response, 1, 0);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	App.TournamentState.status = JSON_GET(data, "status", JSON_INT);
	App.TournamentState.winner = JSON_GET(data, "winner", JSON_INT);
	App.TournamentState.game_3_id = JSON_GET(data, "game_3_id", JSON_INT);
	App.TournamentState.g3_lside = JSON_GET(data, "game_3_lside", JSON_INT);
	return 0;
}

int get_tournament_owner()
{
	char response[RESPONSE_SIZE];
	char body[80];
	snprintf(body, sizeof(body), TOURNAMENT_GET_OWNER"?tournament_id=%d", App.tournamentId);
	int res = do_curl_get(body, response, 1, 0);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	return JSON_GET(data, "id", JSON_INT);
}
