/*
 * cai-osm.c
 *
 * Copyright 2017 Naveen Karuthedath <naveen.karuthedath@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cairo/cairo.h>

int lon2x(double lon, int z)
{
	float txf = (lon + 180.0) / 360.0 * pow(2.0, z);
	return (int)((txf-(floor(txf)))*256);
}

int lat2y(double lat, int z)
{
	float tyf = (1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z);
	return (int)((tyf-(floor(tyf)))*256);
}

int lon2tilex(double lon, int z)
{
	float txf = (lon + 180.0) / 360.0 * pow(2.0, z);
	return (int)(floor(txf));
}

int lat2tiley(double lat, int z)
{
	float tyf = (1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z);
	return (int) (floor(tyf));
}

double tilex2lon(int x, int z)
{
	return x / pow(2.0, z) * 360.0 - 180;
}

double tiley2lat(int y, int z)
{
	double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

int main(int argc, char **argv)
{
	char url[100];
	int zoom, x, y;
	double lat, lat1;
	double lon, lon1;

	if(argc!=4){
		printf("%s lat lon zoom\n", argv[0]);
		exit(-1);
	}

	lat = atof(argv[1]);
	lon = atof(argv[2]);
	zoom = atoi(argv[3]);
	x = lon2x(lon, zoom);
	y = lat2y(lat, zoom);

	sprintf(url , "curl -o tmp http://a.tile.openstreetmap.org/%d/%d/%d.png",
		zoom, lon2tilex(lon, zoom), lat2tiley(lat, zoom));
	system(url);

	cairo_surface_t *surface = cairo_image_surface_create_from_png("tmp");
	cairo_t *cr = cairo_create(surface);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

	cairo_set_source_rgba(cr, 0, 0.5, 0, 0.5);
	cairo_set_line_width(cr, 30);
	cairo_move_to(cr, x, y);
	cairo_close_path(cr);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 5);
	cairo_move_to(cr, x, y);
	cairo_close_path(cr);
	cairo_stroke(cr);
/*
	cairo_surface_t *img = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 256, 512);
	cairo_t *cr2 = cairo_create(img);
	cairo_set_source_rgba(cr2, 0,0,0, 1);
	cairo_set_source_surface(cr2, surface, 0, 0);
	cairo_paint(cr2);
   	cairo_surface_flush(img);
    	cairo_destroy(cr2);
	cairo_surface_write_to_png(img, "result.png");
*/
	cairo_surface_write_to_png(surface, "output.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	system("eog output.png");
	return 0;
}

// http://[abc].tile.openstreetmap.org/zoom/x/y.png
