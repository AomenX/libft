/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ammomen <ammomen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 22:07:40 by marvin            #+#    #+#             */
/*   Updated: 2026/10/25 20:19:59 by ammomen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

static void	free_split(char **words)
{
	size_t	i;

	if (!words)
		return ;
	i = 0;
	while (words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
}

static void	test_itoa_case(char *label, int n, char *expected)
{
	char	*p;

	p = ft_itoa(n);
	check_int(label, p && strcmp(p, expected) == 0, 1);
	free(p);
}

static void	test_putchar_case(char *label, char c)
{
	int		fd[2];
	char	result;
	ssize_t	n;

	if (pipe(fd) == -1)
	{
		check_int(label, 0, 1);
		return ;
	}
	ft_putchar_fd(c, fd[1]);
	close(fd[1]);
	n = read(fd[0], &result, 1);
	close(fd[0]);
	check_int(label, n == 1 && result == c, 1);
}

static void	test_putstr_case(char *label, char *s, char *expected)
{
	int		fd[2];
	char	buffer[128];
	ssize_t	n;
	size_t	len;

	if (pipe(fd) == -1)
	{
		check_int(label, 0, 1);
		return ;
	}
	ft_putstr_fd(s, fd[1]);
	close(fd[1]);
	len = strlen(expected);
	n = read(fd[0], buffer, sizeof(buffer));
	close(fd[0]);
	check_int(label, n == (ssize_t)len
		&& memcmp(buffer, expected, len) == 0, 1);
}

static void	test_putendl_case(char *label, char *s, char *expected)
{
	int		fd[2];
	char	buffer[128];
	ssize_t	n;
	size_t	len;

	if (pipe(fd) == -1)
	{
		check_int(label, 0, 1);
		return ;
	}
	ft_putendl_fd(s, fd[1]);
	close(fd[1]);
	len = strlen(expected);
	n = read(fd[0], buffer, sizeof(buffer));
	close(fd[0]);
	check_int(label, n == (ssize_t)len
		&& memcmp(buffer, expected, len) == 0, 1);
}

static void	test_putnbr_case(char *label, int value, char *expected)
{
	int		fd[2];
	char	buffer[64];
	ssize_t	n;
	size_t	len;

	if (pipe(fd) == -1)
	{
		check_int(label, 0, 1);
		return ;
	}
	ft_putnbr_fd(value, fd[1]);
	close(fd[1]);
	len = strlen(expected);
	n = read(fd[0], buffer, sizeof(buffer));
	close(fd[0]);
	check_int(label, n == (ssize_t)len
		&& memcmp(buffer, expected, len) == 0, 1);
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

static void	test_strlcat(void)
{
	char	dst1[8] = "Hi";
	char	dst2[5] = "Hi";
	char	dst3[10] = "Hello";
	char	dst4[5] = "";
	char	dst5[10] = "Hi";
	char	dst6[5] = "Hi";

	section("ft_strlcat");

	check_size("Enough space",
		ft_strlcat(dst1, "1337", 8), 6);
	check_int("Enough space content", strcmp(dst1, "Hi1337"), 0);

	check_size("Truncation",
		ft_strlcat(dst2, "1337", 5), 6);
	check_int("Truncation content", strcmp(dst2, "Hi13"), 0);

	check_size("Size smaller than destination",
		ft_strlcat(dst3, "1337", 2), 6);
	check_int("Destination unchanged", strcmp(dst3, "Hello"), 0);

	check_size("Empty destination",
		ft_strlcat(dst4, "abc", 5), 3);
	check_int("Empty destination content", strcmp(dst4, "abc"), 0);

	check_size("Empty source",
		ft_strlcat(dst5, "", 10), 2);
	check_int("Empty source content", strcmp(dst5, "Hi"), 0);

	check_size("Zero size",
		ft_strlcat(dst6, "abc", 0), 3);
	check_int("Zero size leaves destination unchanged",
		strcmp(dst6, "Hi"), 0);
}

static void	test_strnstr(void)
{
	char	big[] = "Hello World";
	char	banana[] = "banana";
	char	empty[] = "";

	section("ft_strnstr");

	check_ptr("Substring found",
		ft_strnstr(big, "World", 11), &big[6]);
	check_ptr("Substring outside limit",
		ft_strnstr(big, "World", 10), NULL);
	check_ptr("First occurrence",
		ft_strnstr(banana, "ana", 6), &banana[1]);
	check_ptr("Substring not found",
		ft_strnstr(big, "xyz", 11), NULL);
	check_ptr("Empty needle",
		ft_strnstr(big, "", 11), big);
	check_ptr("Empty needle with zero limit",
		ft_strnstr(big, "", 0), big);
	check_ptr("Zero search limit",
		ft_strnstr(big, "H", 0), NULL);
	check_ptr("Exact limit",
		ft_strnstr(big, "Hello", 5), big);
	check_ptr("Match exceeds limit",
		ft_strnstr(big, "Hello", 4), NULL);
	check_ptr("Empty haystack",
		ft_strnstr(empty, "a", 5), NULL);
	check_ptr("Both strings empty",
		ft_strnstr(empty, "", 0), empty);
}

static void	test_memset(void)
{
	char			str[6] = "Hello";
	char			zero[6] = "Hello";
	char			full[6] = "Hello";
	unsigned char	bytes[4] = {1, 2, 3, 4};

	section("ft_memset");

	check_ptr("Returns original pointer",
		ft_memset(str, 'X', 3), str);
	check_int("First byte", str[0], 'X');
	check_int("Second byte", str[1], 'X');
	check_int("Third byte", str[2], 'X');
	check_int("Unmodified fourth byte", str[3], 'l');
	check_int("Null terminator preserved", str[5], '\0');

	ft_memset(zero, 'X', 0);
	check_int("Zero length leaves data unchanged",
		strcmp(zero, "Hello"), 0);

	ft_memset(full, 'A', sizeof(full));
	check_int("Entire buffer filled", full[5], 'A');

	ft_memset(bytes, 256 + 65, 2);
	check_int("Integer converted to unsigned char", bytes[0], 65);
	check_int("Second byte converted", bytes[1], 65);
	check_int("Remaining byte unchanged", bytes[2], 3);
}

static void	test_bzero(void)
{
	char	str[] = "Hello";
	char	unchanged[] = "Hello";

	section("ft_bzero");

	ft_bzero(str, 2);
	check_int("First byte zero", str[0], '\0');
	check_int("Second byte zero", str[1], '\0');
	check_int("Third byte unchanged", str[2], 'l');

	ft_bzero(unchanged, 0);
	check_int("Zero size leaves buffer unchanged",
		strcmp(unchanged, "Hello"), 0);
}

static void	test_memcpy(void)
{
	char	src[] = {'A', '\0', 'B', 'C'};
	char	dst[4] = {0};
	char	unchanged[] = "Hello";

	section("ft_memcpy");

	check_ptr("Returns destination",
		ft_memcpy(dst, src, 4), dst);
	check_int("First byte", dst[0], 'A');
	check_int("Embedded null byte", dst[1], '\0');
	check_int("Third byte", dst[2], 'B');
	check_int("Fourth byte", dst[3], 'C');

	ft_memcpy(unchanged, "XXXXX", 0);
	check_int("Zero bytes leaves buffer unchanged",
		strcmp(unchanged, "Hello"), 0);
}


static void	test_memmove(void)
{
	char	right[] = "ABCDE";
	char	left[] = "ABCDE";
	char	normal[6] = {0};
	char	unchanged[] = "Hello";
	char	bytes[] = {'A', '\0', 'B', 'C'};

	section("ft_memmove");

	check_ptr("Returns destination",
		ft_memmove(normal, "Hello", 6), normal);
	check_int("Non-overlapping copy",
		strcmp(normal, "Hello"), 0);

	ft_memmove(right + 1, right, 4);
	check_int("Overlap to the right",
		strcmp(right, "AABCD"), 0);

	ft_memmove(left, left + 1, 4);
	check_int("Overlap to the left",
		strcmp(left, "BCDEE"), 0);

	ft_memmove(unchanged, unchanged, 5);
	check_int("Source equals destination",
		strcmp(unchanged, "Hello"), 0);

	ft_memmove(unchanged, "XXXXX", 0);
	check_int("Zero bytes copied",
		strcmp(unchanged, "Hello"), 0);

	ft_memmove(bytes + 1, bytes, 3);
	check_int("Embedded null, first byte", bytes[1], 'A');
	check_int("Embedded null, second byte", bytes[2], '\0');
	check_int("Embedded null, third byte", bytes[3], 'B');
}

static void	test_memchr(void)
{
	char	data[] = {'A', 'B', '\0', 'C', 'D'};
	char	str[] = "Hello";

	section("ft_memchr");
	check_ptr("First byte", ft_memchr(str, 'H', 5), &str[0]);
	check_ptr("Middle byte", ft_memchr(str, 'l', 5), &str[2]);
	check_ptr("Missing byte", ft_memchr(str, 'z', 5), NULL);
	check_ptr("Zero length", ft_memchr(str, 'H', 0), NULL);
	check_ptr("Find null byte", ft_memchr(data, '\0', 5), &data[2]);
	check_ptr("Search after null", ft_memchr(data, 'C', 5), &data[3]);
	check_ptr("Respect limit", ft_memchr(data, 'C', 3), NULL);
	check_ptr("Integer conversion", ft_memchr(str, 328, 5), &str[0]);
}

static void	test_memcmp(void)
{
	char	a[] = {'A', '\0', 'B'};
	char	b[] = {'A', '\0', 'C'};
	unsigned char	high[] = {255};
	unsigned char	low[] = {1};

	section("ft_memcmp");
	check_int("Equal first two bytes", ft_memcmp(a, b, 2), 0);
	check_int("Different third byte", ft_memcmp(a, b, 3), -1);
	check_int("Reverse comparison", ft_memcmp(b, a, 3), 1);
	check_int("Equal buffers", ft_memcmp(a, a, 3), 0);
	check_int("Zero length", ft_memcmp(a, b, 0), 0);
	check_int("Unsigned comparison", ft_memcmp(high, low, 1), 254);
}

static void	test_atoi(void)
{
	section("ft_atoi");

	check_int("Positive number", ft_atoi("42"), 42);
	check_int("Negative number", ft_atoi("-42"), -42);
	check_int("Explicit plus", ft_atoi("+42"), 42);
	check_int("Zero", ft_atoi("0"), 0);
	check_int("Negative zero", ft_atoi("-0"), 0);

	check_int("Leading spaces", ft_atoi("   123"), 123);
	check_int("Leading whitespace", ft_atoi(" \t\n\r\v\f-123"), -123);
	check_int("Leading zeros", ft_atoi("00042"), 42);
	check_int("Spaces and zeros", ft_atoi("  -0012"), -12);

	check_int("Trailing letters", ft_atoi("123abc"), 123);
	check_int("Trailing spaces", ft_atoi("42   "), 42);
	check_int("Letters first", ft_atoi("abc123"), 0);
	check_int("Empty string", ft_atoi(""), 0);
	check_int("Only spaces", ft_atoi("   "), 0);

	check_int("Double minus", ft_atoi("--42"), 0);
	check_int("Double plus", ft_atoi("++42"), 0);
	check_int("Plus minus", ft_atoi("+-42"), 0);
	check_int("Minus plus", ft_atoi("-+42"), 0);
	check_int("Sign only", ft_atoi("-"), 0);

	check_int("INT_MAX", ft_atoi("2147483647"), 2147483647);
	check_int("INT_MIN", ft_atoi("-2147483648"), (-2147483648));
}


static void	test_calloc(void)
{
	unsigned char	*p;
	size_t			i;
	int				all_zero;
	void			*zero_ptr;

	section("ft_calloc");
	p = ft_calloc(10, sizeof(unsigned char));
	check_int("Allocation succeeds", p != NULL, 1);
	if (p != NULL)
	{
		all_zero = 1;
		i = 0;
		while (i < 10)
		{
			if (p[i] != 0)
				all_zero = 0;
			i++;
		}
		check_int("All bytes initialized to zero", all_zero, 1);
		free(p);
	}
	check_ptr("Multiplication overflow",
		ft_calloc((size_t)-1, 2), NULL);
	zero_ptr = ft_calloc(0, 10);
	free(zero_ptr);
	check_int("Zero count handled safely", 1, 1);
}

static void	test_strdup(void)
{
	char	*copy;
	char	*empty;
	char	original[] = "Hello";
	char	*independent;

	section("ft_strdup");
	copy = ft_strdup("1337 School");
	check_int("Allocation succeeds", copy != NULL, 1);
	if (copy)
	{
		check_int("Correct content", strcmp(copy, "1337 School"), 0);
		free(copy);
	}
	empty = ft_strdup("");
	check_int("Empty string allocation", empty != NULL, 1);
	if (empty)
	{
		check_int("Empty string content", strcmp(empty, ""), 0);
		free(empty);
	}
	independent = ft_strdup(original);
	check_int("Independent allocation", independent != original, 1);
	if (independent)
	{
		original[0] = 'X';
		check_int("Original modification does not affect copy",
			strcmp(independent, "Hello"), 0);
		free(independent);
	}
}


static void	test_substr(void)
{
	char	*p;
	char	original[] = "Hello";

	section("ft_substr");
	p = ft_substr("Hello World", 6, 5);
	check_int("Normal substring", p && strcmp(p, "World") == 0, 1);
	free(p);

	p = ft_substr("Hello", 2, 100);
	check_int("Length exceeds remaining", p && strcmp(p, "llo") == 0, 1);
	free(p);

	p = ft_substr("Hello", 10, 3);
	check_int("Start beyond string", p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_substr("Hello", 5, 3);
	check_int("Start at terminator", p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_substr("Hello", 1, 0);
	check_int("Zero length", p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_substr("", 0, 5);
	check_int("Empty source", p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_substr(original, 1, 3);
	check_int("Expected content", p && strcmp(p, "ell") == 0, 1);
	check_int("Independent allocation", p && p != original + 1, 1);
	free(p);
}

static void	test_strjoin(void)
{
	char	*p;
	char	s1[] = "Hello";

	section("ft_strjoin");
	p = ft_strjoin("Hello ", "World");
	check_int("Normal join", p && strcmp(p, "Hello World") == 0, 1);
	free(p);

	p = ft_strjoin("", "World");
	check_int("Empty first string", p && strcmp(p, "World") == 0, 1);
	free(p);

	p = ft_strjoin("Hello", "");
	check_int("Empty second string", p && strcmp(p, "Hello") == 0, 1);
	free(p);

	p = ft_strjoin("", "");
	check_int("Both strings empty", p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_strjoin(s1, " World");
	check_int("Independent allocation", p && p != s1, 1);
	if (p)
	{
		s1[0] = 'X';
		check_int("Copy unchanged after source modification",
			strcmp(p, "Hello World"), 0);
	}
	free(p);
}

static void	test_strtrim(void)
{
	char	*p;

	section("ft_strtrim");
	p = ft_strtrim("***1337***", "*");
	check_int("Trim both ends", p && strcmp(p, "1337") == 0, 1);
	free(p);

	p = ft_strtrim("!?Hello?!", "!?");
	check_int("Multiple trim characters", p && strcmp(p, "Hello") == 0, 1);
	free(p);

	p = ft_strtrim("xxHelxloxx", "x");
	check_int("Preserve middle characters",
		p && strcmp(p, "Helxlo") == 0, 1);
	free(p);

	p = ft_strtrim("Hello", "x");
	check_int("Nothing to trim", p && strcmp(p, "Hello") == 0, 1);
	free(p);

	p = ft_strtrim("xxxx", "x");
	check_int("Everything trimmed", p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_strtrim("", "x");
	check_int("Empty source", p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_strtrim("Hello", "");
	check_int("Empty set", p && strcmp(p, "Hello") == 0, 1);
	free(p);

	p = ft_strtrim("  Hello  ", " ");
	check_int("Trim spaces", p && strcmp(p, "Hello") == 0, 1);
	free(p);
}

static void	test_split(void)
{
	char	**w;

	section("ft_split");

	w = ft_split("Hello,World,1337", ',');
	check_int("Normal split", w && w[0] && w[1] && w[2]
		&& !strcmp(w[0], "Hello") && !strcmp(w[1], "World")
		&& !strcmp(w[2], "1337") && w[3] == NULL, 1);
	free_split(w);

	w = ft_split(",,Hello,,World,,", ',');
	check_int("Consecutive delimiters", w && w[0] && w[1]
		&& !strcmp(w[0], "Hello") && !strcmp(w[1], "World")
		&& w[2] == NULL, 1);
	free_split(w);

	w = ft_split("", ',');
	check_int("Empty string", w && w[0] == NULL, 1);
	free_split(w);

	w = ft_split(",,,,", ',');
	check_int("Only delimiters", w && w[0] == NULL, 1);
	free_split(w);

	w = ft_split("Hello", ',');
	check_int("No delimiter", w && w[0]
		&& !strcmp(w[0], "Hello") && w[1] == NULL, 1);
	free_split(w);

	w = ft_split("a b c", ' ');
	check_int("Split by spaces", w && w[0] && w[1] && w[2]
		&& !strcmp(w[0], "a") && !strcmp(w[1], "b")
		&& !strcmp(w[2], "c") && w[3] == NULL, 1);
	free_split(w);

	w = ft_split("abc", '\0');
	check_int("Null delimiter", w && w[0]
		&& !strcmp(w[0], "abc") && w[1] == NULL, 1);
	free_split(w);

	w = ft_split(",a,", ',');
	check_int("Leading and trailing delimiters", w && w[0]
		&& !strcmp(w[0], "a") && w[1] == NULL, 1);
	free_split(w);

	w = ft_split("x", 'x');
	check_int("Single delimiter", w && w[0] == NULL, 1);
	free_split(w);

	w = ft_split("x", ',');
	check_int("Single character word", w && w[0]
		&& !strcmp(w[0], "x") && w[1] == NULL, 1);
	free_split(w);
}

static void	test_itoa(void)
{
	section("ft_itoa");
	test_itoa_case("Zero", 0, "0");
	test_itoa_case("Positive single digit", 7, "7");
	test_itoa_case("Negative single digit", -7, "-7");
	test_itoa_case("Positive number", 1234, "1234");
	test_itoa_case("Negative number", -1234, "-1234");
	test_itoa_case("Multiple zeros", 100200, "100200");
	test_itoa_case("INT_MAX", 2147483647, "2147483647");
	test_itoa_case("INT_MIN", (-2147483647 - 1), "-2147483648");
}


static char	map_upper(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static char	map_alternate(unsigned int i, char c)
{
	if (i % 2 == 0 && c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static char	map_identity(unsigned int i, char c)
{
	(void)i;
	return (c);
}

static void	test_strmapi(void)
{
	char	*p;
	char	original[] = "hello";

	section("ft_strmapi");
	p = ft_strmapi(original, map_upper);
	check_int("Uppercase transform",
		p && strcmp(p, "HELLO") == 0, 1);
	check_int("Original unchanged", strcmp(original, "hello"), 0);
	free(p);

	p = ft_strmapi("hello", map_alternate);
	check_int("Index-dependent transform",
		p && strcmp(p, "HeLlO") == 0, 1);
	free(p);

	p = ft_strmapi("", map_identity);
	check_int("Empty string",
		p && strcmp(p, "") == 0, 1);
	free(p);

	p = ft_strmapi("123", map_identity);
	check_int("Identity transform",
		p && strcmp(p, "123") == 0, 1);
	free(p);

	check_ptr("NULL callback", ft_strmapi("abc", NULL), NULL);
	check_ptr("NULL source", ft_strmapi(NULL, map_upper), NULL);
}

static void	iter_upper(unsigned int i, char *c)
{
	(void)i;
	if (*c >= 'a' && *c <= 'z')
		*c -= 32;
}

static void	iter_index(unsigned int i, char *c)
{
	if (i % 2 == 0)
		*c = '*';
}

static void	test_putchar_fd(void)
{
	section("ft_putchar_fd");
	test_putchar_case("Uppercase A", 'A');
	test_putchar_case("Lowercase z", 'z');
	test_putchar_case("Digit 5", '5');
	test_putchar_case("Space", ' ');
	test_putchar_case("Newline", '\n');
	test_putchar_case("Null byte", '\0');
	test_putchar_case("High byte", (char)255);
}

static void	test_striteri(void)
{
	char	s1[] = "hello";
	char	s2[] = "abcdef";
	char	empty[] = "";

	section("ft_striteri");
	ft_striteri(s1, iter_upper);
	check_int("Uppercase in place", strcmp(s1, "HELLO"), 0);
	ft_striteri(s2, iter_index);
	check_int("Index-based modification",
		strcmp(s2, "*b*d*f"), 0);
	ft_striteri(empty, iter_upper);
	check_int("Empty string", strcmp(empty, ""), 0);
	ft_striteri(NULL, iter_upper);
	ft_striteri(s1, NULL);
}

static void	test_putstr_fd(void)
{
	section("ft_putstr_fd");
	test_putstr_case("Normal string", "Hello", "Hello");
	test_putstr_case("Empty string", "", "");
	test_putstr_case("Numbers", "1337", "1337");
	test_putstr_case("Spaces", "Hello World", "Hello World");
	test_putstr_case("Newline", "Hello\nWorld", "Hello\nWorld");
	test_putstr_case("Special characters", "!@#$%^&*", "!@#$%^&*");
}

static void	test_putendl_fd(void)
{
	section("ft_putendl_fd");
	test_putendl_case("Normal string", "Hello", "Hello\n");
	test_putendl_case("Empty string", "", "\n");
	test_putendl_case("Numbers", "1337", "1337\n");
	test_putendl_case("Spaces", "Hello World", "Hello World\n");
	test_putendl_case("Existing newline", "Hello\n", "Hello\n\n");
	test_putendl_case("Special characters", "!@#", "!@#\n");
}

static void	test_putnbr_fd(void)
{
	section("ft_putnbr_fd");
	test_putnbr_case("Zero", 0, "0");
	test_putnbr_case("Positive single digit", 7, "7");
	test_putnbr_case("Negative single digit", -7, "-7");
	test_putnbr_case("Positive number", 1337, "1337");
	test_putnbr_case("Negative number", -42, "-42");
	test_putnbr_case("Multiple zeros", 100200, "100200");
	test_putnbr_case("INT_MAX", 2147483647, "2147483647");
	test_putnbr_case("INT_MIN", (-2147483647 - 1), "-2147483648");
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
	test_strlcat();
	test_strnstr();
	test_bzero();
	test_memset();
	test_memcpy();
	test_memmove();
	test_memchr();
	test_memcmp();
	test_atoi();
	test_calloc();
	test_strdup();
	test_substr();
	test_strjoin();
	test_strtrim();
	test_split();
	test_itoa();
	test_strmapi();
	test_striteri();
	test_putchar_fd();
	test_putstr_fd();
	test_putendl_fd();
	test_putnbr_fd();

	printf("\n====================================\n");
	printf("RESULTS: %d passed, %d failed\n", g_passed, g_failed);
	printf("====================================\n");

	return (g_failed != 0);
}
