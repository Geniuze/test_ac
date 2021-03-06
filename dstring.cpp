#include <iostream>
using namespace std;

#include <sstream>

#include "dstring.h"
#include "log.h"

uint64_t ntohll(uint64_t val)
{
#ifdef __mips__
    return val;
#else
    return (((uint64_t) ntohl(val)) << 32) + ntohl(val >> 32);
#endif
}
uint64_t htonll(uint64_t val)
{
#ifdef __mips__
    return val;
#else
    return (((uint64_t) htonl(val)) << 32) + htonl(val >> 32);
#endif
}

string toString(uint32_t value)
{
    stringstream ss;
    ss << dec << value;

    return ss.str();
}
string toString(uint64_t value)
{
    stringstream ss;
    ss << dec << value;

    return ss.str();
}
string toString(int value)
{
    stringstream ss;
    ss << dec << value;

    return ss.str();
}
string toString(time_t value)
{
    stringstream ss;
    ss << dec << value;

    return ss.str();
}

string toString(char *value)
{
    stringstream ss;
    ss << value;

    return ss.str();
}

string toString(uint8_t value)
{
    stringstream ss;
    ss << (uint32_t)value;

    return ss.str();
}

string toString(uint16_t value)
{
    stringstream ss;
    ss << (uint32_t)value;

    return ss.str();
}

string toString(bool value)
{
    return toString(value?1:0);
}

uint8_t toInt8(string value)
{
    stringstream ss;
    uint32_t ret = 0;

    ss << value;
    ss >> ret;

    return (uint8_t)ret;

}

uint16_t toInt16(string value)
{
    stringstream ss;
    uint32_t ret = 0;

    ss << value;
    ss >> ret;

    return (uint16_t)ret;

}

uint32_t toInt32(string value)
{
    return toInt(value);
}

uint32_t toInt(string value)
{
    stringstream ss;
    uint32_t ret = 0;

    ss << value;
    ss >> ret;

    return ret;
}

uint64_t toInt64(string value)
{
    stringstream ss;
    uint64_t ret= 0;

    ss << value;
    ss >> ret;
    return ret;
}

vector<string> split(string &str, string d)
{
    vector<string> v;
    string::size_type pos = 0;
    string::size_type lpos = 0;

    while ((pos = str.find(d, lpos)) != string::npos)
    {
        v.push_back(str.substr(lpos, pos-lpos));
        lpos = pos+d.length();
        pos += d.length();
    }

    if (lpos < str.length())
    {
        v.push_back(str.substr(lpos));
    }
    return v;
}
vector<string> split(const char *str, const char *d)
{
    string s = str;
    return split(s, string(d));
}

int format_macaddr(string &mac)
{
    for (size_t i=0; i<mac.length(); i++)
    {
        if ((mac[i] >= 'a') && mac[i] <= 'z')
            mac[i] -= ('a' - 'A');
        if (mac[i] == ':')
            mac[i] = '-';
    }
    return 0;
}

string toUpper(string src)
{
    string dst = src;
    for (size_t i=0; i<dst.size(); i++)
    {
        if (dst[i] >= 'a' && dst[i] <= 'z')
            dst[i] -= ('a'-'A');
    }
    return dst;
}
string toUpper(const char *src)
{
    return toUpper(string(src));
}
