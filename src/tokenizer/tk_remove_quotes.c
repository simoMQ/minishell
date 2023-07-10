/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tk_remove_quotes.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: oswin <oswin@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/09 22:45:59 by oswin         #+#    #+#                 */
/*   Updated: 2022/05/09 22:46:00 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
 * Removes the quotes from a tokenized word.
 * This function can be called in lstiter for a linked list
 * with t_token as value for the content pointer.
 */

#include "minishell.h"

void	tk_remove_quotes(void *token)
{
	int		i;
	char	state;
	char	*word;

	word = ((t_token *)token)->word;
	state = '\0';
	i = 0;
	while (word[i] != '\0')
	{
		if ((word[i] == '\'' || word[i] == '\"') && state == '\0')
		{
			state = word[i];
			ft_memmove(&word[i], &word[i + 1], ft_strlen(&word[i]));
		}
		else if (word[i] == state)
		{
			ft_memmove(&word[i], &word[i + 1], ft_strlen(&word[i]));
			state = '\0';
		}
		else
			i++;
	}
}
