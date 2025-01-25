/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaiz-lo <asaiz-lo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:31:20 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 11:20:34 by asaiz-lo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	int		i;

	if (s == NULL)
		return (NULL);
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
		i++;
	if (start + 1 > (unsigned int)i)
	{
		new[0] = '\0';
		return (new);
	}
	i = 0;
	while (i < (int)len && s[start + i] != '\0')
	{
		new[i] = s[start + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
