#include "String.h"

String::String() : String("")
{
}

char *String::_alloc(size_t amount)
{
    return new char[amount];
}

//takes care of resizing and copying + assigning the new length
char *String::_re_alloc(String& str, size_t len_next)
{
    char *tmp = new char[len_next + 1];
    char *end = std::uninitialized_copy(str._addr, str._addr + str._size, tmp);
    delete[] str._addr;
    str._addr = tmp;
    str._size = len_next;
    
    return end;
}

String::String(const String &src) : _size(src._size)
{
    LOG("constructs (from copy)");
    _addr = _alloc(_size + 1);

    std::strcpy(_addr, src._addr);
}

String::String(const char *str)
{
    LOG("constructs (from char*)");
    _size = std::strlen(str);

    _addr = _alloc(_size + 1);

    if (!_addr)
    {
        std::cerr << "out of memory !" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::strcpy(_addr, str);
}

const String &String::operator=(const char *str)
{
    LOG("assign (from char*)");
    *this = String(str);
    return *this;
}

const String &String::operator=(const String &str)
{
    LOG("assign (from string&)");
    this->~String();
    LOG("after destruction");
    _size = str._size;
    LOG(_size);
    _addr = _alloc(_size + 1);

    LOG("after calloc");
    if (!_addr)
    {
        exit(1);
    }
    std::strcpy(_addr, str._addr);

    return *this;
}

String String::_repeat(std::size_t amount) const
{
    if (amount == 0)
    {
        return String("");
    }
    else if (amount == 1)
    {
        return String(*this);
    }
    else
    {
        String copy(*this);
        copy._size *= amount;
        LOG("new size : " << copy._size);
        char *tmp = copy._addr;
        tmp = (char *)std::realloc((void *)copy._addr, (copy._size + 1) * sizeof(char));
        if (!tmp)
        {
            //handle not enough memory
            exit(1);
        }
        while (amount > 1)
        {
            LOG("Loop");
            copy._addr = tmp;
            std::strcat(copy._addr, this->_addr);
            --amount;
        }
        copy.set_zero();
        return copy;
    }
};

String String::operator+(const char *str) const
{
    String copy(*this);
    size_t len = std::strlen(str);
    size_t full_len = len + _size;
    char *end = _re_alloc(copy, full_len);
    std::uninitialized_copy(str, str + len, end);

    return copy;
}

const String &String::operator+=(const char *str)
{
    *this = *this + str;
    return *this;
}

const String &String::operator+=(const String &str)
{
    *this = *this + str._addr;
    return *this;
}

String::~String()
{
    LOG("destructs");
    delete[] _addr;
    _addr = nullptr;
}

const char *String::address() const
{
    return _addr;
}

std::size_t String::size() const
{
    return _size;
}

std::ostream &operator<<(std::ostream &os, const String &str)
{
    return os << str._addr;
}

String operator+(const char *left, const String &right)
{
    return String(left) + right;
}

String String::operator+(const String &right) const
{
    return *this + right._addr;
}

String &String::set_zero()
{
    _addr[_size] = '\0';
    return *this;
}

StringIterator String::begin()
{
    return StringIterator(_addr);
}

StringIterator String::end()
{
    return StringIterator(_addr + _size);
}

bool String::operator==(const String &str) const
{
    return _comp(str) == 0;
};

bool String::operator!=(const String &str) const
{
    return _comp(str) != 0;
};

bool String::operator>(const String &str) const
{
    return _comp(str) > 0;
};

bool String::operator<(const String &str) const
{
    return _comp(str) < 0;
};

bool String::operator>=(const String &str) const
{
    int res = _comp(str);
    return res == 0 || res > 0;
};

bool String::operator<=(const String &str) const
{
    int res = _comp(str);
    return res == 0 || res < 0;
};

int String::_comp(const String &str) const
{
    return std::strcmp(_addr, str._addr);
}