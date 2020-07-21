/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_pipe_left.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_42sh_pipe.h"

static void		fn_to_file(register char *buff,
register int fd_write, register int fd_read)
{
	register ssize_t		tempos;

	while ((tempos = read(fd_read, buff, BUFFER_OUT_42SH)) > 0)
		write(fd_write, buff, tempos);
}

static void		fn_while(register t_pipe_42sh *pipe_end,
register int fd, register int fd_write, register t_pipe_search_in_42sh *in)
{
	register t_pipe_42sh	*pipe;
	register size_t			b_flag;

	pipe = in->pipe;
	while (pipe < pipe_end)
	{
		if (((b_flag = pipe->b_flag) & PIPE_CLOSE_42SH) == 0 &&
		fd == pipe->fd_1)
		{//Засчет file mode mismatch on fd считвем что left и right не могут иметь одинаковых fd_1
			if ((b_flag & (PIPE_LEFT2_42SH ^ PIPE_LEFT_42SH)) != 0)
				ft_42sh_pipe_left_hrdc(in->array, pipe, fd_write);
			else
				fn_to_file(in->b, fd_write, pipe->fd_2);
		}
		pipe++;
	}
}

static size_t	fn_fork(register t_main_42sh *array,
register t_jobs_42sh *jobs, register int fd, t_pipe_search_in_42sh *in)
{
	register pid_t			pid;

	if ((pid = fork()) == 0)
	{
		ft_42sh_signal_default(array, jobs);
		setpgid(0, array->pr.pid_fork);//Добавим в группу процессов в которой запускаемся
		ft_42sh_pipe_close_fd_left(jobs);//Освободим fd и pipe right иначе подвиснит// Что бы процесс завершался
		close(FD_TERMINAL_42SH);
		ft_42sh_jobs_fd_close_future(jobs);
		fn_while(&jobs->pipe[jobs->n], fd,
		jobs->fds[(fd << 1) + PIPE_WRITE_42SH], in);
		ft_42sh_cm_exit_fun(array, E_CODE_42SH);//Это лишь для valgrind - по факту не надо
	}
	else if (pid < 0)
		return (0);
	setpgid(0, array->pr.pid_fork);//Добавим в группу процессов в которой запускаемся
	jobs->pipe_pid[fd] = pid;
	return (1);
}

size_t			ft_42sh_pipe_left(register t_main_42sh *array,
register t_jobs_42sh *jobs)
{
	register size_t			count;
	register int			fd;
	t_pipe_search_in_42sh	in;

	count = jobs->b_fd_left;
	fd = 0;
	jobs->b_pipe_fd |= count;
	in.array = array;
	in.pipe = &jobs->pipe[0];
	in.b = array->buff_out;
	in.b_fd_left = jobs->b_fd_left;
	while (count != 0)
	{
		if ((count & 0x1) != 0)
			if (fn_fork(array, jobs, fd, &in) == 0)
				return (0);
		count = count >> 1;
		fd++;
	}
	return (1);
}
