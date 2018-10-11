#include <EEPROM.h>

#define INVALID_MEM 0

void Write(int addr, byte val)
{
//  if(addr >= EEPROM.length() || addr < 0)
//  {
//    DoProblemBlinky(INVALID_MEM);  
//  }

  EEPROM.write(addr, val);
}

byte Read(int addr)
{
//  if(addr >= EEPROM.length() || addr < 0)
//  {
//    DoProblemBlinky(INVALID_MEM);  
//  }
  return EEPROM.read(addr);
}

void WriteInt(int addr, long int val)
{
//  if(addr+4 >= EEPROM.length() || addr < 0)
//  {
//    DoProblemBlinky(INVALID_MEM);  
//  }

  EEPROM.write(addr, static_cast<byte>(val >> 24));
  EEPROM.write(addr+1, static_cast<byte>(val >> 16) & 0xFF);
  EEPROM.write(addr+2, static_cast<byte>(val >> 8) & 0xFF);
  EEPROM.write(addr+3, static_cast<byte>(val) & 0xFF);
}

long int ReadInt(int addr)
{
//  if(addr+4 >= EEPROM.length() || addr < 0)
//  {
//    DoProblemBlinky(INVALID_MEM);  
//  }

  long int val = static_cast<long int>(EEPROM.read(addr)) << 24;
  val |= static_cast<long int>(EEPROM.read(addr+1)) << 16;
  val |= static_cast<long int>(EEPROM.read(addr+2)) << 8;
  val |= static_cast<long int>(EEPROM.read(addr+3));
  return val;
}
