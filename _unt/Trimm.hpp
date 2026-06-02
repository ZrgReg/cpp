#ifndef TRIMM_HPP
#define TRIMM_HPP


#include <QString>
#include <algorithm>

namespace detail {

struct IsSpace final {
    QString const ws;
    QString::const_iterator begin = ws.begin(), end = ws.end();

    explicit IsSpace(const QString &whitespace) : ws(sorted(whitespace)) {}

    static QString sorted(QString s)
        { std::sort(s.begin(), s.end()); return s; }


    bool (IsSpace::*test)(QChar) const =
        ws.isEmpty()     ? &IsSpace::tst1 :
        (ws.size() <= 8) ? &IsSpace::tst2 : &IsSpace::tst3;

    bool tst1(const QChar c) const { return c.isSpace(); }
    bool tst2(const QChar c) const { return ws.contains(c); }
    bool tst3(const QChar c) const { return std::binary_search(begin, end, c); }

    inline bool operator()(QChar c) const { return (*this.*test)(c); }
};

}// namespace detail

enum TrimmedOption { TrimmedLeft = 1, TrimmedRight = 2 };
Q_DECLARE_FLAGS(TrimmedOptions, TrimmedOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(TrimmedOptions)
QStringRef trimmedRef(
    const QString &src, const QString &whitespace
  , TrimmedOptions opt = TrimmedLeft | TrimmedRight )
{
  detail::IsSpace const isSpace {whitespace};
  int l = 0;
  if(opt & TrimmedLeft)
    { while(l < src.length() && isSpace(src[l])) { l++; }}
  int r = src.length();
  if (opt & TrimmedRight)
    while (r > 0 && isSpace(src[r-1]))
      r--;
  return {&src, l, r-l};
}

QStringRef trimmed(
    const QString &src, const QString &whitespace
  , TrimmedOptions opt = TrimmedLeft | TrimmedRight)
    { return trimmedRef(src, whitespace, opt); }

QStringRef trimmedLeftRef(const QString &src, const QString &whitespace = {})
    { return trimmedRef(src, whitespace, TrimmedLeft); }

QStringRef trimmedRightRef(const QString &src, const QString &whitespace = {})
    { return trimmed(src, whitespace, TrimmedRight); }

QStringRef trimmedLeft(const QString &src, const QString &whitespace = {})
    { return trimmedRef(src, whitespace, TrimmedLeft); }

QStringRef trimmedRight(const QString &src, const QString &whitespace = {})
    { return trimmedRef(src, whitespace, TrimmedRight); }

#endif // TRIMM_HPP
