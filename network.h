
void parse_network_data(char *buffer)
{
  static  char *ptr, *ptr2, other_text[RECEIVE_SIZE], 
    hold_char, tag[4], data[RECEIVE_SIZE], *tag_start;
  static  int pin_number, data_length, buf_pos, holding;
  static char holdover_text[RECEIVE_SIZE];
  
  if(holding == 1) {
    holding = 0;
    strncat(holdover_text, buffer, data_length);
    ptr = holdover_text;
    ptr2 = (char *)strstr(ptr,"#K%");
    tag_start = ptr;
    ptr += 3;
    ptr2 = ptr + 5;
    hold_char = *ptr2;
    *ptr2 = 0;
    pin_number = atoi(ptr);
    *ptr2 = hold_char;
    ptr = ptr2;
    ptr2 += 3;
    hold_char = *ptr2;
    *ptr2 = 0;
    data_length = atoi(ptr);
    //from_mud(ptr);          // Debugging
    *ptr2 = hold_char;
    ptr = ptr2 + 3;
    strncpy(tag, ptr2,3);
    tag[3] = 0;
    ptr2 += data_length;
    hold_char = *ptr2;
    *ptr2 = 0;
    strncpy(data, ptr, data_length - 3);
    *(data + data_length - 2) = 0;
	//      from_mud("\nTag Data: ");
	//      from_mud(ptr);          //Debugging
	//      from_mud("\n");
    process_tag(tag, data);
  }


  //  if(_debug) from_mud(buffer);
  buf_pos = 0;
  other_text[0] = 0;
  ptr = buffer;
  ptr2 = (char *)strstr(ptr,"#K%");
  if(ptr2 == NULL) 
    {
      from_mud(buffer);
      return;
    }
  while(ptr2 != NULL)
    {
      if(ptr2 != ptr)
	{
	  *ptr2 = 0;
	  strcat(other_text,ptr);
	  *ptr2 = '#';
	  ptr = ptr2;
	}
      tag_start = ptr;
      ptr += 3;
      ptr2 = ptr + 5;
      hold_char = *ptr2;
      *ptr2 = 0;
      pin_number = atoi(ptr);
      *ptr2 = hold_char;
      ptr = ptr2;
      ptr2 += 3;
      hold_char = *ptr2;
      *ptr2 = 0;
      data_length = atoi(ptr);
      //from_mud(ptr);          // Debugging
      *ptr2 = hold_char;
      ptr = ptr2 + 3;
      strncpy(tag, ptr2,3);
      tag[3] = 0;
      ptr2 += data_length;
      hold_char = *ptr2;
      *ptr2 = 0;
      buf_pos = (int) ((sizeof(buffer)) - (ptr - buffer));
      //      if(buf_pos - data_length - 3 > 0) {
      if(1){
	strncpy(data, ptr, data_length - 3);
	*(data + data_length - 2) = 0;
	//      from_mud("\nTag Data: ");
	//      from_mud(ptr);          //Debugging
	//      from_mud("\n");
	process_tag(tag, data);
	*ptr2 = hold_char;
	ptr = ptr2;
	ptr2 = (char *)strstr(ptr,"#K%");
	if(!ptr2)strcat(other_text,ptr);
      }
      else {
	strncpy(holdover_text, tag_start, tag_start - buffer);
	*ptr2 = hold_char; ptr = ptr2; ptr2 = NULL;
      }
    }
  from_mud(other_text);
}




