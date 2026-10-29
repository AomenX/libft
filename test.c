/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ammomen <ammomen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 14:38:08 by marvin            #+#    #+#             */
/*   Updated: 2026/10/28 15:46:14 by ammomen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Local Libft validation only; NOT a file to submit. */
#define _DEFAULT_SOURCE 1
#include "libft.h"
#include <locale.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

static int  g_passed = 0;
static int  g_failed = 0;
static int  g_deleted_count = 0;

static void check_int(const char *name, int actual, int expected)
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

static void check_size(const char *name, size_t actual, size_t expected)
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

static void check_ptr(const char *name, const void *actual,
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

static void free_split(char **words)
{
    size_t  i;

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

static void test_itoa_case(char *label, int n, char *expected)
{
    char    *p;

    p = ft_itoa(n);
    check_int(label, p && strcmp(p, expected) == 0, 1);
    free(p);
}

static void test_putchar_case(char *label, char c)
{
    int     fd[2];
    char    result;
    ssize_t n;

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

static void test_putstr_case(char *label, char *s, char *expected)
{
    int     fd[2];
    char    buffer[128];
    ssize_t n;
    size_t  len;

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

static void test_putendl_case(char *label, char *s, char *expected)
{
    int     fd[2];
    char    buffer[128];
    ssize_t n;
    size_t  len;

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

static void test_putnbr_case(char *label, int value, char *expected)
{
    int     fd[2];
    char    buffer[64];
    ssize_t n;
    size_t  len;

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

static void section(const char *name)
{
    printf("\n=== %s ===\n", name);
}

static void test_isalpha(void)
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

static void test_isdigit(void)
{
    section("ft_isdigit");
    check_int("Digit 0", ft_isdigit('0'), 1);
    check_int("Digit 5", ft_isdigit('5'), 1);
    check_int("Digit 9", ft_isdigit('9'), 1);
    check_int("Before 0", ft_isdigit('/'), 0);
    check_int("After 9", ft_isdigit(':'), 0);
    check_int("Letter A", ft_isdigit('A'), 0);
}

static void test_isalnum(void)
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

static void test_isascii(void)
{
    section("ft_isascii");
    check_int("Minimum 0", ft_isascii(0), 1);
    check_int("ASCII 65", ft_isascii(65), 1);
    check_int("Maximum 127", ft_isascii(127), 1);
    check_int("Below range -1", ft_isascii(-1), 0);
    check_int("Above range 128", ft_isascii(128), 0);
    check_int("Large value 256", ft_isascii(256), 0);
}

static void test_isprint(void)
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

static void test_toupper(void)
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

static void test_tolower(void)
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

static void test_strlen(void)
{
    section("ft_strlen");
    check_size("Hello", ft_strlen("Hello"), 5);
    check_size("Empty string", ft_strlen(""), 0);
    check_size("1337", ft_strlen("1337"), 4);
    check_size("Hello World", ft_strlen("Hello World"), 11);
    check_size("Newline counts", ft_strlen("Hi\n"), 3);
    check_size("Embedded null", ft_strlen("Hello\0World"), 5);
}

static void test_strchr(void)
{
    char    str[] = "Hello";
    char    empty[] = "";
    char    repeated[] = "banana";

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

static void test_strrchr(void)
{
    char    str[] = "banana";
    char    empty[] = "";

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

static void test_strncmp(void)
{
    section("ft_strncmp");
    check_int("Equal strings", ft_strncmp("abc", "abc", 3), 0);
    check_int("First smaller", ft_strncmp("abc", "abd", 3) < 0, 1);
    check_int("First greater", ft_strncmp("abd", "abc", 3) > 0, 1);
    check_int("Limited comparison", ft_strncmp("abc", "abd", 2), 0);
    check_int("First shorter", ft_strncmp("abc", "abcd", 4) < 0, 1);
    check_int("First longer", ft_strncmp("abcd", "abc", 4) > 0, 1);
    check_int("Zero length", ft_strncmp("abc", "xyz", 0), 0);
    check_int("Empty strings", ft_strncmp("", "", 5), 0);
}

static void test_strlcpy(void)
{
    char    dst1[10];
    char    dst2[4];
    char    dst3[1];
    char    dst4[5] = "ABCD";
    char    dst5[5];

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

static void test_strlcat(void)
{
    char    dst1[8] = "Hi";
    char    dst2[5] = "Hi";
    char    dst3[10] = "Hello";
    char    dst4[5] = "";
    char    dst5[10] = "Hi";
    char    dst6[5] = "Hi";

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

static void test_strnstr(void)
{
    char    big[] = "Hello World";
    char    banana[] = "banana";
    char    empty[] = "";

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

static void test_memset(void)
{
    char            str[6] = "Hello";
    char            zero[6] = "Hello";
    char            full[6] = "Hello";
    unsigned char   bytes[4] = {1, 2, 3, 4};

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

static void test_bzero(void)
{
    char    str[] = "Hello";
    char    unchanged[] = "Hello";

    section("ft_bzero");

    ft_bzero(str, 2);
    check_int("First byte zero", str[0], '\0');
    check_int("Second byte zero", str[1], '\0');
    check_int("Third byte unchanged", str[2], 'l');

    ft_bzero(unchanged, 0);
    check_int("Zero size leaves buffer unchanged",
        strcmp(unchanged, "Hello"), 0);
}

static void test_memcpy(void)
{
    char    src[] = {'A', '\0', 'B', 'C'};
    char    dst[4] = {0};
    char    unchanged[] = "Hello";

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


static void test_memmove(void)
{
    char    right[] = "ABCDE";
    char    left[] = "ABCDE";
    char    normal[6] = {0};
    char    unchanged[] = "Hello";
    char    bytes[] = {'A', '\0', 'B', 'C'};

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

static void test_memchr(void)
{
    char    data[] = {'A', 'B', '\0', 'C', 'D'};
    char    str[] = "Hello";

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

static void test_memcmp(void)
{
    char    a[] = {'A', '\0', 'B'};
    char    b[] = {'A', '\0', 'C'};
    unsigned char   high[] = {255};
    unsigned char   low[] = {1};

    section("ft_memcmp");
    check_int("Equal first two bytes", ft_memcmp(a, b, 2), 0);
    check_int("Different third byte", ft_memcmp(a, b, 3) < 0, 1);
    check_int("Reverse comparison", ft_memcmp(b, a, 3) > 0, 1);
    check_int("Equal buffers", ft_memcmp(a, a, 3), 0);
    check_int("Zero length", ft_memcmp(a, b, 0), 0);
    check_int("Unsigned comparison", ft_memcmp(high, low, 1) > 0, 1);
}

static void test_atoi(void)
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


static void test_calloc(void)
{
    unsigned char   *p;
    size_t          i;
    int             all_zero;
    void            *zero_ptr;

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

static void test_strdup(void)
{
    char    *copy;
    char    *empty;
    char    original[] = "Hello";
    char    *independent;

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


static void test_substr(void)
{
    char    *p;
    char    original[] = "Hello";

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

static void test_strjoin(void)
{
    char    *p;
    char    s1[] = "Hello";

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

static void test_strtrim(void)
{
    char    *p;

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

static void test_split(void)
{
    char    **w;

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

static void test_itoa(void)
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


static char map_upper(unsigned int i, char c)
{
    (void)i;
    if (c >= 'a' && c <= 'z')
        return (c - 32);
    return (c);
}

static char map_alternate(unsigned int i, char c)
{
    if (i % 2 == 0 && c >= 'a' && c <= 'z')
        return (c - 32);
    return (c);
}

static char map_identity(unsigned int i, char c)
{
    (void)i;
    return (c);
}

static void test_strmapi(void)
{
    char    *p;
    char    original[] = "hello";

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

    /* NULL callback/source are tested in isolated child processes. */
}

static void iter_upper(unsigned int i, char *c)
{
    (void)i;
    if (*c >= 'a' && *c <= 'z')
        *c -= 32;
}

static void iter_index(unsigned int i, char *c)
{
    if (i % 2 == 0)
        *c = '*';
}

static void test_putchar_fd(void)
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

static void test_striteri(void)
{
    char    s1[] = "hello";
    char    s2[] = "abcdef";
    char    empty[] = "";

    section("ft_striteri");
    ft_striteri(s1, iter_upper);
    check_int("Uppercase in place", strcmp(s1, "HELLO"), 0);
    ft_striteri(s2, iter_index);
    check_int("Index-based modification",
        strcmp(s2, "*b*d*f"), 0);
    ft_striteri(empty, iter_upper);
    check_int("Empty string", strcmp(empty, ""), 0);
    /* NULL callback/source are tested in isolated child processes. */
}

static void test_putstr_fd(void)
{
    section("ft_putstr_fd");
    test_putstr_case("Normal string", "Hello", "Hello");
    test_putstr_case("Empty string", "", "");
    test_putstr_case("Numbers", "1337", "1337");
    test_putstr_case("Spaces", "Hello World", "Hello World");
    test_putstr_case("Newline", "Hello\nWorld", "Hello\nWorld");
    test_putstr_case("Special characters", "!@#$%^&*", "!@#$%^&*");
}

static void test_putendl_fd(void)
{
    section("ft_putendl_fd");
    test_putendl_case("Normal string", "Hello", "Hello\n");
    test_putendl_case("Empty string", "", "\n");
    test_putendl_case("Numbers", "1337", "1337\n");
    test_putendl_case("Spaces", "Hello World", "Hello World\n");
    test_putendl_case("Existing newline", "Hello\n", "Hello\n\n");
    test_putendl_case("Special characters", "!@#", "!@#\n");
}

static void test_putnbr_fd(void)
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

static void test_lstnew(void)
{
    t_list  *node;
    int     number;

    section("ft_lstnew");
    number = 42;
    node = ft_lstnew(&number);
    check_int("Node allocation", node != NULL, 1);
    if (node)
    {
        check_ptr("Content pointer", node->content, &number);
        check_ptr("Next is NULL", node->next, NULL);
        check_int("Content value", *(int *)node->content, 42);
        free(node);
    }
    node = ft_lstnew(NULL);
    check_int("NULL content accepted", node != NULL, 1);
    if (node)
    {
        check_ptr("NULL content stored", node->content, NULL);
        check_ptr("Next initialized", node->next, NULL);
        free(node);
    }
}

static void test_lstadd_front(void)
{
    t_list  *head;
    t_list  *first;
    t_list  *second;

    section("ft_lstadd_front");
    first = ft_lstnew("A");
    second = ft_lstnew("B");
    if (!first || !second)
    {
        free(first);
        free(second);
        return ;
    }
    head = first;
    ft_lstadd_front(&head, second);
    check_ptr("Head is B", head, second);
    check_ptr("B points to A", head->next, first);
    check_ptr("A points to NULL", first->next, NULL);
    free(second);
    free(first);
}

static void test_lstsize(void)
{
    t_list  *head;
    t_list  *second;
    t_list  *third;

    section("ft_lstsize");
    check_int("Empty list", ft_lstsize(NULL), 0);
    head = ft_lstnew("A");
    second = ft_lstnew("B");
    third = ft_lstnew("C");
    if (!head || !second || !third)
    {
        free(head);
        free(second);
        free(third);
        return ;
    }
    check_int("One node", ft_lstsize(head), 1);
    head->next = second;
    check_int("Two nodes", ft_lstsize(head), 2);
    second->next = third;
    check_int("Three nodes", ft_lstsize(head), 3);
    free(third);
    free(second);
    free(head);
}

static void test_lstlast(void)
{
    t_list  *head;
    t_list  *second;
    t_list  *third;

    section("ft_lstlast");
    check_ptr("Empty list", ft_lstlast(NULL), NULL);
    head = ft_lstnew("A");
    second = ft_lstnew("B");
    third = ft_lstnew("C");
    if (!head || !second || !third)
    {
        free(head);
        free(second);
        free(third);
        return ;
    }
    check_ptr("One node", ft_lstlast(head), head);
    head->next = second;
    check_ptr("Two nodes", ft_lstlast(head), second);
    second->next = third;
    check_ptr("Three nodes", ft_lstlast(head), third);
    free(third);
    free(second);
    free(head);
}


static void test_lstadd_back(void)
{
    t_list  *head;
    t_list  *a;
    t_list  *b;
    t_list  *c;

    section("ft_lstadd_back");
    head = NULL;
    a = ft_lstnew("A");
    b = ft_lstnew("B");
    c = ft_lstnew("C");
    if (!a || !b || !c)
    {
        free(a);
        free(b);
        free(c);
        return ;
    }
    ft_lstadd_back(&head, a);
    check_ptr("Empty list: head is A", head, a);
    check_ptr("A next is NULL", a->next, NULL);
    ft_lstadd_back(&head, b);
    check_ptr("Head still A", head, a);
    check_ptr("A points to B", a->next, b);
    ft_lstadd_back(&head, c);
    check_ptr("B points to C", b->next, c);
    check_ptr("C next is NULL", c->next, NULL);
    check_int("List size is 3", ft_lstsize(head), 3);
    free(c);
    free(b);
    free(a);
}

static void count_and_free(void *content);

static void test_lstdelone(void)
{
    t_list  *node;
    char    *content;

    section("ft_lstdelone");
    content = ft_strdup("Hello");
    if (!content)
        return ;
    node = ft_lstnew(content);
    if (!node)
    {
        free(content);
        return ;
    }
    check_ptr("Content assigned", node->content, content);
    g_deleted_count = 0;
    ft_lstdelone(node, count_and_free);
    check_int("Deletion callback invoked once", g_deleted_count, 1);
    ft_lstdelone(NULL, free);
    /* NULL node is also tested in the isolated NULL audit. */
}

static void count_and_free(void *content)
{
    g_deleted_count++;
    free(content);
}

static void test_lstclear(void)
{
    t_list  *head;
    t_list  *node;
    char    *one_content;
    char    *new_content;
    int     i;

    section("ft_lstclear");

    /* Test 1: Empty list */
    head = NULL;
    g_deleted_count = 0;
    ft_lstclear(&head, count_and_free);
    check_ptr("Empty list stays NULL", head, NULL);
    check_int("No callback for empty list", g_deleted_count, 0);

    /* Test 2: NULL list pointer */
    ft_lstclear(NULL, count_and_free);
    /* NULL list pointer is also tested in the isolated NULL audit. */

    /* Test 3: Single node */
    one_content = ft_strdup("Hello");
    head = NULL;
    if (one_content)
        head = ft_lstnew(one_content);
    g_deleted_count = 0;
    if (head && head->content)
    {
        ft_lstclear(&head, count_and_free);
        check_ptr("Single node: head is NULL", head, NULL);
        check_int("Single node: callback count", g_deleted_count, 1);
    }
    else
    {
        check_int("Single-node test setup allocation", 0, 1);
        free(one_content);
        free(head);
    }

    /* Test 4: Multiple nodes */
    head = NULL;
    i = 0;
    while (i < 5)
    {
        new_content = ft_strdup("1337");
        node = NULL;
        if (new_content)
            node = ft_lstnew(new_content);
        if (!node || !node->content)
        {
            free(new_content);
            free(node);
            ft_lstclear(&head, free);
            return ;
        }
        ft_lstadd_back(&head, node);
        i++;
    }
    check_int("List contains 5 nodes", ft_lstsize(head), 5);
    g_deleted_count = 0;
    ft_lstclear(&head, count_and_free);
    check_ptr("Multiple nodes: head is NULL", head, NULL);
    check_int("Multiple nodes: callback count", g_deleted_count, 5);

    /* Test 5: Clear already cleared list */
    ft_lstclear(&head, count_and_free);
    check_ptr("Second clear: head is NULL", head, NULL);
    check_int("Second clear: no additional calls", g_deleted_count, 5);
}


static void double_value(void *content)
{
    int *n;

    n = (int *)content;
    *n *= 2;
}

static void test_lstiter(void)
{
    t_list  *head;
    t_list  *second;
    t_list  *third;
    int     a;
    int     b;
    int     c;

    section("ft_lstiter");
    a = 10;
    b = 20;
    c = 30;
    head = ft_lstnew(&a);
    second = ft_lstnew(&b);
    third = ft_lstnew(&c);
    if (!head || !second || !third)
    {
        free(head);
        free(second);
        free(third);
        return ;
    }
    ft_lstadd_back(&head, second);
    ft_lstadd_back(&head, third);
    ft_lstiter(head, double_value);
    check_int("First value doubled", a, 20);
    check_int("Second value doubled", b, 40);
    check_int("Third value doubled", c, 60);
    check_int("List size unchanged", ft_lstsize(head), 3);
    ft_lstiter(NULL, double_value);
    /* NULL callback is tested in isolated child processes. */
    free(third);
    free(second);
    free(head);
}


static void *map_double(void *content)
{
    int *result;

    result = malloc(sizeof(int));
    if (!result)
        return (NULL);
    *result = *(int *)content * 2;
    return (result);
}

static void test_lstmap(void)
{
    int     a;
    int     b;
    int     c;
    t_list  *original;
    t_list  *second;
    t_list  *third;
    t_list  *mapped;

    section("ft_lstmap");
    a = 10;
    b = 20;
    c = 30;
    original = ft_lstnew(&a);
    second = ft_lstnew(&b);
    third = ft_lstnew(&c);
    if (!original || !second || !third)
    {
        free(original);
        free(second);
        free(third);
        return ;
    }
    ft_lstadd_back(&original, second);
    ft_lstadd_back(&original, third);
    mapped = ft_lstmap(original, map_double, free);
    check_int("Mapped list allocated", mapped != NULL, 1);
    if (mapped)
    {
        check_int("Mapped size", ft_lstsize(mapped), 3);
        check_int("Mapped contents allocated",
            mapped->content && mapped->next && mapped->next->content
            && mapped->next->next && mapped->next->next->content, 1);
        if (mapped->content && mapped->next && mapped->next->content
            && mapped->next->next && mapped->next->next->content)
        {
            check_int("First mapped value",
                *(int *)mapped->content, 20);
            check_int("Second mapped value",
                *(int *)mapped->next->content, 40);
            check_int("Third mapped value",
                *(int *)mapped->next->next->content, 60);
        }
        check_int("Original unchanged", a == 10 && b == 20
            && c == 30, 1);
        check_int("Separate first node", mapped != original, 1);
        ft_lstclear(&mapped, free);
        check_ptr("Mapped list cleared", mapped, NULL);
    }
    free(third);
    free(second);
    free(original);
    /* Original contents were stack integers, not heap allocations. */
}



/* ================================================================ */
/* ADDITIONAL VALID-INPUT EDGE CASES — ALL 43 LIBFT FUNCTIONS       */
/* ================================================================ */

static void check_string_and_free(const char *label, char *actual,
                                  const char *expected)
{
    check_int(label, actual && strcmp(actual, expected) == 0, 1);
    free(actual);
}

static void test_extra_classification(void)
{
    section("EXTRA: character classification and case conversion");
    check_int("isalpha: NUL", ft_isalpha(0), 0);
    check_int("isalpha: DEL", ft_isalpha(127), 0);
    check_int("isdigit: negative", ft_isdigit(-1), 0);
    check_int("isalnum: after digit range", ft_isalnum(':'), 0);
    check_int("isascii: 255", ft_isascii(255), 0);
    check_int("isprint: 33", ft_isprint(33), 1);
    check_int("isprint: 0", ft_isprint(0), 0);
    check_int("toupper: 0", ft_toupper(0), 0);
    check_int("toupper: -1/EOF", ft_toupper(-1), -1);
    check_int("tolower: 0", ft_tolower(0), 0);
    check_int("tolower: 200 unchanged", ft_tolower(200), 200);
}

static void test_extra_strings_part1(void)
{
    char a[] = {'A', (char)0xff, 'B', '\0'};
    char destination[8] = "ab";
    char small[3] = "zz";

    section("EXTRA: Part 1 string boundaries");
    check_size("strlen: single character", ft_strlen("x"), 1);
    check_ptr("strchr: high unsigned byte", ft_strchr(a, 255), a + 1);
    check_ptr("strchr: -1 becomes 255", ft_strchr(a, -1), a + 1);
    check_ptr("strrchr: high unsigned byte", ft_strrchr(a, 255), a + 1);
    check_int("strncmp: first high byte > ASCII",
              ft_strncmp("\xff", "\x01", 1) > 0, 1);
    check_int("strncmp: compare only prefix",
              ft_strncmp("abcdef", "abcxyz", 3), 0);
    check_int("strncmp: NUL terminates comparison",
              ft_strncmp("a", "aZ", (size_t)-1) < 0, 1);
    check_size("strlcpy: exact fit", ft_strlcpy(small, "ok", 3), 2);
    check_int("strlcpy: exact fit contents", strcmp(small, "ok"), 0);
    check_size("strlcat: size equals original dst length",
               ft_strlcat(destination, "XYZ", 2), 5);
    check_int("strlcat: no write if size equals dst length",
              strcmp(destination, "ab"), 0);
    check_size("strlcat: append up to capacity",
               ft_strlcat(destination, "XYZ", sizeof(destination)), 5);
    check_int("strlcat: output", strcmp(destination, "abXYZ"), 0);
}

static void test_extra_strnstr(void)
{
    char haystack[] = "aaab";
    char repeated[] = "abcabcabc";

    section("EXTRA: strnstr precise boundaries");
    check_ptr("strnstr: overlapping prefix", ft_strnstr(haystack, "aab", 4),
              haystack + 1);
    check_ptr("strnstr: first duplicate occurrence",
              ft_strnstr(repeated, "abc", 9), repeated);
    check_ptr("strnstr: partial match past limit",
              ft_strnstr(haystack, "aab", 3), NULL);
    check_ptr("strnstr: huge limit with short strings",
              ft_strnstr(haystack, "aab", (size_t)-1), haystack + 1);
}

static void test_extra_memory(void)
{
    unsigned char a[6] = {1, 2, 0, 255, 5, 6};
    unsigned char b[6] = {0};
    unsigned char c[6] = {1, 2, 0, 255, 5, 6};
    unsigned char zeroed[6] = {1, 2, 3, 4, 5, 6};

    section("EXTRA: byte-accurate memory operations");
    check_ptr("memset: same destination", ft_memset(b, 255, 2), b);
    check_int("memset: 0xFF first byte", b[0], 255);
    check_int("memset: byte after range unchanged", b[2], 0);
    ft_bzero(zeroed + 2, 2);
    check_int("bzero: prefix preserved", zeroed[1], 2);
    check_int("bzero: byte 2 cleared", zeroed[2], 0);
    check_int("bzero: byte 3 cleared", zeroed[3], 0);
    check_int("bzero: suffix preserved", zeroed[4], 5);
    check_ptr("memcpy: returns dst", ft_memcpy(b, a, sizeof(a)), b);
    check_int("memcpy: raw bytes including NUL and 0xFF",
              memcmp(a, b, sizeof(a)), 0);
    check_ptr("memmove: same pointer, 0 size", ft_memmove(b, b, 0), b);
    ft_memmove(b + 1, b, 5);
    check_int("memmove: overlapping binary bytes", memcmp(b + 1, a, 5), 0);
    check_ptr("memchr: find 0xFF", ft_memchr(a, 255, 6), a + 3);
    check_ptr("memchr: converted 511 to 0xFF", ft_memchr(a, 511, 6), a + 3);
    check_int("memcmp: binary equals", ft_memcmp(a, c, 6), 0);
    c[3] = 1;
    check_int("memcmp: unsigned 0xFF > 1", ft_memcmp(a, c, 6) > 0, 1);
}

static void test_extra_atoi_calloc_strdup(void)
{
    unsigned char *p;
    char *dup;
    char text[] = "x";

    section("EXTRA: atoi / calloc / strdup");
    check_int("atoi: + and leading tabs", ft_atoi("\t\n+0000042x"), 42);
    check_int("atoi: negative trailing data", ft_atoi("-12.5"), -12);
    check_int("atoi: standalone plus", ft_atoi("+"), 0);
    p = ft_calloc(3, sizeof(int));
    check_int("calloc: three ints allocation", p != NULL, 1);
    if (p)
    {
        check_int("calloc: all 3 integers zero", memcmp(p,
                  (unsigned char[3 * sizeof(int)]){0}, 3 * sizeof(int)), 0);
        free(p);
    }
    check_ptr("calloc: huge multiplication overflow",
              ft_calloc((size_t)-1 / 2 + 1, 2), NULL);
    dup = ft_strdup(text);
    check_int("strdup: one-character string", dup && !strcmp(dup, "x"), 1);
    check_int("strdup: separate allocation", dup && dup != text, 1);
    free(dup);
}

static void test_extra_substr_join_trim(void)
{
    char left[1025];
    char right[1025];
    char *joined;
    size_t i;

    section("EXTRA: substr / strjoin / strtrim");
    check_string_and_free("substr: maximal start", ft_substr("abc", UINT_MAX, 4), "");
    check_string_and_free("substr: huge length", ft_substr("abcdef", 2, SIZE_MAX), "cdef");
    check_string_and_free("substr: one byte", ft_substr("abc", 1, 1), "b");
    check_string_and_free("strjoin: embedded NUL truncation",
                          ft_strjoin("ab\0ignored", "c"), "abc");
    i = 0;
    while (i < 1024)
    {
        left[i] = 'L';
        right[i] = 'R';
        i++;
    }
    left[1024] = '\0';
    right[1024] = '\0';
    joined = ft_strjoin(left, right);
    check_int("strjoin: 2048-byte concatenation",
              joined && strlen(joined) == 2048 && joined[0] == 'L'
              && joined[1023] == 'L' && joined[1024] == 'R'
              && joined[2047] == 'R' && joined[2048] == '\0', 1);
    free(joined);
    check_string_and_free("strtrim: whitespace set",
                          ft_strtrim(" \t\nhello\n\t ", " \t\n"), "hello");
    check_string_and_free("strtrim: entire string belongs to set",
                          ft_strtrim("abbaabba", "ab"), "");
    check_string_and_free("strtrim: different chars at edges",
                          ft_strtrim("xyHellozy", "xyz"), "Hello");
}

static void test_extra_split(void)
{
    char **words;
    char many[256];
    int i;
    int ok;

    section("EXTRA: split boundaries and many words");
    words = ft_split("", '\0');
    check_int("split: empty string, NUL separator",
              words && words[0] == NULL, 1);
    free_split(words);
    words = ft_split("one\n\ntwo\n", '\n');
    check_int("split: newline and repeated delimiter",
              words && words[0] && words[1] && !strcmp(words[0], "one")
              && !strcmp(words[1], "two") && words[2] == NULL, 1);
    free_split(words);
    /* 100 single-character words separated by commas. */
    i = 0;
    while (i < 100)
    {
        many[i * 2] = 'x';
        if (i < 99)
            many[i * 2 + 1] = ',';
        i++;
    }
    many[199] = '\0';
    words = ft_split(many, ',');
    ok = (words != NULL);
    i = 0;
    while (ok && i < 100)
    {
        if (!words[i] || strcmp(words[i], "x") != 0)
            ok = 0;
        i++;
    }
    check_int("split: 100 words + NULL terminator",
              ok && words[100] == NULL, 1);
    free_split(words);
}

static int g_strmapi_calls;
static int g_striteri_calls;

static char map_count(unsigned int idx, char c)
{
    (void)idx;
    g_strmapi_calls++;
    return (c);
}

static void iter_count(unsigned int idx, char *c)
{
    (void)idx;
    (void)c;
    g_striteri_calls++;
}

static void test_extra_maps_and_numbers(void)
{
    char input[] = "a\0bc";
    char *mapped;

    section("EXTRA: itoa / strmapi / striteri");
    test_itoa_case("itoa: positive tens", 10, "10");
    test_itoa_case("itoa: negative tens", -10, "-10");
    test_itoa_case("itoa: trailing zeros", -100200, "-100200");
    g_strmapi_calls = 0;
    mapped = ft_strmapi(input, map_count);
    check_int("strmapi: stops at first NUL", g_strmapi_calls, 1);
    check_int("strmapi: output NUL termination", mapped && mapped[0] == 'a'
              && mapped[1] == '\0', 1);
    free(mapped);
    g_striteri_calls = 0;
    ft_striteri(input, iter_count);
    check_int("striteri: stops at first NUL", g_striteri_calls, 1);
}

static void test_extra_fd(void)
{
    section("EXTRA: file-descriptor output");
    test_putchar_case("putchar: tab byte", '\t');
    test_putchar_case("putchar: 0x80 byte", (char)0x80);
    test_putstr_case("putstr: embedded NUL", "ab\0cd", "ab");
    test_putendl_case("putendl: embedded NUL", "ab\0cd", "ab\n");
    test_putendl_case("putendl: only newline", "", "\n");
    test_putnbr_case("putnbr: 10", 10, "10");
    test_putnbr_case("putnbr: -10", -10, "-10");
    test_putnbr_case("putnbr: 1000000000", 1000000000, "1000000000");
}

static int g_iter_seen;
static void iter_seen(void *content)
{
    (void)content;
    g_iter_seen++;
}

static void *map_copy(void *content)
{
    int *out = malloc(sizeof(*out));
    if (out)
        *out = *(int *)content;
    return (out);
}

static void test_extra_lists(void)
{
    t_list *head;
    t_list *a;
    t_list *b;
    t_list *mapped;
    int values[2] = {7, 8};

    section("EXTRA: all nine linked-list functions");
    a = ft_lstnew(&values[0]);
    b = ft_lstnew(&values[1]);
    if (!a || !b)
    {
        check_int("List setup allocation", 0, 1);
        free(a);
        free(b);
        return ;
    }
    check_ptr("lstnew: stores original content pointer", a->content, &values[0]);
    head = NULL;
    ft_lstadd_front(&head, a);
    check_ptr("lstadd_front: empty head", head, a);
    ft_lstadd_back(&head, b);
    check_int("lstsize: linked two nodes", ft_lstsize(head), 2);
    check_ptr("lstlast: points to B", ft_lstlast(head), b);
    ft_lstadd_front(&head, NULL);
    check_ptr("lstadd_front: NULL new unchanged", head, a);
    ft_lstadd_back(&head, NULL);
    check_ptr("lstadd_back: NULL new unchanged", a->next, b);
    g_iter_seen = 0;
    ft_lstiter(head, iter_seen);
    check_int("lstiter: callback executed twice", g_iter_seen, 2);
    mapped = ft_lstmap(head, map_copy, free);
    check_int("lstmap: builds a separate list", mapped != NULL
              && mapped != head && ft_lstsize(mapped) == 2, 1);
    if (mapped)
        ft_lstclear(&mapped, free);
    check_ptr("lstmap: empty list returns NULL",
              ft_lstmap(NULL, map_copy, free), NULL);
    /* No deletion callback is supplied because content lives on the stack. */
    ft_lstdelone(NULL, free);
    ft_lstclear(NULL, free);
    /* NULL list operations are also tested in the isolated audit. */
    free(b);
    free(a);
}

/* ================================================================ */
/* EXHAUSTIVE / SYSTEMATIC TESTS (ADDED)                             */
/* ================================================================ */

static void test_exhaustive_calloc_zero(void)
{
    void *p;

    section("EXHAUSTIVE: calloc(0,0) and zero-size variants");
    p = ft_calloc(0, 0);
    check_int("calloc(0,0) non-NULL", p != NULL, 1);
    free(p);
    p = ft_calloc(0, 42);
    check_int("calloc(0,42) non-NULL", p != NULL, 1);
    free(p);
    p = ft_calloc(42, 0);
    check_int("calloc(42,0) non-NULL", p != NULL, 1);
    free(p);
    p = ft_calloc(0, sizeof(long));
    check_int("calloc(0,sizeof(long)) non-NULL", p != NULL, 1);
    free(p);
    check_ptr("calloc(SIZE_MAX,SIZE_MAX) overflow",
        ft_calloc(SIZE_MAX, SIZE_MAX), NULL);
    check_ptr("calloc(SIZE_MAX,2) overflow",
        ft_calloc(SIZE_MAX, 2), NULL);
    check_ptr("calloc(2,SIZE_MAX) overflow",
        ft_calloc(2, SIZE_MAX), NULL);
}

static void test_exhaustive_classification(void)
{
    int i;
    int ok;

    section("EXHAUSTIVE: classification vs libc for 0-127");
    ok = 1;
    i = 0;
    while (i < 128)
    {
        if (!!ft_isalpha(i) != !!isalpha(i))
            ok = 0;
        i++;
    }
    check_int("isalpha matches libc", ok, 1);
    ok = 1;
    i = 0;
    while (i < 128)
    {
        if (!!ft_isdigit(i) != !!isdigit(i))
            ok = 0;
        i++;
    }
    check_int("isdigit matches libc", ok, 1);
    ok = 1;
    i = 0;
    while (i < 128)
    {
        if (!!ft_isalnum(i) != !!isalnum(i))
            ok = 0;
        i++;
    }
    check_int("isalnum matches libc", ok, 1);
    ok = 1;
    i = 0;
    while (i < 128)
    {
        if (!!ft_isprint(i) != !!isprint(i))
            ok = 0;
        i++;
    }
    check_int("isprint matches libc", ok, 1);
    ok = 1;
    i = 0;
    while (i < 128)
    {
        if (ft_toupper(i) != toupper(i))
            ok = 0;
        i++;
    }
    check_int("toupper matches libc", ok, 1);
    ok = 1;
    i = 0;
    while (i < 128)
    {
        if (ft_tolower(i) != tolower(i))
            ok = 0;
        i++;
    }
    check_int("tolower matches libc", ok, 1);
    ok = 1;
    i = 0;
    while (i < 256)
    {
        int expected = (i < 128) ? 1 : 0;
        if (!!ft_isascii(i) != expected)
            ok = 0;
        i++;
    }
    check_int("isascii: 0-127 true, 128-255 false", ok, 1);
}

static void test_exhaustive_itoa(void)
{
    char buf[32];
    char *p;
    int i;
    int ok;

    section("EXHAUSTIVE: itoa vs snprintf for -1000..1000");
    ok = 1;
    i = -1000;
    while (i <= 1000)
    {
        snprintf(buf, sizeof(buf), "%d", i);
        p = ft_itoa(i);
        if (!p || strcmp(p, buf) != 0)
        {
            printf("    itoa(%d): expected \"%s\", got \"%s\"\n",
                i, buf, p ? p : "(null)");
            ok = 0;
        }
        free(p);
        i++;
    }
    check_int("itoa matches snprintf", ok, 1);
}

static void test_exhaustive_atoi(void)
{
    static const char *cases[] = {
        "0", "+0", "-0", "1", "-1", "42", "-42", "+42",
        "2147483647", "-2147483647", "-2147483648",
        "0000000000", "-0000000000",
        "  123", "\t\n\v\f\r 42", "abc", "", "   ",
        "12abc", "  42xyz", NULL
    };
    int i;
    int ok;

    section("EXHAUSTIVE: atoi vs libc");
    ok = 1;
    i = 0;
    while (cases[i])
    {
        if (ft_atoi(cases[i]) != atoi(cases[i]))
        {
            printf("    atoi(\"%s\"): ft=%d libc=%d\n",
                cases[i], ft_atoi(cases[i]), atoi(cases[i]));
            ok = 0;
        }
        i++;
    }
    check_int("atoi matches libc", ok, 1);
}

static void test_long_strings(void)
{
    char *long_str;
    char *p;
    size_t i;
    size_t len = 100000;

    section("EXHAUSTIVE: long strings (100k chars)");
    long_str = malloc(len + 1);
    if (!long_str)
    {
        check_int("Setup allocation", 0, 1);
        return ;
    }
    i = 0;
    while (i < len)
    {
        long_str[i] = 'a' + (i % 26);
        i++;
    }
    long_str[len] = '\0';
    check_size("strlen on 100k string", ft_strlen(long_str), len);

    p = ft_strdup(long_str);
    check_int("strdup on 100k string", p && strcmp(p, long_str) == 0, 1);
    free(p);

    p = ft_substr(long_str, 50000, 100);
    check_int("substr middle of 100k string",
        p && strlen(p) == 100 && p[0] == long_str[50000], 1);
    free(p);

    p = ft_strjoin(long_str, long_str);
    check_int("strjoin two 100k strings",
        p && strlen(p) == 2 * len, 1);
    free(p);

    p = ft_strtrim(long_str, "XYZ");
    check_int("strtrim 100k string unchanged",
        p && strcmp(p, long_str) == 0, 1);
    free(p);

    free(long_str);
}

static void test_memmove_thorough(void)
{
    char buf[16];

    section("EXHAUSTIVE: memmove overlap patterns");

    strcpy(buf, "0123456789");
    ft_memmove(buf + 2, buf, 5);
    check_int("right overlap by 2", strcmp(buf, "0101234789"), 0);

    strcpy(buf, "0123456789");
    ft_memmove(buf, buf + 2, 5);
    check_int("left overlap by 2", strcmp(buf, "2345656789"), 0);

    strcpy(buf, "0123456789");
    ft_memmove(buf + 1, buf, 1);
    check_int("size 1, dst = src + 1", strcmp(buf, "0023456789"), 0);

    strcpy(buf, "0123456789");
    ft_memmove(buf, buf + 1, 1);
    check_int("size 1, dst = src - 1", strcmp(buf, "1123456789"), 0);

    strcpy(buf, "0123456789");
    ft_memmove(buf + 1, buf, 2);
    check_int("1-byte overlap right", strcmp(buf, "0013456789"), 0);

    strcpy(buf, "0123456789");
    ft_memmove(buf, buf + 1, 2);
    check_int("1-byte overlap left", strcmp(buf, "1223456789"), 0);

    strcpy(buf, "0123456789");
    ft_memmove(buf, buf, 10);
    check_int("same pointer", strcmp(buf, "0123456789"), 0);
}

static void test_strlcat_sizes(void)
{
    char dst[16];
    size_t ret;

    section("EXHAUSTIVE: strlcat with various sizes");

    strcpy(dst, "abc");
    ret = ft_strlcat(dst, "XYZW", 0);
    check_size("size 0: return 0+4", ret, 4);
    check_int("size 0: dst unchanged", strcmp(dst, "abc"), 0);

    strcpy(dst, "abc");
    ret = ft_strlcat(dst, "XYZW", 2);
    check_size("size 2 (<dstlen): return 2+4", ret, 6);
    check_int("size 2: dst unchanged", strcmp(dst, "abc"), 0);

    strcpy(dst, "abc");
    ret = ft_strlcat(dst, "XYZW", 3);
    check_size("size == dstlen: return 3+4", ret, 7);
    check_int("size == dstlen: dst unchanged", strcmp(dst, "abc"), 0);

    strcpy(dst, "abc");
    ret = ft_strlcat(dst, "XYZW", 5);
    check_size("size 5: return 3+4", ret, 7);
    check_int("size 5: copies 1 char", strcmp(dst, "abcX"), 0);

    strcpy(dst, "abc");
    ret = ft_strlcat(dst, "XYZW", 8);
    check_size("size 8: exact fit, return 7", ret, 7);
    check_int("size 8: contents", strcmp(dst, "abcXYZW"), 0);

    strcpy(dst, "abc");
    ret = ft_strlcat(dst, "XYZW", sizeof(dst));
    check_size("size actual capacity: return 7", ret, 7);
    check_int("size actual capacity: contents", strcmp(dst, "abcXYZW"), 0);
}

static void test_strnstr_needle_longer(void)
{
    char hay[] = "abc";

    section("EXHAUSTIVE: strnstr needle longer than haystack");
    check_ptr("needle longer", ft_strnstr(hay, "abcd", 10), NULL);
    check_ptr("needle longer, limit = haystack len",
        ft_strnstr(hay, "abcd", 3), NULL);
    check_ptr("needle == haystack",
        ft_strnstr(hay, hay, 3), hay);
    check_ptr("needle == haystack, larger limit",
        ft_strnstr(hay, hay, 100), hay);
    check_ptr("needle is prefix", ft_strnstr(hay, "ab", 3), hay);
    check_ptr("needle is suffix", ft_strnstr(hay, "bc", 3), hay + 1);
}

static void test_split_long_word(void)
{
    char **w;
    char *long_word;
    size_t i;

    section("EXHAUSTIVE: split with a single long word");

    long_word = malloc(10001);
    if (!long_word)
    {
        check_int("Setup allocation", 0, 1);
        return ;
    }
    i = 0;
    while (i < 10000)
    {
        long_word[i] = 'x';
        i++;
    }
    long_word[10000] = '\0';

    w = ft_split(long_word, ',');
    check_int("split 10000-char word",
        w && w[0] && !w[1] && strlen(w[0]) == 10000, 1);
    free_split(w);
    free(long_word);
}

static void test_strchr_all_bytes(void)
{
    char hay[] = "abcdefghij";
    int i;
    int ok;

    section("EXHAUSTIVE: strchr with all 256 search values");
    ok = 1;
    i = 0;
    while (i < 256)
    {
        char *p = ft_strchr(hay, i);
        if (i == 0)
        {
            if (p != hay + 10)
                ok = 0;
        }
        else if (i >= 'a' && i <= 'j')
        {
            if (p != hay + (i - 'a'))
                ok = 0;
        }
        else if (p != NULL)
            ok = 0;
        i++;
    }
    check_int("strchr all 256 values", ok, 1);
}

static void test_memchr_all_bytes(void)
{
    unsigned char hay[4] = {0, 128, 200, 255};
    int i;
    int ok;
    void *p;

    section("EXHAUSTIVE: memchr with all 256 search values");
    ok = 1;
    i = 0;
    while (i < 256)
    {
        p = ft_memchr(hay, i, 4);
        if (i == 0)
        {
            if (p != hay)
                ok = 0;
        }
        else if (i == 128)
        {
            if (p != hay + 1)
                ok = 0;
        }
        else if (i == 200)
        {
            if (p != hay + 2)
                ok = 0;
        }
        else if (i == 255)
        {
            if (p != hay + 3)
                ok = 0;
        }
        else if (p != NULL)
            ok = 0;
        i++;
    }
    check_int("memchr all 256 values", ok, 1);
}


/* ================================================================ */
/* INDEPENDENT GLIBC DIFFERENTIAL / RANDOMIZED TESTS                 */
/* Only VALID inputs are compared. Out-of-range atoi is unspecified. */
/* strlcpy, strlcat and strnstr use independent BSD-style oracles   */
/* because they are not available in older glibc versions.           */
/* ================================================================ */

static unsigned int g_rng = 0x1337c0deU;

static unsigned int next_random(void)
{
    g_rng ^= g_rng << 13;
    g_rng ^= g_rng >> 17;
    g_rng ^= g_rng << 5;
    return (g_rng);
}

static size_t reference_strlcpy(char *dst, const char *src, size_t size)
{
    size_t length;
    size_t count;

    length = strlen(src);
    if (size != 0)
    {
        count = length;
        if (count >= size)
            count = size - 1;
        memcpy(dst, src, count);
        dst[count] = '\0';
    }
    return (length);
}

static size_t reference_strlcat(char *dst, const char *src, size_t size)
{
    size_t dlen;
    size_t slen;
    size_t copied;

    dlen = strnlen(dst, size);
    slen = strlen(src);
    if (dlen == size)
        return (size + slen);
    copied = slen;
    if (copied >= size - dlen)
        copied = size - dlen - 1;
    memcpy(dst + dlen, src, copied);
    dst[dlen + copied] = '\0';
    return (dlen + slen);
}

static char *reference_strnstr(const char *big, const char *little, size_t n)
{
    size_t i;
    size_t j;

    if (*little == '\0')
        return ((char *)big);
    i = 0;
    while (i < n && big[i] != '\0')
    {
        j = 0;
        while (j < n - i && little[j] && big[i + j]
            && big[i + j] == little[j])
            j++;
        if (!little[j])
            return ((char *)big + i);
        i++;
    }
    return (NULL);
}

static int same_sign(int a, int b)
{
    return ((a > 0) == (b > 0) && (a < 0) == (b < 0));
}

static void random_string(char *s, size_t maxlen)
{
    size_t i;
    size_t n;

    n = next_random() % (maxlen + 1);
    i = 0;
    while (i < n)
    {
        s[i] = (char)(1 + next_random() % 255);
        i++;
    }
    s[i] = '\0';
}

static void test_glibc_character_domain(void)
{
    int c;
    int ok[7];

    section("GLIBC: all legal ctype inputs (-1 and 0..255)");
    for (c = 0; c < 7; c++) ok[c] = 1;
    c = -1;
    while (c <= 255)
    {
        if (!!ft_isalpha(c) != !!isalpha(c)) ok[0] = 0;
        if (!!ft_isdigit(c) != !!isdigit(c)) ok[1] = 0;
        if (!!ft_isalnum(c) != !!isalnum(c)) ok[2] = 0;
        if (!!ft_isprint(c) != !!isprint(c)) ok[3] = 0;
        if (ft_toupper(c) != toupper(c)) ok[4] = 0;
        if (ft_tolower(c) != tolower(c)) ok[5] = 0;
        if (!!ft_isascii(c) != !!isascii(c)) ok[6] = 0;
        c++;
    }
    check_int("isalpha full domain", ok[0], 1);
    check_int("isdigit full domain", ok[1], 1);
    check_int("isalnum full domain", ok[2], 1);
    check_int("isprint full domain", ok[3], 1);
    check_int("toupper full domain", ok[4], 1);
    check_int("tolower full domain", ok[5], 1);
    check_int("isascii full domain", ok[6], 1);
    check_int("ctype: int below EOF NOT tested (libc UB)", 1, 1);
}

static void test_glibc_atoi_random(void)
{
    char input[96];
    int i;
    int value;
    int ok;

    section("GLIBC: atoi 12000 in-range formatted values");
    ok = 1;
    i = 0;
    while (i < 12000)
    {
        value = (int)next_random();
        if (i % 3 == 0)
            snprintf(input, sizeof(input), "  %dtrail", value);
        else if (i % 3 == 1)
            snprintf(input, sizeof(input), "\t%+015d", value);
        else
            snprintf(input, sizeof(input), "%d", value);
        if (ft_atoi(input) != atoi(input))
        {
            if (ok)
                printf("    first mismatch: atoi(\"%s\")\n", input);
            ok = 0;
        }
        i++;
    }
    check_int("atoi matches glibc, no out-of-range strings", ok, 1);
}

static void test_glibc_strings_random(void)
{
    char a[80];
    char b[80];
    char out_ft[100];
    char out_ref[100];
    char *copy;
    size_t n;
    size_t size;
    int ch;
    int i;
    int ok[9];

    section("GLIBC: 6000 randomized string / BSD strl tests");
    for (i = 0; i < 9; i++)
        ok[i] = 1;
    i = 0;
    while (i < 6000)
    {
        random_string(a, 64);
        random_string(b, 64);
        n = next_random() % 80;
        ch = (int)(next_random() & 0x3ff) - 256;
        if (ft_strlen(a) != strlen(a)) ok[0] = 0;
        if (ft_strchr(a, ch) != strchr(a, ch)) ok[1] = 0;
        if (ft_strrchr(a, ch) != strrchr(a, ch)) ok[2] = 0;
        if (!same_sign(ft_strncmp(a, b, n), strncmp(a, b, n))) ok[3] = 0;
        copy = ft_strdup(a);
        if (!copy || copy == a || strcmp(copy, a)) ok[4] = 0;
        free(copy);
        memset(out_ft, 'Q', sizeof(out_ft));
        memset(out_ref, 'Q', sizeof(out_ref));
        size = next_random() % 81;
        if (ft_strlcpy(out_ft, a, size)
            != reference_strlcpy(out_ref, a, size)
            || memcmp(out_ft, out_ref, sizeof(out_ft))) ok[5] = 0;
        strcpy(out_ft, a);
        strcpy(out_ref, a);
        size = next_random() % sizeof(out_ft);
        if (ft_strlcat(out_ft, b, size)
            != reference_strlcat(out_ref, b, size)
            || memcmp(out_ft, out_ref, sizeof(out_ft))) ok[6] = 0;
        if (ft_strnstr(a, b, n) != reference_strnstr(a, b, n))
            ok[7] = 0;
        if (ft_strnstr(a, "", n) != a) ok[8] = 0;
        i++;
    }
    check_int("strlen vs glibc", ok[0], 1);
    check_int("strchr vs glibc (incl. int-to-byte)", ok[1], 1);
    check_int("strrchr vs glibc (incl. int-to-byte)", ok[2], 1);
    check_int("strncmp sign vs glibc", ok[3], 1);
    check_int("strdup vs glibc contract", ok[4], 1);
    check_int("strlcpy vs BSD reference", ok[5], 1);
    check_int("strlcat vs BSD reference", ok[6], 1);
    check_int("strnstr vs independent reference", ok[7], 1);
    check_int("strnstr empty needle", ok[8], 1);
}

static void fill_random_bytes(unsigned char *a, size_t n)
{
    size_t i;

    i = 0;
    while (i < n)
    {
        a[i] = (unsigned char)next_random();
        i++;
    }
}

static void test_glibc_memory_random(void)
{
    unsigned char a[128];
    unsigned char b[128];
    unsigned char c[128];
    unsigned char d[128];
    size_t n;
    size_t from;
    size_t to;
    int ch;
    int ok[7];
    int i;

    section("GLIBC: 12000 randomized binary/memory rounds");
    for (i = 0; i < 7; i++)
        ok[i] = 1;
    i = 0;
    while (i < 12000)
    {
        fill_random_bytes(a, sizeof(a));
        fill_random_bytes(b, sizeof(b));
        memcpy(c, a, sizeof(a));
        memcpy(d, a, sizeof(a));
        n = next_random() % (sizeof(a) + 1);
        ch = (int)(next_random() & 0x3ff);
        if (ft_memset(c, ch, n) != c
            || memset(d, ch, n) != d || memcmp(c, d, 128)) ok[0] = 0;
        memcpy(c, a, sizeof(a));
        memcpy(d, a, sizeof(a));
        ft_bzero(c, n);
        bzero(d, n);
        if (memcmp(c, d, 128)) ok[1] = 0;
        memcpy(c, a, sizeof(a));
        memcpy(d, a, sizeof(a));
        if (ft_memcpy(c, b, n) != c || memcpy(d, b, n) != d
            || memcmp(c, d, 128)) ok[2] = 0;
        memcpy(c, a, sizeof(a));
        memcpy(d, a, sizeof(a));
        from = next_random() % 128;
        to = next_random() % 128;
        n = next_random() % (129 - (from > to ? from : to));
        if (ft_memmove(c + to, c + from, n) != c + to
            || memmove(d + to, d + from, n) != d + to
            || memcmp(c, d, 128)) ok[3] = 0;
        n = next_random() % 129;
        if (ft_memchr(a, ch, n) != memchr(a, ch, n)) ok[4] = 0;
        if (!same_sign(ft_memcmp(a, b, n), memcmp(a, b, n)))
            ok[5] = 0;
        if (!same_sign(ft_memcmp(a, a, n), memcmp(a, a, n)))
            ok[6] = 0;
        i++;
    }
    check_int("memset vs glibc", ok[0], 1);
    check_int("bzero vs glibc", ok[1], 1);
    check_int("memcpy vs glibc (non-overlap)", ok[2], 1);
    check_int("memmove vs glibc (overlaps)", ok[3], 1);
    check_int("memchr vs glibc", ok[4], 1);
    check_int("memcmp sign vs glibc", ok[5], 1);
    check_int("memcmp same buffer", ok[6], 1);
}

static void test_glibc_calloc_random(void)
{
    unsigned char *a;
    unsigned char *b;
    size_t count;
    size_t size;
    size_t n;
    size_t k;
    int i;
    int ok;

    section("GLIBC: calloc 2000 valid allocation cases");
    ok = 1;
    i = 0;
    while (i < 2000)
    {
        count = next_random() % 50;
        size = next_random() % 50;
        n = count * size;
        a = ft_calloc(count, size);
        b = calloc(count, size);
        if ((a == NULL) != (b == NULL)) ok = 0;
        if (a)
        {
            k = 0;
            while (k < n)
            {
                if (a[k] != 0) ok = 0;
                k++;
            }
        }
        free(a);
        free(b);
        i++;
    }
    check_int("calloc return policy and zero bytes vs glibc", ok, 1);
}

/* ================================================================ */
/* PART 2 — STRING OWNERSHIP / DATA / LONG-RUN STRESS                 */
/* ================================================================ */

static int reference_trimmed(const char *s, const char *set, char *out)
{
    size_t begin;
    size_t end;
    size_t i;

    begin = 0;
    end = strlen(s);
    while (begin < end && strchr(set, (unsigned char)s[begin]))
        begin++;
    while (end > begin && strchr(set, (unsigned char)s[end - 1]))
        end--;
    i = 0;
    while (begin < end)
        out[i++] = s[begin++];
    out[i] = '\0';
    return (1);
}

static void test_part2_random_strings(void)
{
    char a[90];
    char b[90];
    char expected[190];
    char trimmed[90];
    char *p;
    size_t start;
    size_t n;
    size_t actual;
    int ok[4];
    int i;

    section("PART 2: 4000 randomized substr/join/trim/itoa rounds");
    for (i = 0; i < 4; i++) ok[i] = 1;
    i = 0;
    while (i < 4000)
    {
        random_string(a, 80);
        random_string(b, 80);
        start = next_random() % 100;
        n = next_random() % 100;
        actual = strlen(a);
        p = ft_substr(a, (unsigned int)start, n);
        if (start >= actual) expected[0] = '\0';
        else
        {
            actual -= start;
            if (n < actual) actual = n;
            memcpy(expected, a + start, actual);
            expected[actual] = '\0';
        }
        if (!p || strcmp(p, expected)) ok[0] = 0;
        free(p);
        strcpy(expected, a);
        strcat(expected, b);
        p = ft_strjoin(a, b);
        if (!p || strcmp(p, expected)) ok[1] = 0;
        free(p);
        reference_trimmed(a, "xYZ \t", trimmed);
        p = ft_strtrim(a, "xYZ \t");
        if (!p || strcmp(p, trimmed)) ok[2] = 0;
        free(p);
        {
            int value = (int)next_random();
            char decimal[32];
            snprintf(decimal, sizeof(decimal), "%d", value);
            p = ft_itoa(value);
            if (!p || strcmp(p, decimal)) ok[3] = 0;
            free(p);
        }
        i++;
    }
    check_int("substr reference", ok[0], 1);
    check_int("strjoin reference", ok[1], 1);
    check_int("strtrim reference", ok[2], 1);
    check_int("itoa vs snprintf (full int range)", ok[3], 1);
}

static int split_matches(const char *s, char delimiter, char **words)
{
    const char *p;
    const char *start;
    size_t len;
    size_t i;

    if (!words) return (0);
    p = s;
    i = 0;
    while (*p)
    {
        while (*p && *p == delimiter) p++;
        if (!*p) break;
        start = p;
        while (*p && *p != delimiter) p++;
        len = (size_t)(p - start);
        if (!words[i] || strlen(words[i]) != len
            || memcmp(words[i], start, len)) return (0);
        i++;
    }
    return (words[i] == NULL);
}

static void test_part2_split_random(void)
{
    char text[160];
    char old[160];
    char **parts;
    char delimiter;
    int i;
    int ok;

    section("PART 2: 4000 randomized split patterns");
    ok = 1;
    i = 0;
    while (i < 4000)
    {
        size_t j = 0;
        size_t len = next_random() % 150;
        delimiter = ",|x \t\n"[next_random() % 6];
        while (j < len)
        {
            text[j] = "xx,|abc  \t\n"[next_random() % 11];
            j++;
        }
        text[j] = '\0';
        strcpy(old, text);
        parts = ft_split(text, delimiter);
        if (!split_matches(old, delimiter, parts)
            || strcmp(old, text)) ok = 0;
        free_split(parts);
        i++;
    }
    check_int("split token content/count + source unchanged", ok, 1);
}

static char map_xor_index(unsigned int i, char c)
{
    if (i % 2 == 0 && c >= 'a' && c <= 'z')
        return ((char)(c - 32));
    return (c);
}

static void iter_xor_index(unsigned int i, char *c)
{
    *c = map_xor_index(i, *c);
}

static void test_part2_maps_random(void)
{
    char s[120];
    char expected[120];
    char mutable[120];
    char *mapped;
    size_t i;
    int round;
    int ok[2];

    section("PART 2: 3000 index-sensitive mapi/iteri rounds");
    ok[0] = 1;
    ok[1] = 1;
    round = 0;
    while (round < 3000)
    {
        random_string(s, 110);
        strcpy(mutable, s);
        i = 0;
        while (s[i])
        {
            expected[i] = map_xor_index((unsigned int)i, s[i]);
            i++;
        }
        expected[i] = '\0';
        mapped = ft_strmapi(s, map_xor_index);
        if (!mapped || memcmp(expected, mapped, i + 1)
            || strcmp(s, mutable)) ok[0] = 0;
        free(mapped);
        ft_striteri(mutable, iter_xor_index);
        if (memcmp(expected, mutable, i + 1)) ok[1] = 0;
        round++;
    }
    check_int("strmapi output and input immutability", ok[0], 1);
    check_int("striteri in-place index effects", ok[1], 1);
}

static void test_part2_fd_long(void)
{
    char name[] = "/tmp/libft-test-XXXXXX";
    char payload[4097];
    char output[4110];
    int fd;
    int ok;
    int i;
    ssize_t n;

    section("PART 2: large file descriptor writes");
    fd = mkstemp(name);
    if (fd < 0)
    {
        check_int("mkstemp test setup", 0, 1);
        return ;
    }
    unlink(name);
    i = 0;
    while (i < 4096)
    {
        payload[i] = 'A' + (i % 26);
        i++;
    }
    payload[4096] = '\0';
    ft_putstr_fd(payload, fd);
    lseek(fd, 0, SEEK_SET);
    n = read(fd, output, sizeof(output));
    ok = n == 4096 && !memcmp(payload, output, 4096);
    check_int("putstr_fd 4096-byte content", ok, 1);
    ftruncate(fd, 0);
    lseek(fd, 0, SEEK_SET);
    ft_putendl_fd(payload, fd);
    lseek(fd, 0, SEEK_SET);
    n = read(fd, output, sizeof(output));
    ok = n == 4097 && !memcmp(payload, output, 4096)
        && output[4096] == '\n';
    check_int("putendl_fd 4096 bytes + newline", ok, 1);
    close(fd);
}

static void test_lists_large(void)
{
    int values[256];
    t_list *head;
    t_list *node;
    t_list *mapped;
    int i;
    int ok;

    section("LISTS: 256 nodes / ownership / callbacks");
    head = NULL;
    i = 0;
    ok = 1;
    while (i < 256)
    {
        values[i] = i;
        node = ft_lstnew(&values[i]);
        if (!node)
        {
            ok = 0;
            break ;
        }
        ft_lstadd_back(&head, node);
        i++;
    }
    if (ok && ft_lstsize(head) != 256) ok = 0;
    if (ok && ft_lstlast(head)->content != &values[255]) ok = 0;
    g_iter_seen = 0;
    ft_lstiter(head, iter_seen);
    if (g_iter_seen != i) ok = 0;
    mapped = NULL;
    if (ok) mapped = ft_lstmap(head, map_copy, free);
    if (ok && (!mapped || ft_lstsize(mapped) != 256)) ok = 0;
    if (mapped)
    {
        t_list *curr = mapped;
        int j = 0;
        while (curr && j < 256)
        {
            if (!curr->content || *(int *)curr->content != values[j])
                ok = 0;
            curr = curr->next;
            j++;
        }
        ft_lstclear(&mapped, free);
        if (mapped != NULL) ok = 0;
    }
    while (head)
    {
        node = head->next;
        free(head);
        head = node;
    }
    check_int("list 256 nodes + deep map + clear", ok, 1);
}

static void run_more_tests(void)
{
    test_glibc_character_domain();
    test_glibc_atoi_random();
    test_glibc_strings_random();
    test_glibc_memory_random();
    test_glibc_calloc_random();
    test_part2_random_strings();
    test_part2_split_random();
    test_part2_maps_random();
    test_part2_fd_long();
    test_lists_large();
}

/* ================================================================ */
/* OPTIONAL NULL INPUT AUDIT: RUN IN CHILD PROCESSES                 */
/* Unlike valid-input tests, these are informational and do not     */
/* make the main Libft suite fail. Several are outside the subject. */
/* ================================================================ */

typedef struct s_null_case
{
    const char *label;
    int id;
} t_null_case;

static void null_probe(int id)
{
    char buf[16] = "abc";
    t_list list;
    t_list *head;
    int value = 1;

    head = &list;
    list.content = &value;
    list.next = NULL;
    switch (id)
    {
        case 0: _exit(ft_substr(NULL, 0, 1) == NULL ? 0 : 2);
        case 1: _exit(ft_strjoin(NULL, "x") == NULL ? 0 : 2);
        case 2: _exit(ft_strjoin("x", NULL) == NULL ? 0 : 2);
        case 3: _exit(ft_strjoin(NULL, NULL) == NULL ? 0 : 2);
        case 4: _exit(ft_strtrim(NULL, "x") == NULL ? 0 : 2);
        case 5: _exit(ft_strtrim("x", NULL) == NULL ? 0 : 2);
        case 6: _exit(ft_split(NULL, ',') == NULL ? 0 : 2);
        case 7: _exit(ft_strmapi(NULL, map_count) == NULL ? 0 : 2);
        case 8: _exit(ft_strmapi("abc", NULL) == NULL ? 0 : 2);
        case 9: ft_striteri(NULL, iter_count); _exit(0);
        case 10: ft_striteri(buf, NULL); _exit(0);
        case 11: ft_putstr_fd(NULL, 1); _exit(0);
        case 12: ft_putendl_fd(NULL, 1); _exit(0);
        case 13: _exit(ft_strlen(NULL) == 0 ? 0 : 2);
        case 14: _exit(ft_strchr(NULL, 'x') == NULL ? 0 : 2);
        case 15: _exit(ft_strrchr(NULL, 'x') == NULL ? 0 : 2);
        case 16: _exit(ft_strncmp(NULL, "x", 1) == 0 ? 0 : 2);
        case 17: _exit(ft_strncmp("x", NULL, 1) == 0 ? 0 : 2);
        case 18: _exit(ft_strlcpy(buf, NULL, sizeof(buf)) == 0 ? 0 : 2);
        case 19: _exit(ft_strlcat(buf, NULL, sizeof(buf)) == 0 ? 0 : 2);
        case 20: _exit(ft_strnstr(NULL, "x", 2) == NULL ? 0 : 2);
        case 21: _exit(ft_strnstr("x", NULL, 2) == NULL ? 0 : 2);
        case 22: _exit(ft_memset(NULL, 0, 1) == NULL ? 0 : 2);
        case 23: ft_bzero(NULL, 1); _exit(0);
        case 24: _exit(ft_memcpy(buf, NULL, 1) == NULL ? 0 : 2);
        case 25: _exit(ft_memmove(buf, NULL, 1) == NULL ? 0 : 2);
        case 26: _exit(ft_memchr(NULL, 'a', 1) == NULL ? 0 : 2);
        case 27: _exit(ft_memcmp(NULL, buf, 1) == 0 ? 0 : 2);
        case 28: _exit(ft_atoi(NULL) == 0 ? 0 : 2);
        case 29: _exit(ft_strdup(NULL) == NULL ? 0 : 2);
        case 30: ft_lstadd_front(NULL, &list); _exit(0);
        case 31: ft_lstadd_back(NULL, &list); _exit(0);
        case 32: ft_lstdelone(NULL, free); _exit(0);
        case 33: ft_lstclear(NULL, free); _exit(0);
        case 34: ft_lstiter(NULL, iter_seen); _exit(0);
        case 35: _exit(ft_lstmap(NULL, map_copy, free) == NULL ? 0 : 2);
        case 36: _exit(ft_lstlast(NULL) == NULL ? 0 : 2);
        case 37: _exit(ft_lstsize(NULL) == 0 ? 0 : 2);
        case 38: _exit(ft_lstmap(&list, NULL, free) == NULL ? 0 : 2);
        case 39: _exit(ft_lstmap(&list, map_copy, NULL) == NULL ? 0 : 2);
        case 40: ft_lstdelone(&list, NULL); _exit(0);
        case 41: ft_lstclear(&head, NULL); _exit(0);
        case 42: ft_lstiter(&list, NULL); _exit(0);
        case 43: ft_lstadd_front(&head, NULL); _exit(0);
        case 44: ft_lstadd_back(&head, NULL); _exit(0);
        default: _exit(3);
    }
}

static void run_null_case(const char *label, int id, int strict)
{
    pid_t pid;
    int status;

    fflush(NULL); /* Avoid printing buffered output twice after fork(). */
    pid = fork();
    if (pid < 0)
    {
        printf("  [ERROR] %-45s fork() failed\n", label);
        if (strict) g_failed++;
        return ;
    }
    if (pid == 0)
    {
        struct rlimit lim = {0, 0};
        int devnull = open("/dev/null", O_WRONLY);
        setrlimit(RLIMIT_CORE, &lim); /* No core files from intentional crashes. */
        if (devnull >= 0)
        {
            dup2(devnull, STDOUT_FILENO);
            close(devnull);
        }
        null_probe(id);
        _exit(3);
    }
    if (waitpid(pid, &status, 0) < 0)
    {
        printf("  [ERROR] %-45s waitpid() failed\n", label);
        if (strict) g_failed++;
        return ;
    }
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        printf("  [PASS] %-45s returned without crashing\n", label);
        if (strict) g_passed++;
    }
    else if (WIFSIGNALED(status))
    {
        printf("  [%s] %-45s signal %d\n", strict ? "FAIL" : "CRASH",
            label, WTERMSIG(status));
        if (strict) g_failed++;
    }
    else
    {
        printf("  [%s] %-45s unexpected child exit %d\n",
            strict ? "FAIL" : "OTHER", label,
            WIFEXITED(status) ? WEXITSTATUS(status) : -1);
        if (strict) g_failed++;
    }
}

static void test_null_guards(int all, int strict)
{
    static const t_null_case cases[] = {
        {"ft_substr(NULL,...)", 0},
        {"ft_strjoin(NULL,\"x\")", 1},
        {"ft_strjoin(\"x\",NULL)", 2},
        {"ft_strjoin(NULL,NULL)", 3},
        {"ft_strtrim(NULL,\"x\")", 4},
        {"ft_strtrim(\"x\",NULL)", 5},
        {"ft_split(NULL,',')", 6},
        {"ft_strmapi(NULL,f)", 7},
        {"ft_strmapi(\"abc\",NULL)", 8},
        {"ft_striteri(NULL,f)", 9},
        {"ft_striteri(\"abc\",NULL)", 10},
        {"ft_putstr_fd(NULL,1)", 11},
        {"ft_putendl_fd(NULL,1)", 12},
        {"ft_strlen(NULL)", 13},
        {"ft_strchr(NULL,'x')", 14},
        {"ft_strrchr(NULL,'x')", 15},
        {"ft_strncmp(NULL,\"x\",1)", 16},
        {"ft_strncmp(\"x\",NULL,1)", 17},
        {"ft_strlcpy(buf,NULL,16)", 18},
        {"ft_strlcat(buf,NULL,16)", 19},
        {"ft_strnstr(NULL,\"x\",2)", 20},
        {"ft_strnstr(\"x\",NULL,2)", 21},
        {"ft_memset(NULL,0,1)", 22},
        {"ft_bzero(NULL,1)", 23},
        {"ft_memcpy(buf,NULL,1)", 24},
        {"ft_memmove(buf,NULL,1)", 25},
        {"ft_memchr(NULL,'a',1)", 26},
        {"ft_memcmp(NULL,buf,1)", 27},
        {"ft_atoi(NULL)", 28},
        {"ft_strdup(NULL)", 29},
        {"ft_lstadd_front(NULL,node)", 30},
        {"ft_lstadd_back(NULL,node)", 31},
        {"ft_lstdelone(NULL,free)", 32},
        {"ft_lstclear(NULL,free)", 33},
        {"ft_lstiter(NULL,f)", 34},
        {"ft_lstmap(NULL,f,free)", 35},
        {"ft_lstlast(NULL)", 36},
        {"ft_lstsize(NULL)", 37},
        {"ft_lstmap(node,NULL,free)", 38},
        {"ft_lstmap(node,f,NULL)", 39},
        {"ft_lstdelone(node,NULL)", 40},
        {"ft_lstclear(&head,NULL)", 41},
        {"ft_lstiter(node,NULL)", 42},
        {"ft_lstadd_front(&head,NULL)", 43},
        {"ft_lstadd_back(&head,NULL)", 44}
    };
    size_t i;
    size_t count = sizeof(cases) / sizeof(cases[0]);

    section(all ? "OPTIONAL NULL AUDIT: Parts 1, 2 and linked lists"
                : "OPTIONAL NULL AUDIT: Part 2 only");
    puts("  [GUARDED] = returned safely with the specified defensive result");
    puts("  [CRASH]   = signal (usually a NULL dereference)");
    puts("  [OTHER]   = returned safely but not with the probed result");
    puts(strict ? "  Part 2 NULL checks are strict (your requested policy)."
                : "  Extra invalid-input checks are informational only.");
    i = 0;
    while (i < count)
    {
        if (all || cases[i].id <= 12)
            run_null_case(cases[i].label, cases[i].id, strict && cases[i].id <= 12);
        i++;
    }
}

int main(int argc, char **argv)

{

    setlocale(LC_CTYPE, "C");
    setvbuf(stdout, NULL, _IOLBF, 0);
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        puts("Usage: ./test [--null | --null-all | --no-null | --help]");
        puts("  default    all correctness tests and STRICT Part 2 NULL tests");
        puts("  --null     only strict Part 2 NULL guards");
        puts("  --null-all informational NULL probes for all parts");
        puts("  --no-null  correctness and differential tests only");
        return (0);
    }
    if (argc > 1 && strcmp(argv[1], "--null-all") == 0)
    {
        test_null_guards(1, 0);
        return (0);
    }
    if (argc > 1 && strcmp(argv[1], "--null") == 0)
    {
        test_null_guards(0, 1);
        printf("RESULTS: %d passed, %d failed\n", g_passed, g_failed);
        return (g_failed != 0);
    }
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

    test_lstnew();

    test_lstadd_front();

    test_lstsize();

    test_lstlast();

    test_lstadd_back();

    test_lstdelone();

    test_lstclear();

    test_lstiter();

    test_lstmap();



    test_extra_classification();
    test_extra_strings_part1();
    test_extra_strnstr();
    test_extra_memory();
    test_extra_atoi_calloc_strdup();
    test_extra_substr_join_trim();
    test_extra_split();
    test_extra_maps_and_numbers();
    test_extra_fd();
    test_extra_lists();

    test_exhaustive_calloc_zero();
    test_exhaustive_classification();
    test_exhaustive_itoa();
    test_exhaustive_atoi();
    test_long_strings();
    test_memmove_thorough();
    test_strlcat_sizes();
    test_strnstr_needle_longer();
    test_split_long_word();
    test_strchr_all_bytes();
    test_memchr_all_bytes();

    run_more_tests();
    if (argc <= 1 || strcmp(argv[1], "--no-null") != 0)
        test_null_guards(0, 1);

    printf("\n====================================\n");

    printf("RESULTS: %d passed, %d failed\n", g_passed, g_failed);

    printf("====================================\n");



    return (g_failed != 0);

}
