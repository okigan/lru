/* Enter your code here. Read input from STDIN. Print output to STDOUT *//// @author: Igor Okulist

#include <stdio.h>
#include <string.h>

#include <string>
#include <deque>
#include <algorithm>
#include <map>
#include <vector>
#include <unordered_map>
#include <list>
#include <sstream>
#include <cmath>
#include <fstream>

#include "lru.hxx"

//#define LRU_TEST

template <typename T, unsigned int N>
int get_array_length( const T a[N] ) {
    return N;
}

template <unsigned int LENGTH = 10>
class short_string
{
public:
    short_string()
    {
        _chars[0] = '\0';
    }
    
    short_string(const std::string & str)
    {
        strncpy(_chars, str.c_str(), get_array_length(_chars));
    }
    
    operator std::string() const
    {
        return std::string(_chars);
    }
    
    operator const char* () const
    {
        return _chars;
    }
    
    const char* c_str() const
    {
        return _chars;
    }
    
    bool operator < (const short_string & right ) const
    {
        return strcmp(this->_chars, right._chars) < 0;
    }
    
    bool operator == (const short_string & right) const
    {
        return 0 == strcmp(this->_chars, right._chars);
    }
    
    char char_at(int i) const
    {
        return _chars[i];
    }
private:
    char _chars[11];
};

// Specialise std::hash for foo.

struct custom_hash
{
public:
    template < unsigned int N>
    size_t operator()( const short_string<N>& f ) const
    {
        size_t hash = 7;
        
        for(int i = 0; f.char_at(i) != '\0'; i++)
            hash += hash*31 + f.char_at(i);
        
        return hash;
    }
};


void process(FILE * in, FILE * out)
{
    
    int command_number = 0;
    fscanf(in, "%d", &command_number);
    
    command_number = std::min(command_number, 1000*1000);
    // debugging option
    // command_number = 99;
    
    //lrc<short_string, short_string, custom_hash> lrc;
    lrc<std::string, std::string> lrc;
    
    for(int index = 0; index < command_number; index++) {
        char command[20] = {0};
        fscanf(in, "%s", command);
        
        std::transform(std::begin(command), std::end(command), std::begin(command), ::toupper);
        
        
        if( 0 == strcmp("BOUND", command) ) {
            int bound = 0;
            fscanf(in, "%d", &bound);
            
            lrc.bound_set(bound);
        } else if( 0 == strcmp("SET", command) ) {
            char key[20] = {0};
            char value[20] = {0};
            fscanf(in, "%s %s", key, value);
            
            lrc.set(key, value);
            
        } else if( 0 == strcmp("GET", command) ) {
            char key[20] = {0};
            fscanf(in, "%s", key);
            
            std::string value = lrc.get(key, false);
            fprintf(out, "%s\n", value.c_str());
        } else if( 0 == strcmp("PEEK", command) ) {
            char key[20] = {0};
            fscanf(in, "%s", key);
            
            std::string value = lrc.get(key, true);
            fprintf(out, "%s\n", value.c_str());
        } else if( 0 == strcmp("DUMP", command) ) {
            lrc.dump(out);
        } else {
            //std::cerr << "Unknown command '" << command << " "'"td::endl"""""""";
            //return -1;
        }
    }
}

int main(int argc, char* argv[])
{
    //std iostream magic (though only relativly small speed up, probably imbue still limiting perf)
    //std::ios_base::sync_with_stdio(false);
    //char buffer[1048576];
    //std::cin.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
    
#ifdef LRU_TEST
    std::stringstream sin;
    std::stringstream sout;
    
    sin << "8     "
    "BOUND 2   "
    "SET a 2   "
    "SET b 4   "
    "GET b     "
    "PEEK a    "
    "SET c 5   "
    "GET a     "
    "DUMP      ";
    
    process(sin, sout);
    
    bool passed = sout.str() ==
    "4"     "\n"
    "2"     "\n"
    "NULL"  "\n"
    "b 4"   "\n"
    "c 5"   "\n";
    
    std::cout << passed << std::endl;
#else
    if( argc == 2 ) {
        FILE * in = fopen(argv[1], "rt");
        FILE * out= stdout;
        process( in, out );
    } else {
        process(stdin, stdout);
        
    }
    
#endif
    
    return 0;
}