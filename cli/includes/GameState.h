/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameState.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 19:59:34 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/12 18:55:47 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <curl/curl.h>

typedef struct
{
	int		score;
	float	lpos;
	float	rpos;
	float	bposx;
	float	bposy;
}	GameState;

int get_game_data(GameState *);
char	*itoa(int);
int post_input(int);

extern CURL *curl;
