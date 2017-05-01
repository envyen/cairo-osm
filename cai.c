#include <stdio.h>
#include <math.h>
#include <cairo/cairo.h>
#include <stdlib.h>

int lon2x(double lon, int z)
{
	float txf = (lon + 180.0) / 360.0 * pow(2.0, z);
	int   tx = (floor(txf));

	printf("x = %f %d\n",(txf-tx)*256 , tx);

	return (int)((txf-tx)*256);
}

int lat2y(double lat, int z)
{
	float tyf = (1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z);
	int   ty = (floor(tyf));

	printf("y = %f %d\n",(tyf-ty)*256 , ty);
	return (int)((tyf-ty)*256);
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
	int zoom = 10;
	double lat = 9.5587335, lat1;
	double lon = 76.8978232, lon1;
	double x;

	if(argc!=4){
		printf("%s lat lon zoom\n", argv[0]);
		exit(-1);
	}
	lat = atof(argv[1]);
	lon = atof(argv[2]);
	zoom = atoi(argv[3]);

	sprintf(url , "curl -o tmp http://a.tile.openstreetmap.org/%d/%d/%d.png",
			zoom, lon2tilex(lon, zoom), lat2tiley(lat, zoom));

	system(url);

//	sprintf(url, "%d.png", lat2tiley(lat, zoom));
//	cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 256, 256);

	cairo_surface_t *surface =
		cairo_image_surface_create_from_png("tmp");

	cairo_t *cr = cairo_create(surface);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

	cairo_set_source_rgba(cr, 0, 0.5, 0, 0.5);
	cairo_set_line_width(cr, 30);
	cairo_move_to(cr, lon2x(lon, zoom), lat2y(lat, zoom));
	cairo_close_path(cr);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 5);
	cairo_move_to(cr, lon2x(lon, zoom), lat2y(lat, zoom));
	cairo_close_path(cr);
	cairo_stroke(cr);


	cairo_surface_write_to_png(surface, "output.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	system("eog output.png");
	return 0;
}

// http://[abc].tile.openstreetmap.org/zoom/x/y.png



void main2() {
	char url[100];
	int zoom = 10;
	double lat = 9.5587335, lat1;
	double lon = 76.8978232, lon1;

	printf("http://a.tile.openstreetmap.org/%d/%d/%d.png\n",
	zoom, lon2tilex(lon, zoom), lat2tiley(lat, zoom));

}
