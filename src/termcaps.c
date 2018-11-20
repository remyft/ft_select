/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:44:02 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 05:15:10 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int			ft_pchar(int nb)
{
	return (write(STDIN_FILENO, &nb, 1));
}

void		ft_term_restore(void)
{
	struct termios	termios;

	tputs(tgetstr("te", NULL), 1, ft_pchar);
	tputs(tgetstr("ve", NULL), 1, ft_pchar);
	if (tcgetattr(0, &termios) != 0)
	{
		ft_putendl("Fatal error: unable to the term attributes.");
		exit(2);
	}
	termios.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(0, TCSANOW, &termios) == -1)
	{
		ft_putendl("Fatal error: unable to restore the term attributes.");
		exit(2);
	}
}

void		ft_define_new_term_cap(t_slct *env)
{
	if (tcgetattr(0, &(env->save)) != 0)
	{
		ft_putendl("Fatal error: unable to get term attributes.");
		exit(2);
	}
	env->save.c_lflag &= ~(ICANON | ECHO);
	env->save.c_cc[VMIN] = 1;
	env->save.c_cc[VTIME] = 0;
	if ((tcsetattr(0, TCSANOW, &(env->save))) == -1)
	{
		ft_putendl("Fatal error: unable to set the new term attributes.");
		exit(2);
	}
}
