/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 22:46:53 by oswin             #+#    #+#             */
/*   Updated: 2023/07/06 20:42:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_relative(t_execute *info, t_list *env)
{
	char	**paths;
	char	*pathjoined;
	char	*tmp;
	char	**env_array;
	int		i;

	i = 0;
	paths = get_path_options(env);
	if (paths == NULL)
		exit(127);
	env_array = ft_lst_to_array(env, ev_turn_envlist);
	ft_malloc_fail_check(env_array);
	while (paths[i] != NULL)
	{
		tmp = ft_strjoin(paths[i], "/");
		ft_malloc_fail_check(tmp);
		pathjoined = ft_strjoin(tmp, info->arg[0]);
		free(tmp);
		ft_malloc_fail_check(pathjoined);
		execve(pathjoined, info->arg, env_array);
		i++;
	}
	ft_putstringtostderror(info->arg[0], ": command not found");
	exit(127);
}

void	execute_absolute(t_execute *info, t_list *env)
{
	char	*s;
	char	**env_array;

	env_array = ft_lst_to_array(env, ev_turn_envlist);
	ft_malloc_fail_check(env_array);
	execve(info->arg[0], info->arg, env_array);
	s = ft_strjoin(info->arg[0], ": command not found");
	ft_malloc_fail_check(s);
	ft_putendl_fd(s, 2);
	exit(127);
}

void	exe_child_process(t_execute *info, t_mini *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(info->fd_input, 0);
	dup2(info->fd_output, 1);
	if (is_buildin(info->arg[0]))
		exe_buildin(info, data);
	else if (info->arg[0][0] == '/' || info->arg[0][0] == '.')
		execute_absolute(info, info->envlst);
	else
		execute_relative(info, info->envlst);
	exit(0);
}

int	exe_fork(t_execute *info, t_mini *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_disruptive_exit("fork failed", 43);
	if (pid == 0)
		exe_child_process(info, data);
	// ms_parenting_signals();
	close(info->fd_input);
	close(info->fd_output);
	fd_cleanup(data);
	return (0);
}

int	exe_pre_fork(t_pipe *pipe, t_mini *data)
{
	t_execute	info;
	int			ret;

	exe_pipe_to_execute(pipe, &info, data);
	info.arg = fd_redirections(&info);
	ret = exe_fork(&info, data);
	free_string_array(info.arg);
	return (ret);
}
