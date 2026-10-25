/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ammomen <ammomen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 01:13:18 by marvin            #+#    #+#             */
/*   Updated: 2026/10/25 09:25:59 by ammomen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_digits(int n)
{
	size_t	count;
	long	num;

	count = 0;
	num = n;
	if (num < 0)
	{
		count = 1;
		num = -num;
	}
	if (num == 0)
		return (1);
	while (num != 0)
	{
		num /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long	num;
	size_t	i;
	char	*p;

	i = count_digits(n);
	p = malloc(i + 1);
	if (!p)
		return (NULL);
	p[i] = '\0';
	num = n;
	if (num < 0)
		num = -num;
	if (n == 0)
		p[0] = '0';
	while (num > 0)
	{
		i--;
		p[i] = (num % 10) + '0';
		num /= 10;
	}
	if (n < 0)
		p[0] = '-';
	return (p);
}
