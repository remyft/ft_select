/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:36:41 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 05:04:07 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	select_key(t_slct *env)
{
	env->lst->selec = env->lst->selec ? 0 : 1;
	env->lst->udline = 0;
	env->lst = env->lst->next ? env->lst->next : env->begin;
	env->lst->udline = 1;
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	put_list(env);
}

void	unselect_key(t_slct *env)
{
	t_mln	*save;

	if (!env->lst->next && !env->lst->prev)
	{
		free(env->lst);
		tputs(tgetstr("cl", NULL), 1, ft_pchar);
		deal_exit(env);
	}
	save = env->lst->next ? env->lst->next : env->begin;
	if (env->lst && env->lst->prev)
		env->lst->prev->next = env->lst->next;
	else
		env->begin = env->lst->next;
	if (env->lst && env->lst->next)
		env->lst->next->prev = env->lst->prev;
	else
		env->end = env->lst->prev;
	if (env->lst)
		free(env->lst);
	else
		deal_exit(env);
	env->lst = save;
	env->lst->udline = 1;
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	put_list(env);
}

void	enter_key(t_slct *env)
{
	ft_term_restore();
	env->lst = env->begin;
	while (env->lst)
	{
		if (env->lst->selec)
		{
			ft_putstr_fd(env->lst->str, STDOUT_FILENO);
			env->lst = env->lst->next;
			break ;
		}
		env->lst = env->lst->next;
	}
	while (env->lst)
	{
		if (env->lst->selec)
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(env->lst->str, STDOUT_FILENO);
		}
		env->lst = env->lst->next;
	}
	close(env->fd);
	exit(0);
}

void	deal_exit(t_slct *env)
{
	(void)env;
	ft_term_restore();
	tputs(tgetstr("ve", NULL), 1, ft_pchar);
	tputs(tgetstr("te", NULL), 1, ft_pchar);
	exit(0);
}
