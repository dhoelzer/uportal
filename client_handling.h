
void handle_mudlag(char *text)
{
  strcpy(MUDLAG,text);
  do_status_bar();
}

void handle_uptime(char *text)
{
  strcpy(UPTIME, text);
  do_status_bar();
}
