/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ammomen <ammomen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 11:48:43 by marvin            #+#    #+#             */
/*   Updated: 2026/10/15 12:44:09 by ammomen          ###   ########.fr       */
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

	// tests for ft_isprint
	printf("tests for ft_isprint -->\n");
	printf("space: %d\n", ft_isprint(' '));
	printf("a: %d\n", ft_isprint('a'));
	printf("@: %d\n", ft_isprint('@'));
	printf("newline: %d\n", ft_isprint('\n'));
	printf("tab: %d\n", ft_isprint('\t'));
	printf("\n");

	// tests for ft_toupper
	printf("tests for ft_toupper -->\n");
	printf("a --> %c\n", ft_toupper('a'));
	printf("b --> %c\n", ft_toupper('b'));
	printf("z --> %c\n", ft_toupper('z'));
	printf("Z --> %c\n", ft_toupper('Z'));
	printf("{ --> %c\n", ft_toupper('{'));
	printf("! --> %c\n", ft_toupper('!'));
	printf("\n");

	// test for ft_tolower
	printf("tests for ft_tolower -->\n");
	printf("X --> %c\n", ft_tolower('X'));
	printf("Y --> %c\n", ft_tolower('Y'));
	printf("z --> %c\n", ft_tolower('z'));
	printf("1 --> %c\n", ft_tolower('1'));
	printf("* --> %c\n", ft_tolower('*'));
	printf("\n");
}

