/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:29:58 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 17:06:55 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*allocate_word(const char *s, char c, int *index)
{
	int		start;
	int		len;
	char	*word;
	int		i;

	start = *index;
	len = letter_count(s, c, start);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (s[start] != c && s[start] != '\0')
	{
		word[i++] = s[start++];
	}
	word[i] = '\0';
	*index = start;
	return (word);
}

int	populate_words(const char *s, char c, char **tab, int words)
{
	int		i;
	int		k;
	char	*word;

	i = 0;
	k = 0;
	skip_delimiters(s, c, &k);
	while (i < words)
	{
		word = allocate_word(s, c, &k);
		if (!word)
		{
			while (i-- > 0)
				free(tab[i]);
			free(tab);
			return (0);
		}
		tab[i++] = word;
		skip_delimiters(s, c, &k);
	}
	tab[i] = NULL;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	int		words;
	char	**tab;

	if (!s)
		return (NULL);
	words = word_count(s, c);
	tab = malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	if (!populate_words(s, c, tab, words))
		return (NULL);
	return (tab);
}
