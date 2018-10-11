#include <LiquidCrystal.h>

//RS pin 4, mapped to digital pin 8 (was 9, postit probably wrong)
//EN pin 5, mapped to digitial pin 7 (was 8, postit probably wrong)
//DB4 pin 11, 6
//DB5 pin 12, 5
//DB6 pin 13, 4
//DB7 pin 14, 3
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

int led = 13;

void PrintScrollingText(char* message, 
                        int repeats)
{
   const int display_width = 16;
   char* tempstr = (char*) calloc(strlen(message)+1, sizeof(char));
   bool first_time = true;
   int delay_ms = 3000;

   lcd.clear();
      
   for(int repeat = 0; repeat < repeats; ++repeat)
   {
     if(!first_time) delay(delay_ms);
     strcpy(tempstr, message); 
     
     int row = 0;
     int col = 0;   
     lcd.clear();
     
     char* splt = strtok(tempstr, " ");
     while(splt != NULL)
     {
       int len = strlen(splt);
       if(len > display_width) 
       {
         len = display_width;
         Serial.println("WORD_TOO_LONG");
       }
  
       if(col + len > display_width)
       {
         if(row == 0) 
         {
           row = 1;
           col = 0;
         }
         else if(row == 1)
         {
           delay(delay_ms);
           lcd.clear(); //ClearScreen();
           row = 0;
           col = 0;  
           continue;
         }
       }

       lcd.setCursor(col, row);
       lcd.print(splt);
       col += len + 1;
       
       splt = strtok(NULL, " ");     
     }

     //pause for the last words to linger
     delay(delay_ms);
   }
   first_time = false;
   delete tempstr;   
}

void DoProblemBlinky(int delay_ms)
{  
  while(true)
  {
    digitalWrite(led, HIGH);
    delay(delay_ms);
    digitalWrite(led, LOW);
    delay(delay_ms);
  }
}
