/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:31:29 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/12 18:06:51 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#define RESPONSE_SIZE				512
#define API_BASE					"http://localhost:3000/"
#define PONG						API_BASE "pong"
#define PONG_GET					PONG "/get"
#define PONG_POST					PONG "/post"
#define CURL_USER_AGENT				"libcurl-agent/1.0"

#define CURL_OPT(opt, param)		curl_easy_setopt(curl, opt, param)

#define JSON_INT					valueint
#define JSON_DOUBLE					valuedouble
#define JSON_GET(from, field, ...)	cJSON_GetObjectItemCaseSensitive(from, field)__VA_OPT__(->__VA_ARGS__)
