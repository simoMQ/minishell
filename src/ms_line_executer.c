/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_line_executer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 22:45:09 by oswin             #+#    #+#             */
/*   Updated: 2023/06/25 21:39:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	unclose_quote(char	*str)
// {
// 	int		i;
// 	t_quote	quote;

// 	i = 0;
// 	quote = NONE;
// 	while (str[i])
// 	{
// 		if (str[i] == '"')
// 		{
// 			if (quote == NONE)
// 				quote = DOUBLE;
// 			else if (quote == DOUBLE)
// 				quote = NONE;
// 		}
// 		if (str[i] == '\'')
// 		{
// 			if (quote == NONE)
// 				quote = SINGLE;
// 			else if (quote == SINGLE)
// 				quote = NONE;
// 		}
// 		i++;
// 	}
// 	return (quote);
// }

// void	print_err_exit(t_mini *data)
// {
// 		write(2, "minishell: miss quote\n", 22);
// 		free_all(cmd_line);
// }

void	del_pipe(void *content)
{
	t_pipe	*pipe;

	pipe = (t_pipe *)content;
	if (pipe->tokens != NULL)
		free_string_array(pipe->tokens);
	free(pipe);
}

int	wait_for_children(int number_of_children)
{
	int	status;

	while (number_of_children)
	{
		wait(&status);
		number_of_children--;
	}
	ms_signals();
	if (WIFSIGNALED(status))
		return (130);
	return (WEXITSTATUS(status));
}

int	exe_pipe_and_run(t_list *pipes, t_mini *data)
{
	int	pipefd[2];
	int	nbr_of_pipes;

	if (pipes->next == NULL
		&& is_buildin(((t_pipe *)pipes->content)->tokens[0]))
		return (exe_pre_buildin((t_pipe *)pipes->content, data));
	nbr_of_pipes = ft_lstsize(pipes);
	while (pipes->next != NULL)
	{
		if (pipe(pipefd) == -1)
			ft_disruptive_exit("pipe failed", 42);
		((t_pipe *)pipes->content)->output_fd = pipefd[1];
		((t_pipe *)pipes->next->content)->input_fd = pipefd[0];
		exe_pre_fork((t_pipe *)pipes->content, data);
		pipes = pipes->next;
	}
	exe_pre_fork((t_pipe *)pipes->content, data);
	fd_cleanup(data);
	return (wait_for_children(nbr_of_pipes));
}

void	ms_line_executer(t_mini *data)
{
	t_list	*tokens;
	t_list	*pipes;

		if (ft_strnstr(data->input, "$?", ft_strlen(data->input)))
			replaceexitstatus(data);
		data->input = tk_expander(data->input, data->env);
		tokens = ms_tokenizer(data->input);
		
		// while (tokens->next)
		// {
		// 	t_token *t = (t_token *)tokens->content;
		// 	printf("%s\n", t->word);
		// 	tokens = tokens->next;
		// }
		// t_token *v = (t_token *)tokens->content;
		// printf("%s\n", v->word);
		
		free(data->input);
		if (tokens == NULL)
			return ;
		ft_lstiter(tokens, tk_remove_quotes);
		// wildcards_expander(&tokens);
		if (ms_lexer(tokens) != 0)
		{
			ft_lstclear(&tokens, ms_del_token);
			data->last_return = 258;
			return ;
		}
		pipes = ms_parser(&tokens);
		

	// int i = 0;
	// t_pipe *t;
	// while (pipes->next)
	// {
	// 	t = (t_pipe *)pipes->content;
	// 	while (t->tokens[i])
	// 	{
	// 		printf("%s\n", t->tokens[i]);
	// 		i++;
	// 	}
	// 	printf("\n");
	// 	i = 0;
	// 	pipes = pipes->next;
	// }
	// i = 0;
	// t = (t_pipe *)pipes->content;
	// while (t->tokens[i])
	// {
	// 	printf("%s\n", t->tokens[i]);
	// 	i++;
	// }

		ft_lstclear(&tokens, ms_del_token);
		data->last_return = exe_pipe_and_run(pipes, data);
		ft_lstclear(&pipes, del_pipe);
	}
