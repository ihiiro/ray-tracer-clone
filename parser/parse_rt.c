/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-yaqi <yel-yaqi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 10:26:12 by yel-yaqi          #+#    #+#             */
/*   Updated: 2024/10/01 20:53:37 by yel-yaqi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../data_structs/data_structs.h"
#include "../data_structs/data_funcs.h"
#include "get_next_line/get_next_line.h"
#include <libc.h>

void	parse_colors(t_sphere *sp, const char *line)
{
	int	is_fraction;

	is_fraction = 1;
	sp->material.color.x = ft_atoi(line, &is_fraction);
	reach_for(&line, ',', 1);
	sp->material.color.y = ft_atoi(line, &is_fraction);
	reach_for(&line, ',', 1);
	sp->material.color.z = ft_atoi(line, &is_fraction);
	if (!valid_color(sp->material.color))
		exitf("sphere parse error\n");
}

void	alloc_and_parse_pos(t_sphere **sp, t_object_ **object,
t_world **world, const char **line)
{
	int	is_fraction;

	is_fraction = 10;
	*sp = malloc(sizeof(t_sphere));
	*object = malloc(sizeof(t_object_));
	**sp = sphere(0);
	(*sp)->material.ambient = (*world)->ambient_intensity;
	reach_for(line, ' ', 0);
	(*sp)->center.x = ft_atoi(*line, &is_fraction) / (10.0 / is_fraction);
	is_fraction = 10;
	reach_for(line, ',', 1);
	(*sp)->center.y = ft_atoi(*line, &is_fraction) / (10.0 / is_fraction);
	is_fraction = 10;
	reach_for(line, ',', 1);
	(*sp)->center.z = ft_atoi(*line, &is_fraction) / (10.0 / is_fraction);
	is_fraction = 10;
	reach_for(line, ' ', 0);
}

void	get_values(const char *line, t_light_ **lights_list,
t_object_ **objects_list, t_world **world)
{
	size_t		i;
	t_tuple		light_rgb;
	t_sphere	*sp;
	t_object_	*object;
	t_light_	*light;

	i = 0;
	if (line[0] == 'A' && line[1] == ' ')
		parse_ambient(line, world, 10);
	else if (line[0] == 'C' && line[1] == ' ')
		parse_camera(line, world, 10);
	else if (line[0] == 'L' && line[1] == ' ')
		parse_lights(line, lights_list, 10);
	else if (line[0] == 's' && line[1] == 'p' && line[2] == ' ')
		parse_sphere(line, world, objects_list, 10);
}

t_world	*parse(const char *file)
{
	t_parser	parser_;

	parser_.lights_list = NULL;
	parser_.objects_list = NULL;
	parser_.world = malloc(sizeof(t_world));
	parser_.fd = open(file, O_RDONLY);
	if (parser_.fd < 0)
		exit(EXIT_FAILURE);
	parser_.line = get_next_line(parser_.fd);
	while (parser_.line)
	{
		get_values(parser_.line, &parser_.lights_list,
			&parser_.objects_list, &parser_.world);
		parser_.line = get_next_line(parser_.fd);
	}
	parser_.world->lights_list = parser_.lights_list;
	parser_.world->objects_list = parser_.objects_list;
	clean_w(parser_.world);
	return (parser_.world);
}

// int main()
// {
// 	t_world *world = parse("test_cfiles/test0.rt");

// 	printf("world ambient intensity: [%.1f]\n", world->ambient_intensity);
// 	printf("world ambient rgb: [%.1f %.1f %.1f]\n", world->ambient_color.x, world->ambient_color.y, world->ambient_color.z);
// 	printf("world camera:\n\t3d_coords[%.1f %.1f %.1f]\n", world->camera.pos.x, world->camera.pos.y, world->camera.pos.z);
// 	printf("\torientation vector[%.1f %.1f %.1f]\n", world->camera.vec.x, world->camera.vec.y, world->camera.vec.z);
// 	printf("\tFOV[%.1f]\n", world->camera.fov);
// 	printf("world objects(spheres):\n");
// 	for (; world->objects_list; world->objects_list = world->objects_list->next)
// 	{
// 		t_sphere *sp = world->objects_list->object;
// 		printf("\tform[%d]\n\tcpos[%.1f %.1f %.1f]\n\tr[%.1f]\n\t&trans[%p]\n\twamb[%.1f]\n\trgb[%.1f %.1f %.1f]\n\n", world->objects_list->form, sp->center.x, sp->center.y, sp->center.z,
// 		sp->radius, sp->transform, sp->material.ambient,sp->material.color.x, sp->material.color.y, sp->material.color.z);
// 		// fprintf(stderr, "F");

// 	}
// 	printf("\nworld lights:\n");
// 	for (; world->lights_list; world->lights_list = world->lights_list->next)
// 	{
// 		printf("\tposition[%.1f %.1f %.1f]", world->lights_list->pos.x, world->lights_list->pos.y, world->lights_list->pos.z);
// 		printf("\n\trgb[%.1f %.1f %.1f]", world->lights_list->color.x, world->lights_list->color.y, world->lights_list->color.z);
// 		printf("\n\tintensity[%.1f]\n\n", world->lights_list->intensity);
// 	} 
// }