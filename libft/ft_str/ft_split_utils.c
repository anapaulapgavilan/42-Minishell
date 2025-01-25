/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 12:30:52 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 17:04:40 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	letter_count(const char *s, char c, int start)
{
	int	length;

	length = 0;
	while (s[start] != c && s[start] != '\0')
	{
		length++;
		start++;
	}
	return (length);
}

void	skip_delimiters(const char *s, char c, int *index)
{
	while (s[*index] == c && s[*index] != '\0')
	{
		(*index)++;
	}
}

int	word_count(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}
