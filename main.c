#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NUM_OPTIONS 6

int main(void) {
  char name[50];
  int highlight = 0;
  int ch;
  bool selected[NUM_OPTIONS] = {false, false, false, false, false, false};

  // The list of options to choose from
  char *options[NUM_OPTIONS] = {"l.HDORKBIBA%2C", "l.HDORKBIBB%2C",
                                "l.HDHSBIB%2C",   "l.HDTELEA%2C",
                                "l.HDTELEB%2C",   "l.Teknikutl%C3%A5ning%2C"};

  /* ---------------------------
     1. Initialize ncurses mode
     --------------------------- */
  initscr(); // Start curses mode
  clear();

  /* -----------------------------------------------------------
     2. Prompt the user for their name (using ncurses input)
     ----------------------------------------------------------- */
  printw("Enter your name: ");
  echo(); // Enable input echoing so the user can see what they type
  getnstr(name, sizeof(name) - 1);
  noecho();
  clear();

  keypad(stdscr, TRUE); // Enable special keys (arrow keys, etc.)

  /* -----------------------------------------------------------
     3. Display the options and let the user select one or more
        Use arrow keys to navigate and spacebar to toggle selection.
        Press Enter to finish.
     ----------------------------------------------------------- */
  while (1) {
    // Display each option with a checkbox
    for (int i = 0; i < NUM_OPTIONS; i++) {
      if (i == highlight)
        attron(A_REVERSE); // Highlight the current option

      mvprintw(i, 0, "[%c] %s", selected[i] ? 'X' : ' ', options[i]);

      if (i == highlight)
        attroff(A_REVERSE);
    }
    refresh();

    // Get user input and handle key events
    ch = getch();
    switch (ch) {
    case KEY_UP:
      highlight--;
      if (highlight < 0)
        highlight = NUM_OPTIONS - 1;
      break;
    case KEY_DOWN:
      highlight++;
      if (highlight >= NUM_OPTIONS)
        highlight = 0;
      break;
    case ' ':
      // Toggle the selection state of the current option
      selected[highlight] = !selected[highlight];
      break;
    case '\n':
      // Exit the selection loop when Enter is pressed
      goto exit_loop;
    default:
      break;
    }
  }
exit_loop:
  endwin(); // End ncurses mode

  /* -----------------------------------------------------------
     4. Build the "resurser" part of the URL based on selections.
        We loop over the options and concatenate the selected ones.
     ----------------------------------------------------------- */
  char resurser[256] = "";
  for (int i = 0; i < NUM_OPTIONS; i++) {
    if (selected[i]) {
      strcat(resurser, options[i]);
    }
  }

  /* -----------------------------------------------------------
     5. Construct the final URL string.
        The {name here} is replaced with the user's name, and the
        {l. things here} part is replaced with the concatenated options.
     ----------------------------------------------------------- */
  char link[512];
  sprintf(link,
          "https://schema.mau.se/setup/jsp/"
          "Schema.jsp?startDatum=idag&intervallTyp=a&intervallAntal=1&moment=%"
          "s&sokMedAND=true&sprak=SV&resurser=%s",
          name, resurser);

  /* -----------------------------------------------------------
     6. Print the final URL to the standard output.
     ----------------------------------------------------------- */
  printf("Generated URL:\n%s\n", link);

  return 0;
}