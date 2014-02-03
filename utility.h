int char_to_int(char character)
{
  int value;

  switch(character)
	{
	case '0' :
	  value = 0;
	  break;
	case '1' :
	  value = 1;
	  break;
	case '2' :
	  value = 2;
	  break;
	case '3' :
	  value = 3;
	  break;
	case '4' :
	  value = 4;
	  break;
	case '5' :
	  value = 5;
	  break;
	case '6' :
	  value = 6;
	  break;
	case '7' :
	  value = 7;
	  break;
	case '8' :
	  value = 8;
	  break;
	case '9' :
	  value = 9;
	  break;
	case 'a' :
	case 'A' :
	  value = 10;
	  break;
	case 'b' :
	case 'B' :
	  value = 11;
	  break;
	case 'c' :
	case 'C' :
	  value = 12;
	  break;
	case 'd' :
	case 'D' :
	  value = 13;
	  break;
	case 'e' :
	case 'E' :
	  value = 14;
	  break;
	case 'f' :
	case 'F' :
	  value = 15;
	  break;
	}
  return value;
}

int text_to_hex(char *text)
{
  int value;
  int place_value;
  int str_length;
  int return_value;

  value = 0;
  return_value = 0;
  place_value = 1;
  str_length = strlen(text);
  while(str_length)
    {
      value = char_to_int(text[str_length - 1]);
      return_value = return_value + ( value * place_value );
      place_value *= 16;
      str_length --;
    }
  return(return_value);
}

void output(char *text)
{
  if(_debug)from_mud(text);
}
