/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-yaqi <yel-yaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:06:28 by yel-yaqi          #+#    #+#             */
/*   Updated: 2024/10/28 17:17:08 by yel-yaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data_structs.h"
#include "data_funcs.h"
#include "../maths/maths.h"
#include <math.h>
#include <stdlib.h>

t_matrix	*view_transform(t_tuple from, t_tuple to, t_tuple up)
{
	t_tuple		forward;
	t_tuple		upn;
	t_tuple		left;
	t_tuple		true_up;
	t_matrix	*orientation;
	t_matrix	*orient_translate_product;
	t_matrix	*translation_matrix;

	if (to.w == POINT)
		forward = normalize_vec(sub_tuples(to, from));
	else
		forward = to;
	if (equal(forward.y, 1) || equal(forward.y, -1))
		forward = normalize_vec(vector(0, forward.y, NUDGE));
	upn = normalize_vec(up);
	left = vec_cross(forward, upn);
	true_up = vec_cross(left, forward);
	orientation = return_4_by_4_matrix(
	return_tuple(left.x, left.y, left.z, 0),
	return_tuple(true_up.x, true_up.y, true_up.z, 0),
	return_tuple(-forward.x, -forward.y, -forward.z, 0),
	return_tuple(0, 0, 0, 1));
	translation_matrix = translation(-from.x, -from.y, -from.z);
	orient_translate_product = matrix_multiply(orientation, translation_matrix, 4);
	return (free(orientation), free(translation_matrix), orient_translate_product);
}

void	camera(double hsize, double vsize, t_camera_ *c)
{
	double	half_view;
	double	aspect;

	c->hsize = hsize;
	c->vsize = vsize;
	c->transform = NULL;
	if (equal(c->vec.y, 1) || equal(c->vec.y, -1))
		half_view = tan(radians(c->fov * (NUDGE - .002) / 2.0));
	else
		half_view = tan(radians(c->fov / 2.0));
	aspect = c->hsize / c->vsize;
	if (aspect >= 1)
	{
		c->half_width = half_view;
		c->half_height = half_view / aspect;
	}
	else
	{
		c->half_width = half_view * aspect;
		c->half_height = half_view;
	}
	c->pixel_size = c->half_width * 2 / c->hsize;
}

t_ray	ray_for_pixel(t_camera_ *cam, double px, double py)
{
	t_camray	camray;
	t_matrix	*inversion;

	inversion = invert_matrix(cam->transform, 4);
	camray.xoffset = cam->pixel_size * (px + .5);
	camray.yoffset = cam->pixel_size * (py + .5);
	camray.world_x = cam->half_width - camray.xoffset;
	camray.world_y = cam->half_height - camray.yoffset;
	camray.pixel = multiply_matrix_by_tuple(inversion,
	point(camray.world_x, camray.world_y, -1));
	camray.origin = multiply_matrix_by_tuple(inversion, point(0, 0, 0));
	camray.direction = normalize_vec(sub_tuples(camray.pixel, camray.origin));
	return (free(inversion), return_ray(camray.origin, camray.direction));
}

void	render(t_canvas *canvas, t_camera_ *cam, t_world *world)
{
	int		x;
	int		y;
	t_ray	ray;
	t_tuple	rgb;

	y = 0;
	while (y < cam->vsize)
	{
		x = 0;
		while (x < cam->hsize)
		{
			ray = ray_for_pixel(cam, x, y);
			rgb = multiply_color_by_scalar(color_at(world, ray), 255);
			write_pixel(canvas, x, y, rgb);
			x++;
		}
		y++;
	}
}
