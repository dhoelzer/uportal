
void window_update()
{
  wnoutrefresh(Input);
  doupdate();
}

void setup_windows()
{
  int Lines, half_width;
  char Message[30];


  initscr();
  start_color();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(1);

  init_pair(0,COLOR_BLACK,COLOR_WHITE);
  init_pair(1,COLOR_RED,COLOR_BLACK);
  init_pair(2,COLOR_GREEN,COLOR_BLACK);
  init_pair(3,COLOR_YELLOW,COLOR_BLACK);
  init_pair(4,COLOR_BLUE,COLOR_BLACK);
  init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(6,COLOR_CYAN,COLOR_BLACK);
  init_pair(7,COLOR_WHITE,COLOR_BLACK);

  Lines = LINES;
  half_width = COLS/2 - 1;

  Input = newwin(2, COLS, Lines-2, 0);
  Lines -= 3;
  Empty_Bar = newwin(1,COLS,Lines,0);
  Lines--;
  HPBar2 = newwin(1, half_width, Lines, 0);
  HPBar3 = newwin(1, half_width, Lines, half_width+2);
  Lines--;
  HPBar = newwin(1, COLS, Lines, 0);
  Lines--;
  Attacker_Bar = newwin(1, COLS, Lines, 0);
  Lines--;
  Mud = newwin(Lines-1, COLS-2, 1, 1);
  Mud_Window_Output_Line = Lines-3;
  
  Status_Bar=newwin(1,COLS,0,0);

  sprintf(Message,"Main Mud Window %i %i",LINES,COLS);
  //wborder(Mud, '|','|','-','-','+','+','+','+');
  
  scrollok(Mud, TRUE);
  scrollok(Input, TRUE);

  waddstr(Empty_Bar, Bar);
  wnoutrefresh(Empty_Bar);
  waddstr(HPBar2, "HPBar 2");
  waddstr(HPBar3, "HPBar 3");
  hp();
  do_status_bar();
}
