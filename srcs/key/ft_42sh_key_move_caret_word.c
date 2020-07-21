/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_key_move_caret_word.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

static void			*fn_offset_left(register unsigned char *current)
{
	register unsigned char		litter;

	while (((litter = current--[-1]) & 0x80) != 0 && (litter & 0x40) == 0)//Учитываем utf-8 что бы знать на сколько байт сместить в лево
		;
	return (current);
}

void				ft_42sh_key_word_left(register t_main_42sh *array,
register t_in_42sh *list)
{
	register unsigned char		*current;
	register unsigned char		*b;
	register size_t				count_litter;
	register unsigned char		litter;

	b = (unsigned char *)list->lp_b;
	current = (unsigned char *)list->lp_current;
	if (current == b)//Что бы не ути за левую границу
		return ;
	count_litter = 0;
	while (current > b && (litter = current[-1]) <= 0x20)
	{
		if (litter == '\n')
			current--;
		else
			count_litter++;
		current--;
	}
	while (current > b && current[-1] > 0x20)
	{
		count_litter++;
		current = fn_offset_left(current);
	}
	ft_42sh_dsp_caret_left(array, list, count_litter,
	list->lp_current - (char *)current);
}

static void			fn_whiles_right(register t_main_42sh *array,
register t_in_42sh *list, register unsigned char *current,
register unsigned char *end)
{
	register size_t				count_litter;
	register unsigned char		litter;

	count_litter = 0;
	while (current < end && (litter = current[0]) <= 0x20)
	{
		count_litter += (litter != '\n') ? 1 : -1;
		current++;
	}
	while (current < end && (litter = current[0]) > 0x20)
	{
		count_litter++;
		current++;
		if ((litter & 0x80) != 0)//Учитываем utf-8 что бы знать на сколько байт сместить в право
			while (((litter = litter << 1) & 0x80) != 0)
				current++;
	}
	while (current < end && (litter = current[0]) <= 0x20)
	{
		count_litter += (litter != '\n') ? 1 : -1;
		current++;
	}
	ft_42sh_dsp_caret_right(array, list, count_litter,
	(char *)current - list->lp_current);
}

void				ft_42sh_key_word_right(register t_main_42sh *array,
register t_in_42sh *list)
{
	register unsigned char		*current;
	register unsigned char		*end;

	end = (unsigned char *)(list->lp_b + list->count);
	current = (unsigned char *)list->lp_current;
	if (list->count_litter_current == list->count_litter && end == current)//Что бы не ути за правую границу
		return ;
	fn_whiles_right(array, list, current, end);
}
