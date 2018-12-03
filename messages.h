#include "display.h"

void clear_message(char* message, int buffer_len)
{
  for (int i = 0; i < buffer_len; ++i)
  {
    message[i] = ' ';
  }
  message[buffer_len-1] = 0;
}

void PrintClue(char* message,
               const int buffer_len,
               const byte active_milestone)
{
  clear_message(message, buffer_len);
  int index = 0;

  if (active_milestone == 1)
  {
    message[index++] = 'C';
    message[index++] = 'l';
    message[index++] = 'u';
    message[index++] = 'e';
    message[index++] = '1';
  }
  else if (active_milestone == 2)
  {
    message[index++] = 'C';
    message[index++] = 'l';
    message[index++] = 'u';
    message[index++] = 'e';
    message[index++] = '2';

  }
  else if (active_milestone == 3)
  {
    message[index++] = 'C';
    message[index++] = 'l';
    message[index++] = 'u';
    message[index++] = 'e';
    message[index++] = '3';
  }
  else
  {
    Serial.println("active_milestone == 4, awkward");
  }

  if ( index > buffer_len )
  {
    Serial.println("Message has overflown message buffer space");
  }

  PrintScrollingText(message, 2);
}

void FinalMessage(char* message,
                  int buffer_len)
{
  clear_message(message, buffer_len);
  int index = 0;
  message[index++] = 'W';
  message[index++] = '0';
  message[index++] = '0';
  message[index++] = 't';

  if ( index > buffer_len )
  {
    Serial.println("Message has overflown message buffer space");
  }

  PrintScrollingText(message, 2);
}

void LatLonDeltaMessage(char* message,
                        int buffer_len,
                        float latitude,
                        float longitude,
                        float lat_delta,
                        float lon_delta)
{
  clear_message(message, buffer_len);
  int index = 0;
  message[index++] = 'L';
  message[index++] = 'a';
  message[index++] = 't';
  message[index++] = ':';
  const int width = 10;
  const int decimal_digits = 5;
  dtostrf(latitude, width, decimal_digits, &message[index]);
  index += width;
  Serial.print("next index after Lon: ");
  Serial.println(index);
  message[index++] = ' ';
  message[index++] = 'L';
  message[index++] = 'o';
  message[index++] = 'n';
  message[index++] = ':';
  dtostrf(longitude, width, decimal_digits, &message[index]);
  PrintScrollingText(message, 2);

  clear_message(message, buffer_len);
  index = 0;
  lon_delta = lon_delta < 0 ? -1 * lon_delta : lon_delta;
  Serial.println("lat delta");
  Serial.println(lat_delta, 5);
  Serial.println("lon delta");
  Serial.println(lon_delta, 5);

  message[index++] = 'd';
  message[index++] = 'L';
  message[index++] = 'a';
  message[index++] = 't';
  message[index++] = ':';
  dtostrf(lat_delta, width, decimal_digits, &message[index]);
  index += width;
  Serial.print("next index after dLon: ");
  Serial.println(index);

  //so frustrating, why do I need the extra spaces here to make this go to new line? the width argument to dtostrf should be doing this..
  //is because my display method is chopping out whitespace... i think
  message[index++] = ' ';
  message[index++] = 'd';
  message[index++] = 'L';
  message[index++] = 'o';
  message[index++] = 'n';
  message[index++] = ':';
  dtostrf(lon_delta, width, decimal_digits, &message[index]);
  index += width;

  if ( index > buffer_len )
  {
    Serial.println("Message has overflown message buffer space");
  }

  PrintScrollingText(message, 2);
}

void AtTargetMessage(char* message, int buffer_len)
{
  clear_message(message, buffer_len);
  int index = 0;
  message[index++] = 'Y';
  message[index++] = 'o';
  message[index++] = 'u';
  message[index++] = ' ';
  message[index++] = 'h';
  message[index++] = 'e';
  message[index++] = 'r';
  message[index++] = 'e';
  message[index++] = '!';

  if ( index > buffer_len )
  {
    Serial.println("Message has overflown message buffer space");
  }

  PrintScrollingText(message, 2);
}

void NoFixMessage(char* message, int buffer_len)
{
  clear_message(message, buffer_len);
  int index = 0;
  message[index++] = 'N';
  message[index++] = 'o';
  message[index++] = ' ';
  message[index++] = 'f';
  message[index++] = 'i';
  message[index++] = 'x';
  message[index++] = ',';
  message[index++] = ' ';
  message[index++] = 'g';
  message[index++] = 'o';
  message[index++] = ' ';
  message[index++] = 'o';
  message[index++] = 'u';
  message[index++] = 't';
  message[index++] = 's';
  message[index++] = 'i';
  message[index++] = 'd';
  message[index++] = 'e';

  if ( index > buffer_len )
  {
    Serial.println("Message has overflown message buffer space");
  }

  PrintScrollingText(message, 2);
}

