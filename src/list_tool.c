/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:43:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 05:53:35 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

t_mln		*get_argv(char **av)
{
	t_mln	*begin;
	t_mln	*curr;
	int		i;
	int		len;

	if (!(begin = (t_mln *)ft_memalloc(sizeof(t_mln))))
		return (NULL);
	begin->str = ft_strdup(av[1]);
	env.max_len = ft_strlen(av[1]);
	begin->udline = 1;
	env.nb_elem = 1;
	curr = begin;
	i = 1;
	while (av[++i])
	{
		curr->next = ft_memalloc(sizeof(t_mln));
		curr->next->prev = curr;
		curr->next->str = ft_strdup(av[i]);
		len = ft_strlen(av[i]);
		env.max_len = len > env.max_len ? len : env.max_len;
		env.nb_elem += 1;
		curr = curr->next;
	}
	return (begin);
}

static void	ft_putchars_fd(char c, int nb, int fd)
{
	while (nb--)
		ft_putchar_fd(c, fd);
}

void		put_list(t_slct *env)
{
	t_mln	*curr;
	int		max_put;
	int		i;

	if (!env->is_print)
		return (ft_putendl_fd("Please resize your window.", STDERR_FILENO));
	max_put = (env->nb_col / (env->max_len + 1)) - 1;
	i = 0;
	curr = env->begin;
	while (curr)
	{
		if (curr->udline)
			tputs(tgetstr("us", NULL), 1, ft_pchar);
		if (curr->selec)
			tputs(tgetstr("mr", NULL), 1, ft_pchar);
		ft_putstr_fd(curr->str, env->fd);
		tputs(tgetstr("ue", NULL), 1, ft_pchar);
		ft_putchars_fd(' ', env->max_len - ft_strlen(curr->str) + 1, env->fd);
		if (i == max_put)
			ft_putendl("");
		i = i == max_put ? 0 : i + 1;
		tputs(tgetstr("me", NULL), 1, ft_pchar);
		curr = curr->next;
	}
}

void		ft_loop(void)
{
	int				nb_read;
	char			buff[11];
	int				i;
	static t_fctn	fctn[] = {
		{ "\x1B", &deal_exit },
		{ "\x20", &select_key },
		{ "\x7F", &unselect_key },
		{ "\x1B\x5B\x33\x7E", &unselect_key },
		{ "\x1B\x5B\x41", &up_key },
		{ "\x1B\x5B\x42", &down_key },
		{ "\x1B\x5B\x43", &right_key },
		{ "\x1B\x5B\x44", &left_key },
		{ "\xA", &enter_key } };

	ft_bzero(buff, 11);
	while (1)
	{
		nb_read = read(0, buff, 10);
		buff[nb_read] = '\0';
		i = -1;
		while (++i < 9)
			if (ft_strcmp(buff, fctn[i].key) == 0)
				fctn[i].f(&env);
	}
}
