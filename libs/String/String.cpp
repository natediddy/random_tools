#include <cfloat>
#include <climits>
#include <cstdio>
#include <cstring>

#include "String.h"

#define NUMBER_BUFFERMAX 24

#define DESTROY(p) \
    do { \
        if ((p) != NULL) { \
            delete [](p); \
            (p) = NULL; \
        } \
    } while (0)

#define NUMBER_BUFFER(fmt, n) \
    char nbuf[NUMBER_BUFFERMAX]; \
    snprintf(nbuf, NUMBER_BUFFERMAX, fmt, n)

namespace {

char *createNewEmpty()
{
    char *n = new char[1];

    n[0] = '\0';
    return n;
}

char *createNewCopy(const char *o)
{
    size_t s = strlen(o);
    char *n = new char[s + 1];

    strncpy(n, o, s);
    n[s] = '\0';
    return n;
}

char *createNewCopyFromSize(const char *o, size_t s)
{
    char *n = NULL;

    if (s == 0)
        n = createNewEmpty();
    else {
        n = new char[s + 1];
        strncpy(n, o, s);
        n[s] = '\0';
    }
    return n;
}

char *createNewCopyFromPosition(const char *o, size_t s, size_t e)
{
    size_t z = strlen(o);
    char *n = new char[e - s + 2];

    for (size_t i = 0, j = s; j <= e; ++i, ++j)
        n[i] = o[j];
    n[e - s + 1] = '\0';
    return n;
}

bool isWhiteSpace(const char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

bool isDigit(const char c)
{
    return c >= '0' && c <= '9';
}

bool isUpperCase(const char c)
{
    return c >= 'A' && c <= 'Z';
}

bool isLowerCase(const char c)
{
    return c >= 'a' && c <= 'z';
}

char toUpperCase(const char c)
{
    if (isLowerCase(c))
        return (c - 'a' + 'A');
    return c;
}

char toLowerCase(const char c)
{
    if (isUpperCase(c))
        return (c - 'A' + 'a');
    return c;
}

bool exactMatch(const char x, const char y, String::CaseSensitivity cs)
{
    if (cs == String::CaseInsensitive)
        return toUpperCase(x) == toUpperCase(y);
    return x == y;
}

bool exactMatch(const char *x, const char *y, String::CaseSensitivity cs)
{
    size_t x_size = strlen(x);
    int (*compare_func)(const char *, const char *, size_t);

    if (cs == String::CaseInsensitive)
        compare_func = &strncasecmp;
    else
        compare_func = &strncmp;
    return (strlen(y) == x_size) && (compare_func(x, y, x_size) == 0);
}

size_t properIndex(ssize_t index, size_t limit)
{
    size_t p_index;

    if (index < 0) {
        index = -index;
        if (index >= limit)
            p_index = limit - 1;
        else
            p_index = limit - index;
    } else if (index >= limit)
        p_index = limit - 1;
    else
        p_index = index;

    return p_index;
}

} // namespace

/*--------------------------------------------------------------------------*/

String::String(String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{}

String::String(const char c, String::CaseSensitivity cs)
    : mSize((c == '\0') ? 0 : 1)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    if (c == '\0')
        mBuffer = createNewEmpty();
    else {
        mBuffer = new char[mSize + 1];
        mBuffer[0] = c;
        mBuffer[1] = '\0';
    }
}

String::String(const char *s, String::CaseSensitivity cs)
    : mSize(strlen(s))
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    if (s != NULL && s[0] == '\0')
        mBuffer = createNewEmpty();
    else if (s != NULL && s[0] != '\0')
        mBuffer = createNewCopy(s);
}

String::String(const char *s, size_t size, String::CaseSensitivity cs)
    : mSize(size)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    if (s != NULL && s[0] == '\0')
        mBuffer = createNewEmpty();
    else if (s != NULL && s[0] != '\0')
        mBuffer = createNewCopyFromSize(s, mSize);
}

String::String(const char *s, ssize_t start_pos,
        ssize_t end_pos, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    if (s != NULL && s[0] == '\0')
        mBuffer = createNewEmpty();
    else if (s != NULL && s[0] != '\0') {
        size_t size = strlen(s);
        if (start_pos < 0) {
            start_pos = -start_pos;
            if (start_pos < size)
                start_pos = size - start_pos;
        }
        if (end_pos < 0) {
            end_pos = -end_pos;
            if (end_pos < size)
                end_pos = size - end_pos;
        }
        if (start_pos >= size) {
            mBuffer = createNewCopy(s);
            mSize = size;
        } else if (end_pos >= size) {
            mBuffer = createNewCopyFromPosition(s, start_pos, size - 1);
            mSize = size - start_pos;
        } else {
            mBuffer = createNewCopyFromPosition(s, start_pos, end_pos);
            mSize = end_pos - start_pos;
        }
    }
}

String::String(const String &s, String::CaseSensitivity cs)
    : mSize(s.mSize)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    if (s.mBuffer != NULL && s.mBuffer[0] == '\0')
        mBuffer = createNewEmpty();
    else if (s.mBuffer != NULL && s.mBuffer[0] != '\0')
        mBuffer = createNewCopy(s.mBuffer);
}

String::String(const String &s, size_t size, String::CaseSensitivity cs)
    : mSize(size)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    if (s.mBuffer != NULL && s.mBuffer[0] == '\0')
        mBuffer = createNewEmpty();
    else if (s.mBuffer != NULL && s.mBuffer[0] != '\0')
        mBuffer = createNewCopyFromSize(s.mBuffer, mSize);
}

String::String(const String &s, ssize_t start_pos,
        ssize_t end_pos, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    if (s.mBuffer != NULL && s.mBuffer[0] == '\0')
        mBuffer = createNewEmpty();
    else if (s.mBuffer != NULL && s.mBuffer[0] != '\0') {
        if (start_pos < 0) {
            start_pos = -start_pos;
            if (start_pos < s.mSize)
                start_pos = s.mSize - start_pos;
        }
        if (end_pos < 0) {
            end_pos = -end_pos;
            if (end_pos < s.mSize)
                end_pos = s.mSize - end_pos;
        }
        if (start_pos >= s.mSize) {
            mBuffer = createNewCopy(s.mBuffer);
            mSize = s.mSize;
        } else if (end_pos >= s.mSize) {
            mBuffer = createNewCopyFromPosition(s.mBuffer, start_pos,
                    s.mSize - 1);
            mSize = s.mSize - start_pos;
        } else {
            mBuffer = createNewCopyFromPosition(s.mBuffer, start_pos,
                    end_pos);
            mSize = end_pos - start_pos;
        }
    }
}

String::String(const long long n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%lli", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const unsigned long long n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%llu", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const long n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%li", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const unsigned long n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%lu", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const int n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%i", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const unsigned int n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%u", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const short n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%hi", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const unsigned short n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%hu", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const long double n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%Lg", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const double n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%g", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::String(const float n, String::CaseSensitivity cs)
    : mSize(0)
    , mBuffer(NULL)
    , mCaseSensitivity(cs)
{
    NUMBER_BUFFER("%g", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
}

String::~String()
{
    DESTROY(mBuffer);
}

void String::setCaseSensitivity(String::CaseSensitivity cs)
{
    mCaseSensitivity = cs;
}

String::CaseSensitivity String::caseSensitivity() const
{
    return mCaseSensitivity;
}

size_t String::size() const
{
    return mSize;
}

const char *String::buffer() const
{
    return mBuffer;
}

bool String::null() const
{
    return mBuffer == NULL;
}

bool String::empty() const
{
    return mBuffer != NULL && mBuffer[0] == '\0';
}

bool String::valid() const
{
    return mBuffer != NULL && mBuffer[0] != '\0';
}

bool String::equals(const char c) const
{
    return (c == '\0' && mBuffer[0] == '\0' && mSize == 0) ||
           (exactMatch(c, mBuffer[0], mCaseSensitivity) && mSize == 1);
}

bool String::equals(const char *s) const
{
    return exactMatch(s, mBuffer, mCaseSensitivity);
}

bool String::equals(const String &s) const
{
    return equals(s.mBuffer);
}

ssize_t String::firstPositionOf(const char c) const
{
    ssize_t ret = -1;

    if (c != '\0') {
        for (size_t i = 0; i < mSize; ++i) {
            if (exactMatch(mBuffer[i], c, mCaseSensitivity)) {
                ret = (ssize_t)i;
                break;
            }
        }
    }
    return ret;
}

ssize_t String::firstPositionOf(const char *s) const
{
    ssize_t ret = -1;

    if (s != NULL && s[0] != '\0') {
        size_t s_size = strlen(s);
        if (s_size == 1)
            return firstPositionOf(s[0]);
        for (size_t pos = 0; pos < mSize; ++pos) {
            if (exactMatch(mBuffer[pos], s[0], mCaseSensitivity)) {
                size_t pos_save = pos;
                size_t pos_temp = pos + 1;
                size_t s_pos;
                for (s_pos = 1; s_pos < s_size; ++s_pos, ++pos_temp)
                    if (!exactMatch(mBuffer[pos_temp], s[s_pos],
                                mCaseSensitivity))
                        break;
                if (pos_save == (pos_temp - s_size)) {
                    ret = (ssize_t)pos_save;
                    break;
                }
            }
        }
    }
    return ret;
}

ssize_t String::firstPositionOf(const String &s) const
{
    return firstPositionOf(s.mBuffer);
}

ssize_t String::firstPositionOf(const long long n) const
{
    NUMBER_BUFFER("%lli", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const unsigned long long n) const
{
    NUMBER_BUFFER("%llu", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const long n) const
{
    NUMBER_BUFFER("%li", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const unsigned long n) const
{
    NUMBER_BUFFER("%lu", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const int n) const
{
    NUMBER_BUFFER("%i", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const unsigned int n) const
{
    NUMBER_BUFFER("%u", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const short n) const
{
    NUMBER_BUFFER("%hi", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const unsigned short n) const
{
    NUMBER_BUFFER("%hu", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const long double n) const
{
    NUMBER_BUFFER("%Lg", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const double n) const
{
    NUMBER_BUFFER("%g", n);
    return firstPositionOf(nbuf);
}

ssize_t String::firstPositionOf(const float n) const
{
    NUMBER_BUFFER("%g", n);
    return firstPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const char c) const
{
    ssize_t ret = -1;

    if (c != '\0') {
        for (size_t pos = mSize - 1; pos >= 0; --pos) {
            if (exactMatch(mBuffer[pos], c, mCaseSensitivity)) {
                ret = (ssize_t)pos;
                break;
            }
        }
    }
    return ret;
}

ssize_t String::lastPositionOf(const char *s) const
{
    ssize_t ret = -1;

    if (s != NULL && s[0] != '\0') {
        size_t s_size = strlen(s);
        if (s_size == 1)
            return lastPositionOf(s[0]);
        for (size_t pos = mSize - 1; pos >= 0; --pos) {
            if (exactMatch(mBuffer[pos], s[s_size - 1], mCaseSensitivity)) {
                size_t pos_save = pos;
                size_t pos_temp = pos - 1;
                size_t s_pos;
                for (s_pos = s_size - 2; s_pos >= 0; --s_pos, --pos_temp)
                    if (!exactMatch(mBuffer[pos], s[s_pos], mCaseSensitivity))
                        break;
                if (pos_save == (pos_temp + s_size)) {
                    ret = (ssize_t)pos_save;
                    break;
                }
            }
        }
    }
    return ret;
}

ssize_t String::lastPositionOf(const String &s) const
{
    return lastPositionOf(s.mBuffer);
}

ssize_t String::lastPositionOf(const long long n) const
{
    NUMBER_BUFFER("%lli", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const unsigned long long n) const
{
    NUMBER_BUFFER("%llu", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const long n) const
{
    NUMBER_BUFFER("%li", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const unsigned long n) const
{
    NUMBER_BUFFER("%lu", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const int n) const
{
    NUMBER_BUFFER("%i", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const unsigned int n) const
{
    NUMBER_BUFFER("%u", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const short n) const
{
    NUMBER_BUFFER("%hi", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const unsigned short n) const
{
    NUMBER_BUFFER("%hu", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const long double n) const
{
    NUMBER_BUFFER("%Lg", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const double n) const
{
    NUMBER_BUFFER("%g", n);
    return lastPositionOf(nbuf);
}

ssize_t String::lastPositionOf(const float n) const
{
    NUMBER_BUFFER("%g", n);
    return lastPositionOf(nbuf);
}

bool String::startsWith(const char c) const
{
    return firstPositionOf(c) == 0;
}

bool String::startsWith(const char *s) const
{
    return firstPositionOf(s) == 0;
}

bool String::startsWith(const String &s) const
{
    return firstPositionOf(s) == 0;
}

bool String::startsWith(const long long n) const
{
    NUMBER_BUFFER("%lli", n);
    return startsWith(nbuf);
}

bool String::startsWith(const unsigned long long n) const
{
    NUMBER_BUFFER("%llu", n);
    return startsWith(nbuf);
}

bool String::startsWith(const long n) const
{
    NUMBER_BUFFER("%li", n);
    return startsWith(nbuf);
}

bool String::startsWith(const unsigned long n) const
{
    NUMBER_BUFFER("%lu", n);
    return startsWith(nbuf);
}

bool String::startsWith(const int n) const
{
    NUMBER_BUFFER("%i", n);
    return startsWith(nbuf);
}

bool String::startsWith(const unsigned int n) const
{
    NUMBER_BUFFER("%u", n);
    return startsWith(nbuf);
}

bool String::startsWith(const short n) const
{
    NUMBER_BUFFER("%hi", n);
    return startsWith(nbuf);
}

bool String::startsWith(const unsigned short n) const
{
    NUMBER_BUFFER("%hu", n);
    return startsWith(nbuf);
}

bool String::startsWith(const long double n) const
{
    NUMBER_BUFFER("%Lg", n);
    return startsWith(nbuf);
}

bool String::startsWith(const double n) const
{
    NUMBER_BUFFER("%g", n);
    return startsWith(nbuf);
}

bool String::startsWith(const float n) const
{
    NUMBER_BUFFER("%g", n);
    return startsWith(nbuf);
}

bool String::endsWith(const char c) const
{
    return lastPositionOf(c) == mSize - 1;
}

bool String::endsWith(const char *s) const
{
    return lastPositionOf(s) == mSize - strlen(s);
}

bool String::endsWith(const String &s) const
{
    return lastPositionOf(s) == mSize - s.mSize;
}

bool String::endsWith(const long long n) const
{
    NUMBER_BUFFER("%lli", n);
    return endsWith(nbuf);
}

bool String::endsWith(const unsigned long long n) const
{
    NUMBER_BUFFER("%llu", n);
    return endsWith(nbuf);
}

bool String::endsWith(const long n) const
{
    NUMBER_BUFFER("%li", n);
    return endsWith(nbuf);
}

bool String::endsWith(const unsigned long n) const
{
    NUMBER_BUFFER("%lu", n);
    return endsWith(nbuf);
}

bool String::endsWith(const int n) const
{
    NUMBER_BUFFER("%i", n);
    return endsWith(nbuf);
}

bool String::endsWith(const unsigned int n) const
{
    NUMBER_BUFFER("%u", n);
    return endsWith(nbuf);
}

bool String::endsWith(const short n) const
{
    NUMBER_BUFFER("%hi", n);
    return endsWith(nbuf);
}

bool String::endsWith(const unsigned short n) const
{
    NUMBER_BUFFER("%hu", n);
    return endsWith(nbuf);
}

bool String::endsWith(const long double n) const
{
    NUMBER_BUFFER("%Lg", n);
    return endsWith(nbuf);
}

bool String::endsWith(const double n) const
{
    NUMBER_BUFFER("%g", n);
    return endsWith(nbuf);
}

bool String::endsWith(const float n) const
{
    NUMBER_BUFFER("%g", n);
    return endsWith(nbuf);
}

bool String::contains(const char c) const
{
    return firstPositionOf(c) != -1;
}

bool String::contains(const char *s) const
{
    return firstPositionOf(s) != -1;
}

bool String::contains(const String &s) const
{
    return firstPositionOf(s) != -1;
}

bool String::contains(const long long n) const
{
    NUMBER_BUFFER("%lli", n);
    return contains(nbuf);
}

bool String::contains(const unsigned long long n) const
{
    NUMBER_BUFFER("%llu", n);
    return contains(nbuf);
}

bool String::contains(const long n) const
{
    NUMBER_BUFFER("%li", n);
    return contains(nbuf);
}

bool String::contains(const unsigned long n) const
{
    NUMBER_BUFFER("%lu", n);
    return contains(nbuf);
}

bool String::contains(const int n) const
{
    NUMBER_BUFFER("%i", n);
    return contains(nbuf);
}

bool String::contains(const unsigned int n) const
{
    NUMBER_BUFFER("%u", n);
    return contains(nbuf);
}

bool String::contains(const short n) const
{
    NUMBER_BUFFER("%hi", n);
    return contains(nbuf);
}

bool String::contains(const unsigned short n) const
{
    NUMBER_BUFFER("%hu", n);
    return contains(nbuf);
}

bool String::contains(const long double n) const
{
    NUMBER_BUFFER("%Lg", n);
    return contains(nbuf);
}

bool String::contains(const double n) const
{
    NUMBER_BUFFER("%g", n);
    return contains(nbuf);
}

bool String::contains(const float n) const
{
    NUMBER_BUFFER("%g", n);
    return contains(nbuf);
}

unsigned int String::occurences(const char c) const
{
    if (c == '\0')
        return 0;

    unsigned int ret = 0;

    for (size_t i = 0; i < mSize; ++i)
        if (exactMatch(mBuffer[i], c, mCaseSensitivity))
            ++ret;
    return ret;
}

unsigned int String::occurences(const char *s) const
{
    if (s == NULL || s[0] == '\0')
        return 0;

    size_t s_size = strlen(s);

    if (s_size == 1)
        return occurences(s[0]);

    unsigned int ret = 0;

    for (size_t i = 0; i < mSize; ++i) {
        if (exactMatch(mBuffer[i], s[0], mCaseSensitivity)) {
            size_t i_save = i;
            size_t i_temp = i + 1;
            for (size_t s_pos = 1; s_pos < s_size; ++i_temp, ++s_pos)
                if (!exactMatch(mBuffer[i_temp], s[s_pos], mCaseSensitivity))
                    break;
            if (i_save == (i_temp - s_size)) {
                ++ret;
                i += s_size - 1;
            }
        }
    }
    return ret;
}

unsigned int String::occurences(const String &s) const
{
    return occurences(s.mBuffer);
}

unsigned int String::occurences(const long long n) const
{
    NUMBER_BUFFER("%lli", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const unsigned long long n) const
{
    NUMBER_BUFFER("%llu", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const long n) const
{
    NUMBER_BUFFER("%li", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const unsigned long n) const
{
    NUMBER_BUFFER("%lu", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const int n) const
{
    NUMBER_BUFFER("%i", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const unsigned int n) const
{
    NUMBER_BUFFER("%u", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const short n) const
{
    NUMBER_BUFFER("%hi", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const unsigned short n) const
{
    NUMBER_BUFFER("%hu", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const long double n) const
{
    NUMBER_BUFFER("%Lg", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const double n) const
{
    NUMBER_BUFFER("%g", n);
    return occurences(nbuf);
}

unsigned int String::occurences(const float n) const
{
    NUMBER_BUFFER("%g", n);
    return occurences(nbuf);
}

String &String::insert(const char c, ssize_t pos)
{
    if (c == '\0')
        return *this;

    size_t index = properIndex(pos, mSize + 1);
    size_t size_copy = mSize;
    char buffer_copy[size_copy + 1];

    strncpy(buffer_copy, mBuffer, size_copy);
    buffer_copy[size_copy] = '\0';
    DESTROY(mBuffer);
    mSize += 1;
    mBuffer = new char[mSize + 1];

    if (index == 0) {
        mBuffer[0] = c;
        strncpy(mBuffer + 1, buffer_copy, size_copy);
    } else if (index == size_copy) {
        strncpy(mBuffer, buffer_copy, size_copy);
        mBuffer[index] = c;
    } else {
        strncpy(mBuffer, buffer_copy, index);
        mBuffer[index] = c;
        strncpy(mBuffer + index + 1, buffer_copy + index, size_copy - index);
    }

    mBuffer[mSize] = '\0';
    return *this;
}

String &String::insert(const char *s, ssize_t pos)
{
    if (s == NULL || s[0] == '\0')
        return *this;

    size_t index = properIndex(pos, mSize + 1);
    size_t size_copy = mSize;
    char buffer_copy[size_copy + 1];
    size_t s_size = strlen(s);

    strncpy(buffer_copy, mBuffer, size_copy);
    buffer_copy[size_copy] = '\0';
    DESTROY(mBuffer);
    mSize += s_size;
    mBuffer = new char[mSize + 1];

    if (index == 0) {
        strncpy(mBuffer, s, s_size);
        strncpy(mBuffer + s_size, buffer_copy, size_copy);
    } else if (index == size_copy) {
        strncpy(mBuffer, buffer_copy, size_copy);
        strncpy(mBuffer + size_copy, s, s_size);
    } else {
        strncpy(mBuffer, buffer_copy, index);
        strncpy(mBuffer + index, s, s_size);
        strncpy(mBuffer + index + s_size,
                buffer_copy + index,
                size_copy - index);
    }

    mBuffer[mSize] = '\0';
    return *this;
}

String &String::insert(const String &s, ssize_t pos)
{
    return insert(s.mBuffer, pos);
}

String &String::insert(const long long n, ssize_t pos)
{
    NUMBER_BUFFER("%lli", n);
    return insert(nbuf, pos);
}

String &String::insert(const unsigned long long n, ssize_t pos)
{
    NUMBER_BUFFER("%llu", n);
    return insert(nbuf, pos);
}

String &String::insert(const long n, ssize_t pos)
{
    NUMBER_BUFFER("%li", n);
    return insert(nbuf, pos);
}

String &String::insert(const unsigned long n, ssize_t pos)
{
    NUMBER_BUFFER("%lu", n);
    return insert(nbuf, pos);
}

String &String::insert(const int n, ssize_t pos)
{
    NUMBER_BUFFER("%i", n);
    return insert(nbuf, pos);
}

String &String::insert(const unsigned int n, ssize_t pos)
{
    NUMBER_BUFFER("%u", n);
    return insert(nbuf, pos);
}

String &String::insert(const short n, ssize_t pos)
{
    NUMBER_BUFFER("%hi", n);
    return insert(nbuf, pos);
}

String &String::insert(const unsigned short n, ssize_t pos)
{
    NUMBER_BUFFER("%hu", n);
    return insert(nbuf, pos);
}

String &String::insert(const long double n, ssize_t pos)
{
    NUMBER_BUFFER("%Lg", n);
    return insert(nbuf, pos);
}

String &String::insert(const double n, ssize_t pos)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, pos);
}

String &String::insert(const float n, ssize_t pos)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, pos);
}

String &String::append(const char c)
{
    return insert(c, mSize);
}

String &String::append(const char *s)
{
    return insert(s, mSize);
}

String &String::append(const String &s)
{
    return insert(s, mSize);
}

String &String::prepend(const char c)
{
    return insert(c, 0);
}

String &String::prepend(const char *s)
{
    return insert(s, 0);
}

String &String::prepend(const String &s)
{
    return insert(s, 0);
}

String &String::replace(const char o, const char n)
{
    if (o == '\0')
        return *this;

    if (n == '\0')
        return remove(o);

    for (size_t i = 0; i < mSize; ++i)
        if (exactMatch(mBuffer[i], o, mCaseSensitivity))
            mBuffer[i] = n;
    return *this;
}

String &String::replace(const char o, const char *n)
{
    if (o == '\0')
        return *this;

    if (n == NULL || n[0] == '\0')
        return remove(o);

    size_t n_size = strlen(n);

    if (n_size == 1)
        return replace(o, n[0]);

    int o_count = 0;

    for (size_t i = 0; i < mSize; ++i)
        if (exactMatch(mBuffer[i], o, mCaseSensitivity))
            ++o_count;

    if (o_count == 0)
        return *this;

    size_t size_copy = mSize;
    char buffer_copy[size_copy + 1];

    strncpy(buffer_copy, mBuffer, size_copy);
    buffer_copy[size_copy] = '\0';
    DESTROY(mBuffer);
    mSize = (size_copy - o_count) + (o_count * n_size);
    mBuffer = new char[mSize + 1];

    for (size_t i = 0, j = 0; i < mSize; ++i, ++j) {
        if (exactMatch(buffer_copy[j], o, mCaseSensitivity)) {
            strncpy(mBuffer + i, n, n_size);
            i += n_size - 1;
            continue;
        }
        mBuffer[i] = buffer_copy[j];
    }

    mBuffer[mSize] = '\0';
    return *this;
}

String &String::replace(const char o, const String &n)
{
    return replace(o, n.mBuffer);
}

String &String::replace(const char *o, const char n)
{
    if (o == NULL || o[0] == '\0')
        return *this;

    if (n == '\0')
        return remove(o);

    size_t o_size = strlen(o);

    if (o_size == 1)
        return replace(o[0], n);

    int o_count = 0;

    for (size_t i = 0; i < mSize; ++i) {
        if (exactMatch(mBuffer[i], o[0], mCaseSensitivity)) {
            size_t i_save = i;
            size_t i_temp = i + 1;
            size_t o_pos;
            for (o_pos = 1; o_pos < o_size; ++i_temp, ++o_pos)
                if (!exactMatch(mBuffer[i_temp], o[o_pos], mCaseSensitivity))
                    break;
            if (i_save == (i_temp - o_size)) {
                ++o_count;
                i += o_size - 1;
            }
        }
    }

    if (o_count == 0)
        return *this;

    size_t size_copy = mSize;
    char buffer_copy[size_copy + 1];

    strncpy(buffer_copy, mBuffer, size_copy);
    buffer_copy[size_copy] = '\0';
    DESTROY(mBuffer);
    mSize = (size_copy - (o_count * o_size)) + o_count;
    mBuffer = new char[mSize + 1];

    for (size_t i = 0, j = 0; i < size_copy; ++i, ++j) {
        if (exactMatch(buffer_copy[i], o[0], mCaseSensitivity)) {
            size_t i_save = i;
            size_t i_temp = i + 1;
            size_t o_pos;
            for (o_pos = 1; o_pos < o_size; ++i_temp, ++o_pos)
                if (!exactMatch(buffer_copy[i_temp], o[o_pos],
                            mCaseSensitivity))
                    break;
            if (i_save == (i_temp - o_size)) {
                mBuffer[j] = n;
                i += o_size - 1;
                continue;
            }
        }
        mBuffer[j] = buffer_copy[i];
    }

    mBuffer[mSize] = '\0';
    return *this;
}

String &String::replace(const char *o, const char *n)
{
    if (o == NULL || o[0] == '\0')
        return *this;

    if (n == NULL || n[0] == '\0')
        return remove(o);

    size_t o_size = strlen(o);
    size_t n_size = strlen(n);

    if (o_size == 1 && n_size == 1)
        return replace(o[0], n[0]);

    if (o_size == 1 && n_size > 1)
        return replace(o[0], n);

    if (o_size > 1 && n_size == 1)
        return replace(o, n[0]);

    int o_count = 0;

    for (size_t i = 0; i < mSize; ++i) {
        if (exactMatch(mBuffer[i], o[0], mCaseSensitivity)) {
            size_t i_save = i;
            size_t i_temp = i + 1;
            size_t o_pos;
            for (o_pos = 1; o_pos < o_size; ++i_temp, ++o_pos)
                if (!exactMatch(mBuffer[i_temp], o[o_pos], mCaseSensitivity))
                    break;
            if (i_save == (i_temp - o_size)) {
                ++o_count;
                i += o_size - 1;
            }
        }
    }

    if (o_count == 0)
        return *this;

    size_t size_copy = mSize;
    char buffer_copy[size_copy + 1];

    strncpy(buffer_copy, mBuffer, size_copy);
    buffer_copy[size_copy] = '\0';
    DESTROY(mBuffer);
    mSize = size_copy - (o_count * o_size) + (o_count * n_size);
    mBuffer = new char[mSize + 1];

    for (size_t i = 0, j = 0; i < size_copy; ++i, ++j) {
        if (exactMatch(buffer_copy[i], o[0], mCaseSensitivity)) {
            size_t i_save = i;
            size_t i_temp = i + 1;
            size_t o_pos;
            for (o_pos = 1; o_pos < o_size; ++i_temp, ++o_pos)
                if (!exactMatch(buffer_copy[i_temp], o[o_pos],
                            mCaseSensitivity))
                    break;
            if (i_save == (i_temp - o_size)) {
                strncpy(mBuffer + j, n, n_size);
                j += n_size - 1;
                i += o_size - 1;
                continue;
            }
        }
        mBuffer[j] = buffer_copy[i];
    }

    mBuffer[mSize] = '\0';
    return *this;
}

String &String::replace(const char *o, const String &n)
{
    return replace(o, n.mBuffer);
}

String &String::replace(const String &o, const char n)
{
    return replace(o.mBuffer, n);
}

String &String::replace(const String &o, const char *n)
{
    return replace(o.mBuffer, n);
}

String &String::replace(const String &o, const String &n)
{
    return replace(o.mBuffer, n.mBuffer);
}

String &String::remove(const char c)
{
    if (c == '\0')
        return *this;

    unsigned int count = occurences(c);

    if (count == 0)
        return *this;

    size_t size_copy = mSize;
    char buffer_copy[size_copy + 1];

    strncpy(buffer_copy, mBuffer, size_copy);
    buffer_copy[size_copy] = '\0';

    DESTROY(mBuffer);
    mSize = size_copy - count;

    if (mSize == 0) {
        mBuffer = createNewEmpty();
        return *this;
    }

    mBuffer = new char[mSize + 1];

    for (size_t i = 0, j = 0; i < size_copy; ++i)
        if (!exactMatch(buffer_copy[i], c, mCaseSensitivity))
            mBuffer[j++] = buffer_copy[i];

    mBuffer[mSize] = '\0';
    return *this;
}

String &String::remove(const char *s)
{
    if (s == NULL || s[0] == '\0')
        return *this;

    size_t s_size = strlen(s);

    if (s_size == 1)
        return remove(s[0]);

    unsigned int count = occurences(s);

    if (count == 0)
        return *this;

    size_t size_copy = mSize;
    char buffer_copy[size_copy + 1];

    strncpy(buffer_copy, mBuffer, size_copy);
    buffer_copy[size_copy] = '\0';

    DESTROY(mBuffer);
    mSize = size_copy - (s_size * count);

    if (mSize == 0) {
        mBuffer = createNewEmpty();
        return *this;
    }

    mBuffer = new char[mSize + 1];

    for (size_t i = 0, j = 0; i < size_copy; ++i) {
        if (exactMatch(buffer_copy[i], s[0], mCaseSensitivity)) {
            size_t i_save = i;
            size_t i_temp = i + 1;
            for (size_t s_pos = 1; s_pos < s_size; ++i_temp, ++s_pos)
                if (!exactMatch(buffer_copy[i_temp], s[s_pos],
                            mCaseSensitivity))
                    break;
            if (i_save == (i_temp - s_size)) {
                i += s_size - 1;
                continue;
            }
        }
        mBuffer[j++] = buffer_copy[i];
    }

    mBuffer[mSize] = '\0';
    return *this;
}

String &String::remove(const String &s)
{
    return remove(s.mBuffer);
}

String &String::remove(const long long n)
{
    NUMBER_BUFFER("%lli", n);
    return remove(nbuf);
}

String &String::remove(const unsigned long long n)
{
    NUMBER_BUFFER("%llu", n);
    return remove(nbuf);
}

String &String::remove(const long n)
{
    NUMBER_BUFFER("%li", n);
    return remove(nbuf);
}

String &String::remove(const unsigned long n)
{
    NUMBER_BUFFER("%lu", n);
    return remove(nbuf);
}

String &String::remove(const int n)
{
    NUMBER_BUFFER("%i", n);
    return remove(nbuf);
}

String &String::remove(const unsigned int n)
{
    NUMBER_BUFFER("%u", n);
    return remove(nbuf);
}

String &String::remove(const short n)
{
    NUMBER_BUFFER("%hi", n);
    return remove(nbuf);
}

String &String::remove(const unsigned short n)
{
    NUMBER_BUFFER("%hu", n);
    return remove(nbuf);
}

String &String::remove(const long double n)
{
    NUMBER_BUFFER("%Lg", n);
    return remove(nbuf);
}

String &String::remove(const double n)
{
    NUMBER_BUFFER("%g", n);
    return remove(nbuf);
}

String &String::remove(const float n)
{
    NUMBER_BUFFER("%g", n);
    return remove(nbuf);
}

String &String::toUpper()
{
    for (size_t i = 0; i < mSize; ++i)
        mBuffer[i] = toUpperCase(mBuffer[i]);
    return *this;
}

String &String::toLower()
{
    for (size_t i = 0; i < mSize; ++i)
        mBuffer[i] = toLowerCase(mBuffer[i]);
    return *this;
}

String &String::reverse()
{
    char buffer_copy[mSize + 1];

    strncpy(buffer_copy, mBuffer, mSize);
    buffer_copy[mSize] = '\0';

    for (ssize_t i = mSize - 1, j = 0; i >= 0; --i, ++j)
        mBuffer[j] = buffer_copy[i];
    return *this;
}

long long String::toLongLong() const
{
    long long n = 0LL;

    if (mBuffer != NULL && mBuffer[0] != '\0') {
        long long ig = 0LL;
        int sign = 1;
        size_t i = 0;
        while (isWhiteSpace(mBuffer[i]))
            i++;
        if (mBuffer[i] == '-')
            sign = -1;
        while (mBuffer[i] != '\0') {
            if (isDigit(mBuffer[i])) {
                ig = ig * 10LL + mBuffer[i++] - '0';
                continue;
            }
            i++;
        }
        n = ig * (long long)sign;
    }
    return n;
}

unsigned long long String::toUnsignedLongLong() const
{
    unsigned long long n = 0LLU;

    if (mBuffer != NULL && mBuffer[0] != '\0') {
        unsigned long long ig = 0LLU;
        size_t i = 0;
        while (isWhiteSpace(mBuffer[i]))
            i++;
        while (mBuffer[i] != '\0') {
            if (isDigit(mBuffer[i])) {
                ig = ig * 10LLU + mBuffer[i++] - '0';
                continue;
            }
            i++;
        }
        n = ig;
    }
    return n;
}

long String::toLong() const
{
    long long n = toLongLong();

    if (n > LONG_MAX || n < LONG_MIN)
        n = 0LL;
    return (long)n;
}

unsigned long String::toUnsignedLong() const
{
    unsigned long long n = toUnsignedLongLong();

    if (n > ULONG_MAX)
        n = 0LLU;
    return (unsigned long)n;
}

int String::toInt() const
{
    long long n = toLongLong();

    if (n > INT_MAX || n < INT_MIN)
        n = 0LL;
    return (int)n;
}

unsigned int String::toUnsignedInt() const
{
    unsigned long long n = toUnsignedLongLong();

    if (n > UINT_MAX)
        n = 0LLU;
    return (unsigned int)n;
}

short String::toShort() const
{
    long long n = toLongLong();

    if (n > SHRT_MAX || n < SHRT_MIN)
        n = 0LL;
    return (short)n;
}

unsigned short String::toUnsignedShort() const
{
    unsigned long long n = toUnsignedLongLong();

    if (n > USHRT_MAX)
        n = 0LLU;
    return (unsigned short)n;
}

long double String::toLongDouble() const
{
    long double n = 0.0;

    if (mBuffer != NULL && mBuffer[0] != '\0') {
        int sign = 1;
        size_t i = 0;
        long double ip = 0.0;
        long double fp = 0.0;
        long double fd = 1.0;
        while (isWhiteSpace(mBuffer[i]))
            i++;
        if (mBuffer[i] == '-')
            sign = -1;
        while (mBuffer[i] != '.' && mBuffer[i] != '\0') {
            if (isDigit(mBuffer[i])) {
                ip = ip * 10 + mBuffer[i++] - '0';
                continue;
            }
            i++;
        }
        if (mBuffer[i] == '.') {
            i++;
            while (mBuffer[i] != '\0') {
                if (isDigit(mBuffer[i])) {
                    fd *= 0.1;
                    fp += (mBuffer[i] - '0') * fd;
                }
                i++;
            }
        }
        n = (ip + fp) * (long double)sign;
    }
    return n;
}

double String::toDouble() const
{
    long double n = toLongDouble();

    if (n > DBL_MAX || n < DBL_MIN)
        n = 0.0;
    return (double)n;
}

float String::toFloat() const
{
    long double n = toLongDouble();

    if (n > FLT_MAX || n < FLT_MIN)
        n = 0.0;
    return (float)n;
}

String::operator bool() const
{
    return mBuffer != NULL && mBuffer[0] != '\0';
}

String &String::operator=(const char c)
{
    DESTROY(mBuffer);

    if (c == '\0') {
        mSize = 0;
        mBuffer = createNewEmpty();
    } else {
        mSize = 1;
        mBuffer = new char[mSize + 1];
        mBuffer[0] = c;
        mBuffer[1] = '\0';
    }
    return *this;
}

String &String::operator=(const char *s)
{
    DESTROY(mBuffer);

    if (s == NULL)
        mSize = 0;
    else if (s[0] == '\0') {
        mSize = 0;
        mBuffer = createNewEmpty();
    } else if (s[0] != '\0') {
        mSize = strlen(s);
        mBuffer = createNewCopy(s);
    }
    return *this;
}

String &String::operator=(const String &s)
{
    return operator=(s.mBuffer);
}

String &String::operator=(const long long n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%lli", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const unsigned long long n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%llu", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const long n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%li", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const unsigned long n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%lu", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const int n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%i", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const unsigned int n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%u", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const short n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%hi", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const unsigned short n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%hu", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const long double n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%Lg", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const double n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%g", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator=(const float n)
{
    DESTROY(mBuffer);
    NUMBER_BUFFER("%g", n);
    mBuffer = createNewCopy(nbuf);
    mSize = strlen(mBuffer);
    return *this;
}

String &String::operator+=(const char c)
{
    return insert(c, mSize);
}

String &String::operator+=(const char *s)
{
    return insert(s, mSize);
}

String &String::operator+=(const String &s)
{
    return insert(s, mSize);
}

String &String::operator+=(const long long n)
{
    NUMBER_BUFFER("%lli", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const unsigned long long n)
{
    NUMBER_BUFFER("%llu", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const long n)
{
    NUMBER_BUFFER("%li", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const unsigned long n)
{
    NUMBER_BUFFER("%lu", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const int n)
{
    NUMBER_BUFFER("%i", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const unsigned int n)
{
    NUMBER_BUFFER("%u", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const short n)
{
    NUMBER_BUFFER("%hi", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const unsigned short n)
{
    NUMBER_BUFFER("%hu", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const long double n)
{
    NUMBER_BUFFER("%Lg", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const double n)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, mSize);
}

String &String::operator+=(const float n)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const char c)
{
    return insert(c, mSize);
}

String &String::operator+(const char *s)
{
    return insert(s, mSize);
}

String &String::operator+(const String &s)
{
    return insert(s, mSize);
}

String &String::operator+(const long long n)
{
    NUMBER_BUFFER("%lli", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const unsigned long long n)
{
    NUMBER_BUFFER("%llu", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const long n)
{
    NUMBER_BUFFER("%li", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const unsigned long n)
{
    NUMBER_BUFFER("%lu", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const int n)
{
    NUMBER_BUFFER("%i", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const unsigned int n)
{
    NUMBER_BUFFER("%u", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const short n)
{
    NUMBER_BUFFER("%hi", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const unsigned short n)
{
    NUMBER_BUFFER("%hu", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const long double n)
{
    NUMBER_BUFFER("%Lg", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const double n)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, mSize);
}

String &String::operator+(const float n)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, mSize);
}

String &String::operator-=(const char c)
{
    return remove(c);
}

String &String::operator-=(const char *s)
{
    return remove(s);
}

String &String::operator-=(const String &s)
{
    return remove(s);
}

String &String::operator-=(const long long n)
{
    NUMBER_BUFFER("%lli", n);
    return remove(nbuf);
}

String &String::operator-=(const unsigned long long n)
{
    NUMBER_BUFFER("%llu", n);
    return remove(nbuf);
}

String &String::operator-=(const long n)
{
    NUMBER_BUFFER("%li", n);
    return remove(nbuf);
}

String &String::operator-=(const unsigned long n)
{
    NUMBER_BUFFER("%lu", n);
    return remove(nbuf);
}

String &String::operator-=(const int n)
{
    NUMBER_BUFFER("%i", n);
    return remove(nbuf);
}

String &String::operator-=(const unsigned int n)
{
    NUMBER_BUFFER("%u", n);
    return remove(nbuf);
}

String &String::operator-=(const short n)
{
    NUMBER_BUFFER("%hi", n);
    return remove(nbuf);
}

String &String::operator-=(const unsigned short n)
{
    NUMBER_BUFFER("%hu", n);
    return remove(nbuf);
}

String &String::operator-=(const long double n)
{
    NUMBER_BUFFER("%Lg", n);
    return remove(nbuf);
}

String &String::operator-=(const double n)
{
    NUMBER_BUFFER("%g", n);
    return remove(nbuf);
}

String &String::operator-=(const float n)
{
    NUMBER_BUFFER("%g", n);
    return remove(nbuf);
}

String &String::operator-(const char c)
{
    return remove(c);
}

String &String::operator-(const char *s)
{
    return remove(s);
}

String &String::operator-(const String &s)
{
    return remove(s);
}

String &String::operator-(const long long n)
{
    NUMBER_BUFFER("%lli", n);
    return remove(nbuf);
}

String &String::operator-(const unsigned long long n)
{
    NUMBER_BUFFER("%llu", n);
    return remove(nbuf);
}

String &String::operator-(const long n)
{
    NUMBER_BUFFER("%li", n);
    return remove(nbuf);
}

String &String::operator-(const unsigned long n)
{
    NUMBER_BUFFER("%lu", n);
    return remove(nbuf);
}

String &String::operator-(const int n)
{
    NUMBER_BUFFER("%i", n);
    return remove(nbuf);
}

String &String::operator-(const unsigned int n)
{
    NUMBER_BUFFER("%u", n);
    return remove(nbuf);
}

String &String::operator-(const short n)
{
    NUMBER_BUFFER("%hi", n);
    return remove(nbuf);
}

String &String::operator-(const unsigned short n)
{
    NUMBER_BUFFER("%hu", n);
    return remove(nbuf);
}

String &String::operator-(const long double n)
{
    NUMBER_BUFFER("%Lg", n);
    return remove(nbuf);
}

String &String::operator-(const double n)
{
    NUMBER_BUFFER("%g", n);
    return remove(nbuf);
}

String &String::operator-(const float n)
{
    NUMBER_BUFFER("%g", n);
    return remove(nbuf);
}

String &String::operator<<(const char c)
{
    return insert(c, mSize);
}

String &String::operator<<(const char *s)
{
    return insert(s, mSize);
}

String &String::operator<<(const String &s)
{
    return insert(s, mSize);
}

String &String::operator<<(const long long n)
{
    NUMBER_BUFFER("%lli", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const unsigned long long n)
{
    NUMBER_BUFFER("%llu", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const long n)
{
    NUMBER_BUFFER("%li", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const unsigned long n)
{
    NUMBER_BUFFER("%lu", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const int n)
{
    NUMBER_BUFFER("%i", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const unsigned int n)
{
    NUMBER_BUFFER("%u", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const short n)
{
    NUMBER_BUFFER("%hi", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const unsigned short n)
{
    NUMBER_BUFFER("%hu", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const long double n)
{
    NUMBER_BUFFER("%Lg", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const double n)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, mSize);
}

String &String::operator<<(const float n)
{
    NUMBER_BUFFER("%g", n);
    return insert(nbuf, mSize);
}

bool String::operator==(const size_t z) const
{
    return mSize == z;
}

bool String::operator==(const char c) const
{
    return equals(c);
}

bool String::operator==(const char *s) const
{
    return equals(s);
}

bool String::operator==(const String &s) const
{
    return equals(s);
}

bool String::operator!=(const size_t z) const
{
    return mSize != z;
}

bool String::operator!=(const char c) const
{
    return !equals(c);
}

bool String::operator!=(const char *s) const
{
    return !equals(s);
}

bool String::operator!=(const String &s) const
{
    return !equals(s);
}

bool String::operator<(const size_t z) const
{
    return mSize < z;
}

bool String::operator<(const char c) const
{
    if (c == '\0')
        return false;
    return mSize < 1;
}

bool String::operator<(const char *s) const
{
    return mSize < strlen(s);
}

bool String::operator<(const String &s) const
{
    return mSize < s.mSize;
}

bool String::operator<=(const size_t z) const
{
    return mSize <= z;
}

bool String::operator<=(const char c) const
{
    if (c == '\0')
        return mSize <= 0;
    return mSize <= 1;
}

bool String::operator<=(const char *s) const
{
    return mSize <= strlen(s);
}

bool String::operator<=(const String &s) const
{
    return mSize <= s.mSize;
}

bool String::operator>(const size_t z) const
{
    return mSize > z;
}

bool String::operator>(const char c) const
{
    if (c == '\0')
        return mSize > 0;
    return mSize > 1;
}

bool String::operator>(const char *s) const
{
    return mSize > strlen(s);
}

bool String::operator>(const String &s) const
{
    return mSize > s.mSize;
}

bool String::operator>=(const size_t z) const
{
    return mSize >= z;
}

bool String::operator>=(const char c) const
{
    if (c == '\0')
        return mSize >= 0;
    return mSize >= 1;
}

bool String::operator>=(const char *s) const
{
    return mSize >= strlen(s);
}

bool String::operator>=(const String &s) const
{
    return mSize >= s.mSize;
}

const char String::operator[](ssize_t index) const
{
    return mBuffer[properIndex(index, mSize)];
}

