/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-yaqi <yel-yaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:35:58 by yel-yaqi          #+#    #+#             */
/*   Updated: 2024/10/30 12:26:40 by yel-yaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdbool.h>
#include "../data_structs/data_funcs.h"

bool equal(double a, double b);

/* VECTOR MATH */
double	vector_magnitude(t_tuple t);
t_tuple	normalize_vec(t_tuple t);
double	vec_dot(t_tuple a, t_tuple b);
t_tuple	vec_cross(t_tuple a, t_tuple b);

/* CONSTANTS */
# define EPSILON .0001
# define NUDGE   .01