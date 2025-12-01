/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 15:55:35 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/30 16:20:51 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <signal.h>
#include <Application.h>
#include <defines.h>

void handler(UNUSED int sig, UNUSED siginfo_t *info, UNUSED void *ucontext) {};
void prohibit_sig()
{
	struct sigaction sa = {0};

	sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
