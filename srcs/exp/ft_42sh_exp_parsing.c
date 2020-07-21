/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_exp_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_42sh_exp.h"

static unsigned char	*fn_finish(register t_replase_in_42sh *in,
register unsigned char *dest, unsigned char **src, register unsigned char *end)
{
	register unsigned char			*b;
	register unsigned char			*tmp;
	register size_t					tempos;
	register t_env_42sh				*list;

	b = *src;
	tmp = ft_42sh_exp_name_special(b, end);
	if ((tempos = tmp - b) == 0)
	{
		if (tmp == end || (tmp[0] != '\'' && tmp[0] != '"'))
		{
			dest++[0] = '$';
			return (dest);//Просто $
		}
		return (dest);
	}
	*src = tmp;
	if ((list = ft_42sh_list_find_key(&in->array->env.root,
	(void *)b, tempos)) != 0 &&
	(tempos = (size_t)ft_42sh_exp_parsing_value(in->array, list,
	dest, in->b_mode)) != (size_t)dest)
		return ((void *)tempos);
	return (dest);
}

static unsigned char	*fn_shield(t_replase_in_42sh *in,
register unsigned char *dest, unsigned char **src,
register unsigned char *b)
{
	*src = b + 4;
	if ((in->b_mode & PARSING_MODE_CMD_42SH) != 0)
	{
		dest[0] = '\\';//"\\n"
		dest[1] = 'n';
		dest += 2;
	}
	else
		dest++[0] = '\n';//"\n"
	return (dest);
}

unsigned char			*ft_42sh_exp_parsing(register t_replase_in_42sh *in,
register unsigned char *dest, unsigned char **src, register unsigned char *end)//Заменяем валидную переменную среды ее значением
{
	register unsigned char			*b;
	register unsigned char			litter;

	b = *src;
	if (b + 4 <= end && ft_strncmp(b, "'\\n'", 4) == 0)
		return (fn_shield(in, dest, src, b));
	if (b == end || in->array->env.root.first == 0)//Если нет переменых среды или один символ доллара
	{
		dest++[0] = '$';
		return (dest);//Считаем просто как символ
	}
	if ((litter = b[0]) == '{')
		return (ft_42sh_exp_pars_exp(in, dest, src, end));
	return (fn_finish(in, dest, src, end));
}
