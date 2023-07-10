/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 22:42:46 by oswin             #+#    #+#             */
/*   Updated: 2023/06/23 11:02:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "libft.h"

typedef enum e_token_id
{
	Pipe,
	Str,
	Heredoc,
	Read,
	Write,
	Append
}				t_token_id;

typedef struct s_token
{
	t_token_id	id;
	char		*word;
}				t_token;

t_list	*ms_tokenizer(char *input);

void	ms_del_token(void *incomming);
int		ms_lstadd_token(t_list **tokens, char *word);
int		ms_find_offset(char *in, char *delimiters, int offset);
int		ms_add_tokens(t_list **tokens, char *input);
int		ms_skip_chars(char *in, char *to_skip, int offset);

void	tk_expand_env(void *in, t_list *env);
void	tk_expand_all(t_list *tokens, t_list *env);
char	*tk_expander(char *word, t_list *env);

void	tk_name_token(void *in);
void	tk_remove_quotes(void *token);
char	**token_list_to_array(t_list *tokens, int n);

#endif