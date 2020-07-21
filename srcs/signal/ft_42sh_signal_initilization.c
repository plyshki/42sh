/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_signal_initilization.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_42sh_signal.h"

static char	**fn_set(void)
{
	static	void		*lp_msg_sgnl[SIGUSR2 + 1] = {MSG_JOBS_NULL_42SH,//+1 на нулеввой
	MSG_JOBS_HUP_42SH, MSG_JOBS_INT_42SH, MSG_JOBS_QUIT_42SH,
	MSG_JOBS_ILL_42SH, MSG_JOBS_TRAP_42SH, MSG_JOBS_ABRT_42SH,
	MSG_JOBS_EMT_42SH, MSG_JOBS_FPE_42SH, MSG_JOBS_KILL_42SH,
	MSG_JOBS_BUS_42SH, MSG_JOBS_SEGV_42SH, MSG_JOBS_SYS_42SH,
	MSG_JOBS_PIPE_42SH, MSG_JOBS_ALRM_42SH, MSG_JOBS_TERM_42SH,
	MSG_JOBS_GURD_42SH, MSG_JOBS_STOP_42SH, MSG_JOBS_STR_42SH,
	MSG_JOBS_CONT_42SH, MSG_JOBS_CHLD_42SH, MSG_JOBS_TTIN_42SH,
	MSG_JOBS_TTON_42SH, MSG_JOBS_GIO_42SH, MSG_JOBS_XCPU_42SH,
	MSG_JOBS_XFSZ_42SH, MSG_JOBS_TALR_42SH, MSG_JOBS_PROF_42SH,
	MSG_JOBS_WINC_42SH, MSG_JOBS_INFO_42SH, MSG_JOBS_USR1_42SH,
	MSG_JOBS_USR2_42SH};

	return ((char **)lp_msg_sgnl);
}

void		ft_42sh_signal_initilization_script(register t_main_42sh *array)//Иницилизируем сигналы
{
	array->pr.lp_msg_sgnl = fn_set();
	if ((array->b_mode & MODE_SIGCHILD_42SH) != 0)//Проверяем какой режим работы шела
		signal(SIGCHLD, ft_42sh_signal_child);//Смотрим за дочерним процессыми убитыми без моего ведома  если не в реиме валгринд
}

void		ft_42sh_signal_initilization(register t_main_42sh *array)//Иницилизируем сигналы
{
	array->pr.lp_msg_sgnl = fn_set();
	signal(SIGWINCH, ft_42sh_signal_winch);//Установим обработчик для кантроля размера терминалп
	if ((array->b_mode & MODE_SIGCHILD_42SH) != 0)//Проверяем какой режим работы шела
		signal(SIGCHLD, ft_42sh_signal_child);//Смотрим за дочерним процессыми убитыми без моего ведома  если не в реиме валгринд
	signal(SIGTSTP, SIG_IGN);//Сигнал остановки процесса //Игнорим//Что бы с дочернего канонического не перешло
	signal(SIGQUIT, SIG_IGN);//Сигнал выхода процесса //Игнорим ctr '\'//Что бы с дочернего канонического не перешло
	signal(SIGINT, SIG_IGN);//Сигнал выхода процесса //Игнорим ctr 'C'//Что бы с дочернего канонического не перешло
	signal(SIGTTOU, SIG_IGN);//Что бы в случае если  в фоне не уходило в suspend
	signal(SIGTTIN, SIG_IGN);//Что бы в случае если  в фоне не уходило в suspend
	signal(SIGTERM, SIG_IGN);//Запретим просто так завершать нас
}

void		ft_42sh_signal_default(register t_main_42sh *array,
register t_jobs_42sh *jobs)//Востановим обработчики сигналов
{
	signal(SIGWINCH, SIG_DFL);//Установим обработчик для кантроля размера терминалп
	if ((array->b_mode & MODE_SIGCHILD_42SH) != 0)//Проверяем какой режим работы шела
		signal(SIGCHLD, SIG_DFL);//Смотрим за дочерним процессыми убитыми без моего ведома
	signal(SIGTSTP, SIG_DFL);//Сигнал остановки процесса
	signal(SIGQUIT, SIG_DFL);//Сигнал выхода процесса
	signal(SIGINT, SIG_DFL);//Сигнал выхода процесса
	signal(SIGTTOU, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTERM, SIG_DFL);//Установим обработчик на случай если нас попытаються завершить
	(void)jobs;
}
