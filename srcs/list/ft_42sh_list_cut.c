/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_list_cut.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

static void		fn_change(register t_past_sort_42sh *root,
register t_std_key_42sh *list, register t_std_key_42sh *prev,
register t_std_key_42sh *next)
{
	if (next == 0)
	{
		prev->next = 0;
		root->last = prev;
	}
	else if (prev == 0)
	{
		next->prev = 0;
		root->first = next;
	}
	else
	{
		next->prev = prev;
		prev->next = next;
	}
	if (root->center != list)
		return ;
	root->center = (next == 0) ? prev : next;
}

void			ft_42sh_list_cut(register t_past_sort_42sh *root,
register t_std_key_42sh *list)//Вырежим данный лист
{
	register t_std_key_42sh			*prev;
	register t_std_key_42sh			*next;

	prev = list->prev;
	next = list->next;
	if (next != prev)
		fn_change(root, list, prev, next);
	else//Вырезали последний лист
		root->first = 0;
}
