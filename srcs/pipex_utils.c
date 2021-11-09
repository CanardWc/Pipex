#include <pipex.h>

void	pipex_error(void)
{
	if (errno == 0)
		ft_printf("Wrong arguments\n");
	else
		perror(strerror(errno));
	exit(0);
}

char	*pipex_cut_commands(char *av)
{
	int	i;

	i = 0;
	while (av[i] && av[i] != ' ')
		i++;
	if (av[i])
		av = ft_substr(av, 0, i);
	if (av == NULL)
		pipex_error();
	return (av);
}

char	*pipex_find_path(char *av, char **path_env)
{
	char	*tmp;
	char	*tmp2;

	av = pipex_cut_commands(av);
	if (av && *av != '/')
	{
		while (*path_env)
		{
			tmp = ft_strjoin(*path_env++, "/");
			if (tmp == NULL)
				pipex_error();
			tmp2 = ft_strjoin(tmp, av);
			free(tmp);
			tmp = NULL;
			if (tmp2 == NULL)
				pipex_error();
			if (!access(tmp2, F_OK))
				return (tmp2);
			free(tmp2);
			tmp2 = NULL;
		}
	}
	else if (!access(av, F_OK))
		return (av);
	return (NULL);
}
