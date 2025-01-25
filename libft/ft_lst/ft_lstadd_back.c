/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaiz-lo <asaiz-lo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:25:05 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 11:09:17 by asaiz-lo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*temp;

	if (!alst)
		return ;
	if (*alst != NULL)
	{
		temp = *alst;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	else
		*alst = new;
}
