/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:25:51 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/12 18:58:27 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "GameState.h"
#include <defines.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
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
	curl_easy_reset(curl);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_USERAGENT, CURL_USER_AGENT);
	CURL_OPT(CURLOPT_WRITEFUNCTION, write_callback);
	CURL_OPT(CURLOPT_WRITEDATA, response);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "%s\n", curl_easy_strerror(res));
		return 2;
	}
	return 0;
}

static int do_curl_post(char *url, char *data)
{
	curl_easy_reset(curl);
	CURL_OPT(CURLOPT_URL, url);
	CURL_OPT(CURLOPT_POST, 1L);
	CURL_OPT(CURLOPT_POSTFIELDS, data);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "%s\n", curl_easy_strerror(res));
		return 2;
	}
	return 0;
}

int get_game_data(GameState *gameState)
{
	char response[RESPONSE_SIZE];

	int res = do_curl_get(PONG_GET, response);
	if (res)
		return 1;

	const cJSON *data = cJSON_Parse(response);
	gameState->score = JSON_GET(data, "score", JSON_INT);
	gameState->lpos = JSON_GET(data, "lpos", JSON_DOUBLE);
	gameState->rpos = JSON_GET(data, "rpos", JSON_DOUBLE);
	gameState->bposx = JSON_GET(data, "bposx", JSON_DOUBLE);
	gameState->bposy = JSON_GET(data, "bposy", JSON_DOUBLE);
	printf("%d\n", gameState->score);
	printf("%f\n", gameState->lpos);
	printf("%f\n", gameState->rpos);
	printf("%f\n", gameState->bposx);
	printf("%f\n", gameState->bposy);
	return 0;
}

int post_input(int inputs)
{
	int res = do_curl_post(PONG_POST, itoa(inputs));
	if (res)
		return 1;
	return 0;
}
