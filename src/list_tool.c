/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:43:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 20:48:18 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static t_mln	*get_next(char **av, t_mln *prev)
{
	t_mln	*curr;
	int		len;

	curr = NULL;
	if (*av)
	{
		if (!(curr = ft_memalloc(sizeof(t_mln))))
			return (NULL);
		curr->prev = prev;
		curr->str = ft_strdup(*av);
		len = ft_strlen(*av);
		env.max_len = len > env.max_len ? len : env.max_len;
		env.nb_elem += 1;
		curr->pos = env.nb_elem;
		curr->next = get_next(&av[1], curr);
	}
	return (curr);
}

t_mln			*get_argv(char **av)
{
	t_mln	*begin;

	if (!(begin = (t_mln *)ft_memalloc(sizeof(t_mln))))
		return (NULL);
	begin->str = ft_strdup(av[1]);
	env.max_len = ft_strlen(av[1]);
	begin->udline = 1;
	env.nb_elem = 1;
	begin->pos = env.nb_elem;
	begin->next = get_next(&av[1], begin);
	return (begin);
}

static void		ft_putchars_fd(char c, int nb, int fd)
{
	while (nb--)
		ft_putchar_fd(c, fd);
}

void			put_list(t_slct *env)
{
	t_mln	*curr;
	int		max_put;
	int		i;

	tputs(tgetstr("cl", NULL), 1, ft_pchar);
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
		ft_putstr_fd(curr->str, 0);
		tputs(tgetstr("ue", NULL), 1, ft_pchar);
		ft_putchars_fd(' ', env->max_len - ft_strlen(curr->str) + 1, 0);
		if (i == max_put)
			ft_putendl_fd("", 0);
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
		{ UP_KEY, &up_key },
		{ DOWN_KEY, &down_key },
		{ RIGHT_KEY, &right_key },
		{ LEFT_KEY, &left_key },
		{ ESC_KEY, &deal_exit },
		{ ENTER_KEY, &enter_key },
		{ SELECT_KEY, &select_key },
		{ ERASE_KEY1, &unselect_key },
		{ ERASE_KEY2, &unselect_key } };

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
