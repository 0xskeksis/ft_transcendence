/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:31:29 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/26 18:41:00 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define RESPONSE_SIZE				512
#define SERVER_BASE					"http://10.11.4.2:"
#define LOGIN_PORT					3000
#define API_PORT					8000
#define PONG						SERVER_BASE STR(API_PORT) "/pong"
#define PONG_GET					PONG "/get-data"
#define PONG_POST					PONG "/post-input"
#define PONG_CREATE					PONG "/create-game"
#define PONG_START					PONG "/start-game"
#define PONG_DEBUG					PONG "/debug"
#define PONG_SET_BALL				PONG_DEBUG "/set-ball"
#define REGISTER					SERVER_BASE STR(LOGIN_PORT) "/register"
#define LOGIN						SERVER_BASE STR(LOGIN_PORT) "/login"
#define CURL_USER_AGENT				"libcurl-agent/1.0"

#define CURL_OPT(opt, param)		curl_easy_setopt(App.curl, opt, param)

#define JSON_INT					valueint
#define JSON_DOUBLE					valuedouble
#define JSON_GET(from, field, ...)	cJSON_GetObjectItemCaseSensitive(from, field)__VA_OPT__(->__VA_ARGS__)

#define UNUSED						__attribute__((unused))

#define DESIGN_W					1920.0
#define DESIGN_H					1080.0
#define PADDLE_W_N					(10.0 / DESIGN_W)
#define PADDLE_H_N					(90.0 / DESIGN_H)
#define BALL_RADIUS_N				(12.0 / DESIGN_H)
#define L_PAD_OFFSET_N				(20.0 / DESIGN_W)
#define R_PAD_OFFSET_N				(20.0 / DESIGN_W)
#define PX_W(ratio, width)			((int)((ratio) * (width)  + 0.5))
#define PX_H(ratio, height)			((int)((ratio) * (height) + 0.5))
#define PX_MIN(ratio, w, h)			((int)((ratio) * (((w)<(h))?(w):(h)) + 0.5))
