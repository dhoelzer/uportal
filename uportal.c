#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/types.h>
#include<errno.h>
#include<curses.h>

//--------------------------------------------------------------------------------
//
//  Defines
//
//--------------------------------------------------------------------------------

#define Version_Number "1.01"
#define RECEIVE_SIZE   1500
#define Bar "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"

struct TriggerEvent {
  char event[80];
  char actions[80][20];
};

//--------------------------------------------------------------------------------
//
//  Function Prototypes
//
//--------------------------------------------------------------------------------

void parse_network_data(char *buffer);
void direct_to_mud(char *what);
void file_download(int mode, char *data);
int char_to_int(char character);
int text_to_hex(char *text);
int process_tag(char *tag, char *data);
void output(char *text);
void handle_mudlag(char *text);
void handle_uptime(char *text);
void setup_windows();
void window_update();
void initialize_vars();
void hp();
void to_inputline(char *what);
void from_mud(char *what);
void do_status_bar();
void handle_composite(char *data);
void build_glines();
void local_command(char *line);
void add_trigger(char *data);
void perform_event(struct TriggerEvent trigger);

//--------------------------------------------------------------------------------
//
//  Global Variables
//
//--------------------------------------------------------------------------------

FILE *log_file;

int sockfd,
  logging = 0,
  _debug,
  HP, 
  hp_trigger,
  sp_trigger,
  gp_trigger,
  gp2_trigger,
  MAXHP, 
  SpellPoints, 
  MAXSpellPoints, 
  GP1, 
  GP1MAX, 
  GP2, 
  GP2MAX,
  Mud_Window_Output_Line=0,
  ENEMY, 
  ENEMYMAX,
  Port_Number,
  ATTACKCOND;

WINDOW *Input, 
  *Mud, 
  *HPBar, 
  *HPBar2, 
  *HPBar3, 
  *Status_Bar, 
  *Attacker_Bar,
  *Empty_Bar;

struct TriggerEvent triggers[50];

char HPBAR[256],
  Host[60],
  hp_trigger_action[256],
  sp_trigger_action[256],
  gp_trigger_action[256],
  gp2_trigger_action[256],
  HPBAR1[256],
  HPBAR2[256], 
  input_line[512], 
  STATUS_BAR[256], 
  ATTACKER[30], 
  ATTACKERBAR[100],
  UPTIME[8], 
  MUDLAG[8],
  command_lines[512];


//--------------------------------------------------------------------------------
//
//  Included Functions
//
//--------------------------------------------------------------------------------

#include"client_codes.h"
#include"client_handling.h"
#include"input.h"
#include"network.h"
#include"output.h"
#include"status_bars.h"
#include"utility.h"
#include"windows.h"
#include"local_commands.h"
#include"triggers.h"

//--------------------------------------------------------------------------------
//
//  Main Functions
//
//--------------------------------------------------------------------------------

void initialize_vars()
{
  FILE *configfile;
  HP=0;
  MAXHP=0;
  hp_trigger = -500;
  sp_trigger = -500;
  gp_trigger = -500;
  gp2_trigger = -500;
  strcpy(hp_trigger_action, "None");
  strcpy(sp_trigger_action, "None");
  strcpy(gp_trigger_action, "None");
  strcpy(gp2_trigger_action, "None");
  SpellPoints=0;
  MAXSpellPoints=0;
  GP1=0;
  GP1MAX=0;
  GP2=0;
  GP2MAX=0;
  ENEMY=0;
  ENEMYMAX=0;
  strcpy(MUDLAG,"0");
  strcpy(ATTACKER,"None");
  ATTACKCOND=100;
  strcpy(UPTIME,"00:00");
  // Load the config
      
  configfile = fopen(".uportal","r");
  if(!configfile)
    {
      //      from_mud("\n%% No config file!\n");
      return;
    }
  fscanf(configfile, "HP:%d:%s\n", &hp_trigger, hp_trigger_action);
  fscanf(configfile, "SP:%d:%s\n", &sp_trigger, sp_trigger_action);
  fscanf(configfile, "GP:%d:%s\n", &gp_trigger, gp_trigger_action);
  fscanf(configfile, "GP2:%d:%s\n", &gp2_trigger, gp2_trigger_action);
  fclose(configfile);
  //  from_mud("\n%% Configuration Loaded.\n");
  return;
  
}

int main(int argc, char **argv)
{
  struct sockaddr_in servaddr;
  char rbuffer[RECEIVE_SIZE], sbuffer[RECEIVE_SIZE], hold_buffer[RECEIVE_SIZE * 2],
    *buffer_end, *bufptr, hold_char, *buffer_head;
  int num_read;
  int last_read;
  int select_retval;
  struct timeval tv;
  fd_set rfds;

  initialize_vars();
  setup_windows();


  _debug = 0;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) 
    {
      from_mud("Could not grab socket.\n");
      return 1;
    }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(4242);
  inet_pton(AF_INET, "54.197.137.221", &servaddr.sin_addr);
  from_mud("Connecting...");
  if(connect(sockfd, &servaddr, sizeof(servaddr))!=0)
    {
      from_mud("Error connecting!\n");
      switch(errno)
	{
	case EBADF :
	  from_mud("Bad Descriptor.\n");
	  break;
	case EFAULT :
	  from_mud("Fault- Address Space violation.\n");
	  break;
	case ENOTSOCK :
	  from_mud("Descriptor not a socket.\n");
	  break;
	case EISCONN :
	  from_mud("Socket already connected.\n");
	  break;
	case ECONNREFUSED :
	  from_mud("Connection Refused.\n");
	  break;
	case ETIMEDOUT :
	  from_mud("Connection timed out.\n");
	  break;
	case ENETUNREACH :
	  from_mud("Network Unreachable.\n");
	  break;
	case EADDRINUSE :
	  from_mud("Address in use.\n");
	  break;
	case EINPROGRESS :
	  from_mud("Non Blocking connection in progress.\n");
	  break;
	case EALREADY :
	  from_mud("Previous connection not completed.\n");
	  break;
	case EAFNOSUPPORT :
	  from_mud("Wrong Family.\n");
	  break;
	case EACCES :
	  from_mud("Broadcast Illegal.\n");
	  break;
	default :
	  from_mud("Unknown Error.\n");
	}
      endwin();
      exit(0);
    }
  from_mud("Established.\n");
  FD_ZERO(&rfds);
  FD_SET(0, &rfds);
  FD_SET(sockfd, &rfds);
  tv.tv_sec = 0;
  tv.tv_usec = 200;
  buffer_end = hold_buffer;
  buffer_head = hold_buffer;
  select_retval = 1;
  while(select_retval > -1)
    {
      select_retval = select((sockfd + 1), &rfds, NULL, NULL, &tv);
      num_read = 0;
      if(FD_ISSET(0, &rfds)) 
	 {
	   bzero(sbuffer, RECEIVE_SIZE);
	   last_read = 0;
	   num_read = (int) read(0, sbuffer+last_read, 
				 RECEIVE_SIZE - 1);
	   to_inputline(sbuffer);
	 }
      if(FD_ISSET(sockfd, &rfds) )
	 {
	   num_read = (int)read(sockfd, rbuffer, RECEIVE_SIZE - 1);
	   last_read = num_read;
	   rbuffer[last_read] = 0;
	   strcpy(buffer_end, rbuffer);
	   buffer_end = hold_buffer + strlen(hold_buffer);
	   buffer_head = hold_buffer;
	   if(strstr(buffer_head,"3Kingdoms welcomes you"))
	     {
	       sprintf(sbuffer,"3klient 99999~Po%s\n",Version_Number);
	       write(sockfd, sbuffer, strlen(sbuffer));
	     }
	   if(buffer_end - hold_buffer > 7)
	     {
	       bufptr = buffer_end - 7;
	       if((bufptr = strchr(bufptr, '')))
		 {
		   hold_char = *bufptr;
		   *bufptr = 0;
		   parse_network_data(buffer_head);
		   *bufptr= hold_char;
		   strcpy(hold_buffer, bufptr);
		   buffer_end = hold_buffer + strlen(hold_buffer);
		 }
	       else parse_network_data(hold_buffer);
	     }
	   else
	     { parse_network_data(hold_buffer); }
	   buffer_end = hold_buffer;
	 }
      tv.tv_sec = 5;
      tv.tv_usec = 200;
      FD_ZERO(&rfds);
      FD_SET(0, &rfds);
      FD_SET(sockfd, &rfds);
    }
  return 0;
}





