/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ammomen <ammomen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 00:33:56 by marvin            #+#    #+#             */
/*   Updated: 2026/10/28 03:09:18 by ammomen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	t_bytes;
	char	*p;

	if (!s1 || !s2)
    	return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	t_bytes = len1 + len2 + 1;
	p = malloc(t_bytes);
	if (!p)
		return (NULL);
	ft_memcpy(p, s1, len1);
	ft_memcpy(p + len1, s2, len2);
	p[t_bytes - 1] = '\0';
	return (p);
}
