#include <clibdict.h>
#include <clibstr.h>
#include <clibio.h>

typedef unsigned char byte;

size_t ByteLen;
byte Buffer[10000000];

struct strArr{
    size_t size;
    clib::string* strings;

    strArr(clib::string s, char splitChar)
    {
        strings = s.split(splitChar, &size);
    }

    strArr() { }
    clib::string operator[] (size_t index) { return strings[index]; }
}; 

void Error(size_t line, size_t pos, char* fileName)
{
    std::cout << "ERROR:" << fileName << ":" << line << ":" << pos  << ": No, that no worky" << std::endl;
    std::exit(1);
}

strArr ParseLine(clib::string line)
{
    strArr words(line,' ');
    return words;
}

int main(){
    
    size_t size;
    clib::string rawInput = clib::FileReadAllText((char*)"test.coreth");
    strArr lines(rawInput,';');
    //std::cout<<lines.size << std::endl;
    size = lines.size;
    strArr words[size]; 
    for (size_t i = 0; i < size; i++)
    {
        words[i] = ParseLine(lines[i]);
    }

    for(size_t i = 0; i < size; i++) 
    {
        for (size_t j = 0; j < words[i].size; j++)
        {
            std::cout << words[i][j] << std::endl;
        }
        
    }
}