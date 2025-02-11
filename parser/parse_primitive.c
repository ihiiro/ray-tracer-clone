/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-yaqi <yel-yaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:50:07 by yel-yaqi          #+#    #+#             */
/*   Updated: 2024/10/30 12:34:56 by yel-yaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data_structs/data_structs.h"
#include "../data_structs/data_funcs.h"
#include "../maths/maths.h"
#include <stdlib.h>
#include <math.h>

t_matrix *rotation_axis_angle(t_tuple axis, double angle) 
{
    double magnitude = vector_magnitude(axis);
    t_tuple normalized_axis = return_tuple(axis.x / magnitude, axis.y / magnitude, axis.z / magnitude, axis.w);
    double x = normalized_axis.x;
    double y = normalized_axis.y;
    double z = normalized_axis.z;
    double cos_theta = cos(angle);
    double sin_theta = sin(angle);
    double one_minus_cos = 1 - cos_theta;
    t_tuple row1 = return_tuple(
        cos_theta + x * x * one_minus_cos,
        x * y * one_minus_cos - z * sin_theta,
        x * z * one_minus_cos + y * sin_theta,
        0
    );
    t_tuple row2 = return_tuple(
        y * x * one_minus_cos + z * sin_theta,
        cos_theta + y * y * one_minus_cos,
        y * z * one_minus_cos - x * sin_theta,
        0
    );
    t_tuple row3 = return_tuple(
        z * x * one_minus_cos - y * sin_theta,
        z * y * one_minus_cos + x * sin_theta,
        cos_theta + z * z * one_minus_cos,
        0
    );
    t_tuple row4 = return_tuple(0, 0, 0, 1);
    return return_4_by_4_matrix(row1, row2, row3, row4);
}

t_matrix    *align_vector_to_axis(t_tuple vec, t_tuple target_axis)
{
    t_tuple axis = vec_cross(target_axis, vec);
    double angle = acos(vec_dot(target_axis, vec));
    if (vector_magnitude(axis) == 0)
        return identity();
    return rotation_axis_angle(axis, angle);
}

void	init_parse_plane(t_plane **pl, t_object_ **object)
{
	*pl = malloc(sizeof(t_plane));
	*object = malloc(sizeof(t_object_));
	**pl = plane();
}

void	parse_pip(t_plane *pl, const char **line)
{
	pl->pip.x = atodbl(*line);
	reach_for(line, ',', 1);
	pl->pip.y = atodbl(*line);
	reach_for(line, ',', 1);
	pl->pip.z = atodbl(*line);
}

void	parse_plane(const char *line, t_object_ **objects_list)
{
	t_plane		*pl;
	t_object_	*object;

	init_parse_plane(&pl, &object);
	validate_line((char *)line);
	reach_for(&line, ' ', 0);
	parse_pip(pl, &line);
	reach_for(&line, ' ', 0);
	pl->normal.x = atodbl(line);
	reach_for(&line, ',', 1);
	pl->normal.y = atodbl(line);
	reach_for(&line, ',', 1);
	pl->normal.z = atodbl(line);
	pl->normal.w = VECTOR;
	pl->normal = normalize_vec(pl->normal);
	reach_for(&line, ' ', 0);
	pl->transform = translation(pl->pip.x, pl->pip.y, pl->pip.z);
	pl->pip = point(0, 0, 0);
	parse_colors(&pl->material.color, line);
	object->form = PLANE;
	object->object = pl;
	object->next = NULL;
	append_objects(objects_list, object);
}

void	parse_cylinder_center_and_vector(t_cylinder **cy,
const char **line, t_object_ **object)
{
	*cy = malloc(sizeof(t_cylinder));
	*object = malloc(sizeof(t_object_));
	**cy = cylinder();
	validate_line(*(char **)line);
	reach_for(line, ' ', 0);
	(*cy)->center.x = atodbl(*line);
	reach_for(line, ',', 1);
	(*cy)->center.y = atodbl(*line);
	reach_for(line, ',', 1);
	(*cy)->center.z = atodbl(*line);
	reach_for(line, ' ', 0);
	(*cy)->vec.x = atodbl(*line);
	reach_for(line, ',', 1);
	(*cy)->vec.y = atodbl(*line);
	reach_for(line, ',', 1);
	(*cy)->vec.z = atodbl(*line);
	reach_for(line, ' ', 0);
	(*cy)->vec.w = VECTOR;
	(*cy)->vec = normalize_vec((*cy)->vec);
}

void	parse_cylinder(const char *line, t_object_ **objects_list)
{
	t_cylinder	*cy;
	t_object_	*object;
	t_tuple	    direction;
	t_matrix	*rotation_matrix;
	int			shape;
	t_matrix	*translation_mt;
	t_matrix	*scaling_mt;
	t_matrix	*rotate_scale_product;

	if (line[1] == 'o')
		shape = CONE;
	else
		shape = CYLINDER;
	parse_cylinder_center_and_vector(&cy, &line, &object);
	while (*line == ' ')
		line++;
	cy->radius = atodbl(line) / 2;
	reach_for(&line, ' ', 0);
	while (*line == ' ')
		line++;
	cy->height = atodbl(line);
	reach_for(&line, ' ', 0);
	if (cy->height <= 0 || cy->radius <= 0)
		exitf("cy: height or radius <= 0\n");
	direction = cy->vec;
   	rotation_matrix = align_vector_to_axis(direction, vector(0, 1, 0));
	translation_mt = translation(cy->center.x, cy->center.y, cy->center.z);
	scaling_mt = scaling(cy->radius, 1, cy->radius);
	rotate_scale_product = matrix_multiply(rotation_matrix, scaling_mt, 4);
	cy->transform = matrix_multiply(translation_mt, rotate_scale_product, 4);
	free(translation_mt);
	free(rotation_matrix);
	free(scaling_mt);
	free(rotate_scale_product);
	cy->center = point(0, 0, 0);
	parse_colors(&cy->material.color, line);
	object->form = shape;
	object->object = cy;
	object->next = NULL;
	append_objects(objects_list, object);
}
