/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:38:02 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/25 22:40:45 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <curl/curl.h>

int		get_game_data();
char	*itoa(int);
int		post_input(int);
int		set_ball(double, double, int);
int		register_user(char *username, char *email, char *password);
int		login_user(char *email, char *password);
int		create_game();
int		start_game();
