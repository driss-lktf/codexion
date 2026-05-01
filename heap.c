/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *h, int capacity)
{
	h->arr = malloc(sizeof(t_hnode) * capacity);
	if (!h->arr)
		return (1);
	h->size = 0;
	h->capacity = capacity;
	return (0);
}

int	heap_push(t_heap *h, t_hnode node)
{
	if (h->size >= h->capacity)
		return (1);
	h->arr[h->size] = node;
	heap_sift_up(h, h->size);
	h->size++;
	return (0);
}

t_hnode	heap_pop(t_heap *h)
{
	t_hnode	top;

	top = h->arr[0];
	h->size--;
	if (h->size > 0)
	{
		h->arr[0] = h->arr[h->size];
		heap_sift_down(h, 0);
	}
	return (top);
}

t_hnode	heap_peek(t_heap *h)
{
	return (h->arr[0]);
}

int	heap_size(t_heap *h)
{
	return (h->size);
}
