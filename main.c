/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkellum <nkellum@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:29:43 by nkellum           #+#    #+#             */
/*   Updated: 2019/08/22 20:30:58 by nkellum          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/*
	Cool colors:
	226 237 185
*/

void	plot(int x, int y, t_mlx *mlx, int iteration)
{
	int index;

	index = 4 * (y * 600) + 4 * x;
	mlx->img_str[index] = (char)mlx->r * iteration;
	mlx->img_str[index + 1] = (char)mlx->g * iteration;
	mlx->img_str[index + 2] = (char)mlx->b * iteration;
}

void draw_cross(t_mlx *mlx)
{
	int i = 0;
	while(i < 600)
	{
		plot(i, 300, mlx, 0);
		i++;
	}
	i = 0;
	while(i < 600)
	{
		plot(300, i, mlx, 0);
		i++;
	}
}

void fill_background(t_mlx *mlx)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while(y < 600)
	{
		x = 0;
		while(x < 600)
		{
			plot(x, y, mlx, 1);
			x++;
		}
		y++;
	}
}


int mouse_pressed(int button, int x, int y, void *param)
{
	t_mlx *mlx;

	mlx = (t_mlx *) param;
	if(button == 1)
	{
		mlx->frozen = mlx->frozen ? 0 : 1;
	}
	if(button == 4 && mlx->frozen) // scroll in
	{
		if(x >= 300)
			mlx->horiz += (0.00583333 / mlx->zoom) * (x - 300);
		else
			mlx->horiz -= (300 - x) * 0.00583333 / mlx->zoom;
		if(y >= 300)
			mlx->vert += (0.005 / mlx->zoom) * (y - 300);
		else
			mlx->vert -= (300 - y) * (0.005 / mlx->zoom);
		zoom_in(mlx);
	}
	if(button == 5 && mlx->zoom > 1 && mlx->frozen) // scroll out
	{
		if(x >= 300)
			mlx->horiz += (0.00583333 / mlx->zoom) * (x - 300);
		else
			mlx->horiz -= (300 - x) * 0.00583333 / mlx->zoom;
		if(y >= 300)
			mlx->vert += (0.005 / mlx->zoom) * (y - 300);
		else
			mlx->vert -= (300 - y) * (0.005 / mlx->zoom);
		zoom_out(mlx);
	}

	// printf("mouse button %d pressed at %d %d\n", button, x, y);
	return 1;
}

int	mouse_moved(int x, int y, void *param)
{
	t_mlx *mlx;

	mlx = (t_mlx *) param;
	if(!mlx->frozen)
	{
		mlx->cx = map(x, 0, 600, -1, 1);
		mlx->cy = map(y, 0, 600, -1, 1);
		redraw(mlx);
	}
	return 1;
}


void redraw(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, 600, 600);
	fill_background(mlx);
	if(mlx->fractal == 1)
		mandelbrot(mlx);
	if(mlx->fractal == 2)
		julia(mlx);
	if(mlx->fractal == 3)
		burning_ship(mlx);
	if(mlx->fractal == 4)
		tricorn(mlx);
	mlx->img_str =  mlx_get_data_addr(mlx->img_ptr,
		&(mlx->bpp), &(mlx->size_line), &(mlx->endian));
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
}

int deal_key(int key, void *param)
{
	t_mlx *mlx;
	double direction;

	direction = 0;
	double offset = 0.3;
	mlx = (t_mlx *) param;
	//printf("key %d\n", key);

	if(key == 18) // 1 key
	{
		mlx->fractal = 1;
		key = 15;
		redraw(mlx);
	}
	if(key == 19) // 2 key
	{
		mlx->fractal = 2;
		key = 15;
		redraw(mlx);
	}
	if(key == 20) // 3 key
	{
		mlx->fractal = 3;
		key = 15;
		redraw(mlx);
	}
	if(key == 21) // 4 key
	{
		mlx->fractal = 4;
		key = 15;
		redraw(mlx);
	}
	if(key == 1) // s for one color
	{
		mlx->r = 1;
		mlx->g = 1;
		mlx->b = 1;
		redraw(mlx);
	}
	if(key == 24) // '-' key (+)
	{
		mlx->max_iteration += 10;
		redraw(mlx);
	}
	if(key == 27) // '=' key (-)
	{
		mlx->max_iteration += 10;
		redraw(mlx);
	}
	if(key == 53 || key == 65307)
		exit(0);
	if(key == 15) // RESET with 'r'
	{
		mlx->zoom = 1.0;
		mlx->horiz = 0;
		mlx->vert = 0;
		mlx->r = 226;
		mlx->g = 237;
		mlx->b = 185;
		mlx->max_iteration = 100;
		redraw(mlx);
	}
	if(key == 69)
	{
		mlx->vert = 0;
		redraw(mlx);
	}
	if(key == 8) // print colors with with 'c'
	{
		srand(time(NULL));
		mlx->r = rand() % 255;
		mlx->g = rand() % 255;
		mlx->b = rand() % 255;
		redraw(mlx);
	}
	if(key == 53 || key == 65307)
		exit(0);
	if(key == 123) // LEFT
	{
		mlx->horiz -= offset / mlx->zoom;
		redraw(mlx);
	}
	if(key == 124) // RIGHT
	{
		mlx->horiz += offset / mlx->zoom;
		redraw(mlx);
	}
	if(key == 126) // UP
	{
		mlx->vert -= offset / mlx->zoom;
		redraw(mlx);
	}
	if(key == 125) // DOWN
	{
		mlx->vert += offset / mlx->zoom;
		redraw(mlx);
	}
	// printf("zoom: %f vert: %f horiz: %f\n", mlx->zoom, mlx->vert, mlx->horiz);
	return (0);
}

int	win_close(void *param)
{
	(void)param;
	exit(0);
}

void initialize_mlx(t_mlx *mlx)
{
	mlx->r = 226;
	mlx->g = 237;
	mlx->b = 185;
	mlx->mlx_ptr = mlx_init();
	mlx->offset = 0.1;
	mlx->zoom = 1.0;
	mlx->horiz = 0;
	mlx->horiz_last = 0;
	mlx->vert = 0;
	mlx->oldvert = 0;
	mlx->vert_last = 0;
	mlx->oldhoriz = 0;
	mlx->max_iteration = 100;
	mlx->fractal = 1;
	mlx->cx = 0.0;
	mlx->cy = 0.0;
	mlx->frozen = 0;
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, 600, 600, "Fractol");
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, 600, 600);
	mlx->img_str =  mlx_get_data_addr(mlx->img_ptr, &(mlx->bpp),
	&(mlx->size_line), &(mlx->endian));
}

double map(double val, double start1, double stop1, double start2, double stop2)
{
	return start2 + (stop2 - start2) * ((val - start1) / (stop1 - start1));
}

int main(int argc, char **argv)
{
	int        fd;
	t_mlx *mlx;

	if((mlx = malloc(sizeof(t_mlx))) == NULL)
    	return (0);
	initialize_mlx(mlx);
	fill_background(mlx);

	if(mlx->fractal == 1)
		mandelbrot(mlx);
	if(mlx->fractal == 2)
		julia(mlx);
	if(mlx->fractal == 3)
		burning_ship(mlx);
	if(mlx->fractal == 4)
		tricorn(mlx);

	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, mlx->img_ptr, 0, 0);
	mlx_hook(mlx->win_ptr, 2, 0, deal_key, mlx);
	mlx_hook(mlx->win_ptr, 4, 0, mouse_pressed, mlx);
	mlx_hook(mlx->win_ptr, 17, 0, win_close, mlx);
	mlx_hook(mlx->win_ptr, 6, 0, mouse_moved, mlx);
	mlx_loop(mlx->mlx_ptr);
}
