#include <termios.h>
#include <term.h>
#include "../libft/libft.h"

typedef struct			s_history
{
	char				*content;
	char				*tmp;
	size_t				c_size;
	struct s_history	*begin;
	struct s_history	*prev;
	struct s_history	*next;
}						t_hist;

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

static void sig_hdlr(int sig)
{
	ft_putstr("Sig : ");
	ft_putnbr(sig);
	ft_putchar('\n');
}

int		go_home(int index)
{
	int		i;
	char	*term;
	int		ret;
	int		nb_col;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	i = index / nb_col;
	while (i--)
		tputs(tgetstr("up", NULL), 1, ft_pchar);
	tputs(tgoto(tgetstr("ch", NULL), 0, 3), 1, ft_pchar);
	index = 0;
	return (index);
}

int		go_end(int index, int len)
{
	char	*term;
	int		ret;
	int		nb_col;
	int		i;
	int		j;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	i = index / nb_col - 1;
	j = len / nb_col;
	while (++i < j)
		tputs(tgetstr("do", NULL), 1, ft_pchar);
	tputs(tgoto(tgetstr("ch", NULL), 0, (len + 3) % nb_col), 1, ft_pchar);
	return (len);
}

void	left_arrow(int *index, int i)
{
	char	*term;
	int		ret;
	int		nb_col;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	*index = *index > 0 ? *index - 1 : 0;
	if (*index && (*index + 3) % nb_col == nb_col - 1)
		tputs(tgetstr("up", NULL), 1, ft_pchar);
	tputs(tgoto(tgetstr("ch", NULL), 0, (*index + 3) % nb_col), 1, ft_pchar);
}

void	right_arrow(int *index, int i)
{
	char	*term;
	int		ret;
	int		nb_col;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	*index = *index < i ? *index + 1 : i;
	if (*index && (*index + 3) % nb_col == 0)
		tputs(tgetstr("do", NULL), 1, ft_pchar);
	tputs(tgoto(tgetstr("ch", NULL), 0, (*index + 3) % nb_col), 1, ft_pchar);
}

int		up_arrow(int *index, char *buff, char *buff_tmp, t_hist **curr)
{
	int		i;
	int		len;

	len = ft_strlen(buff_tmp);
	i = ft_strlen(buff);
	if (*curr && (*curr)->next && ((ft_strcmp((*curr)->content, buff) == 0 && buff_tmp[8193]) || *curr != (*curr)->begin))
	{
		if ((*curr)->tmp)
			free((*curr)->tmp);
		(*curr)->tmp = ft_strdup(buff);
		(*curr) = (*curr)->next;
		while ((*curr)->next && (ft_strstr((*curr)->content, buff_tmp) != (*curr)->content || (*curr)->c_size <= len))
			(*curr) = (*curr)->next;
		while ((*curr)->prev && (ft_strstr((*curr)->content, buff_tmp) != (*curr)->content || (*curr)->c_size <= len))
			(*curr) = (*curr)->prev;
	}
	else if ((*curr)->next)
	{
		len = ft_strlen(buff);
		while ((*curr)->next && (ft_strstr((*curr)->content, buff) != (*curr)->content || (*curr)->c_size <= len))
			*curr = (*curr)->next;
		if (*curr != (*curr)->begin && (ft_strstr((*curr)->content, buff) != (*curr)->content || (*curr)->c_size <= len))
			while ((*curr)->prev)
				*curr = (*curr)->prev;
		ft_strcpy(buff_tmp, buff);
		buff_tmp[8193] = 1;
	}
	if (*curr && ft_strstr((*curr)->content, buff_tmp) == (*curr)->content && (*curr)->c_size > len)
	{
		ft_bzero(buff, i);
		if ((*curr)->tmp)
			ft_strcpy(buff, (*curr)->tmp);
		else
			ft_strcpy(buff, (*curr)->content);
	}
	*index = go_home(*index);
	tputs(tgetstr("cr", NULL), 1, ft_pchar);
	tputs(tgetstr("cd", NULL), 1, ft_pchar);
	ft_putstr("$> ");
	ft_putstr(buff);
	*index = ft_strlen(buff);
	return (*index);
}

int		down_arrow(int *index, char *buff, char *buff_tmp, t_hist **curr)
{
	int		i;
	int		len;

	len = ft_strlen(buff_tmp);
	i = ft_strlen(buff);
	if (*curr && (*curr)->prev)
	{
		if ((*curr)->tmp)
			free((*curr)->tmp);
		(*curr)->tmp = ft_strdup(buff);
		*curr = (*curr)->prev;
		while ((*curr)->prev && (ft_strstr((*curr)->content, buff_tmp) != (*curr)->content || (*curr)->c_size <= len))
			*curr = (*curr)->prev;
		if (ft_strstr((*curr)->content, buff_tmp) == (*curr)->content && (*curr)->c_size > len)
		{
			ft_bzero(buff, i);
			if ((*curr)->tmp)
				ft_strcpy(buff, (*curr)->tmp);
			else
				ft_strcpy(buff, (*curr)->content);
		}
		else
		{
			ft_bzero(buff, i);
			ft_strcpy(buff, buff_tmp);
		}
	}
	else if (*curr && buff_tmp[8193])
	{
		ft_bzero(buff, i);
		ft_strcpy(buff, buff_tmp);
		ft_bzero(buff_tmp, 8194);
	}
	*index = go_home(*index);
	tputs(tgetstr("cr", NULL), 1, ft_pchar);
	tputs(tgetstr("cd", NULL), 1, ft_pchar);
	ft_putstr("$> ");
	ft_putstr(buff);
	*index = ft_strlen(buff);
	return (*index);
}

void	ft_exit(struct termios save, t_hist **begin)
{
	t_hist *curr;

	ft_term_restore(save);
	ft_putchar('\n');
	if (*begin)
		while (*begin)
		{
			curr = (*begin)->next;
			free(*begin);
			(*begin) = curr;
		}
	exit(0);
}

int		ft_cancel(int index, int i, char *buff_tmp, t_hist **curr)
{
	char	*term;
	int		ret;
	int		nb_col;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	i = i % nb_col < nb_col ? i + (nb_col - i % nb_col) : i;
	while ((index = index + nb_col) < i)
		ft_putchar('\n');
	*curr = (*curr)->begin;
	ft_bzero(buff_tmp, 8194);
	while ((*curr)->next)
	{
		if ((*curr)->tmp)
			free((*curr)->tmp);
		(*curr)->tmp = NULL;
		(*curr) = (*curr)->next;
	}
	if ((*curr)->tmp)
		free((*curr)->tmp);
	(*curr)->tmp = NULL;
	*curr = (*curr)->begin;
	return (-1);
}

void	del_lines(int index, int len, int nb_col)
{
	int		i;
	int		j;

	i = index / nb_col - 1;
	j = len / nb_col;
	tputs(tgetstr("sc", NULL), 1, ft_pchar);
	while (++i < j)
	{
		tputs(tgetstr("do", NULL), 1, ft_pchar);
		tputs(tgetstr("cr", NULL), 1, ft_pchar);
		tputs(tgetstr("dl", NULL), 1, ft_pchar);
	}
	tputs(tgetstr("rc", NULL), 1, ft_pchar);
}

int		del_left(int *index, char *buff, char *buff_tmp, t_hist *curr)
{
	int		j;
	char	*term;
	int		ret;
	int		nb_col;
	int		i;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	j = 0;
	i = ft_strlen(buff);
	if (*index && (*index + 2) % nb_col == nb_col - 1)
		tputs(tgetstr("up", NULL), 1, ft_pchar);
	if (*index != 0)
	{
		*index = *index > 0 ? *index - 1 : 0;
		while (*index + j < i)
		{
			buff[*index + j] = buff[*index + j + 1];
			j++;
		}
		buff[i] = '\0';
		i = i > 0 ? i - 1 : 0;
		tputs(tgoto(tgetstr("ch", NULL), 0, (*index + 3) % nb_col), 1, ft_pchar);
		tputs(tgetstr("dc", NULL), 1, ft_pchar);
	}
	if ((*index + 3) % nb_col == nb_col - 1)
	{
		tputs(tgetstr("sc", NULL), 1, ft_pchar);
		ft_putchar(' ');
		tputs(tgetstr("rc", NULL), 1, ft_pchar);
	}
	if (i + 3 > nb_col - 1)
	{
		del_lines(*index, i, nb_col);
		tputs(tgetstr("sc", NULL), 1, ft_pchar);
		*index < i ? ft_putstr(&buff[*index]) : ft_putstr("");
		tputs(tgetstr("rc", NULL), 1, ft_pchar);
	}
	if (curr && ft_strcmp(curr->content, buff) != 0)
		ft_strcpy(buff_tmp, buff);
	return (i);
}

void	del_right(int index, int *i, char *buff)
{
	int		j;
	char	*term;
	int		ret;
	int		nb_col;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	j = -1;
	while (index + ++j < *i)
		buff[index + j] = buff[index + j + 1];
	tputs(tgetstr("dc", NULL), 1, ft_pchar);
	del_lines(index, *i, nb_col);
	tputs(tgetstr("sc", NULL), 1, ft_pchar);
	index < *i ? ft_putstr(&buff[index]) : ft_putstr("");
	tputs(tgetstr("rc", NULL), 1, ft_pchar);
	*i = *i > index ? *i - 1 : index;
}

void	ft_clear(char *buff)
{
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	ft_putstr("$> ");
	ft_putstr(buff);
}

void	next_word(int *index, int len, char *buff)
{
	char	*term;
	int		ret;
	int		nb_col;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	while (buff[*index] != ' ' && *index < len)
	{
		if (len > nb_col)
			if ((*index + 3) % nb_col == 0 && *index < len)
				tputs(tgetstr("do", NULL), 1, ft_pchar);
		*index = *index + 1;
	}
	while (*index < len && buff[*index] == ' ')
		*index = *index + 1;
	tputs(tgoto(tgetstr("ch", NULL), 0, (*index + 3) % nb_col), 1, ft_pchar);
}

void	prev_word(int *index, int len, char *buff)
{
	char	*term;
	int		ret;
	int		nb_col;

	term = getenv("TERM");
	ret = tgetent(NULL, term);
	nb_col = tgetnum("co");
	if (*index == len && len != 0)
		*index = *index - 1;
	if (ft_isprint(buff[*index]) && buff[*index] != ' ' && *index > 0)
		*index = *index - 1;
	while (buff[*index] == ' ')
		*index = *index - 1;
	while (buff[*index] != ' ' && *index > 0)
	{
		if (len > nb_col)
			if ((*index + 3) % nb_col == 0 && *index > 0)
				tputs(tgetstr("up", NULL), 1, ft_pchar);
		*index = *index - 1;
	}
	while (*index > 0 && buff[*index] == ' ')
		*index = *index + 1;
	tputs(tgoto(tgetstr("ch", NULL), 0, (*index + 3) % nb_col), 1, ft_pchar);
}

int		get_typing(int *index, char *buff, char *tmp, int nb_read)
{
	int		len;
	int		cp;
	int		j;
	char	cbis;
	char	tchar;

	cp = 0;
	len = ft_strlen(buff);
	while (cp < nb_read && ft_isprint(tmp[cp]))
	{
		tchar = buff[*index + 1];
		if (*index != len)
			buff[*index + 1] = buff[*index];
		buff[(*index)++] = tmp[cp];
		len++;
		j = 1;
		ft_putchar(tmp[cp++]);
		if (*index != len)
		{
			while (*index + j < len)
			{
				cbis = buff[*index + j];
				buff[*index + j] = tchar;
				tchar = cbis;
				j++;
			}
			tputs(tgetstr("sc", NULL), 1, ft_pchar);
			ft_putstr(&buff[*index]);
			tputs(tgetstr("rc", NULL), 1, ft_pchar);
		}
	}
	return (len);
}

void	deal_commande(int index, char *buff, char *buff_tmp, t_hist **curr)
{
	int		j;
	char	*term;
	int		fd;
	int		nb_col;
	t_hist	*tmp;

	fd = open(".21sh_history", O_RDWR | O_APPEND | O_CREAT, 0644);
	term = getenv("TERM");
	tgetent(NULL, term);
	nb_col = tgetnum("co");
	index = index / nb_col - 1;
	j = ft_strlen(buff) / nb_col;
	while (++index < j)
		tputs(tgetstr("do", NULL), 1, ft_pchar);
	if (*curr)
	{
		if (ft_strcmp(buff, (*curr)->begin->content) != 0)
		{
			ft_putendl_fd(buff, fd);
			tmp = *curr;
			*curr = ft_memalloc(sizeof(t_hist));
			(*curr)->content = ft_strdup(buff);
			(*curr)->c_size = ft_strlen(buff);
			(*curr)->next = tmp->begin;
			tmp->begin->prev = *curr;
			(*curr)->begin = *curr;
			while ((*curr)->next)
			{
				if ((*curr)->tmp)
					free((*curr)->tmp);
				(*curr)->tmp = NULL;
				if ((*curr)->prev)
					(*curr)->begin = (*curr)->prev->begin;
				*curr = (*curr)->next;
			}
			(*curr)->begin = tmp->begin;
			*curr = (*curr)->begin;
		}
		else 
			*curr = (*curr)->begin;
	}
	else
	{
		ft_putendl_fd(buff, fd);
		*curr = ft_memalloc(sizeof(t_hist));
		(*curr)->content = ft_strdup(buff);
		(*curr)->c_size = ft_strlen(buff);
		(*curr)->begin = *curr;
	}
	ft_putendl(buff);
	ft_bzero(buff_tmp, 8194);
	close(fd);
}

void	create_hist(t_hist **begin)
{
	int		continu;
	t_hist	*curr;
	int		fd;

	continu = 1;
	fd = open(".21sh_history", O_RDWR | O_APPEND | O_CREAT, 0644);
	while (continu)
	{
		curr = ft_memalloc(sizeof(t_hist));
		if (*begin)
		{
			curr->next = *begin;
			(*begin)->prev = curr;
			curr->begin = *begin;
		}
		continu = get_next_line(fd, &(curr->content));
		curr->c_size = ft_strlen(curr->content);
		if (!continu)
		{
			free(curr);
			if (*begin)
				(*begin)->prev = NULL;
		}
		else
			*begin = curr;
	}
	curr = *begin;
	while (curr)
	{
		curr->begin = *begin;
		curr = curr->next;
	}
	close(fd);
}

int		main(void)
{
	char	tmp[10];
	struct termios	save;
	int		nb_read;
	int		i;
	char	buff[8193];
	int		index;
	t_hist	*curr;
	char	*term;
	char	buff_tmp[8194];

	term = getenv("TERM");
	tgetent(NULL, term);
	curr = NULL;
	create_hist(&curr);
	curr = curr->begin;
	ft_define_new_term_cap(&save);
	nb_read = 0;
	signal(SIGINT, &sig_hdlr);
	signal(SIGQUIT, &sig_hdlr);
	tputs(tgetstr("cl", NULL), 1, ft_pchar);
	ft_bzero(buff_tmp, 8194);
	while (1)
	{
		ft_putstr("$> ");
		ft_bzero(buff, 8193);
		i = 0;
		index = 0;
		tmp[0] = '\0';
		while (tmp[0] != 10 && tmp[0] != -1)
		{
			//	j = -1;
			//	while (++j < nb_read)
			//		ft_putnbend(tmp[j], "  ");
			if (i + (nb_read = read(0, tmp, 10)) < 8192) /* Type and cmd+V */
				i = get_typing(&index, buff, tmp, nb_read);
			if (nb_read == 1 && tmp[0] == 4 && !buff[0]) /* ctrl+D*/
				ft_exit(save, &(curr->begin));
			else if (nb_read == 1 && tmp[0] == 3) /* ctrl+C */
				tmp[0] = ft_cancel(index, i, buff_tmp, &curr);
			else if (nb_read == 3 && tmp[0] == 27 && tmp[1] == 91 && tmp[2] == 65) /* up arrow */
				i = up_arrow(&index, buff, buff_tmp, &curr);
			else if (nb_read == 3 && tmp[0] == 27 && tmp[1] == 91 && tmp[2] == 66) /* down key */
				i = down_arrow(&index, buff, buff_tmp, &curr);
			else if (nb_read == 3 && tmp[0] == 27 && tmp[1] == 91 && tmp[2] == 67) /* right key*/
				right_arrow(&index, i);
			else if (nb_read == 3 && tmp[0] == 27 && tmp[1] == 91 && tmp[2] == 68) /* left key */
				left_arrow(&index, i);
			else if (nb_read == 1 && tmp[0] == 127) /* delete left */
				i = del_left(&index, buff, buff_tmp, curr);
			else if (nb_read == 4 && tmp[0] == 27 && tmp[1] == 91 && tmp[2] == 51 && tmp[3] == 126) /* delete right */
				del_right(index, &i, buff);
			else if (nb_read == 1 && tmp[0] == 12) /* ctrl+L */
				ft_clear(buff);
			else if (nb_read == 3 && tmp[0] == 27 && tmp[1] == 91 && tmp[2] == 72) /* home */
				index = go_home(index);
			else if (nb_read == 3 && tmp[0] == 27 && tmp[1] == 91 && tmp[2] == 70) /* end */
				index = go_end(index, i);
			else if (nb_read == 1 && tmp[0] == 23) /* ctrl+W */
				next_word(&index, i, buff);
			else if (nb_read == 1 && tmp[0] == 2) /* ctrl+B */
				prev_word(&index, i, buff);
		}
		ft_putchar('\n');
		if (buff[0] && tmp[0] != -1)
			deal_commande(index, buff, buff_tmp, &curr);
		/*nb_read = read(0, tmp, 10);
		  i = -1;
		  while (++i < nb_read)
		  {
		  ft_putnbr(tmp[i]);
		  ft_putchar('\n');
		  }
		  if (tmp[0] == 'c') {
		  ft_term_restore(save);
		  pid_t  pid = fork();
		  if (pid == 0)
		  {
		  char *cmd[] = { "/sbin/ping", "google.fr", NULL };
		  execve(cmd[0], cmd, NULL);
		  exit(1);
		  } else if (pid > 0) {
		  int status;
		  ft_putendl("waiting...");
		  waitpid(pid, &status, 0);
		  ft_putstr("ret=");
		  ft_putnbr(status);
		  ft_putchar('\n');
		  }
		  ft_define_new_term_cap(&save);
		  }
		  if (tmp[0] == 'q')
		  break;*/
	}
	ft_term_restore(save);
	return (0);
}
