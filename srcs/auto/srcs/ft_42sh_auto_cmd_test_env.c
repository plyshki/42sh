/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_auto_cmd_test_env.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_auto.h"

static void				fn_env_add(register t_main_42sh *array,
register t_in_42sh *list, register t_env_42sh *cmd, register size_t n)
{
	register unsigned char			*tmp;
	register size_t					count;
	t_add_litter_42sh				in;

	ft_42sh_auto_cmd_env_close(array, list, n);//если надо закрываем все свобки
	tmp = (void *)cmd->std.lp_key + n;
	count = cmd->std.key_count - n;
	in.count = count;
	in.count_litter = ft_strlen_utf8_n((void *)tmp, (void *)tmp + count);//В имени не может быть перевод строки
	ft_42sh_str(array, list, tmp, &in);//В имени не может быть перевод строки
}

static void				fn_replase(register t_main_42sh *array,
register t_in_42sh *list, register t_env_42sh *cmd, register size_t n)
{
	register unsigned char		*tmp;
	unsigned char				*start;
	t_shield_out_42sh			lp;

	n++;//На $
	tmp = (void *)(list->lp_current - n);
	ft_42sh_str_delete(array, list, (void *)tmp,
	ft_42sh_parsing_litter_n((void *)tmp, (void *)(tmp + n)));
	if ((n = ft_42sh_exp_parsing_value_count(array, cmd,
	PARSING_MODE_ARG_42SH)) == 0)
		return ;
	if ((tmp = ft_malloc(n)) == 0)
		ft_42sh_exit(E_MEM_CODE_42SH, __FILE__, __func__, __LINE__);
	ft_42sh_exp_parsing_value(array, cmd, tmp, PARSING_MODE_ARG_42SH);
	start = ft_42sh_str_shield(tmp, tmp + n, SHIELD_EXTERNALLY, &lp);//Все что выводиться на экран автодополнением нужно экранировать
	ft_42sh_str_add(array, (void *)start, (void *)start + lp.count, 1);//Уже опрботанно поэтому тут нет лишнего пробела с переводос строки
	ft_free(tmp);
	ft_free(start);
}

static size_t			fn_stub(register t_main_42sh *array,
register t_in_42sh *list, register t_env_42sh *cmd, register size_t n)
{
	if (n < cmd->std.key_count)//Доболним слово до нужного значения
		fn_env_add(array, list, cmd, n);
	else if (array->pguitar.comp.figure != 0)
	{
		ft_42sh_auto_cmd_env_replase(array, list,
		(void *)(list->lp_current - n));//Заменим на содержащееся в нем значение
		ft_42sh_auto_toogle(array);//Так как заменили то сбросим указатель что сейчас на переменных среды
	}
	else
	{
		fn_replase(array, list, cmd, n);
		ft_42sh_auto_toogle(array);//Так как заменили то сбросим указатель что сейчас на переменных среды
	}
	return (0);
}

size_t					ft_42sh_auto_cmd_test_env(register t_main_42sh *array,
register t_in_42sh *list, register t_all_cmd_42sh **spl,
register t_all_cmd_42sh **spl_end)//Проверяем переменная среды и надо ее расщирить или начать перебор
{
	register size_t				n;
	register t_env_42sh			*cmd;

	if (array->lp_auto != &array->auto_env)
		return (1);
	n = array->lp_auto->auto_len;
	cmd = (t_env_42sh *)spl[0];
	if (n != 0)
	{
		if (cmd == (t_env_42sh *)spl_end[0] ||
		(n == cmd->std.key_count && array->lp_auto->b_view == 0))
			return (fn_stub(array, list, cmd, n));
	}
	ft_42sh_auto_cmd_env_close(array, list, n);//если надо закрываем все свобки
	return (1);
}
