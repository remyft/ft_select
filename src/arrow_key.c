/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrow_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:35:32 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 05:25:33 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	left_key(t_slct *env)
{
	env->lst->udline = 0;
	if (env->lst->prev)
		env->lst = env->lst->prev;
	else
		env->lst = env->end;
	env->lst->udline = 1;
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	put_list(env);
}

void	right_key(t_slct *env)
{
	env->lst->udline = 0;
	if (env->lst->next)
		env->lst = env->lst->next;
	else
		env->lst = env->begin;
	env->lst->udline = 1;
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	put_list(env);
}

void	down_key(t_slct *env)
{
	int		i;
	int		max_elem;

	i = 0;
	max_elem = (env->nb_col / (env->max_len + 1) - 1);
	env->lst->udline = 0;
	while (env->lst->next && i < max_elem)
	{
		env->lst = env->lst->next;
		i++;
	}
	env->lst->udline = 1;
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	put_list(env);
}

void	up_key(t_slct *env)
{
	int		i;
	int		max_elem;

	i = 0;
	max_elem = (env->nb_col / (env->max_len + 1) - 1);
	env->lst->udline = 0;
	while (env->lst->prev && i < max_elem)
	{
		env->lst = env->lst->prev;
		i++;
	}
	env->lst->udline = 1;
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	put_list(env);
}
