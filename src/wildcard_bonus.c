/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wildcard_bonus.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: oswin <oswin@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/09 22:45:24 by oswin         #+#    #+#                 */
/*   Updated: 2022/05/09 22:45:25 by oswin         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

int	ft_strrcmp(char *haystack, char *needle)
{
	int	hl;
	int	nl;

	hl = ft_strlen(haystack);
	nl = ft_strlen(needle);
	while (hl >= 0 && nl >= 0)
	{
		if (haystack[hl] != needle[nl])
			return (1);
		hl--;
		nl--;
	}
	return (nl + 1);
}

int	wildcard_compare(char *name, char *wildcard)
{
	int	wci;

	if ((*name == '.' && *wildcard != '.')
		|| (*name != '.' && *wildcard == '.'))
		return (0);
	wci = ft_strclen(wildcard, '*');
	if (wildcard[wci] == '\0')
		return (0);
	if (!ft_strncmp(name, wildcard, wci)
		&& !ft_strrcmp(&name[wci], &wildcard[wci + 1]))
		return (1);
	return (0);
}

t_list	**insert_wildcard_token(t_list **wild, char *word)
{
	t_list	*new;
	t_token	*n_token;

	n_token = malloc(sizeof(t_token));
	ft_malloc_fail_check(n_token);
	new = ft_lstnew((void *)n_token);
	ft_malloc_fail_check(new);
	new->next = *wild;
	*wild = new;
	n_token->id = Str;
	n_token->word = ft_strdup(word);
	ft_malloc_fail_check(n_token->word);
	return (&new->next);
}

void	wildcard_single_expander(t_list **wild)
{
	t_list			*original;
	DIR				*d;
	struct dirent	*dir;

	original = *wild;
	wild = &(*wild)->next;
	d = opendir(".");
	if (d)
	{
		dir = readdir(d);
		while (dir != NULL)
		{
			if (wildcard_compare(dir->d_name,
					((t_token *)original->content)->word))
			{
				wild = insert_wildcard_token(wild, dir->d_name);
			}
			dir = readdir(d);
		}
		closedir(d);
	}
}

void	wildcards_expander(t_list **tokens)
{
	t_token	*tmp;
	t_list	*next;
	int		lstlen;

	while (*tokens != NULL)
	{
		tmp = (t_token *)(*tokens)->content;
		if (tmp->word[ft_strclen(tmp->word, '*')] == '*')
		{
			lstlen = ft_lstsize(*tokens);
			wildcard_single_expander(tokens);
			if (lstlen < ft_lstsize(*tokens))
			{
				next = (*tokens)->next;
				ms_del_token((*tokens)->content);
				free(*tokens);
				*tokens = next;
			}
		}
		tokens = &((*tokens)->next);
	}
}
