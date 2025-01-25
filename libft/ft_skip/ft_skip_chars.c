/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_chars.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-pper <ana-pper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:29:38 by ana-pper          #+#    #+#             */
/*   Updated: 2024/10/30 12:29:41 by ana-pper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	ft_skip_chars(const char *str, int *i, char *base)
{
	while (ft_isin(str[*i], base))
		(*i)++;
}
