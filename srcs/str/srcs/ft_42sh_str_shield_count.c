/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_str_shield_count.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_str.h"

static size_t		fn_finish(register t_shield_out_42sh *lp,
register size_t count, register size_t count_litter)
{
	if (lp != 0)//Если не равно нулю запишем сколько байт и символов заняло
	{
		lp->count_litter = count_litter;
		lp->count = count;
	}
	return (count);
}

static size_t		fn_set(register size_t count,
size_t *count_litter)
{
	*count_litter = *count_litter + count;
	return (count);
}

static size_t		fn_set_test(size_t *count_litter,
register uint_fast8_t b_test)
{
	register unsigned char		lit;

	lit = (b_test & SHIELD_EXTERNALLY) != 0 ? 5 : 3;//$'\n' - 2 лишний пробел и его экранировка
	*count_litter = *count_litter + lit;
	return (lit);
}

static void			*fn_untf8(register unsigned char *b, size_t *count,
size_t *count_litter, register unsigned char lit)
{
	*count = *count + 1;
	*count_litter = *count_litter + 1;
	if ((lit & 0x80) == 0)//Учитываем utf-8
		return (b);
	while (((lit = lit << 1) & 0x80) != 0)
	{
		b++;
		*count = *count + 1;
	}
	return (b);
}

size_t				ft_42sh_str_shield_count(register unsigned char *b,
register unsigned char *e, register uint_fast8_t b_test,
register t_shield_out_42sh *lp)//Экранирует нужные символы в зависимости от флага и возвращает готовое значение через маллок
{
	size_t						count;
	size_t						count_litter;
	register unsigned char		lit;

	count = 0;
	count_litter = 0;
	while (b < e)
	{
		if ((lit = b++[0]) == '\n')
			count += fn_set_test(&count_litter, b_test);
		else if (lit == '\'' || lit == '"' || lit == '$' || lit == '\\' ||
		lit == ' ' || lit == '[' || lit == ']' || lit == '*' || lit == '?')
			count += fn_set(2, &count_litter);
		else
			b = fn_untf8(b, &count, &count_litter, lit);
	}
	return (fn_finish(lp, count, count_litter));
}
