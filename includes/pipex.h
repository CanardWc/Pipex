#ifndef PIPEX_H
# define PIPEX_H

# include <libft.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

typedef struct	s_child
{
	int	**fd;
	pid_t	*child;
	int	status;
}		t_child;

void	pipex_error(void);
char	*pipex_find_path(char *av, char **path_env);

void	pipex_launcher(int ac, char **av, char **path_env);

#endif
