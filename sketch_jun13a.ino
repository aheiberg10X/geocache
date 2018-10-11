#include "display.h"
#include "storage.h"
#include "gps.h"
#include "PWMServo.h"

PWMServo servo;
int servo_pos = 0;

int loop_index = 0;
const int buffer_len = 66;
char message[buffer_len];
const byte milestone_count = 2;
const byte milestone_addr = 0;
byte active_milestone = 0;

//backyard
//goog maps: 32.946352, -117.113197
//observations
//32.94627, -117.11318

//dinner table
//goog maps: 32.946324, -117.113103
//observations:
//32.94619, -117.11324

//middle of street, down four houses
//middle of park
const float google_target_latitudes[milestone_count] = {32.945908, 32.945934};
const float google_target_longitudes[milestone_count] = {-117.113693, -117.116320};
float eps = .0001;
const float google_maps_lat_delta = -0.00009;
const float google_maps_lon_delta = -0.00003;
const float target_latitudes[milestone_count] = {google_target_latitudes[0] + google_maps_lat_delta, google_target_latitudes[1] + google_maps_lat_delta};
const float target_longitudes[milestone_count] = {google_target_longitudes[0] + google_maps_lon_delta, google_target_longitudes[1] + google_maps_lon_delta};


void setup()  
{
  GpsSetup();

  servo.attach(10);

  lcd.begin(16,2);

  active_milestone = Read(milestone_addr);

  //This break the GPS parsing??
  //pinMode(led, OUTPUT);
}

void clear_message()
{
  for(int i=0; i < buffer_len; ++i)
  {
     message[i] = ' ';
  }
}

void loop()                     // run over and over again
{
  Serial.print("Loop index: ");
  Serial.println(loop_index++); 
  Serial.println("Milestone number: "),
  Serial.println(active_milestone);
  

    
  bool got_fix;
  float latitude, longitude;
  GpsParse(got_fix, latitude, longitude);  

  const float target_latitude = target_latitudes[active_milestone]; //google_target_latitude + google_maps_lat_delta;
  const float target_longitude = target_longitudes[active_milestone]; //google_target_longitude + google_maps_lon_delta;
  Serial.println("target lat");
  Serial.println(target_latitude,5);
  Serial.println("target lon");
  Serial.println(target_longitude,5);

  if(got_fix)
  {
    int index = 0;
    message[index++] = 'L';
    message[index++] = 'a';
    message[index++] = 't';
    message[index++] = ':';
    message[index++] = ' ';
    const int width = 10;
    dtostrf(latitude, width, 5, &message[index]);
    index += width;
    message[index++] = ' ';
    message[index++] = 'L';
    message[index++] = 'o';
    message[index++] = 'n';
    message[index++] = ':';
    message[index++] = ' ';
    dtostrf(longitude, width, 5, &message[index]);
    index += width;
//    for( ; index < buffer_len; ++index)
//    {
//      message[index] = ' ';
//    }

    message[index++] = ' '; 

    //"can't do math inside abs or it does the wrong thing... wtf" In my case it rounds
    float lat_delta = latitude - target_latitude;
    float lon_delta = longitude - target_longitude;
    lat_delta = lat_delta < 0 ? -1*lat_delta : lat_delta;
    lon_delta = lon_delta < 0 ? -1*lon_delta : lon_delta;
    Serial.println("lat delta");
    Serial.println(lat_delta,5);
    Serial.println("lon delta");
    Serial.println(lon_delta,5);

     message[index++] = 'D';
     message[index++] = 'e';
     message[index++] = 'l';
     message[index++] = ':';
     message[index++] = ' ';
     dtostrf(lat_delta, width, 5, &message[index]);
     index += width;
     message[index++] = ' ';
     dtostrf(lon_delta, width, 5, &message[index]);       
     index += width;
     for( ; index < buffer_len; ++index)
     {
       message[index] = ' ';
     }

    if( lat_delta <= eps && lon_delta <= eps)
    {
       clear_message();
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
       for( ; index < buffer_len; ++index)
       {
         message[index] = ' ';
       }

       Write(milestone_addr, ++active_milestone);
       servo.write(180);              

    }
    else
    {
//       int index = 0;
//       message[index++] = 'D';
//       message[index++] = 'e';
//       message[index++] = 'l';
//       message[index++] = 't';
//       message[index++] = 'a';
//       message[index++] = ':';
//       message[index++] = ' ';
//       dtostrf(lat_delta, width, 5, &message[index]);
//       index += width;
//       message[index++] = '/';
//       dtostrf(lon_delta, width, 5, &message[index]);       
//       index += width;
//       for( ; index < buffer_len; ++index)
//       {
//         message[index] = ' ';
//       }
    }
  }
  else
  {
    clear_message();
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
    
  }

  

  //message_str = String("hello darkness: ") + String(4.2342342,4) + " asdf: " + String(.2342341,5);
  //message_str.toCharArray(message, buffer_len);
  Serial.println(message);
  PrintScrollingText(message, 2);

}
