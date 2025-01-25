/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaiz-lo <asaiz-lo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:27:21 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 11:15:09 by asaiz-lo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	*q;
	unsigned char	*temp;

	temp = malloc(len * sizeof(unsigned char));
	i = 0;
	p = (unsigned char *)dst;
	q = (unsigned char *)src;
	while (i < len)
	{
		temp[i] = q[i];
		i++;
	}
	i = 0;
	while (i < len)
	{
		p[i] = temp[i];
		i++;
	}
	free(temp);
	return (dst);
}
