void to_inputline(char *what)
{
  static char line_to_mud_screen[RECEIVE_SIZE];

  if(strncmp(what,"OA",3)==0)
    {
      wclear(Input);
      wnoutrefresh(Input);
      strcpy(input_line,command_lines);
      mvwaddstr(Input,0,0,input_line);
      wrefresh(Input);
      return;
    }
  if(*what == '')
    {
      wclear(Input);
      wnoutrefresh(Input);
      input_line[0]=0;
      waddstr(Input,input_line);
      doupdate();
      return;
    }
  waddstr(Input, what);
  strcat(input_line,what);
  if(strchr(what,'\r'))
    {
      input_line[strlen(input_line)-1]=0;
      if(input_line[0]=='/' && input_line[1]!='/') local_command(input_line);
      strcpy(command_lines,input_line);
      line_to_mud_screen[0]=0;
      sprintf(line_to_mud_screen, "[7m[36;1m%s[0m\n",input_line);
      strcat(input_line,"\n");
      if(input_line[0] != '/') write(sockfd, input_line, strlen(input_line));
      if(input_line[0]=='/' && input_line[1]=='/'){
	strcpy(input_line,input_line+1);
	write(sockfd, input_line, strlen(input_line));
      }
      from_mud(line_to_mud_screen);
      input_line[0]=0;
      wclear(Input);
      wrefresh(Input);
      return;
    }
  wrefresh(Input);
}

void direct_to_mud(char *what)
{
  static char line_to_mud_screen[RECEIVE_SIZE];

  line_to_mud_screen[0]=0;
  sprintf(line_to_mud_screen, "[7m[37;1mTrigger --> %s[0m",what);
  write(sockfd, what, strlen(what));
  from_mud(line_to_mud_screen);
  wrefresh(Mud);
  wrefresh(Input);
}
