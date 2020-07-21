/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 20:40:10 by amatilda          #+#    #+#             */
/*   Updated: 2019/09/03 20:40:27 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_malloc(register size_t n)//!!!  прежде чем использовать по серьезному для теста надо учесть что например getcwd вызывает сама   malloc и тут ft_free - не прокатит
{
	register void		*out;

	if ((out = malloc(n)) == 0)
		return (0);
	return (out);
}
