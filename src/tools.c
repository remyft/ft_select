/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 05:50:43 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/21 04:40:27 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

t_mln	*go_end(t_mln *lst)
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

void	putstr_col_fd(char *str, mode_t stat, int fd)
{
	if (stat)
	{
		if (S_ISREG(stat))
		{
			if (stat & S_IXUSR)
				ft_putstr_fd(RED, fd);
		}
		else if (S_ISDIR(stat))
			ft_putstr_fd(CYAN, fd);
		else if (S_ISLNK(stat))
			ft_putstr_fd(MAGENTA, fd);
	}
	ft_putstr_fd(str, fd);
}
