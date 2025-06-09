/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:02:50 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/09 23:04:24 by yusudemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>

int	thread_lock(pthread_mutex_t *mutex)
{
	int	status;

	status = pthread_mutex_lock(mutex);
	if (status)
		return (printf("pthread_mutex_lock() failed: %d.\n", status), 1);
	return (0);
}

int	thread_unlock(pthread_mutex_t *mutex)
{
	int	status;

	status = pthread_mutex_unlock(mutex);
	if (status)
		return (printf("pthread_mutex_unlock() failed: %d.\n", status), 1);
	return (0);
}
