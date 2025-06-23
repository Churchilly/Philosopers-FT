/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yusudemi <yusudemi@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:33:24 by yusudemi          #+#    #+#             */
/*   Updated: 2025/06/23 17:33:44 by yusudemi         ###   ########.fr       */
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
