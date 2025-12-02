/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:44:25 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/02 14:38:42 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <curl/curl.h>
#include <gtk/gtk.h>
#include <stdbool.h>

typedef struct
{
	CURL *curl;
	GtkApplication *gtk;
	char *name;
	struct
	{
		bool up;
		bool down;
	}	Inputs;
	struct
	{
		int		score;
		float	lpos;
		float	rpos;
		float	bposx;
		float	bposy;
	}	GameState;
	struct
	{
		char *jwt;
		int	id;
		char *username;
		char *password;
	}	UserInfo;
	int gameId;
	int side;
} t_App;

extern t_App App;

void delete_app();
void close_app() __attribute__((__noreturn__)) ;
void create_app();
void prohibit_sig();
