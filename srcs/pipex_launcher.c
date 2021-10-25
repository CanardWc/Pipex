#include <pipex.h>

void	pipex_setup_child(int ac, int ***fd, pid_t **child)
{
	int	i;

	*fd = (int **)malloc(sizeof(int *) * (ac - 3));
	if (*fd == NULL)
		pipex_error();
	i = 0;
	while (i < (ac - 3))
	{
		(*fd)[i] = (int *)malloc(sizeof(int) * 2);
		if ((*fd)[i++] == NULL)
			pipex_error();
	}
	*child = (pid_t *)malloc(sizeof(pid_t) * (ac - 3));
	if (*child == NULL)
		pipex_error();
}

void	pipex_stdin(char **av, int **fd, int i)
{
	int	fd_start;

	fd_start = 0;
	if (i == 0)
	{
		fd_start = open(av[1], O_RDONLY);
		if (fd_start < 0)
			pipex_error();
		dup2(fd_start, STDIN_FILENO);
		close(fd_start);
	}
	else
	{
		dup2(fd[i - 1][0], STDIN_FILENO);
		close(fd[i - 1][0]);
	}
}

void	pipex_stdout(int ac, char **av, int **fd, int i)
{
	int	fd_end;

	fd_end = 0;
	if (i == (ac - 4))
	{
		fd_end = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd_end < 0)
			pipex_error();
		dup2(fd_end, STDOUT_FILENO);
		close(fd_end);
		close(fd[i][0]);
	}
	else
	{
		dup2(fd[i][1], STDOUT_FILENO);
		close(fd[i][1]);
	}
}

void	pipex_child(int ac, char **av, char **path_env, int **fd, int i)
{
	char	*path;
	char	**cmd;
	int	j;

	cmd = ft_split(av[i + 2], ' ');
	pipex_stdin(av, fd, i);
	pipex_stdout(ac, av, fd, i);
	path = pipex_find_path(av[i + 2], path_env);
	if (path == NULL)
		pipex_error();
	if (execve(path, cmd, path_env) == -1)
		pipex_error();
	j = 0;
	while (cmd[j])
		free(cmd[j++]);
	free(cmd);
	cmd = NULL;
	free(path);
	path = NULL;
}

void	pipex_launcher(int ac, char **av, char **path_env)
{
	pid_t	*child;
	int	**fd;
	int	status;
	int	i;

	child = NULL;
	fd = NULL;
	pipex_setup_child(ac, &fd, &child);
	i = 0;
	while (i < (ac - 3))
	{
		pipe(fd[i]);
		child[i] = fork();
		if (child[i] < 0)
			pipex_error();
		else if (child[i] > 0)
			close(fd[i][1]);
		else
			pipex_child(ac, av, path_env, fd, i);
		i++;
	}
	i = 0;
	while (i < (ac - 3))
		waitpid(child[i++], &status, 0);
	exit(0);
}
