/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_dsp_start.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

void		ft_42sh_dsp_start(register t_main_42sh *array)
{
	register t_write_buff	*out;

	out = &array->out;
	ft_write_buffer_str_zero(out, PRTF_BOLT);
	ft_write_buffer_str_zero(out, (void *)array->pwd.path_view);
	ft_write_buffer_str_zero(out, "\n"PRTF_RESET);
	ft_write_buffer_str_zero(out, array->msg.pre_msg);
	if ((array->b_location & LOCATION_SCRIPT_42SH) == 0 &&
	array->msg.pre_msg_litter % array->ws.ws_col == 0)
		ft_write_buffer_str_zero(out, " \xD");//Для того что бы перевод строки авто был вставляем пробел и потом смещаем вправо сразу что бы не было видно
}
