void do_status_bar()
{
  int x;

  sprintf(STATUS_BAR,
	  "[---] uPortal %5s [----] Uptime: %5s [----] Mudlag: %4s [ %c%c%c ]",
	  Version_Number, UPTIME, MUDLAG,(logging ? 'L':' '),
	  (0 ? ' ':' '),
	  (0 ? ' ':' '));
  x=COLS-strlen(STATUS_BAR);
  if(x>=0) for(x--;x!=0;x--)strcat(STATUS_BAR,"-");
  strcat(STATUS_BAR,"]");
  mvwaddstr(Status_Bar,0,0,STATUS_BAR);
  wnoutrefresh(Status_Bar);
  window_update();
}

void build_glines()
{
  int i, loop;
  //  char stats[90];
  char *what;
  int anding = 0;
  int handled = 0;
  int color_pair, high, standout, flashing, underline; 
  char *ptr, *length;
  WINDOW *which_window;
  
  wclear(HPBar2);
  wclear(HPBar3);
  wnoutrefresh(HPBar3);
  wnoutrefresh(HPBar2); 
  for(loop=0;loop!=2;loop++){
    handled = 0;
    if(loop==0)
      {
	which_window = HPBar2;
	what = HPBAR1;
      }
    else
      {
	which_window = HPBar3;
	what = HPBAR2;
      }
    length = strlen(what) + what;
    for(i=0,ptr=what;ptr!=length;ptr++)
      {
	handled = 0;
	if(*ptr == '>' && anding)
	  {
	    anding = 0;
	    high = 0;
	    handled = 1;
	    standout = 0;
	    flashing = 0;
	    underline = 0;
	    continue;
	  } 
	if(*ptr=='<' && ( 
			 (*(ptr+1)=='y') ||
			 (*(ptr+1)=='r') ||
			 (*(ptr+1)=='b') ||
			 (*(ptr+1)=='g') ||
			 (*(ptr+1)=='c') ||
			 (*(ptr+1)=='v') ||
			 (*(ptr+1)=='s')))
	  {
	    //sprintf(stats,"Received gline color: %c%c\n",*ptr,*(ptr+1));
	    //from_mud(stats);
	    anding = 1;
	    switch(*(ptr+1)) {
	    case 'y': color_pair=3;
	      break;
	    case 'r': color_pair=1;
	      break;
	    case 'b': color_pair=4;
	      break;
	    case 'g': color_pair=2;
	      break;
	    case 'c': color_pair=6;
	      break;
	    case 'v': color_pair=5;
	      break;
	    case 's': color_pair=7;
	      break;
	    }
	    ptr++;
	    continue;
	  }
	if(isprint(*ptr) || *ptr == '\n')
	  {
	    if(!anding)waddch(which_window,*ptr);
	    else waddch(which_window, *ptr | 
			COLOR_PAIR(color_pair));
	  }
      }
  }
  wnoutrefresh(HPBar2);
  wnoutrefresh(HPBar3);
}

void hp()
{
  int x, width, i, size_str;
  char temp_str[80];

  strcpy(HPBAR,Bar);
  width=COLS/6;
  x=width-8;

  x=2;
  width = 15;

  sprintf(temp_str,"[HP: %4d/%4d]--[SP: %4d/%4d]--[GP: %4d/%4d]"
	  "--[GP2: %4d/%4d]--", HP, MAXHP,
	  SpellPoints,MAXSpellPoints,GP1, GP1MAX, GP2, GP2MAX);
  size_str = strlen(temp_str);
  for(i=0;i!=size_str;i++) HPBAR[x+i] = temp_str[i];

  sprintf(ATTACKERBAR,"Attacker: %-30s   Condition: %3d%%",ATTACKER, ATTACKCOND);

  build_glines();
  mvwaddstr(HPBar,0,0,HPBAR);
  mvwaddstr(Attacker_Bar,0,0,ATTACKERBAR);
  wnoutrefresh(HPBar);
  wnoutrefresh(Attacker_Bar);
  window_update();
}
