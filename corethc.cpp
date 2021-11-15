#include <clibdict.h>
#include <clibstr.h>
#include <clibio.h>
typedef unsigned char byte;

size_t ByteLen;
byte Buffer[10000000];

byte* temp;

clib::dict<clib::string,uint> varDict;

struct strArry
{
    size_t size;
    clib::string* strings;

    clib::string operator[] (int index) 
    {
        return strings[index];
    } 

    strArry(clib::string s, char c) {
        strings = s.split(c,size);
    }

    strArry(){}
};


byte* UintToBytes(uint num)
{
    byte bytes[4];
    for (long i = 0; i < 4; i++)
    {
        bytes[i] = (num >> (3-i)*8);
    }
    temp = bytes;
    return temp;
}

byte* LongToBytes(long num) 
{
    byte bytes[8];
    for (long i = 0; i < 8; i++)
    {
        bytes[i] = (num >> (7-i)*8);
    }
    temp = bytes;
    return temp;
}

void PushByte(byte Byte)
{
    Buffer[ByteLen] = Byte;
    ByteLen++;
}

void PushBytes(byte* bytes, size_t size) 
{
    for (size_t i = 0; i < size; i++)
    {
        PushByte(bytes[i]);
    }
    
}

void GetVars(strArry defineLine)
{
    for (size_t i = 1; i < defineLine.size; i++)
    {
        varDict.add(defineLine[i],(i-1));
    }
    
}

clib::string mkString(const char* c) 
{
    return clib::string((char*)c);
}

void CompileLine(strArry line)
{
    for (int i = line.size-1; i > -1; i--)
    {
        if (line[i] == '=')                     PushByte(3);
        else if (line[i] == '+')                PushByte(4);
        else if (line[i] == '-')                PushByte(5);
        else if (line[i] == '*')                PushByte(6);
        else if (line[i] == '/')                PushByte(7);
        else if (line[i] == '%')                PushByte(8);
        else if (line[i] == mkString("=="))     PushByte(9); 
        else if (line[i] == mkString("!="))     PushByte(10); 
        else if (line[i] == '>')                PushByte(11);  
        else if (line[i] == '<')                PushByte(12);
        else if (line[i] == mkString(">="))     PushByte(13); 
        else if (line[i] == mkString("<="))     PushByte(14);
        else if (line[i] == '!')                PushByte(15);
        else if (line[i] == mkString("print"))  PushByte(16);    
        else if (varDict.hasKey(line[i])) 
        {
            PushByte(1);
            PushBytes(UintToBytes(varDict[line[i]]),4);
        }
        else 
        {
            PushByte(0);
            PushBytes(LongToBytes(std::stol((char*)line[i])),8);
        }
    }
    
}

int main(){

    int defined = 0;
    size_t size;
    clib::string rawInput = clib::FileReadAllText((char*)"test.coreth");
    std::cout<<rawInput<<std::endl;
    size_t len = 0;
    clib::string* lines = rawInput.remove('\n').split(';', len);
    strArry words[len];

    for (size_t i = 0; i < len; i++)
    {
        words[i] = strArry(lines[i], ' ');
    }
    
    if (words[0][0] == mkString("#define"))
    {
        defined=1; GetVars(words[0]);
    }
    
    PushBytes(UintToBytes(varDict.size()),4);
    for (size_t i = defined; i < len; i++)
    {
        CompileLine(words[i]);
    }

    for (size_t i = 0; i < ByteLen; i++)
    {
        std::cout << ((int)Buffer[i]) << ",";
    }
    std::cout << "255";

    
}