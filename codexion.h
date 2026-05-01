/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlaktaf <dlaktaf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 12:00:00 by dlaktaf           #+#    #+#             */
/*   Updated: 2026/05/01 12:00:00 by dlaktaf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

typedef struct s_sim	t_sim;
typedef struct s_coder	t_coder;

typedef enum e_sched
{
	CDX_FIFO,
	CDX_EDF
}	t_sched;

typedef struct s_config
{
	int		nb_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int		compiles_required;
	long	dongle_cooldown;
	t_sched	scheduler;
}	t_config;

typedef struct s_hnode
{
	int		coder_id;
	long	priority;
}	t_hnode;

typedef struct s_heap
{
	t_hnode	*arr;
	int		size;
	int		capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				in_use;
	long			release_time;
	t_heap			wait_queue;
	int				granted_to;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			compiles_done;
	long		last_compile_start;
	long		request_seq;
	pthread_t	thread;
	t_sim		*sim;
}	t_coder;

typedef struct s_sim
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	long			start_time;
	long			seq_counter;
	int				stop;
	pthread_t		monitor;
}	t_sim;

/* utils.c */
long	ft_atol(const char *str, int *ok);
int		ft_strcmp(const char *s1, const char *s2);
long	get_time_ms(void);
long	elapsed_ms(t_sim *sim);
void	precise_sleep(long ms, t_sim *sim);

/* utils2.c */
void	print_status(t_sim *sim, int id, char *msg);
void	print_burnout(t_sim *sim, int id);
int		is_stopped(t_sim *sim);
void	set_stop(t_sim *sim);

/* parse_args.c */
int		parse_args(char **argv, t_config *cfg);

/* parse_args2.c */
int		parse_args2(char **argv, t_config *cfg);

/* validate.c */
int		validate_config(t_config *cfg);

/* heap.c */
int		heap_init(t_heap *h, int capacity);
int		heap_push(t_heap *h, t_hnode node);
t_hnode	heap_pop(t_heap *h);
t_hnode	heap_peek(t_heap *h);
int		heap_size(t_heap *h);

/* heap_utils.c */
void	heap_swap(t_hnode *a, t_hnode *b);
void	heap_sift_up(t_heap *h, int idx);
void	heap_sift_down(t_heap *h, int idx);
void	heap_destroy(t_heap *h);
void	heap_remove(t_heap *h, int coder_id);

/* dongle.c */
int		dongle_init(t_dongle *d, int id, int nb_coders);
void	dongle_destroy(t_dongle *d);
int		dongle_available(t_dongle *d, long cooldown);

/* dongle_acquire.c */
void	enqueue_on_one(t_dongle *d, t_coder *c);
void	dequeue_from_one(t_dongle *d, int coder_id);
int		try_take_both(t_coder *c, int first, int second);

/* dongle_release.c */
void	release_dongle(t_dongle *d, t_sim *sim);
void	wake_all_dongles(t_sim *sim);

/* scheduler.c */
long	get_priority(t_coder *c);
int		heap_cmp(t_hnode a, t_hnode b);

/* init_sim.c */
t_sim	*init_sim(t_config *cfg);

/* init_coders.c */
int		init_coders(t_sim *sim);

/* coder_routine.c */
void	*coder_thread(void *arg);

/* coder_actions.c */
void	do_refactor(t_coder *c);
int		acquire_both_dongles(t_coder *c);
void	release_both_dongles(t_coder *c);

/* monitor.c */
void	*monitor_thread(void *arg);

/* cleanup.c */
void	destroy_sim(t_sim *sim);

/* threads.c */
int		launch_simulation(t_sim *sim);

#endif
