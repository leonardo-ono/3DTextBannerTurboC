/*
	3D Text Banner
	written by Leonardo Ono (ono.leo@gmail.com)
	03/12/2017

	target OS: DOS

	compiled with turbo c

	note: needs to compile with large memory model

	use: tcc -ml banner.c font.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <math.h>

#define TERRAIN 0
#define TEXT 1

typedef unsigned char byte;

extern byte font[128][128];

byte *VGA = (byte *) 0xa0000000;
byte *OFFSCREEN;
char *message = "    3D TEXT BANNER ... BTW, THANKS TO ALL 300+ SUBSCRIBERS !!!         ";

int view_x = 0;

float angle, angle_y, sin_x, cos_x, sin_y, cos_y;
void set_video_mode(byte mode)
{
	union REGS regs;
	regs.h.ah = 0;
	regs.h.al = mode;
	int86(0x10, &regs, &regs);
}

void init_offscreen()
{
	OFFSCREEN = (byte *) malloc(320 * 200);
	if (OFFSCREEN == NULL)
	{
		printf("could not allocate memory for offscreen image !");
		exit(-1);
	}
}

void clear_screen()
{
	unsigned int i;
	for (i = 0; i < 64000; i++)
	{
		OFFSCREEN[i] = 0;
	}
}

void set_pixel_screen(int x, int y, byte color)
{
	if (x < 0 || y < 0 || x > 319 || y > 199)
	{
		return;
	}
	OFFSCREEN[y * 320 + x] = color;
}

void show()
{
	memcpy(VGA, OFFSCREEN, 64000);
}

void set_pixel_3d(int x, int y, int z, byte color, int mode)
{
	float nx, ny, nz;
	int px, py;

	if (mode == TEXT)
	{
		y = -20 + 5 * sin(x * 0.1 + angle);
	}

	/* rotate y */
	nx = x * cos_y - z * sin_y;	nz = z * cos_y + x * sin_y;	x = nx;	z = nz;

	/* rotate x */
	nz = z * cos_x - y * sin_x;	ny = y * cos_x + z * sin_x;	z = nz;	y = ny;

	z = z - 125;
	px = 150 * x / (float) -z;
	py = 150 * y / (float) -z;
	set_pixel_screen(px + 160, py + 100, color);
}

void render(int mode)
{
	byte color;
	int x, y, mx, my, fx, fy, fcol, frow, row, col;
	for (y = 40; y < 80; y++)
	{
		for (x = 0; x < 160; x++)
		{
			mx = view_x + x;
			my = y;
			col = mx / 40;
			row = my / 40;
			fx = (mx % 40) / 5;
			fy = (my % 40) / 5;
			color = 8;
			if (row == 1)
			{
				frow = message[col] / 16;
				fcol = message[col] % 16;
				color = font[8 * frow + fy][8 * fcol + fx] == 1 ? 15 : 8;
				if (mode == TEXT && color == 15)
				{
					set_pixel_3d(x - 80, 0, y - 60, color, mode);
				}
				else if (mode == TERRAIN && color == 8) {
					set_pixel_3d(x - 80, 0, y - 60, color, mode);
				}
			}
			else if (mode == TERRAIN) {
				set_pixel_3d(x - 80, 0, y - 60, 8, mode);
			}
		}
	}
}

void main()
{
	int max_view_x = (strlen(message) - 4) * 40;
	init_offscreen();
	set_video_mode(0x13);
	sin_x = sin(0.9);
	cos_x = cos(0.9);
	while (1)
	{
		angle_y = cos(angle * 0.05);
		sin_y = sin(angle_y);
		cos_y = cos(angle_y);

		clear_screen();
		render(TERRAIN);
		render(TEXT);
		show();

		if (kbhit())
		{
			getch();
			set_video_mode(0x3);
			exit(1);
		}
		view_x += 4;
		if (view_x > max_view_x)
		{
			view_x = 0;
		}
		angle += 1;

		/* TODO: implement timer */
	}
}
