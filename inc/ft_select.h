/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:45:04 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 20:05:04 by rfontain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <unistd.h>
# include <termios.h>
# include <term.h>
# include <signal.h>
# include <sys/ioctl.h>
# include "libft.h"

# define UP_KEY "\x1B\x5B\x41"
# define DOWN_KEY "\x1B\x5B\x42"
# define RIGHT_KEY "\x1B\x5B\x43"
# define LEFT_KEY "\x1B\x5B\x44"
# define ESC_KEY "\x1B"
# define ENTER_KEY "\xA"
# define SELECT_KEY "\x20"
# define ERASE_KEY1 "\x7F"
# define ERASE_KEY2 "\x1B\x5B\x33\x7E"

typedef struct		s_mln
{
	int				udline;
	int				selec;
	int				pos;
	char			*str;
	struct s_mln	*next;
	struct s_mln	*prev;
}					t_mln;

typedef struct		s_select
{
	int				fd;
	int				max_len;
	int				nb_col;
	int				nb_line;
	int				nb_elem;
	int				is_print;
	struct termios	save;
	t_mln			*lst;
	t_mln			*begin;
	t_mln			*end;
}					t_slct;

typedef struct		s_fonction
{
	char			*key;
	void			(*f)(t_slct*);
}					t_fctn;

struct s_select		env;

int					ft_pchar(int nb);
void				ft_term_restore(void);
void				ft_define_new_term_cap(t_slct *env);

t_mln				*go_end(t_mln *lst);
t_mln				*get_argv(char **av);
void				put_list(t_slct *env);
void				ft_loop(void);

void				deal_exit(t_slct *env);
void				select_key(t_slct *env);
void				unselect_key(t_slct *env);
void				enter_key(t_slct *env);

void				left_key(t_slct *env);
void				right_key(t_slct *env);
void				down_key(t_slct *env);
void				up_key(t_slct *env);

void				sig_exit(int sig);
void				sig_tstp(int sig);
void				sig_cont(int sig);
void				sig_winch(int sig);

t_mln				*go_end(t_mln *lst);
void				free_env(void);
void				free_mln(void);

#endif
