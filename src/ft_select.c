/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:53:09 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 20:04:24 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static void	init_window(void)
{
	int				i;
	char			*term;

	ft_define_new_term_cap(&env);
	if (!(term = getenv("TERM")))
	{
		ft_putendl_fd("No environment found.", STDERR_FILENO);
		ft_term_restore();
		exit(0);
	}
	tgetent(NULL, term);
	i = -1;
	while (++i < NSIG)
		signal(i, &sig_exit);
	signal(SIGCONT, &sig_cont);
	signal(SIGTSTP, &sig_tstp);
	signal(SIGWINCH, &sig_winch);
	tputs(tgetstr("ti", NULL), 1, ft_pchar);
	tputs(tgetstr("vi", NULL), 1, ft_pchar);
}

int			main(int ac, char **av)
{
	init_window();
	if (ac >= 2)
	{
		env.lst = get_argv(av);
		env.begin = env.lst;
		env.end = go_end(env.lst);
		sig_winch(0);
		ft_loop();
	}
	ft_term_restore();
	tputs(tgetstr("te", NULL), 1, ft_pchar);
	tputs(tgetstr("ve", NULL), 1, ft_pchar);
}
