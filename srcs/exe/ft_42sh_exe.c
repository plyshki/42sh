/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_exe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_42sh_exe.h"

static size_t	fn_set(register t_main_42sh *array,
register t_jobs_42sh *tmp, register size_t b_test)
{
	if (b_test == '&')
	{
		array->pr.b_auto_view = 1;
		b_test = AUTO_TYPE_RUN_42SH;
	}
	else
	{
		array->pr.jobs_current = tmp;
		b_test = 0;
		if (ft_42sh_stub_ioctl(array, TIOCSETA, &array->tty) == -1)//Установим каонический режим
			ft_42sh_exit(E_IOTL_CODE_42SH, __FILE__, __func__, __LINE__);
	}
	return (b_test);
}

static size_t	fn_finish(register t_main_42sh *array,
register size_t tempos, register size_t out)
{
	if (tempos != 0)
	{
		array->env.exit_status->number = (unsigned char)tempos;
		return (0);
	}
	return (out);
}

static void		fn_pid_view(register t_main_42sh *array,
register t_jobs_42sh *tmp)
{
	array->pr.b_auto_view = 0;
	ft_42sh_jobs_msg(array, tmp, JOBS_MSG_ID_RUN_42SH);
	ft_write_buffer(&array->out);//Выедем на экран
}

size_t			ft_42sh_exe(register t_main_42sh *array,
register size_t b_test)
{
	register t_jobs_42sh		*jobs;
	register t_jobs_42sh		*tmp;
	register size_t				tempos;

	array->pr.jobs_last = 0;//Последний храним для проверки в пайпах
	jobs = array->pr.jb.last;
	tempos = jobs->count;
	tmp = jobs;
	while (--tempos > 0)
		tmp = tmp->prev;
	ft_write_buffer(&array->out);//Записываем все команды что на собирались//Что бы в случае редиректа или пайпа не пошли куда не надо и отобразилось как надо
	b_test = fn_set(array, tmp, b_test);
	tempos = ft_42sh_exe_while(array, tmp, b_test);
	if ((b_test & AUTO_TYPE_RUN_42SH) == 0)
		return (fn_finish(array, tempos, ft_42sh_exe_wait(array, tmp, jobs)));
	array->env.last_pid->number = jobs->pid_view;//Сохраним последний пид
	if (ft_42sh_stub_ioctl(array, TIOCSPGRP, &array->pr.pid_main) == -1)//востановим главный процесс активным
		ft_42sh_exit(E_IOTL_CODE_42SH, __FILE__, __func__, __LINE__);
	if (ft_42sh_stub_ioctl(array, TIOCSETA, &array->tty_change) == -1)//Установим не каонический режим
		ft_42sh_exit(E_IOTL_CODE_42SH, __FILE__, __func__, __LINE__);
	if (array->pr.b_auto_view != 0 && (array->b_location &
	LOCATION_SCRIPT_42SH) == 0)
		fn_pid_view(array, tmp);
	return (fn_finish(array, tempos, 1));
}
