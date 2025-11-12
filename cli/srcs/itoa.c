/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 18:15:46 by ellanglo          #+#    #+#             */
/*   Updated: 2025/11/12 18:17:01 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <GameState.h>
#include <string.h>
#include <stdlib.h>

static int	ft_digit_count(int n)
{
	int	digit;

	digit = 0;
	if (!n)
		return (0);
	while (n)
	{
		n /= 10;
		digit++;
	}
	return (digit);
}

char	*itoa(int n)
{
	char	*nb_str;
	int		len;

	if (n == -2147483648)
		return (strdup("-2147483648"));
	if (!n)
		return (strdup("0"));
	len = ft_digit_count(n) + (n < 0);
	nb_str = calloc(len + 1, sizeof(char));
	if (!nb_str)
		return (NULL);
	if (n < 0)
	{
		*nb_str = '-';
		n = -n;
	}
	while (n)
	{
		nb_str[--len] = (n % 10) + '0';
		n /= 10;
	}
	return (nb_str);
}
