#include "storage.h"
#include "messages.h"
#include "gps.h"
#include "PWMServo.h"

PWMServo servo;
int servo_pos = 0;

int loop_index = 0;
const int buffer_len = 66;
char message[buffer_len];
const byte milestone_count = 3;
const byte milestone_addr = 0;
byte active_milestone = 1;
byte final_milestone = 3;

//backyard
//goog maps: 32.946352, -117.113197
//observations
//32.94627, -117.11318
//32.94634, -117.11321

//dinner table
//goog maps: 32.946324, -117.113103
//observations:
//32.94619, -117.11324
//32.94666, -117.11289
//32.94659, -117.11299
//32.94639, -117.11311

//middle of street, front of house
//google maps
//32.946100, -117.112999
//observations:
//32.94611, -117.11301
//32.94609, -117.11302

////middle of street, down four houses
////middle of park
//const float google_target_latitudes[milestone_count] = {32.945908, 32.945934};
//const float google_target_longitudes[milestone_count] = {-117.113693, -117.116320};

//middle of street, front of house: 32.946105, -117.112999
//middle of street, down four houses: 32.945908, -117.113693
//middle of street, top corner: 32.946395, -117.112243
//middle of park
const float google_target_latitudes[milestone_count] = {32.946105, 32.945908, 32.946395};
const float google_target_longitudes[milestone_count] = { -117.112999, -117.113693, -117.112243};

float eps = .0005;
const float google_maps_lat_delta = 0; //-0.00009;
const float google_maps_lon_delta = 0; //-0.00003;
const float target_latitudes[milestone_count] = {google_target_latitudes[0] + google_maps_lat_delta,
                                                 google_target_latitudes[1] + google_maps_lat_delta,
                                                 google_target_latitudes[2] + google_maps_lat_delta
                                                };

const float target_longitudes[milestone_count] = {google_target_longitudes[0] + google_maps_lon_delta,
                                                  google_target_longitudes[1] + google_maps_lon_delta,
                                                  google_target_longitudes[2] + google_maps_lon_delta
                                                 };


void setup()
{
  GpsSetup();

  servo.attach(10);

  lcd.begin(16, 2);

  active_milestone = Read(milestone_addr);

  servo.write(0);

  //This break the GPS parsing??
  //pinMode(led, OUTPUT);
}

void loop()                     // run over and over again
{
//    Write(milestone_addr, 1);
//    return;

Serial.println("Start o loop");
Serial.println("");
Serial.println("");
  Serial.print("Loop index: ");
  Serial.println(loop_index);
  Serial.println("Milestone number: "),
                 Serial.println(active_milestone);

  bool got_fix;
  float latitude, longitude;
  GpsParse(got_fix, latitude, longitude);

  const float target_latitude = target_latitudes[active_milestone - 1]; //google_target_latitude + google_maps_lat_delta;
  const float target_longitude = target_longitudes[active_milestone - 1]; //google_target_longitude + google_maps_lon_delta;


  if (got_fix)
  {
    //"can't do math inside abs or it does the wrong thing... wtf" In my case it rounds
    float lat_delta = latitude - target_latitude;
    float lon_delta = longitude - target_longitude;
    lat_delta = lat_delta < 0 ? -1 * lat_delta : lat_delta;
    lon_delta = lon_delta < 0 ? -1 * lon_delta : lon_delta;

    Serial.println("lat");
    Serial.println(latitude, 5);
    Serial.println("lon");
    Serial.println(longitude, 5);

    Serial.println("target lat");
    Serial.println(target_latitude, 5);
    Serial.println("target lon");
    Serial.println(target_longitude, 5);

    Serial.println("dlat");
    Serial.println(lat_delta, 5);
    Serial.println("dlon");
    Serial.println(lon_delta, 5);

    bool at_target = lat_delta <= eps && lon_delta <= eps;
    if ( at_target )
    {
      Serial.println("at target");
      Write(milestone_addr,
            ++active_milestone);



      if (active_milestone == final_milestone + 1)
      {
        Serial.println("reached final milestone");
        FinalMessage(message,
                     buffer_len);

        servo.write(180);
      }
      else
      {
        AtTargetMessage(message,
                buffer_len);

        PrintClue(message,
                  buffer_len,
                  active_milestone);
      }
    }
    else
    {
      if (loop_index % 1 == 0)
      {
        PrintClue(message,
                  buffer_len,
                  active_milestone);
      }

      LatLonDeltaMessage(message,
                         buffer_len,
                         latitude,
                         longitude,
                         lat_delta,
                         lon_delta);
    }
  }
  else
  {
    NoFixMessage(message,
                 buffer_len);

  }

  ++loop_index;
}
