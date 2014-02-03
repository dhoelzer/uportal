void file_download(int mode, char *data)
{
  static int downloading, line_number, num_lines;
  static FILE *download_fp;
  static char filename[80], *ptr;
  switch (mode)
    {
    case 0:
      ptr = (char *)strchr(data,'~');
      *ptr = 0;
      num_lines = atoi(data);
      ptr ++;
      downloading = 1;
      download_fp = fopen("uportal_download","w");
      line_number = 0;
      sprintf(filename,"\n%%%% Beginning download: %s\n",ptr);
      from_mud(filename);
      break;
    case 1:
      if(!downloading) break;
      line_number++;
      fprintf(download_fp,"%s\n",data);
      break;
    case 2:
      if(!downloading) break;
      fclose(download_fp);
      downloading = 0;
      sprintf(filename,"\n%%%% Downloaded %d out of %d lines.\n",line_number,num_lines);
      from_mud(filename);
      break;
    }
}
      
void handle_composite(char *data)
{
  char *ptr, *ptr2, *end;

  end=(char *)strchr(data,0);
  ptr=data;
  while(ptr < end){
    ptr2 = ptr+2;
    switch(*ptr)
      {
      case 'A':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	HP=atoi(ptr2);
	if(HP < hp_trigger) direct_to_mud(hp_trigger_action);
	ptr++;
	break;
      case 'B':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	MAXHP=atoi(ptr2);
	ptr++;
	break;
      case 'C':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	SpellPoints=atoi(ptr2);
	if(SpellPoints < sp_trigger) direct_to_mud(sp_trigger_action);
	ptr++;
	break;
      case 'D':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	MAXSpellPoints=atoi(ptr2);
	ptr++;
	break;
      case 'E':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	GP1=atoi(ptr2);
	if(GP1 < gp_trigger) direct_to_mud(gp_trigger_action);
	ptr++;
	break;
      case 'F':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	GP1MAX=atoi(ptr2);
	ptr++;
	break;
      case 'G':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	GP2=atoi(ptr2);
	ptr++;
	break;
      case 'H':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	GP2MAX=atoi(ptr2);
	ptr++;
	break;
      case 'I':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	strcpy(HPBAR1,ptr2);
	ptr++;
	break;
      case 'J':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	strcpy(HPBAR2,ptr2);
	ptr++;
	break;
      case 'K':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	strncpy(ATTACKER,ptr2,29);
	ptr++;
	break;
      case 'L':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	ATTACKCOND=atoi(ptr2);
	ptr++;
	break;
      case 'M':
	ptr=(char *)strchr(ptr2,'~');
	if(!ptr) ptr = end;
	*ptr=0;
	ptr++;
	break;
      default:from_mud(ptr2);
	ptr=end;
      }
  }
  hp();
}


int process_tag(char *tag, char *data)
{
  static int _tag;

  _tag = text_to_hex(tag);
  switch(_tag)
    {
    case 0xAAA:
      //      output("Sound");
      break;
    case 0xAAB:
      //output("Image");
      break;
    case 0xAAC:
      //output("Reboot");
      break;
    case 0xAAF:
      handle_uptime(data);
      break;
    case 0xBAE:
      handle_mudlag(data);
      break;
    case 0xAAD:
      //output("Ad");
      break;
    case 0xAAE:
      //output("PWD");
      break;
    case 0xBAA:
      //output("Special");
      break;
    case 0xBAC:
      //output("Special 2");
      break;
    case 0xBAB:
      //output("Tell");
      break;
    case 0xBAD:
      //output("Room");
      break;
    case 0xBAF:
      //output("Edit");
      break;
    case 0xBBA:
      //output("gp1 mask");
      break;
    case 0xBBB:
      //output("gp2 mask");
      break;
    case 0xBBC:
      //output("HP mask");
      break;
    case 0xBBD:
      //output("SPmask");
      break;
    case 0xCDF:
      file_download(0,data);
      break;
    case 0xCCF:
      file_download(1,data);
      break;
    case 0xCEF:
      file_download(2,data);
      break;
    case 0xCAA:
      //output("Mudchat");
      break;
    case 0xEEE:
      //output("Quit");
      break;
    case 0xFFF:
      handle_composite(data);
      break;
    default :
      output("Unknown code!");
      break;
    }
  return _tag;
}



