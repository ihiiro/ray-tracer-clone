/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-yaqi <yel-yaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 19:30:58 by yel-yaqi          #+#    #+#             */
/*   Updated: 2024/10/30 12:36:33 by yel-yaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data_structs/data_structs.h"
#include "../data_structs/data_funcs.h"
#include "../maths/maths.h"
#include <stdlib.h>

void	parse_sphere(const char *line, t_object_ **objects_list)
{
	t_sphere	*sp;
	t_object_	*object;
	t_matrix	*translation_mt;
	t_matrix	*scaling_mt;

	alloc_and_parse_pos(&sp, &object, &line);
	while (*line == ' ')
		line++;
	sp->radius = atodbl(line) / 2.0;
	if (sp->radius <= 0)
		exitf("sphere parse error\n");
	translation_mt = translation(sp->center.x, sp->center.y, sp->center.z);
	scaling_mt = scaling(sp->radius, sp->radius, sp->radius);
	sp->transform = matrix_multiply(translation_mt, scaling_mt, 4);
	free(translation_mt);
	free(scaling_mt);
	sp->center = point(0, 0, 0);
	sp->radius = 1;
	reach_for(&line, ' ', 0);
	parse_colors(&sp->material.color, line);
	object->form = SPHERE;
	object->object = sp;
	object->next = NULL;
	append_objects(objects_list, object);
}

void	parse_lights(const char *line, t_light_ **lights_list)
{
	t_light_	*light;

	parse_light_pos(&line, &light);
	while (*line == ' ')
		line++;
	light->intensity = atodbl(line);
	reach_for(&line, ' ', 0);
	light->color.x = atodbl(line) / 255.0;
	reach_for(&line, ',', 1);
	light->color.y = atodbl(line) / 255.0;
	reach_for(&line, ',', 1);
	light->color.z = atodbl(line) / 255.0;
	if (!valid_color(light->color) || light->intensity > 1
		|| light->intensity < 0)
		exitf("lights parse error\n");
	light->next = NULL;
	append_lights(lights_list, light);
}

void	parse_light_pos(const char **line, t_light_ **light)
{
	validate_line((char *)*line);
	reach_for(line, ' ', 0);
	*light = malloc(sizeof(t_light_));
	(*light)->pos.x = atodbl(*line);
	reach_for(line, ',', 1);
	(*light)->pos.y = atodbl(*line);
	reach_for(line, ',', 1);
	(*light)->pos.z = atodbl(*line);
	reach_for(line, ' ', 0);
}

#include <libc.h>

void	parse_camera(const char *line, t_world **world)
{
	static int	c;

	c++;
	if (c > 1)
		exitf("C: multiple invokes\n");
	validate_line((char *)line);
	reach_for(&line, ' ', 0);
	(*world)->camera.pos.x = atodbl(line);
	reach_for(&line, ',', 1);
	(*world)->camera.pos.y = atodbl(line);
	reach_for(&line, ',', 1);
	(*world)->camera.pos.z = atodbl(line);
	while (*line != ' ')
		line++;
	(*world)->camera.vec.x = atodbl(line);
	reach_for(&line, ',', 1);
	(*world)->camera.vec.y = atodbl(line);
	reach_for(&line, ',', 1);
	(*world)->camera.vec.z = atodbl(line);
	(*world)->camera.vec = normalize_vec((*world)->camera.vec);
	reach_for(&line, ' ', 0);
	(*world)->camera.fov = atodbl(line);
	if ((*world)->camera.fov < 0 || (*world)->camera.fov > 180)
		exitf("camera parse error\n");
}

void	parse_ambient(const char *line, t_world **world)
{
	static int	a;

	a++;
	if (a != 1)
		exitf("A: multiple invokes\n");
	validate_line((char *)line);
	reach_for(&line, ' ', 0);
	while (*line == ' ')
		line++;
	(*world)->ambient_intensity = atodbl(line);
	reach_for(&line, ' ', 0);
	(*world)->ambient_color.x = atodbl(line) / 255.0;
	reach_for(&line, ',', 1);
	(*world)->ambient_color.y = atodbl(line) / 255.0;
	reach_for(&line, ',', 1);
	(*world)->ambient_color.z = atodbl(line) / 255.0;
	if (!valid_color((*world)->ambient_color)
		|| (*world)->ambient_intensity > 1 || (*world)->ambient_intensity < 0)
		exitf("ambience parse error\n");
}
