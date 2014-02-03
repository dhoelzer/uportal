void local_command(char *line)
{
  char tokens[20][60], *ptr, *ptr2;
  int x;

  ptr = line;
  ptr2 = ptr;
  for(x=0;x!=20&&ptr;x++)
    {
      ptr2 = (char *)strchr(ptr, ' ');
      if(!ptr2)strcpy(tokens[x],ptr);
      else
	{
	  *ptr2 = 0;
	  strcpy(tokens[x],ptr);
	  *ptr2 = ' ';
	  ptr2++;
	}
      ptr = ptr2;
    }
  if(strcmp(tokens[0], "/save") == 0)
    {
      FILE *configfile;
      
      configfile = fopen(".uportal","w");
      if(!configfile)
	{
	  from_mud("\n%% Could not open config file!\n");
	  return;
	}
      fprintf(configfile, "HP:%d:%s\n", hp_trigger, hp_trigger_action);
      fprintf(configfile, "SP:%d:%s\n", sp_trigger, sp_trigger_action);
      fprintf(configfile, "GP:%d:%s\n", gp_trigger, gp_trigger_action);
      fprintf(configfile, "GP2:%d:%s\n", gp2_trigger, gp2_trigger_action);
      fclose(configfile);
      from_mud("\n%% Configuration Saved.\n");
      return;
    }
  if(strcmp(tokens[0],"/trigger")==0)
    {
      if(!tokens[1])
	{
	  from_mud("\n%% Format: /trigger <event> <actions>\n");
	  return;
	}
    }
  if(strcmp(tokens[0],"/hp")==0)
    {
      char *seperator;
      if(strlen(tokens[1])<=0 || strlen(tokens[2])<0)
	{
	  from_mud("\n%% /hp <number> <action[\\action\\action]>\n");
	  return;
	}
      hp_trigger = atoi(tokens[1]);
      strcpy(hp_trigger_action,tokens[2]);
      seperator = strchr(hp_trigger_action,'\\');
      while (seperator)
	{
	  *seperator = '\n';
	  seperator = strchr(seperator,'\\');
	}
      sprintf(hp_trigger_action,"%s\r",hp_trigger_action);
      from_mud("\n%% HP Trigger will now fire.\n");
      return;
    }
  if(strcmp(tokens[0],"/gp")==0)
    {
      char *seperator;
      if(strlen(tokens[1])<=0 || strlen(tokens[2])<0)
	{
	  from_mud("\n%% /gp <number> <action[\\action\\action]>\n");
	  return;
	}
      gp_trigger = atoi(tokens[1]);
      strcpy(gp_trigger_action,tokens[2]);
      seperator = strchr(gp_trigger_action,'\\');
      while (seperator)
	{
	  *seperator = '\n';
	  seperator = strchr(seperator,'\\');
	}
      sprintf(gp_trigger_action,"%s\r",gp_trigger_action);
      from_mud("\n%% GP Trigger will now fire.\n");
      return;
    }
  if(strcmp(tokens[0],"/gp2")==0)
    {
      char *seperator;
      if(strlen(tokens[1])<=0 || strlen(tokens[2])<0)
	{
	  from_mud("\n%% /gp2 <number> <action[\\action\\action]>\n");
	  return;
	}
      gp2_trigger = atoi(tokens[1]);
      strcpy(gp2_trigger_action,tokens[2]);
      seperator = strchr(gp2_trigger_action,'\\');
      while (seperator)
	{
	  *seperator = '\n';
	  seperator = strchr(seperator,'\\');
	}
      sprintf(gp2_trigger_action,"%s\r",gp2_trigger_action);
      from_mud("\n%% GP2 Trigger will now fire.\n");
      return;
    }
  if(strcmp(tokens[0],"/sp")==0)
    {
      char *seperator;
      if(strlen(tokens[1])<=0 || strlen(tokens[2])<0)
	{
	  from_mud("\n%% /sp <number> <action[\\action\\action]>\n");
	  return;
	}
      sp_trigger = atoi(tokens[1]);
      strcpy(sp_trigger_action,tokens[2]);
      seperator = strchr(sp_trigger_action,'\\');
      while (seperator)
	{
	  *seperator = '\n';
	  seperator = strchr(seperator,'\\');
	}
      sprintf(sp_trigger_action,"%s\r",sp_trigger_action);
      from_mud("\n%% SP Trigger will now fire.\n");
      return;
    }
  if(strcmp(tokens[0],"/debug")==0)
    {
      _debug = (_debug?0:1);
      from_mud( (_debug ? "%% Debugging on.\n":
		 "%% Debugging off.\n"));
    }
  if(strcmp(tokens[0],"/quit")==0)
    {
      from_mud("Exiting.\n");
      wrefresh(Mud);
      close(sockfd);
      if(log_file)fclose(log_file);
      endwin();
      exit(0);
    }
  if(strcmp(tokens[0],"/jumpstart")==0)
    {
      char sbuffer[25];
      sprintf(sbuffer,"3klient 99999~Po%s\n",Version_Number);
      write(sockfd, sbuffer, strlen(sbuffer));
      from_mud("%% Jumpstart token sent.\n");
    }
  if(strcmp(tokens[0],"/help")==0)
    {
      int which_help=0;
      char help[1024];
      if(tokens[1])
	{
	  if(strcmp(tokens[1],"jumpstart")==0) which_help = 1;
	  if(strcmp(tokens[1],"log")==0) which_help = 2;
	  if(strcmp(tokens[1],"quit")==0) which_help = 3;
	  if(strcmp(tokens[1],"debug")==0) which_help = 4;
	  if(strcmp(tokens[1],"hp")==0) which_help = 5;
	  if(strcmp(tokens[1],"sp")==0) which_help = 5;
	  if(strcmp(tokens[1],"gp")==0) which_help = 5;
	  if(strcmp(tokens[1],"gp2")==0) which_help = 5;
	  if(strcmp(tokens[1],"save")==0) which_help = 6;
	  if(strcmp(tokens[1],"reconnect")==0) which_help = 7;
	  
	}
      switch(which_help)
	{
	case 0 :
	  sprintf(help,"\n%s\n",
		  "\tHelp\n"
		  "\t----\n\n"
		  "\tHelp is currently available for:\n"
		  "\t\tJumpstart      Log\n"
		  "\t\thp             sp\n"
		  "\t\tgp             gp2\n"
		  "\t\tQuit           Debug\n"
		  "\t\tSave           Reconnect");
	  break;
	case 1 :
	  sprintf(help,"\n%s\n",
		  "\tJumpstart\n"
		  "\t---------\n\n"
		  "\tThe Jumpstart command will send a special 'token' to the mud\n"
		  "\tin an attempt to get the mud to either sync up with the client\n"
		  "\tor to start sending codes if it did not start for some reason.\n");
	  break;
	case 2 :
	  sprintf(help,"\n%s\n",
		  "\tLog\n"
		  "\t---\n\n"
		  "\tThe log command can be used to begin logging all data incoming\n"
		  "\tfrom the mud to a file.  If you do not specify a full path, the\n"
		  "\tlog will be written to the current working directory.\n\n"
		  "\tTo start, use the format '/log <filename>'.  To stop\n"
		  "\trecording, use '/log' or '/log off'.  Using '/log <filename>'\n"
		  "\twhile already logging will cause the last log to be closed\n"
		  "\tand a new log to be opened.\n\n"
		  "\tNote:  /log will not overwrite an existing file.");
	  break;
	case 3 :
	  sprintf(help,"\n%s\n",
		  "\tQuit\n"
		  "\t----\n\n"
		  "\tThe 'Quit' command will close your current connection (if \n"
		  "\tthere is one) and terminate uPortal immediately.");
	  break;
	case 4 :
	  sprintf(help,"\n%s\n",
		  "\tDebug\n"
		  "\t-----\n\n"
		  "\tThe Debug command will turn on or off client debugging\n"
		  "\tmessages and takes no options.");
	  break;
	case 5 :
	  sprintf(help,"\n%s\n",
		  "\tHP/SP/GP/GP2\n"
		  "\t------------\n\n"
		  "\tEach of these commands sets a trigger event based on\n"
		  "\ta value.  For instance, /hp 30 salve\\salve\\salve\n"
		  "\twill set your hp trigger to fire any time your hp\n"
		  "\tis below 30 to fire off the salve command three times.\n");
	  break;
	case 6 :
	  sprintf(help,"\n%s\n",
		  "\tSave\n"
		  "\t------------\n\n"
		  "\tThe save command will write all of your current settings\n"
		  "\tout to ~/.uportal.\n");
	  break;
	}
      from_mud(help);
    }
  if(strcmp(tokens[0],"/sendfile")==0 && tokens[1])
    {
      FILE *fp;
      char line[1024];
      from_mud(tokens[1]);
      from_mud("\n");

      fp = fopen(tokens[1],"r");
      if(!fp){
	from_mud("%% File does not exist.\n");
	return;
      }
      while(!feof(fp))
	{
	  fgets(line,1023,fp);
	  write(sockfd, line, strlen(line));
	  from_mud(line);
	}
      fclose(fp);
      from_mud("%% File Sent.\n");
    }
      
  if(strcmp(tokens[0],"/log")==0)
    {
      if(logging || strcmp(tokens[1],"off")==0 || !tokens[1])
	{
	  fclose(log_file);
	  from_mud("%% Logging disabled.\n");
	  logging = 0;
	}
      else
	{
	  char temp[90];
	  sprintf(temp,"%%%% Logging to %s.\n",tokens[1]);
	  from_mud(temp);
	  log_file = fopen(tokens[1], "a+");
	  logging = 1;
	}
      do_status_bar();
    }
}
void add_trigger(char *data){}
void perform_event(struct TriggerEvent trigger){}

