/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

t_main_42sh			*g_lp_array;

int			main(int argc, char **argv, char **env)
{
	char *str;
	static t_main_42sh		array;

	str = malloc(10);
	str[12] = 'a';
	g_lp_array = &array;//Устанавливаем глобальную переменную для сигналов
	array.b_mode = MODE_DEBUG_42SH;//| MODE_LAUNCH_42SH | MODE_VALG_42SH | MODE_GUARD_42SH | MODE_SIGCHILD_42SH;
	ft_42sh_init(&array, argc, argv, env);//Иницилизируем все что надо
	ft_42sh_read(&array);//Запускаем вечный цикл ожидания ввода от пользователя
	return (0);
}
