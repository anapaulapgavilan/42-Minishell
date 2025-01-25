/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaiz-lo <asaiz-lo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:26:44 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 11:13:42 by asaiz-lo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*space;
	size_t	i;

	i = 0;
	space = malloc(size * count + 1);
	if (!space)
		return (NULL);
	while (i < count * size + 1)
		space[i++] = '\0';
	return (space);
}
