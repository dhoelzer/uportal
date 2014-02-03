void from_mud(char *what)
{
  int i,j, len;
  int anding = 0;
  int color_pair, high, standout, flashing, underline; 
  char *ptr, *ptr2, *end, *length;
  static char stripped[RECEIVE_SIZE];

  if(!what) 
    { 
      from_mud("\n\n%% -> From Mud called with empty pointer\n\n"); 
      return;
    }
  len = strlen(what);
  if(len > RECEIVE_SIZE)
    {
      from_mud("\n\n%% From Mud called with overflow.\n\n");
      return;
    }
  length = len + what;
  for(i=0,j=0;i < len;i++) if(isprint(what[i])) stripped[j++]=what[i];
  stripped[j]=0;
  
  for(i=0,ptr=what;ptr!=length;ptr++)
    {
      if(isprint(*ptr)||strchr("\n\t\b", *ptr))
	{
	  if(logging)fputc(*ptr,log_file);
	  if(!anding) waddch(Mud,*ptr);
	  else 
	    waddch(Mud, *ptr | 
		   COLOR_PAIR(color_pair) | 
		   (high ? A_BOLD : 0) );// |
		   //		   (standout ? A_STANDOUT : 0) |
		   // (flashing ? A_BLINK : 0) |
		   // (underline ? A_UNDERLINE : 0));
	}
      else
	{
	  if(ptr > (length - 4))
	    {
	      //from_mud("\n\n%% Processing Ansi would read past end of buffer.\n\n");
	      break;
	    }
	  if(strncmp(ptr,"[0m",4)==0)
	    {
	      anding = 0;
	      high = 0;
	      standout = 0;
	      flashing = 0;
	      underline = 0;
	      ptr += 3;
	      //	  continue;
	    }
	  if(strncmp(ptr,"[7m",4)==0)
	    {
	      ptr += 3;
	      continue;
	    }
	  if(strncmp(ptr,"[7;",4)==0)
	    {
	      //	  anding = 1;
	      //standout = 1;
	      ptr += 3;
	      //	  continue;
	    }
	  if(strncmp(ptr,"[1m",4)==0)
	    {
	      anding = 1;
	      high = 1;
	      ptr += 3;
	      continue;
	    }
	  if(strncmp(ptr,"[1;",4)==0)
	    {
	      anding = 1;
	      high = 1;
	      ptr += 3;
	      //	  continue;
	    }
	  if(strncmp(ptr,"[5m",4)==0)
	    {
	      ptr += 3;
	      continue;
	    }
	  if(strncmp(ptr,"[5;",4)==0)
	    {
	      //	  anding = 1;
	      //	  flashing = 1;
	      ptr += 3;
	      //	  continue;
	    }
	  if(strncmp(ptr,"[4m",4)==0)
	    {
	      ptr += 3;
	      continue;
	    }
	  if(strncmp(ptr,"[4;",4)==0)
	    {
	      //	  anding = 1;
	      //	  underline = 1;
	      ptr += 3;
	      //	  continue;
	    }
	  if(strncmp(ptr,";3",2)==0) 
	    {
	      //	      if(*ptr == ';') ptr++;
	      end=(char *)strchr(ptr,'m');
	      *end = 0;
	      anding = 1;
	      ptr2 = (char *)strchr(ptr,';');
	      if(ptr2) high = 1; else high = 0;
	      color_pair = atoi(ptr+2);
	      if(color_pair < 0 || color_pair > 6)color_pair = 1;
	      ptr=end;
	      continue;
	    }
	  if(strncmp(ptr,"[3",3)==0) 
	    {
	      //	      if(*ptr == ';') ptr++;
	      end=(char *)strchr(ptr,'m');
	      *end = 0;
	      anding = 1;
	      ptr2 = (char *)strchr(ptr,';');
	      if(ptr2) high = 1; else high = 0;
	      color_pair = atoi(ptr+3);
	      if(color_pair < 0 || color_pair > 6)color_pair = 1;
	      ptr=end;
	    }
	}
    }
  wnoutrefresh(Mud);
  if(strstr(stripped, "dealt the killing blow")) direct_to_mud("dis\n");
  window_update();
}
