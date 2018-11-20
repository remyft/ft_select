/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfontain <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 04:45:04 by rfontain          #+#    #+#             */
/*   Updated: 2018/11/20 05:16:05 by rfontain         ###   ########.fr       */
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

# define ESC_KEY 27
# define ARROW_K1 27
# define ARROW_K2 91
# define UP 65
# define DOWN 66
# define RIGHT 67
# define LEFT 68

typedef struct		s_mln
{
	char			*str;
	int				udline;
	int				selec;
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
t_mln				*go_end(t_mln *lst);
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

#endif
