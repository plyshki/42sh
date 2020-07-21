/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_sh_get.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_sh.h"

static size_t	fn_test_type(register t_main_42sh *array,
register unsigned char *lp_sh, register size_t count, register int fd)
{
	register unsigned char			*lp_sh_e;
	register unsigned char			*tmp;

	close(fd);
	lp_sh_e = lp_sh + count;
	array->sh.lp_sh = lp_sh;
	array->sh.lp_sh_e = lp_sh_e;
	tmp = lp_sh;
	while (tmp < lp_sh_e && tmp[0] != '\n')
		tmp++;
	if (count < 2 || lp_sh[0] != '#' || lp_sh[1] != '!')//Проверим есть шапка указывающая какая шелл скрипт выполняет если нет выполняем в нашем шелле
		return (1);
	if (tmp == lp_sh_e || count < 3)
		ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array,
		MSG_SH_UNKNOWN_42SH), STATUS_FALTURE_42SH);
	if (ft_42sh_pattern(lp_sh, tmp, (void *)MSG_SH_TEST_TYPE_42SH,
	(void *)(MSG_SH_TEST_TYPE_42SH + sizeof(MSG_SH_TEST_TYPE_42SH) - 1)) != 0)
		return (1);
	tmp[0] = 0;//так как там полюбому должен быть байт на перевод строки
	return (0);//Запустим тогда ту оболочку для скриптов что указана
}

void			fn_run(register t_main_42sh *array,
register char **env_spl, register unsigned char *path,
register unsigned char *bin)
{
	register char			**lp_arg;

	if ((lp_arg = ft_malloc(sizeof(void *) * 3)) == 0)
		ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array,
		MSG_SH_INVALID_MEMORY_42SH), STATUS_FALTURE_42SH);
	lp_arg[0] = (void *)bin;
	lp_arg[1] = (void *)path;
	lp_arg[2] = 0;
	execve((void *)bin, lp_arg, env_spl);
	ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array, MSG_SH_UNKNOWN_42SH),
	STATUS_FALTURE_42SH);//Раз сдесь такой оболочки нет или что пошло не так
}

t_in_42sh		*ft_42sh_sh_get(register t_main_42sh *array,
register char **env_spl, register unsigned char *path, register char **lp_arg)
{
	struct stat						st;
	register int					fd;
	register size_t					count;
	register unsigned char			*out;

	if ((fd = open((void *)path, O_RDONLY)) == -1)
		ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array,
		MSG_SH_OPEN_FALLED_42SH), STATUS_FALTURE_42SH);
	if (stat((void *)path, &st) != 0)
		ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array,
		MSG_SH_DEFINED_42SH), STATUS_DEFINED_CMD_42SH);
	if ((count = st.st_size) >= SCRIPT_MAX_SIZE_42SH)
		ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array,
		MSG_SH_INVALID_SIZE_42SH), STATUS_FALTURE_42SH);
	if ((out = ft_malloc(count)) == 0)
		ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array,
		MSG_SH_INVALID_MEMORY_42SH), STATUS_FALTURE_42SH);
	if (read(fd, out, count) == -1)
		ft_42sh_sh_exit(ft_42sh_sh_exit_zero_crit(array,
		MSG_SH_INVALID_READ_42SH), STATUS_FALTURE_42SH);
	if (fn_test_type(array, out, count, fd) == 0)
		fn_run(array, env_spl, path, out + 2);//+2 на #!
	count = ((count << 1) + BUFFER_READ_42SH) & -BUFFER_READ_42SH;// + BUFFER_READ_42SH  для того что бы сделать кратным  не в меньшую  сторону BUFFER_READ_42SH
	ft_42sh_init_script(array, env_spl, lp_arg);//Повторно переиницилизируем все что надо для обработки скрипта
	return (ft_42sh_list_in_create(array, count));//Выделим в два раза больше чем надо для того что бы не париться с переводам строк и пробелами вставляемые
}