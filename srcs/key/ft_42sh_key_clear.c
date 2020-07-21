/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_key_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

void	ft_42sh_key_clear(register t_main_42sh *array,
register t_in_42sh *list)
{
	register t_write_buff		*out;
	register char				*b;
	register size_t				count;
	register size_t				count_litter;

	out = &array->out;
	ft_write_buffer_str_zero(out, "\x1b[65536A\xD\x1b[J");//Перемещаем в верх//Смещаем в крайний право //Удаляем все справо и ниже каретки
	ft_42sh_dsp_start(array);
	count = list->count;
	b = list->lp_b;
	ft_write_buffer_str(out, b, count);
	count_litter = list->count_litter - list->count_litter_current;
	count = b + count - list->lp_current;
	list->count_litter_current += count_litter;
	list->lp_current += count;
	list->slesh_current = list->slesh_max;
	ft_42sh_dsp_caret_left(array, list, count_litter, count);
}
