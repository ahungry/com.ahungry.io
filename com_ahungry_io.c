#include <janet.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>     /* defines STDIN_FILENO, system calls,etc */
#include <termios.h>

// Keep all the state of app in here.

struct erow
{
  int size;
  char *chars;
};
typedef struct erow erow;

struct world_atom
{
  char *udp_out_host;
  char *udp_out_port;
  char *udp_listen_port;
  int cx, cy;
  int rowoff;
  int rows;
  int cols;
  int numrows;
  erow *row;
  struct termios orig_termios;
};

typedef struct world_atom world_atom;

void
init_world ();

world_atom world;

void
init_world ()
{
  world.cx = 0;
  world.cy = 0;
  world.rowoff = 0;
  world.numrows = 0;
  world.row = NULL;
}

void
disable_raw_mode ()
{
  if (-1 == tcsetattr (STDIN_FILENO, TCSAFLUSH, &world.orig_termios))
    fprintf (stderr, "tcsetattr");
}

void
enable_raw_mode ()
{
  if (-1 == tcgetattr (STDIN_FILENO, &world.orig_termios)) fprintf (stderr, "tcgetattr");
  atexit (disable_raw_mode);

  struct termios raw = world.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // Changes \n into \r\n as default when left off
  // raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  // raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1; // Every 10th of second redraw / skip the read (stop block).

  if (-1 == tcsetattr (STDIN_FILENO, TCSAFLUSH, &raw)) fprintf (stderr, "tcsetattr");
}

static Janet
fgetc_wrapped (int32_t argc, Janet *argv)
{
  int nread;
  char c;

  while ((nread = read (STDIN_FILENO, &c, 1)) != 1)
    {
      if (nread == -1 && errno != EAGAIN) fprintf (stderr, "read");
    }

  // int ch = fgetc (stdin);

  return janet_wrap_number (c);
}

static const JanetReg
com_ahungry_io_cfuns[] = {
  {"fgetc", fgetc_wrapped, "Get a single character as an int."},
  {NULL,NULL,NULL}
};

/* extern const unsigned char *pobox_lib_embed; */
/* extern size_t pobox_lib_embed_size; */

JANET_MODULE_ENTRY (JanetTable *env) {
  init_world ();
  enable_raw_mode ();
  janet_cfuns (env, "com_ahungry_io", com_ahungry_io_cfuns);
  /* janet_dobytes(env, */
  /*               pobox_lib_embed, */
  /*               pobox_lib_embed_size, */
  /*               "pobox_lib.janet", */
  /*               NULL); */
}
