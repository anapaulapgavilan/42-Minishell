/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaiz-lo <asaiz-lo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:30:58 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 11:19:33 by asaiz-lo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*res;
	size_t	i;

	i = 0;
	res = malloc((n + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[n] = '\0';
	return (res);
}
