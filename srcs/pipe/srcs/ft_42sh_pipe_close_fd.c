/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_pipe_close_fd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_pipe.h"

void		ft_42sh_pipe_close_fd(register t_jobs_42sh *jobs)//Закрывает не используемые fd это от 3 до 9 - shell их резервирует
{
	register int				fd;
	register size_t				count;
	register size_t				tempos;

	count = jobs->b_fd_left | jobs->b_fd_right;
	if (((tempos = jobs->b_flag_close) & (1 << STDIN_FILENO)) != 0)//Если надо звкроем стандартные fd
		close(STDIN_FILENO);
	if ((tempos & (1 << STDOUT_FILENO)) != 0)
		close(STDOUT_FILENO);
	if ((tempos & (1 << STDERR_FILENO)) != 0)
		close(STDERR_FILENO);
	fd = FD_MAX_STANDART_42SH + 1;
	count = count >> (FD_MAX_STANDART_42SH + 1);
	while (count != 0)
	{
		if ((count & 0x1) == 0)
			close(fd);//закроем не используемы доп fd
		count = count >> 1;
		fd++;
	}
	if (fd <= FD_MAX_STANDART_42SH)
		fd = FD_MAX_STANDART_42SH + 1;
	while (fd <= FD_MAX_SUPPORT_42SH)
		close(fd++);//закроем не используемы доп fd
}

static void	fn_close_pipe(register int *fds, register size_t count)
{
	while (count != 0)
	{
		if ((count & 0x1) != 0)
		{
			close(fds[PIPE_READ_42SH]);
			close(fds[PIPE_WRITE_42SH]);
		}
		count = count >> 1;
		fds += 2;
	}
}

static void	fn_pre(register t_jobs_42sh *jobs)
{
	register size_t				fd;

	fd = 0;
	while (fd <= FD_MAX_SUPPORT_42SH)
		close(fd++);//все лишниие fd
	ft_42sh_jobs_fd_close_future(jobs);
}

void		ft_42sh_pipe_close_fd_left(register t_jobs_42sh *jobs)//Закрываем все right pipe  и не нужный на чтение с стороны left
{
	register t_pipe_42sh		*pipe;
	register t_pipe_42sh		*pipe_end;
	register size_t				b_flag;
	register int				*fds;
	register size_t				fd;

	fn_pre(jobs);
	pipe = &jobs->pipe[0];
	pipe_end = &(jobs->pipe[jobs->n]);
	fds = &jobs->fds[0];
	while (pipe < pipe_end)
	{
		if (((b_flag = pipe->b_flag) & PIPE_RIGHT_42SH) != 0)
			close(pipe->fd_2);
		else if ((b_flag & PIPE_LEFT_42SH) != 0 &&
		(jobs->b_fd_left & (1 << (fd = pipe->fd_1))) != 0)
		{
			close(fds[(fd << 1) + PIPE_READ_42SH]);
			jobs->b_fd_left ^= (1 << fd);//Что бы потом освободить не используемые пайпы для записи в них
		}
		pipe++;
	}
	fn_close_pipe(fds, jobs->b_fd_right | jobs->b_fd_left);
}

void		ft_42sh_pipe_close_fd_right(register t_jobs_42sh *jobs)//Закрываем все left pipe  и не нужный на запись с стороны right
{
	register t_pipe_42sh		*pipe;
	register t_pipe_42sh		*pipe_end;
	register int				*fds;
	register size_t				b_flag;
	register size_t				fd;

	fn_pre(jobs);
	pipe = &jobs->pipe[0];
	pipe_end = &(jobs->pipe[jobs->n]);
	fds = &jobs->fds[0];
	while (pipe < pipe_end)
	{
		if (((b_flag = pipe->b_flag) & PIPE_LEFT_42SH) != 0 &&
		(b_flag & (PIPE_LEFT2_42SH ^ PIPE_LEFT_42SH)) == 0)
			close(pipe->fd_2);
		else if ((b_flag & PIPE_RIGHT_42SH) != 0 &&
		(jobs->b_fd_right & (1 << (fd = pipe->fd_1))) != 0)
		{
			close(fds[(fd << 1) + PIPE_WRITE_42SH]);
			jobs->b_fd_right ^= (1 << fd);//Что бы потом освободить не используемые пайпы для записи в них
		}
		pipe++;
	}
	fn_close_pipe(fds, jobs->b_fd_right | jobs->b_fd_left);
}
