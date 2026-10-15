/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ammomen <ammomen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 11:48:43 by marvin            #+#    #+#             */
/*   Updated: 2026/10/15 11:56:21 by ammomen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>

int	main(void)
{
	// tests for ft_isalpha
	printf("tests for ft_isalpha -->\n");
	printf("A: %d\n", ft_isalpha('A'));
	printf("1: %d\n", ft_isalpha('1'));
	printf("a: %d\n", ft_isalpha('a'));
	printf("\n");

	// tests for ft_isdigit
	printf("tests for ft_isdigit -->\n");
	printf("0: %d\n", ft_isdigit('0'));
	printf("9: %d\n", ft_isdigit('9'));
	printf("/: %d\n", ft_isdigit('/'));
	printf(":: %d\n", ft_isdigit(':'));
	printf("\n");

	// tests for ft_isalnum
	printf("tests for ft_isalnum -->\n");
	printf("0: %d\n", ft_isalnum('0'));
	printf("9: %d\n", ft_isalnum('9'));
	printf("a: %d\n", ft_isalnum('a'));
	printf(":: %d\n", ft_isalnum('@'));
	printf("\n");

	// tests for ft_isascii
	printf("tests for ft_isascii -->\n");
	printf("0: %d\n", ft_isascii(0));
	printf("65: %d\n", ft_isascii(65));
	printf("127: %d\n", ft_isascii(127));
	printf("-1: %d\n", ft_isascii(-1));
	printf("128: %d\n", ft_isascii(128));
	printf("\n");
}
