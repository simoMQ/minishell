/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 15:17:51 by dagmarkrame       #+#    #+#             */
/*   Updated: 2023/06/22 22:06:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*dst1;
	unsigned char	*src1;

	dst1 = (unsigned char *)dst;
	src1 = (unsigned char *)src;
	if (src < dst)
	{
		i = len;
		while (i > 0)
		{
			dst1[i - 1] = src1[i - 1];
			i--;
		}
	}
	else if (src > dst)
	{
		i = 0;
		while (i < len)
		{
			dst1[i] = src1[i];
			i++;
		}
	}
	return (dst);
}
