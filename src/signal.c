/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:38:26 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/29 17:45:56 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void	sig_exit(int sig)
{
	(void)sig;
	deal_exit(&env);
}

void	sig_tstp(int sig)
{
	char	tmp[2];

	(void)sig;
	tmp[0] = env.save.c_cc[VSUSP];
	tmp[1] = 0;
	ft_term_restore();
	signal(SIGTSTP, SIG_DFL);
	ioctl(0, TIOCSTI, tmp);
}

void	sig_cont(int sig)
{
	char	*term;

	(void)sig;
	ft_define_new_term_cap(&env);
	term = getenv("TERM");
	tgetent(NULL, term);
	signal(SIGTSTP, &sig_tstp);
	tputs(tgetstr("ti", NULL), 1, ft_pchar);
	tputs(tgetstr("vi", NULL), 1, ft_pchar);
	put_list(&env);
}

void	sig_winch(int sig)
{
	char	*term;
	int		nb_col;
	int		nb_line;

	(void)sig;
	term = getenv("TERM");
	tgetent(NULL, term);
	nb_col = tgetnum("co");
	nb_line = tgetnum("li");
	if (nb_col != env.nb_col)
		env.nb_col = nb_col;
	if (nb_line != env.nb_line)
		env.nb_line = nb_line;
	if (env.nb_line * (env.nb_col - env.max_len) < env.max_len * env.nb_elem)
		env.is_print = 0;
	else
		env.is_print = 1;
	put_list(&env);
}
