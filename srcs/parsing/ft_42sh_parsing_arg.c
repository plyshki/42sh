/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_parsing_arg.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

static size_t		fn_cout_arg(register t_main_42sh *array,
unsigned char *b, register unsigned char *e)
{
	unsigned char			*s;
	register size_t			count;
	t_replase_in_42sh		in;

	count = 0;
	in.array = array;
	in.b_mode = PARSING_MODE_ARG_42SH;
	b = ft_42sh_parsing_sp(b, e);
	while (ft_42sh_parsing_litter_e_f(b, e) != 0)
	{
		if (ft_42sh_pipe_next(array, &b, e) != 0)//между редиректами подбирает аргументы тем самым
			continue ;
		s = b;
		ft_42sh_replase_count(&in, &s, &b, e);
		if (s != b)
			count++;
	}
	return (count);
}

static char			**fn_set_arg(register t_main_42sh *array,
register char **lp_arg, unsigned char **out, register unsigned char *end)
{
	unsigned char			*start;
	register unsigned char	*tmp;
	register unsigned char	*save;
	register size_t			count;
	t_replase_in_42sh		in;

	in.array = array;
	in.b_mode = PARSING_MODE_ARG_42SH;
	start = *out;
	save = start;
	count = ft_42sh_replase_count(&in, &start, out, end);
	if (start == *out)
		return (lp_arg);
	if ((tmp = ft_malloc(count + 1)) == 0)
		ft_42sh_exit(E_MEM_CODE_42SH, __FILE__, __func__, __LINE__);
	lp_arg++[0] = (char *)tmp;
	tmp[count] = 0;
	ft_42sh_replase(&in, tmp, save, end);
	return (lp_arg);
}

static char			**fn_pre(register t_main_42sh *array,
register char **lp_arg, unsigned char **out, register unsigned char *end)
{
	register unsigned char	*b;
	register unsigned char	litter;

	b = ft_42sh_parsing_sp(*out, end);
	while (b < end && (litter = ft_42sh_parsing_litter_e(b, end)) != 0 &&
	ft_42sh_parsing_test_pipe(b, end, 0x20) == 0)
	{
		lp_arg = fn_set_arg(array, lp_arg, out, end);
		b = *out;
	}
	return (lp_arg);
}

static void			fn_finish(register t_main_42sh *array,
register char **lp_arg, unsigned char **out, register unsigned char *end)
{
	while (ft_42sh_parsing_litter_e_f(*out, end) != 0)
	{
		if (ft_42sh_pipe_next(array, out, end) != 0)
			continue ;
		lp_arg = fn_set_arg(array, lp_arg, out, end);
	}
}

void				ft_42sh_parsing_arg(register t_main_42sh *array,
register t_jobs_42sh *jobs, unsigned char **out, register unsigned char *end)
{
	register char			**lp_arg;
	register size_t			count;

	count = fn_cout_arg(array, *out, end);
	if (array->pr.exit_pars.lp != 0)
	{
		ft_free(jobs->lp_arg);//Освободим передоваемый параметр
		jobs->lp_arg = 0;//Из -за нормы как параметр передаем поэтому обнуляем раз ошибка
		return ;
	}
	if ((lp_arg = ft_malloc(sizeof(char *) * (count + 1 + 1))) == 0)//+1 - на завршающий ноль, для определения конца//+1 первым аргументам всегда пердаеться путь к вызываемому файлу
		ft_42sh_exit(E_MEM_CODE_42SH, __FILE__, __func__, __LINE__);
	lp_arg[0] = (void *)jobs->lp_arg;//первый аргумент являеться именем команды или путем к приложению смотря как пользователь в вел и если не стоит  AUTO_TYPE_ERROR_42SH
	jobs->lp_arg = lp_arg++;//Пропускаем первый аргумент который под имя команды
	lp_arg[count] = 0;
	fn_finish(array, fn_pre(array, lp_arg, out, end), out, end);
}
