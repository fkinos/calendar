#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define TITLE "KOYOMI"
#define WIDTH 152
#define HEIGHT 134

#define BLACK 0x000000
#define WHITE 0xffffff
#define RED 0xff0000
#define BLUE 0x3333de

static int zoom = 1;
static int reqdraw;

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;
static SDL_Texture *gTexture = NULL;
static unsigned int *pixels;

static unsigned char font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*    */
  0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00, /* !  */
  0x36, 0x36, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, /* "  */
  0x14, 0x14, 0x7e, 0x28, 0xfc, 0x50, 0x50, 0x00, /* #  */
  0x08, 0x3c, 0x60, 0x38, 0x0c, 0x78, 0x20, 0x00, /* $  */
  0x5c, 0xa4, 0x48, 0x10, 0x24, 0x4a, 0x04, 0x00, /* %  */
  0x38, 0x6c, 0x60, 0x32, 0x6c, 0x6c, 0x38, 0x00, /* &  */
  0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, /* '  */
  0x0c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0c, 0x00, /* (  */

  /* modified */
  0x7f, 0x41, 0x41, 0x7f, 0x41, 0x41, 0x7f, 0x00, /* ) */
  0x3f, 0x21, 0x3f, 0x21, 0x3f, 0x21, 0x43, 0x00, /* * */
  0x08, 0x49, 0x2a, 0x08, 0x14, 0x14, 0x63, 0x00, /* + */
  0x08, 0x69, 0x2e, 0x2c, 0x2a, 0x49, 0x18, 0x00, /* , */
  0x08, 0x7f, 0x08, 0x1c, 0x2a, 0x49, 0x08, 0x00, /* - */
  0x08, 0x36, 0x41, 0x3e, 0x49, 0x2a, 0x7f, 0x00, /* . */
  0x08, 0x08, 0x3e, 0x08, 0x08, 0x08, 0x7f, 0x00, /* / */
  0x18, 0x24, 0x2c, 0x34, 0x24, 0x24, 0x18, 0x00, /* 0 */
  0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, /* 1 */
  0x38, 0x04, 0x04, 0x18, 0x20, 0x20, 0x3c, 0x00, /* 2 */
  0x38, 0x04, 0x04, 0x18, 0x04, 0x04, 0x38, 0x00, /* 3 */
  0x24, 0x24, 0x24, 0x24, 0x1c, 0x04, 0x04, 0x00, /* 4 */
  0x3c, 0x20, 0x20, 0x38, 0x04, 0x04, 0x38, 0x00, /* 5 */
  0x18, 0x20, 0x20, 0x38, 0x24, 0x24, 0x18, 0x00, /* 6 */
  0x3c, 0x04, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00, /* 7 */
  0x18, 0x24, 0x24, 0x18, 0x24, 0x24, 0x18, 0x00, /* 8 */
  0x18, 0x24, 0x24, 0x1c, 0x04, 0x04, 0x18, 0x00, /* 9 */

  0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, /* :  */
  0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x30, /* ;  */
  0x00, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x00, 0x00, /* <  */
  0x00, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x00, 0x00, /* =  */
  0x00, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x00, 0x00, /* >  */
  0x38, 0x4c, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00, /* ?  */
  0x38, 0x6c, 0x7c, 0x6c, 0x70, 0x64, 0x38, 0x00, /* @  */
  0x38, 0x6c, 0x6c, 0x7c, 0x6c, 0x6c, 0x6c, 0x00, /* A  */
  0x78, 0x6c, 0x6c, 0x78, 0x6c, 0x6c, 0x78, 0x00, /* B  */
  0x38, 0x64, 0x60, 0x60, 0x60, 0x64, 0x38, 0x00, /* C  */
  0x78, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x78, 0x00, /* D  */
  0x7c, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7c, 0x00, /* E  */
  0x7c, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00, /* F  */
  0x38, 0x64, 0x60, 0x60, 0x6c, 0x6c, 0x3c, 0x00, /* G  */
  0x6c, 0x6c, 0x6c, 0x7c, 0x6c, 0x6c, 0x6c, 0x00, /* H  */
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, /* I  */
  0x0c, 0x0c, 0x0c, 0x0c, 0x6c, 0x6c, 0x38, 0x00, /* J  */
  0x6c, 0x6c, 0x6c, 0x78, 0x6c, 0x6c, 0x6c, 0x00, /* K  */
  0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7c, 0x00, /* L  */
  0x82, 0xc6, 0xee, 0xbe, 0x96, 0x86, 0x86, 0x00, /* M  */
  0x44, 0x64, 0x74, 0x7c, 0x5c, 0x4c, 0x44, 0x00, /* N  */
  0x38, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x38, 0x00, /* O  */
  0x78, 0x6c, 0x6c, 0x78, 0x60, 0x60, 0x60, 0x00, /* P  */
  0x38, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x38, 0x0c, /* Q  */
  0x78, 0x6c, 0x6c, 0x78, 0x6c, 0x6c, 0x6c, 0x00, /* R  */
  0x38, 0x64, 0x30, 0x18, 0x0c, 0x4c, 0x38, 0x00, /* S  */
  0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, /* T  */
  0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x38, 0x00, /* U  */
  0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x68, 0x70, 0x00, /* V  */
  0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xfc, 0x00, /* W  */
  0x6c, 0x6c, 0x6c, 0x38, 0x6c, 0x6c, 0x6c, 0x00, /* X  */
  0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x30, 0x00, /* Y  */
  0x7c, 0x0c, 0x18, 0x30, 0x60, 0x60, 0x7c, 0x00, /* Z  */
  0x70, 0x60, 0x60, 0x60, 0x60, 0x60, 0x70, 0x00, /* [  */
  0x30, 0x30, 0x18, 0x18, 0x18, 0x0c, 0x0c, 0x00, /* \ (backslash)  */
  0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x70, 0x00, /* ]  */
  0x18, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, /* ^  */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, /* _  */
  0x38, 0x64, 0x60, 0x70, 0x60, 0x60, 0x7c, 0x00, /* £  */
  0x00, 0x38, 0x4c, 0x3c, 0x6c, 0x6c, 0x3c, 0x00, /* a  */
  0x60, 0x60, 0x78, 0x6c, 0x6c, 0x6c, 0x78, 0x00, /* b  */
  0x00, 0x38, 0x64, 0x60, 0x60, 0x64, 0x38, 0x00, /* c  */
  0x0c, 0x0c, 0x3c, 0x6c, 0x6c, 0x6c, 0x3c, 0x00, /* d  */
  0x00, 0x38, 0x6c, 0x7c, 0x60, 0x64, 0x38, 0x00, /* e  */
  0x1c, 0x30, 0x78, 0x30, 0x30, 0x30, 0x30, 0x00, /* f  */
  0x00, 0x3c, 0x6c, 0x6c, 0x6c, 0x3c, 0x4c, 0x38, /* g  */
  0x60, 0x60, 0x78, 0x6c, 0x6c, 0x6c, 0x6c, 0x00, /* h  */
  0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, /* i  */
  0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x4c, 0x38, /* j  */
  0x60, 0x6c, 0x6c, 0x78, 0x6c, 0x6c, 0x6c, 0x00, /* k  */
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, /* l  */
  0x00, 0xfc, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0x00, /* m  */
  0x00, 0x78, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x00, /* n  */
  0x00, 0x38, 0x6c, 0x6c, 0x6c, 0x6c, 0x38, 0x00, /* o  */
  0x00, 0x78, 0x6c, 0x6c, 0x6c, 0x78, 0x60, 0x60, /* p  */
  0x00, 0x3c, 0x6c, 0x6c, 0x6c, 0x3c, 0x0c, 0x0c, /* q  */
  0x00, 0x6c, 0x70, 0x60, 0x60, 0x60, 0x60, 0x00, /* r  */
  0x00, 0x38, 0x64, 0x30, 0x18, 0x4c, 0x38, 0x00, /* s  */
  0x18, 0x18, 0x3c, 0x18, 0x18, 0x18, 0x0c, 0x00, /* t  */
  0x00, 0x6c, 0x6c, 0x6c, 0x6c, 0x6c, 0x3c, 0x00, /* u  */
  0x00, 0x6c, 0x6c, 0x6c, 0x6c, 0x68, 0x70, 0x00, /* v  */
  0x00, 0xd6, 0xd6, 0xd6, 0xd6, 0xd6, 0xfc, 0x00, /* w  */
  0x00, 0x6c, 0x6c, 0x38, 0x6c, 0x6c, 0x6c, 0x00, /* x  */
  0x00, 0x6c, 0x6c, 0x6c, 0x6c, 0x3c, 0x4c, 0x38, /* y  */
  0x00, 0x7c, 0x0c, 0x18, 0x30, 0x60, 0x7c, 0x00, /* z  */
  0x1c, 0x18, 0x18, 0x70, 0x18, 0x18, 0x1c, 0x00, /* {  */
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, /* |  */
  0x70, 0x30, 0x30, 0x1c, 0x30, 0x30, 0x70, 0x00, /* }  */
  0x6c, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ~  */
  0x3c, 0x42, 0x99, 0xb1, 0xb1, 0x99, 0x42, 0x3c, /* ©  */
};

int abs (int j)
{
  if (j == 0) return 0;
  if (j > 0) return j;
  return -j;
}

static void
clear_screen (unsigned int *pixels, unsigned int color)
{
  int i;
  for (i = 0; i < WIDTH * HEIGHT; i++)
    pixels[i] = color;
}

static void
draw_pixel (unsigned int *pixels, int x, int y, unsigned int color)
{
  if (x < WIDTH && y < HEIGHT && x > 0 && y > 0)
    pixels[y * WIDTH + x] = color;
}

static void
draw_icn (unsigned int *pixels, int x, int y, unsigned char *sprite, unsigned int color)
{
  int y2 = y + 8, h;

  for (; y < y2; y++, sprite++) {
    for (h = 0; h < 8; h++) {
      if (*sprite << h & 0x80)
        draw_pixel (pixels, x + h, y, color);
    }
  }
}

static void
draw_line (unsigned int *pixels, int ax, int ay, int bx, int by, unsigned int color)
{
  int dx = abs (bx - ax), sx = ax < bx ? 1 : -1;
  int dy = -abs (by - ay), sy = ay < by ? 1 : -1;
  int err = dx + dy, e2;
  for (;;) {
    draw_pixel (pixels, ax, ay, color);
    if (ax == bx && ay == by) break;
    e2 = 2 * err;
    if (e2 >= dy) err += dy, ax += sx;
    if (e2 <= dx) err += dx, ay += sy;
  }
}

void
draw_letter (unsigned int *pixels, int x, int y, int letter, unsigned int color)
{
  int idx = letter - 32;

  draw_icn (pixels, x, y, font + (idx * 8), color);
}

void
draw_string (unsigned int *pixels, int x, int y, char *string, unsigned int color)
{
  int i;

  for (i = 0; i < (int)strlen (string); i++) {
    draw_letter (pixels, x + (i * 5), y, string[i], color);
  }
}

static void
set_window_size (SDL_Window *window, int w, int h)
{
  SDL_Point win_old;

  SDL_GetWindowSize (window, &win_old.x, &win_old.y);

  if (w == win_old.x && h == win_old.y) return;

  SDL_RenderClear (gRenderer);
  SDL_SetWindowSize (window, w, h);
}

static void
set_zoom (int z, int win)
{
  if (z < 1) return;

  if (win)
    set_window_size (gWindow, WIDTH * z, HEIGHT * z);

  zoom = z;
}

static void
emu_end (void)
{
    free (pixels);

    SDL_DestroyTexture (gTexture), gTexture = NULL;
    SDL_DestroyRenderer (gRenderer), gRenderer = NULL;
    SDL_DestroyWindow (gWindow), gWindow = NULL;

    SDL_Quit ();

    exit(0);
}

static int
init (void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
      fprintf (stderr, "INIT ERROR: %s\n", SDL_GetError ());

    gWindow = SDL_CreateWindow (TITLE,
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WIDTH, HEIGHT,
                                SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
      fprintf (stderr, "WINDOW ERROR: %s\n", SDL_GetError ());

    gRenderer = SDL_CreateRenderer (gWindow, -1, 0);
    if (gRenderer == NULL)
      fprintf (stderr, "RENDERER ERROR: %s\n", SDL_GetError ());

    gTexture = SDL_CreateTexture(gRenderer,
                                 SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STATIC,
                                 WIDTH, HEIGHT);
    if (gTexture == NULL)
      fprintf (stderr, "TEXTURE ERROR: %s\n", SDL_GetError ());

    pixels = (unsigned int *)malloc (WIDTH * HEIGHT * sizeof (unsigned int));
    if (pixels == NULL)
      fprintf (stderr, "PIXELS ERROR: %s\n", "Failed to allocate memory");

    SDL_ShowCursor (1);

    return 1;
}

static char *weekdays[] = {
  ")", "*", "+", ",", "-", ".", "/",
};

static int months_days[12] = {
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int current_month, actual_month, current_year, actual_year;

int
year ()
{
  struct tm *tm;
  time_t t;

  t = time (NULL);
  tm = localtime (&t);

  return tm->tm_year;
}

int
month ()
{
  struct tm *tm;
  time_t t;

  t = time (NULL);
  tm = localtime (&t);

  return tm->tm_mon;
}

int
weekday (int day)
{
  struct tm *tm;
  time_t t;
  time_t nt;

  t = time (NULL);
  tm = localtime (&t);

  tm->tm_year = current_year;
  tm->tm_mon = current_month;
  tm->tm_mday = day;

  nt = mktime(tm);
  tm = localtime (&nt);

  return tm->tm_wday;
}

void
draw ()
{
  int i, j;
  time_t t;
  time_t nt;
  struct tm *tm;
  char heading[256];
  int calendar[6][7];
  int pos = 0;
  int idx = 0;
  int today, start, days;

  t = time (0);
  tm = localtime (&t);

  tm->tm_yday = current_year;
  tm->tm_mon = current_month;
  nt = mktime (tm);
  tm = localtime (&nt);

  today = tm->tm_mday;
  start = weekday(1) + 1;
  days = months_days[current_month];

  clear_screen (pixels, WHITE);

  sprintf (heading, "%d %d", tm->tm_year + 1900, tm->tm_mon + 1);
  draw_string (pixels, 8, 8, heading, BLACK);

  draw_line (pixels, 8, 20, WIDTH - 9, 20, BLACK);

  /* draw week days */
  for (i = 0; i < 7; i++) {
    int color = BLACK;
    if (i == 0 || i == 6) {
      color = BLUE;
    }
    draw_string (pixels, 12 + (i * 20), 28, weekdays[i], color);
  }

  for (i = 0; i < 6; i++) {
    for (j = 0; j < 7; j++) {
      calendar[i][j] = 0;
      pos++;

      if (pos == start || pos > start) {
        if (idx < days) {
          calendar[i][j] = idx + 1;
          idx++;
        }
      }
    }
  }

  /* draw days */
  for (i = 0; i < 7; i++) {
    for (j = 0; j < 6; j++) {
      if (calendar[j][i] != 0) {
        char day_string[3];
        int color = BLACK;

        if (i == 0 || i == 6)
          color = BLUE;
        if (calendar[j][i] == today && (current_month == actual_month && current_year == actual_year))
          color = RED;

        snprintf (day_string, 3, "%2d", calendar[j][i]);
        draw_string (pixels, 8 + (i * 20), 44 + (j * 15), day_string, color);
      }
    }
  }
}

void
handle_keys (SDL_Event e)
{
  int sym = e.key.keysym.sym;
  int offset = 0;
  int new_current_month = 0;
  int last_current_month = 0;

  switch (sym) {
  case SDLK_LEFT:
    offset = -1;
    break;
  case SDLK_RIGHT:
    offset = 1;
    break;
  }

  last_current_month = current_month;

  new_current_month = (current_month - 1 + (offset % 12) + 12) % 12 + 1;
  current_month = new_current_month;

  if (last_current_month == 1 && new_current_month == 12) {
    current_year -= 1;
  }

  if (last_current_month == 12 && new_current_month == 1) {
    current_year += 1;
  }
}

int
main (int argc, char *argv[])
{
  Uint32 begintime = 0, endtime = 0, delta = 0;

  if (!init ())
    fprintf (stderr, "INIT ERROR: FAILURE\n");
  set_zoom (1, 1);

  current_year = year ();
  actual_year = year ();
  current_month = month ();
  actual_month = month ();

  clear_screen (pixels, BLACK);

  /* Game Loop */
  while (1) {
    SDL_Event e;

    /* don't draw if not needed */
    if (!begintime) {
      begintime = SDL_GetTicks ();
    } else {
      delta = endtime - begintime;
    }

    if (delta < 30)
      SDL_Delay (30 - delta);

    /* gather events from the user and the operation system */
    while (SDL_PollEvent (&e) != 0) {
      switch(e.type) {
      case SDL_QUIT: emu_end (); break;
      case SDL_WINDOWEVENT: reqdraw = 1; break;
      case SDL_KEYDOWN: handle_keys(e); break;
      }
    }

    /* draw */
    draw ();

    /* draw pixels array to SDL texture */
    SDL_UpdateTexture (gTexture, NULL, pixels, WIDTH * sizeof (unsigned int));
    SDL_RenderCopy (gRenderer, gTexture, NULL, NULL);
    SDL_RenderPresent (gRenderer);

    begintime = endtime;
    endtime = SDL_GetTicks ();
  }

  emu_end();

  return 0;
}
