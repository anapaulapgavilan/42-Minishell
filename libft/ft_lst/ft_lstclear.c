/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaiz-lo <asaiz-lo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:25:20 by ana-pper          #+#    #+#             */
/*   Updated: 2024/12/14 11:11:31 by asaiz-lo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*current;
	t_list	*prev;

	current = *lst;
	if (*lst == NULL || !current || !del)
		return ;
	while (current->next != NULL)
	{
		prev = current;
		del(current->content);
		ft_memdel(current);
		current = prev->next;
	}
	del(current->content);
	ft_memdel(current);
	*lst = NULL;
}
