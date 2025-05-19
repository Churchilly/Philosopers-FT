/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:02:50 by yusudemi          #+#    #+#             */
/*   Updated: 2025/05/19 00:56:05 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int	thread_lock(pthread_mutex_t *mutex)
{
	int status;

	status = pthread_mutex_lock(mutex);
	if (status)
		return (printf("pthread_mutex_lock() failed: %d.\n", status), 1);
	return (0);
}

int	thread_unlock(pthread_mutex_t *mutex)
{
	int status;

	status = pthread_mutex_unlock(mutex);
	if (status)
		return (printf("pthread_mutex_unlock() failed: %d.\n", status), 1);
	return (0);
}