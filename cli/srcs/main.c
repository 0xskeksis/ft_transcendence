/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:48:19 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/01 16:11:48 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <http.h>
#include <Application.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <defines.h>
#include <graphic.h>
#include <stdlib.h>
#include <unistd.h>
#include <gestion.h>

void close_app() 
{
	delete_app();
	_Exit(0);
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	prohibit_sig();
	create_app();
	/*
	App.gameId = atoi(av[1]);
	App.side = atoi(av[2]);
	init_gtk(ac, av);
	*/
	user_menu();
}

/*
int main(UNUSED int ac, char **av)
{
	create_app();
	if (*av[1] == 'a')
		register_user(av[2], av[3], av[4]);
	else if (*av[1] == 'b')
		login_user(av[2], av[3]);
	return 0;
}
*/
