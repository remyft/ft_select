/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 05:50:43 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 05:53:21 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

t_mln		*go_end(t_mln *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	free_env(void)
{
	t_mln *curr;

	env.lst = env.begin;
	while (env.lst)
	{
		curr = env.lst->next;
		free(env.lst->str);
		free(env.lst);
		env.lst = curr;
	}
}

void	free_mln(void)
{
	free(env.lst->str);
	free(env.lst);
}
