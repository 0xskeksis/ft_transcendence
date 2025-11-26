/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Application.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:44:25 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/25 21:56:10 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <curl/curl.h>
#include <gtk/gtk.h>

typedef struct
{
	CURL *curl;
	GtkApplication *gtk;
	struct
	{
		bool up;
		bool down;
	}	*Inputs;
	struct
	{
		int		score;
		float	lpos;
		float	rpos;
		float	bposx;
		float	bposy;
	}	GameState;
	int gameId;
} t_App;

extern t_App App;

void delete_app();
void close_app();
void create_app();
