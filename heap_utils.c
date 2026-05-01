/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_swap(t_hnode *a, t_hnode *b)
{
	t_hnode	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_sift_up(t_heap *h, int idx)
{
	int	parent;

	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (heap_cmp(h->arr[idx], h->arr[parent]))
			heap_swap(&h->arr[idx], &h->arr[parent]);
		else
			break ;
		idx = parent;
	}
}

void	heap_sift_down(t_heap *h, int idx)
{
	int	left;
	int	right;
	int	min;

	while (1)
	{
		left = 2 * idx + 1;
		right = 2 * idx + 2;
		if (left >= h->size)
			break ;
		min = left;
		if (right < h->size
			&& heap_cmp(h->arr[right], h->arr[left]))
			min = right;
		if (heap_cmp(h->arr[min], h->arr[idx]))
			heap_swap(&h->arr[min], &h->arr[idx]);
		else
			break ;
		idx = min;
	}
}

void	heap_destroy(t_heap *h)
{
	if (h->arr)
		free(h->arr);
	h->arr = NULL;
	h->size = 0;
	h->capacity = 0;
}

void	heap_remove(t_heap *h, int coder_id)
{
	int	i;

	i = 0;
	while (i < h->size)
	{
		if (h->arr[i].coder_id == coder_id)
		{
			h->size--;
			h->arr[i] = h->arr[h->size];
			if (i < h->size)
			{
				heap_sift_down(h, i);
				heap_sift_up(h, i);
			}
			return ;
		}
		i++;
	}
}
