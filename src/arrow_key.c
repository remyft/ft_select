/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrow_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:35:32 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 20:37:17 by rfontain         ###   ########.fr       */
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
	put_list(env);
}

void	down_key(t_slct *env)
{
	int		max_elem;
	int		pos;

	max_elem = (env->nb_col / (env->max_len + 1));
	env->lst->udline = 0;
	pos = env->lst->pos % max_elem;
	env->lst = env->lst->next ? env->lst->next : env->begin;
	while (env->lst && env->lst->pos % max_elem != pos)
		env->lst = env->lst->next ? env->lst->next : env->begin;
	env->lst->udline = 1;
	put_list(env);
}

void	up_key(t_slct *env)
{
	int		max_elem;
	int		pos;

	max_elem = (env->nb_col / (env->max_len + 1));
	env->lst->udline = 0;
	pos = env->lst->pos % max_elem;
	env->lst = env->lst->prev ? env->lst->prev : env->end;
	while (env->lst && env->lst->pos % max_elem != pos)
		env->lst = env->lst->prev ? env->lst->prev : env->end;
	env->lst->udline = 1;
	put_list(env);
}
