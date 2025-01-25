/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_baselen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaiz-lo <asaiz-lo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:26:15 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 11:12:24 by asaiz-lo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_baselen(long n, int base)
{
	int	size;

	size = 0;
	if (base < 2)
		return (0);
	if (n == 0)
		return (1);
	if (n < 0)
		size++;
	while (n != 0)
	{
		n = n / base;
		size++;
	}
	return (size);
}
