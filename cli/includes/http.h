/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:38:02 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/03 14:30:40 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <curl/curl.h>

int		get_game_data();
char	*itoa(int);
int		post_input(int);
int		set_ball(double, double, int);
int		register_user(char *, char *, char *);
int		login_user(char *, char *, char *);
int		create_game();
int		start_game();
int		join_game(int);
int		get_secret(char *username, char *password);
int		get_owner();
const char *get_jwt_header();

