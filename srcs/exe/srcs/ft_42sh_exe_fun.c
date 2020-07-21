/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_exe_fun.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_exe.h"

static size_t	fn_test_fork(register t_jobs_42sh *jobs)
{

	if ((jobs->b_type & AUTO_TYPE_RUN_42SH) != 0)
		return (1);
	if (jobs->fd_pipe == 0)//Нету пайпа тоже не запускаем в отдельном или есть пайп но устанавливаем локальные переменные
		return (0);
	return (1);
}

static void		fn_not_fork(register t_main_42sh *array,
register t_jobs_42sh *jobs, char **lp_arg,
void (*f)(register t_main_42sh *array, register char **lp_arg))
{
	ioctl(FD_TERMINAL_42SH, TIOCSPGRP, &array->pr.pid_main);//Может быть после пайпа не главным//На ошибку не провераем так как если главный уже то вернет ошибку
	ft_42sh_exe_fun_run(array, jobs, lp_arg, f);
}

static void		fn_fork(register t_main_42sh *array,
register t_jobs_42sh *jobs, char **lp_arg,
void (*f)(register t_main_42sh *array, register char **lp_arg))
{
	ft_42sh_exe_fun_run(array, jobs, lp_arg, f);
	ft_42sh_cm_exit_fun(array, array->env.exit_status->number);
}

static size_t	fn_error(register t_main_42sh *array,
register t_jobs_42sh *jobs, int *fds)
{
	close(fds[PIPE_READ_42SH]);
	close(fds[PIPE_WRITE_42SH]);
	return (ft_42sh_exe_err_foor(array, jobs));
}

size_t			ft_42sh_exe_fun(register t_main_42sh *array,
register t_jobs_42sh *jobs, char **lp_arg,
void (*f)(register t_main_42sh *array, register char **lp_arg))//fd ->write | read<-fd fd2->write | read<-fd2 fd->write | read<-fd
{
	register pid_t				pid;
	int							fds[2];

	if (fn_test_fork(jobs) != 0)//((tmp = jobs->next) == 0 || tmp->count == 1)//Потом сделать что бы последний был не в процессе но тут проплемма с приотановкай//!!!
	{
		if (pipe(&fds[0]) != 0)
			ft_42sh_exit(E_PIPE_CODE_42SH, __FILE__, __func__, __LINE__);
		if ((pid = fork()) == 0)
		{
			ft_42sh_exe_fork(array, jobs, &fds[0]);//Так как мы долго тут не будем и пайп тем самым ну никак не переполнимм то в булт ин не будем закрывать не нужные fd по правильному надо бы
			fn_fork(array, jobs, lp_arg, f);
		}
		else if (pid < 0)//Если ошибка то прекрашаем но сначала закроем лишний пайп
			return (fn_error(array, jobs, &fds[0]));
		ft_42sh_exe_grup(array, jobs, pid, &fds[0]);
		return (1);
	}
	fn_not_fork(array, jobs, lp_arg, f);
	if (jobs->stat_loc == STATUS_JOBS_STOP_42SH)//Значит прекращаем дальнейший выполнение команд
		return (0);
	if (jobs->n != 0)//Если без форка бултын то array->pr.pid_grup == 0 и процессы перенаправления создадуться каждый в своей новой группы в этом случае на это можно забить
		if (ft_42sh_pipe_left(array, jobs) == 0 ||
		ft_42sh_pipe_right(array, jobs) == 0)
			return (ft_42sh_exe_err_foor(array, jobs));
	return (1);
}
