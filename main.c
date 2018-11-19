/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 17:55:04 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/19 02:59:12 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <termios.h>
#include <term.h>
#include <signal.h>
#include "libft/libft.h"

#define ESC_KEY 27
#define ARROW_K1 27
#define ARROW_K2 91
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68

typedef struct	s_mln
{
	char			*str;
	int				udline;
	int				selec;
	struct s_mln	*next;
	struct s_mln	*prev;
}				t_mln;

typedef struct	s_select
{
	struct termios	save;
	t_mln			*lst;
	t_mln			*begin;
	t_mln			*end;
}				t_slct;

typedef struct	s_fonction
{
	char		*key;
	void		(*f)(t_slct*);
}				t_fctn;

char		*ft_tgetstr(char *t)
{
	char            *tmp;

	if ((tmp = tgetstr(t, NULL)) == NULL)
	{
		ft_putstr_fd(t, 2);
		ft_putendl_fd(" not available", 2);
	}
	return (tmp);
}

int			ft_pchar(int nb)
{
	return (write(STDOUT_FILENO, &nb, 1));
}

void		ft_term_restore(struct termios save)
{
	if (tcsetattr(0, TCSANOW, &save) == -1)
	{
		ft_putendl("Fatal error: unable to restore the term attributes.");
		exit(2);
	}
}

void		ft_define_new_term_cap(struct termios *save)
{
	struct termios	termios;

	if (tcgetattr(0, save) != 0)
	{
		ft_putendl("Fatal error: unable to get term attributes.");
		exit(2);
	}
	if (tcgetattr(0, &termios) != 0)
	{
		ft_putendl("Fatal error: unable to the term attributes.");
		exit(2);
	}
	termios.c_lflag &= ~(ICANON | ECHO | ISIG);
	termios.c_cc[VMIN] = 1;
	termios.c_cc[VTIME] = 0;
	if ((tcsetattr(0, TCSANOW, &termios)) == -1)
	{
		ft_putendl("Fatal error: unable to set the new term attributes.");
		exit(2);
	}
}

t_mln	*get_argv(char **av)
{
	t_mln	*begin;
	t_mln	*curr;
	int		i;

	if (!(begin = (t_mln *)ft_memalloc(sizeof(t_mln))))
		return (NULL);
	begin->str = ft_strdup(av[1]);
	begin->udline = 1;
	curr = begin;
	i = 1;
	while (av[++i])
	{
		curr->next = ft_memalloc(sizeof(t_mln));
		curr->next->prev = curr;
		curr->next->str = ft_strdup(av[i]);
		curr = curr->next;
	}
	return (begin);
}

void	put_list(t_slct *env)
{
	t_mln	*curr;

	curr = env->begin;
	while (curr)
	{
		if (curr->selec)
			tputs(ft_tgetstr("mr"), 1, ft_pchar);
		if (curr->udline)
			tputs(ft_tgetstr("us"), 1, ft_pchar);
		ft_putendl(curr->str);
		tputs(ft_tgetstr("ue"), 1, ft_pchar);
		tputs(ft_tgetstr("me"), 1, ft_pchar);
		curr = curr->next;
	}
}

void	deal_exit(t_slct *env)
{
	ft_term_restore(env->save);
	tputs(ft_tgetstr("ve"), 1, ft_pchar);
	exit(0);
}

t_mln	*go_end(t_mln *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	up_arrow(t_slct *env)
{
	env->lst->udline = 0;
	if (env->lst->prev)
		env->lst = env->lst->prev;
	else
		env->lst = env->end;
	env->lst->udline = 1;
	tputs(ft_tgetstr("cl"), 1, ft_pchar);
	put_list(env);
}

void	down_arrow(t_slct *env)
{
	env->lst->udline = 0;
	if (env->lst->next)
		env->lst = env->lst->next;
	else
		env->lst = env->begin;
	env->lst->udline = 1;
	tputs(ft_tgetstr("cl"), 1, ft_pchar);
	put_list(env);
}

void	select_key(t_slct *env)
{
	env->lst->selec = 1;
	env->lst->udline = 0;
	env->lst = env->lst->next ? env->lst->next : env->begin;
	env->lst->udline = 1;
	tputs(ft_tgetstr("cl"), 1, ft_pchar);
	put_list(env);
}

void	unselect_key(t_slct *env)
{
	env->lst->selec = 0;
	tputs(ft_tgetstr("cl"), 1, ft_pchar);
	put_list(env);
}

void	enter_key(t_slct *env)
{
	env->lst = env->begin;
	while (env->lst)
	{
		if (env->lst->selec)
		{
			ft_putstr(env->lst->str);
			env->lst = env->lst->next;
			break ;
		}
		env->lst = env->lst->next;
	}
	while (env->lst)
	{
		if (env->lst->selec)
		{
			ft_putchar(' ');
			ft_putstr(env->lst->str);
		}
		env->lst = env->lst->next;
	}
	deal_exit(env);
}

void	deal_interrupt(t_slct *env)
{
	sigset_t	ens1;

	sigemptyset(&ens1);
	sigaddset(&ens1, SIGINT);
	sigsuspend(&ens1);
}

void	sig_hdlr(int sig)
{
	(void)sig;
}

int		main(int ac, char **av)
{
	struct termios	save;
	int				i;
	int				nb_read;
	char			buff[11];
	char			*term;
	t_slct			*env;
	static t_fctn	fctn[] = {
		{ "\x1B", &deal_exit },
		{ "\x1C", &deal_interrupt },
		{ "\x20", &select_key },
		{ "\x7F", &unselect_key },
		{ "\x1B\x5B\x33\x7E", &unselect_key },
		{ "\x1B\x5B\x41", &up_arrow },
		{ "\x1B\x5B\x42", &down_arrow },
		{ "\xA", &enter_key } };
		//		{ "\x1B\x5B\x43", &right_arrow },
		//		{ "\x1B\x5B\x44", &left_arrow },

	signal(SIGINT, &sig_hdlr);
	signal(SIGQUIT, &sig_hdlr);
	env = ft_memalloc(sizeof(t_slct));
	term = getenv("TERM");
	tgetent(NULL, term);
	ft_define_new_term_cap(&(env->save));
	tputs(ft_tgetstr("cl"), 1, ft_pchar);
	tputs(ft_tgetstr("vi"), 1, ft_pchar);
	ft_bzero(buff, 10);
	if (ac >= 2)
	{
		env->lst = get_argv(av);
		env->begin = env->lst;
		env->end = go_end(env->lst);
		put_list(env);
		while (1)
		{
			nb_read = read(0, buff, 10);
			buff[nb_read] = '\0';
			i = -1;
			while (++i < 8)
				if (ft_strcmp(buff, fctn[i].key) == 0)
				{
					fctn[i].f(env);
					break ;
				}
		}
	}
	ft_term_restore(env->save);
	tputs(ft_tgetstr("ve"), 1, ft_pchar);
}
