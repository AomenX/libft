/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ammomen <ammomen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 22:07:40 by marvin            #+#    #+#             */
/*   Updated: 2026/10/17 18:31:33 by ammomen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"
#include <stdio.h>
#include <string.h>

static int	g_passed = 0;
static int	g_failed = 0;

static void	check_int(const char *name, int actual, int expected)
{
	if (actual == expected)
	{
		printf("  [PASS] %s\n", name);
		g_passed++;
	}
	else
	{
		printf("  [FAIL] %s (expected %d, got %d)\n",
			name, expected, actual);
		g_failed++;
	}
}

static void	check_size(const char *name, size_t actual, size_t expected)
{
	if (actual == expected)
	{
		printf("  [PASS] %s\n", name);
		g_passed++;
	}
	else
	{
		printf("  [FAIL] %s (expected %zu, got %zu)\n",
			name, expected, actual);
		g_failed++;
	}
}

static void	check_ptr(const char *name, const void *actual,
			const void *expected)
{
	if (actual == expected)
	{
		printf("  [PASS] %s\n", name);
		g_passed++;
	}
	else
	{
		printf("  [FAIL] %s (expected %p, got %p)\n",
			name, expected, actual);
		g_failed++;
	}
}

static void	section(const char *name)
{
	printf("\n=== %s ===\n", name);
}

static void	test_isalpha(void)
{
	section("ft_isalpha");
	check_int("Uppercase A", ft_isalpha('A'), 1);
	check_int("Uppercase Z", ft_isalpha('Z'), 1);
	check_int("Lowercase a", ft_isalpha('a'), 1);
	check_int("Lowercase z", ft_isalpha('z'), 1);
	check_int("Digit 1", ft_isalpha('1'), 0);
	check_int("Before A", ft_isalpha('@'), 0);
	check_int("After Z", ft_isalpha('['), 0);
	check_int("Space", ft_isalpha(' '), 0);
}

static void	test_isdigit(void)
{
	section("ft_isdigit");
	check_int("Digit 0", ft_isdigit('0'), 1);
	check_int("Digit 5", ft_isdigit('5'), 1);
	check_int("Digit 9", ft_isdigit('9'), 1);
	check_int("Before 0", ft_isdigit('/'), 0);
	check_int("After 9", ft_isdigit(':'), 0);
	check_int("Letter A", ft_isdigit('A'), 0);
}

static void	test_isalnum(void)
{
	section("ft_isalnum");
	check_int("Uppercase A", ft_isalnum('A'), 1);
	check_int("Lowercase z", ft_isalnum('z'), 1);
	check_int("Digit 0", ft_isalnum('0'), 1);
	check_int("Digit 9", ft_isalnum('9'), 1);
	check_int("Symbol @", ft_isalnum('@'), 0);
	check_int("Space", ft_isalnum(' '), 0);
	check_int("Newline", ft_isalnum('\n'), 0);
}

static void	test_isascii(void)
{
	section("ft_isascii");
	check_int("Minimum 0", ft_isascii(0), 1);
	check_int("ASCII 65", ft_isascii(65), 1);
	check_int("Maximum 127", ft_isascii(127), 1);
	check_int("Below range -1", ft_isascii(-1), 0);
	check_int("Above range 128", ft_isascii(128), 0);
	check_int("Large value 256", ft_isascii(256), 0);
}

static void	test_isprint(void)
{
	section("ft_isprint");
	check_int("Below printable range 31", ft_isprint(31), 0);
	check_int("Space 32", ft_isprint(32), 1);
	check_int("Letter a", ft_isprint('a'), 1);
	check_int("Symbol @", ft_isprint('@'), 1);
	check_int("Last printable 126", ft_isprint(126), 1);
	check_int("DEL 127", ft_isprint(127), 0);
	check_int("Newline", ft_isprint('\n'), 0);
	check_int("Tab", ft_isprint('\t'), 0);
}

static void	test_toupper(void)
{
	section("ft_toupper");
	check_int("Lowercase a", ft_toupper('a'), 'A');
	check_int("Lowercase b", ft_toupper('b'), 'B');
	check_int("Lowercase z", ft_toupper('z'), 'Z');
	check_int("Already uppercase A", ft_toupper('A'), 'A');
	check_int("Already uppercase Z", ft_toupper('Z'), 'Z');
	check_int("Space unchanged", ft_toupper(' '), ' ');
	check_int("Symbol unchanged", ft_toupper('!'), '!');
	check_int("After lowercase range", ft_toupper('{'), '{');
}

static void	test_tolower(void)
{
	section("ft_tolower");
	check_int("Uppercase A", ft_tolower('A'), 'a');
	check_int("Uppercase X", ft_tolower('X'), 'x');
	check_int("Uppercase Z", ft_tolower('Z'), 'z');
	check_int("Already lowercase z", ft_tolower('z'), 'z');
	check_int("Digit unchanged", ft_tolower('1'), '1');
	check_int("Symbol unchanged", ft_tolower('*'), '*');
	check_int("Before uppercase range", ft_tolower('@'), '@');
}

static void	test_strlen(void)
{
	section("ft_strlen");
	check_size("Hello", ft_strlen("Hello"), 5);
	check_size("Empty string", ft_strlen(""), 0);
	check_size("1337", ft_strlen("1337"), 4);
	check_size("Hello World", ft_strlen("Hello World"), 11);
	check_size("Newline counts", ft_strlen("Hi\n"), 3);
	check_size("Embedded null", ft_strlen("Hello\0World"), 5);
}

static void	test_strchr(void)
{
	char	str[] = "Hello";
	char	empty[] = "";
	char	repeated[] = "banana";

	section("ft_strchr");
	check_ptr("First character", ft_strchr(str, 'H'), &str[0]);
	check_ptr("Middle character", ft_strchr(str, 'l'), &str[2]);
	check_ptr("Last character", ft_strchr(str, 'o'), &str[4]);
	check_ptr("Character not found", ft_strchr(str, 'z'), NULL);
	check_ptr("Null terminator", ft_strchr(str, '\0'), &str[5]);
	check_ptr("Integer 328 becomes H", ft_strchr(str, 328), &str[0]);
	check_ptr("Integer 256 becomes null", ft_strchr(str, 256), &str[5]);
	check_ptr("Empty string, missing", ft_strchr(empty, 'a'), NULL);
	check_ptr("Empty string, terminator",
		ft_strchr(empty, '\0'), &empty[0]);
	check_ptr("First repeated character",
		ft_strchr(repeated, 'a'), &repeated[1]);
	check_ptr("Negative search -1", ft_strchr(str, -1), NULL);
}

static void	test_strrchr(void)
{
	char	str[] = "banana";
	char	empty[] = "";

	section("ft_strrchr");
	check_ptr("Last occurrence of a",
		ft_strrchr(str, 'a'), &str[5]);
	check_ptr("Last occurrence of n",
		ft_strrchr(str, 'n'), &str[4]);
	check_ptr("First character b",
		ft_strrchr(str, 'b'), &str[0]);
	check_ptr("Character not found",
		ft_strrchr(str, 'z'), NULL);
	check_ptr("Null terminator",
		ft_strrchr(str, '\0'), &str[6]);
	check_ptr("Integer 353 becomes a",
		ft_strrchr(str, 353), &str[5]);
	check_ptr("Empty string terminator",
		ft_strrchr(empty, '\0'), &empty[0]);
	check_ptr("Empty string missing character",
		ft_strrchr(empty, 'a'), NULL);
	check_ptr("Integer 256 becomes null",
		ft_strrchr(str, 256), &str[6]);
	check_ptr("Negative search -1",
		ft_strrchr(str, -1), NULL);
}

static void	test_strncmp(void)
{
	section("ft_strncmp");
	check_int("Equal strings", ft_strncmp("abc", "abc", 3), 0);
	check_int("First smaller", ft_strncmp("abc", "abd", 3), -1);
	check_int("First greater", ft_strncmp("abd", "abc", 3), 1);
	check_int("Limited comparison", ft_strncmp("abc", "abd", 2), 0);
	check_int("First shorter", ft_strncmp("abc", "abcd", 4), -100);
	check_int("First longer", ft_strncmp("abcd", "abc", 4), 100);
	check_int("Zero length", ft_strncmp("abc", "xyz", 0), 0);
	check_int("Empty strings", ft_strncmp("", "", 5), 0);
}

static void	test_strlcpy(void)
{
	char	dst1[10];
	char	dst2[4];
	char	dst3[1];
	char	dst4[5] = "ABCD";
	char	dst5[5];

	section("ft_strlcpy");

	check_size("Normal copy return",
		ft_strlcpy(dst1, "Hello", sizeof(dst1)), 5);
	check_int("Normal copy content", strcmp(dst1, "Hello"), 0);

	check_size("Truncated copy return",
		ft_strlcpy(dst2, "Hello", sizeof(dst2)), 5);
	check_int("Truncated copy content", strcmp(dst2, "Hel"), 0);

	check_size("Size 1 return",
		ft_strlcpy(dst3, "Hello", sizeof(dst3)), 5);
	check_int("Size 1 produces empty string", dst3[0], '\0');

	check_size("Size 0 return",
		ft_strlcpy(dst4, "Hello", 0), 5);
	check_int("Size 0 leaves buffer unchanged", strcmp(dst4, "ABCD"), 0);

	check_size("Empty source return",
		ft_strlcpy(dst5, "", sizeof(dst5)), 0);
	check_int("Empty source content", dst5[0], '\0');
}

int	main(void)
{
	printf("====================================\n");
	printf("          LIBFT TEST SUITE          \n");
	printf("====================================\n");

	test_isalpha();
	test_isdigit();
	test_isalnum();
	test_isascii();
	test_isprint();
	test_toupper();
	test_tolower();
	test_strlen();
	test_strchr();
	test_strrchr();
	test_strncmp();
	test_strlcpy();

	printf("\n====================================\n");
	printf("RESULTS: %d passed, %d failed\n", g_passed, g_failed);
	printf("====================================\n");

	return (g_failed != 0);
}
