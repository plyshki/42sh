/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_dsp_caret_left.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

static void		fn_write_line(register t_main_42sh *array,
register size_t n, register size_t count)
{
	char						buff[sizeof(uintmax_t) * 3];
	register t_write_buff		*out;

	ft_write_buffer_str_zero((out = &array->out), "\xD");//Сначала для удобства сместим  в крайне правое положение
	if (count != 0)
	{
		ft_write_buffer_str_zero(out, "\x1b[");
		ft_write_buffer_str(out, buff, ft_itoa(buff, count, 10, ITOA_LOWER));
		ft_write_buffer_str_zero(out, "A");
	}
	if (n == 0)
		return ;
	ft_write_buffer_str_zero(out, "\x1b[");
	ft_write_buffer_str(out, buff, ft_itoa(buff, n, 10, ITOA_LOWER));
	ft_write_buffer_str_zero(out, "C");
}

static size_t	fn_slesh_pre(register t_main_42sh *array,
register t_in_42sh *list, register t_slesh_42sh **spl_slesh, size_t *offset)
{
	register t_slesh_42sh		**spl_tmp;
	register size_t				count;
	register size_t				count_litter;

	spl_tmp = list->spl_slesh;
	count = 0;
	count_litter = array->msg.pre_msg_litter;
	while (spl_tmp < spl_slesh)//Подсчитаем количество байт//Подсчитаем количество букв что бы знать текущую позицию относительно списка переводов строк
	{
		count_litter += spl_tmp[0]->count_litter;
		count += spl_tmp++[0]->count + 2;
	}
	count_litter += spl_slesh[0]->count_litter;
	count += spl_slesh[0]->count;
	*offset = count_litter - list->count_litter_current;
	return (spl_slesh[0]->count - (count - (list->lp_current - list->lp_b)));
}

static size_t	fn_stub(register t_main_42sh *array,
register t_in_42sh *list, size_t offset)
{
	return ((list->slesh_current == 0 ? array->msg.pre_msg_litter : 0) +
	list->spl_slesh[list->slesh_current]->count_litter - offset);
}

static void		fn_slesh(register t_main_42sh *array,
register t_in_42sh *list, register size_t count_litter, register size_t n_byte)
{
	register t_slesh_42sh		**spl_slesh;
	register size_t				count;
	size_t						offset;
	register size_t				ws_col;
	register size_t				n;

	spl_slesh = &list->spl_slesh[list->slesh_current];
	ws_col = array->ws.ws_col;
	count = 0;
	if ((n = fn_slesh_pre(array, list, spl_slesh, &offset)) < n_byte)
	{
		count_litter -= spl_slesh[0]->count_litter - offset;
		count += (spl_slesh--[0]->count_litter - offset) / ws_col + 1;
		offset = 0;
		list->slesh_current--;
		while ((n += spl_slesh[0]->count + 2) < n_byte)
		{
			count_litter -= spl_slesh[0]->count_litter;
			count += spl_slesh--[0]->count_litter / ws_col + 1;
			list->slesh_current--;
		}
	}
	n = (fn_stub(array, list, offset) - count_litter) % ws_col;
	fn_write_line(array, n, count + (n + count_litter) / ws_col);
}

void			ft_42sh_dsp_caret_left(register t_main_42sh *array,
register t_in_42sh *list, register size_t count_litter, register size_t count)
{
	register size_t				tempos;
	register size_t				ws_col;

	if (count == 0)
		return ;
	if (list->spl_slesh != 0)
	{
		fn_slesh(array, list, count_litter, count);
		list->lp_current -= count;
		list->count_litter_current -= count_litter;
		return ;
	}
	list->lp_current -= count;
	ws_col = array->ws.ws_col;
	if (count_litter > (tempos = list->count_litter_current % ws_col))
	{
		tempos = count_litter - tempos;
		count = tempos / ws_col;
		count += tempos % ws_col != 0 ? 1 : 0;
	}
	else
		count = 0;
	fn_write_line(array, (list->count_litter_current -= count_litter) % ws_col,
	count);
}
