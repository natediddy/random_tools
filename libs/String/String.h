#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <cstddef>

class String
{
public:
    enum CaseSensitivity
    {
        CaseSensitive,
        CaseInsensitive
    };

    String(CaseSensitivity cs = CaseSensitive);
    String(const char c, CaseSensitivity cs = CaseSensitive);
    String(const char *s, CaseSensitivity cs = CaseSensitive);
    String(const char *s, size_t size, CaseSensitivity cs = CaseSensitive);
    String(const char *s, ssize_t start_pos,
           ssize_t end_pos, CaseSensitivity cs = CaseSensitive);
    String(const String &s, CaseSensitivity cs = CaseSensitive);
    String(const String &s, size_t size, CaseSensitivity cs = CaseSensitive);
    String(const String &s, ssize_t start_pos,
           ssize_t end_pos, CaseSensitivity cs = CaseSensitive);
    String(const long long n, CaseSensitivity cs = CaseSensitive);
    String(const unsigned long long n, CaseSensitivity cs = CaseSensitive);
    String(const long n, CaseSensitivity cs = CaseSensitive);
    String(const unsigned long n, CaseSensitivity cs = CaseSensitive);
    String(const int n, CaseSensitivity cs = CaseSensitive);
    String(const unsigned int n, CaseSensitivity cs = CaseSensitive);
    String(const short n, CaseSensitivity cs = CaseSensitive);
    String(const unsigned short n, CaseSensitivity cs = CaseSensitive);
    String(const long double n, CaseSensitivity cs = CaseSensitive);
    String(const double n, CaseSensitivity cs = CaseSensitive);
    String(const float n, CaseSensitivity cs = CaseSensitive);
    ~String();

    void setCaseSensitivity(CaseSensitivity cs);
    CaseSensitivity caseSensitivity() const;

    size_t size() const;
    const char *buffer() const;
    bool null() const;
    bool empty() const;
    bool valid() const;

    bool equals(const char c) const;
    bool equals(const char *s) const;
    bool equals(const String &s) const;

    ssize_t firstPositionOf(const char c) const;
    ssize_t firstPositionOf(const char *s) const;
    ssize_t firstPositionOf(const String &s) const;
    ssize_t firstPositionOf(const long long n) const;
    ssize_t firstPositionOf(const unsigned long long n) const;
    ssize_t firstPositionOf(const long n) const;
    ssize_t firstPositionOf(const unsigned long n) const;
    ssize_t firstPositionOf(const int n) const;
    ssize_t firstPositionOf(const unsigned int n) const;
    ssize_t firstPositionOf(const short n) const;
    ssize_t firstPositionOf(const unsigned short n) const;
    ssize_t firstPositionOf(const long double n) const;
    ssize_t firstPositionOf(const double n) const;
    ssize_t firstPositionOf(const float n) const;

    ssize_t lastPositionOf(const char c) const;
    ssize_t lastPositionOf(const char *s) const;
    ssize_t lastPositionOf(const String &s) const;
    ssize_t lastPositionOf(const long long n) const;
    ssize_t lastPositionOf(const unsigned long long n) const;
    ssize_t lastPositionOf(const long n) const;
    ssize_t lastPositionOf(const unsigned long n) const;
    ssize_t lastPositionOf(const int n) const;
    ssize_t lastPositionOf(const unsigned int n) const;
    ssize_t lastPositionOf(const short n) const;
    ssize_t lastPositionOf(const unsigned short n) const;
    ssize_t lastPositionOf(const long double n) const;
    ssize_t lastPositionOf(const double n) const;
    ssize_t lastPositionOf(const float n) const;

    bool startsWith(const char c) const;
    bool startsWith(const char *s) const;
    bool startsWith(const String &s) const;
    bool startsWith(const long long n) const;
    bool startsWith(const unsigned long long n) const;
    bool startsWith(const long n) const;
    bool startsWith(const unsigned long n) const;
    bool startsWith(const int n) const;
    bool startsWith(const unsigned int n) const;
    bool startsWith(const short n) const;
    bool startsWith(const unsigned short n) const;
    bool startsWith(const long double n) const;
    bool startsWith(const double n) const;
    bool startsWith(const float n) const;

    bool endsWith(const char c) const;
    bool endsWith(const char *s) const;
    bool endsWith(const String &s) const;
    bool endsWith(const long long n) const;
    bool endsWith(const unsigned long long n) const;
    bool endsWith(const long n) const;
    bool endsWith(const unsigned long n) const;
    bool endsWith(const int n) const;
    bool endsWith(const unsigned int n) const;
    bool endsWith(const short n) const;
    bool endsWith(const unsigned short n) const;
    bool endsWith(const long double n) const;
    bool endsWith(const double n) const;
    bool endsWith(const float n) const;

    bool contains(const char c) const;
    bool contains(const char *s) const;
    bool contains(const String &s) const;
    bool contains(const long long n) const;
    bool contains(const unsigned long long n) const;
    bool contains(const long n) const;
    bool contains(const unsigned long n) const;
    bool contains(const int n) const;
    bool contains(const unsigned int n) const;
    bool contains(const short n) const;
    bool contains(const unsigned short n) const;
    bool contains(const long double n) const;
    bool contains(const double n) const;
    bool contains(const float n) const;

    unsigned int occurences(const char c) const;
    unsigned int occurences(const char *s) const;
    unsigned int occurences(const String &s) const;
    unsigned int occurences(const long long n) const;
    unsigned int occurences(const unsigned long long n) const;
    unsigned int occurences(const long n) const;
    unsigned int occurences(const unsigned long n) const;
    unsigned int occurences(const int n) const;
    unsigned int occurences(const unsigned int n) const;
    unsigned int occurences(const short n) const;
    unsigned int occurences(const unsigned short n) const;
    unsigned int occurences(const long double n) const;
    unsigned int occurences(const double n) const;
    unsigned int occurences(const float n) const;

    String &insert(const char c, ssize_t pos);
    String &insert(const char *s, ssize_t pos);
    String &insert(const String &s, ssize_t pos);
    String &insert(const long long n, ssize_t pos);
    String &insert(const unsigned long long n, ssize_t pos);
    String &insert(const long n, ssize_t pos);
    String &insert(const unsigned long n, ssize_t pos);
    String &insert(const int n, ssize_t pos);
    String &insert(const unsigned int n, ssize_t pos);
    String &insert(const short n, ssize_t pos);
    String &insert(const unsigned short n, ssize_t pos);
    String &insert(const long double n, ssize_t pos);
    String &insert(const double n, ssize_t pos);
    String &insert(const float n, ssize_t pos);

    String &append(const char c);
    String &append(const char *s);
    String &append(const String &s);
    String &append(const long long n);
    String &append(const unsigned long long n);
    String &append(const long n);
    String &append(const unsigned long n);
    String &append(const int n);
    String &append(const unsigned int n);
    String &append(const short n);
    String &append(const unsigned short n);
    String &append(const long double n);
    String &append(const double n);
    String &append(const float n);

    String &prepend(const char c);
    String &prepend(const char *s);
    String &prepend(const String &s);
    String &prepend(const long long n);
    String &prepend(const unsigned long long n);
    String &prepend(const long n);
    String &prepend(const unsigned long n);
    String &prepend(const int n);
    String &prepend(const unsigned int n);
    String &prepend(const short n);
    String &prepend(const unsigned short n);
    String &prepend(const long double n);
    String &prepend(const double n);
    String &prepend(const float n);

    String &replace(const char o, const char n);
    String &replace(const char o, const char *n);
    String &replace(const char o, const String &n);
    String &replace(const char *o, const char n);
    String &replace(const char *o, const char *n);
    String &replace(const char *o, const String &n);
    String &replace(const String &o, const char n);
    String &replace(const String &o, const char *n);
    String &replace(const String &o, const String &n);

    String &remove(const char c);
    String &remove(const char *s);
    String &remove(const String &s);
    String &remove(const long long n);
    String &remove(const unsigned long long n);
    String &remove(const long n);
    String &remove(const unsigned long n);
    String &remove(const int n);
    String &remove(const unsigned int n);
    String &remove(const short n);
    String &remove(const unsigned short n);
    String &remove(const long double n);
    String &remove(const double n);
    String &remove(const float n);

    String &toUpper();
    String &toLower();

    String &reverse();

    long long toLongLong() const;
    unsigned long long toUnsignedLongLong() const;
    long toLong() const;
    unsigned long toUnsignedLong() const;
    int toInt() const;
    unsigned int toUnsignedInt() const;
    short toShort() const;
    unsigned short toUnsignedShort() const;
    long double toLongDouble() const;
    double toDouble() const;
    float toFloat() const;

    operator bool() const;

    String &operator=(const char c);
    String &operator=(const char *s);
    String &operator=(const String &s);
    String &operator=(const long long n);
    String &operator=(const unsigned long long n);
    String &operator=(const long n);
    String &operator=(const unsigned long n);
    String &operator=(const int n);
    String &operator=(const unsigned int n);
    String &operator=(const short n);
    String &operator=(const unsigned short n);
    String &operator=(const long double n);
    String &operator=(const double n);
    String &operator=(const float n);

    String &operator+=(const char c);
    String &operator+=(const char *s);
    String &operator+=(const String &s);
    String &operator+=(const long long n);
    String &operator+=(const unsigned long long n);
    String &operator+=(const long n);
    String &operator+=(const unsigned long n);
    String &operator+=(const int n);
    String &operator+=(const unsigned int n);
    String &operator+=(const short n);
    String &operator+=(const unsigned short n);
    String &operator+=(const long double n);
    String &operator+=(const double n);
    String &operator+=(const float n);

    String &operator+(const char c);
    String &operator+(const char *s);
    String &operator+(const String &s);
    String &operator+(const long long n);
    String &operator+(const unsigned long long n);
    String &operator+(const long n);
    String &operator+(const unsigned long n);
    String &operator+(const int n);
    String &operator+(const unsigned int n);
    String &operator+(const short n);
    String &operator+(const unsigned short n);
    String &operator+(const long double n);
    String &operator+(const double n);
    String &operator+(const float n);

    String &operator-=(const char c);
    String &operator-=(const char *s);
    String &operator-=(const String &s);
    String &operator-=(const long long n);
    String &operator-=(const unsigned long long n);
    String &operator-=(const long n);
    String &operator-=(const unsigned long n);
    String &operator-=(const int n);
    String &operator-=(const unsigned int n);
    String &operator-=(const short n);
    String &operator-=(const unsigned short n);
    String &operator-=(const long double n);
    String &operator-=(const double n);
    String &operator-=(const float n);

    String &operator-(const char c);
    String &operator-(const char *s);
    String &operator-(const String &s);
    String &operator-(const long long n);
    String &operator-(const unsigned long long n);
    String &operator-(const long n);
    String &operator-(const unsigned long n);
    String &operator-(const int n);
    String &operator-(const unsigned int n);
    String &operator-(const short n);
    String &operator-(const unsigned short n);
    String &operator-(const long double n);
    String &operator-(const double n);
    String &operator-(const float n);

    String &operator<<(const char c);
    String &operator<<(const char *s);
    String &operator<<(const String &s);
    String &operator<<(const long long n);
    String &operator<<(const unsigned long long n);
    String &operator<<(const long n);
    String &operator<<(const unsigned long n);
    String &operator<<(const int n);
    String &operator<<(const unsigned int n);
    String &operator<<(const short n);
    String &operator<<(const unsigned short n);
    String &operator<<(const long double n);
    String &operator<<(const double n);
    String &operator<<(const float n);

    bool operator==(const size_t z) const;
    bool operator==(const char c) const;
    bool operator==(const char *s) const;
    bool operator==(const String &s) const;

    bool operator!=(const size_t z) const;
    bool operator!=(const char c) const;
    bool operator!=(const char *s) const;
    bool operator!=(const String &s) const;

    bool operator<(const size_t z) const;
    bool operator<(const char c) const;
    bool operator<(const char *s) const;
    bool operator<(const String &s) const;

    bool operator<=(const size_t z) const;
    bool operator<=(const char c) const;
    bool operator<=(const char *s) const;
    bool operator<=(const String &s) const;

    bool operator>(const size_t z) const;
    bool operator>(const char c) const;
    bool operator>(const char *s) const;
    bool operator>(const String &s) const;

    bool operator>=(const size_t z) const;
    bool operator>=(const char c) const;
    bool operator>=(const char *s) const;
    bool operator>=(const String &s) const;

    const char operator[](ssize_t index) const;

private:
    size_t mSize;
    char *mBuffer;
    CaseSensitivity mCaseSensitivity;
};

#endif /* STRING_H_INCLUDED */

