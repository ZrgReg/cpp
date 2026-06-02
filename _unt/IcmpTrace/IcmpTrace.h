#ifndef ICMPTRACE_H
#define ICMPTRACE_H

// INCLUDE ---------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <QString>
#include <QList>

#include "winsock2.h"
#include "iphlpapi.h"
#include "icmpapi.h"

// Ответ на пинг. *************************************************************/
/******************************************************************************/
struct PingRes { int term, cerr; QString addr; };
struct HopsRes : public PingRes { int hops; };

/* Трассировка пакетов ICMP - запросами. **************************************/
/******************************************************************************/
class IcmpTrace {

 public:
   ~IcmpTrace();
    IcmpTrace();

    IcmpTrace* message(int size, const QString &text);

    PingRes ping(const QString &addr, int deep, int tout);
    int     hops(const QString &addr, int deep, int tout);

 private:
    DWORD send_size = 0, rply_size = 0;
    char *send_buff = nullptr;
    char *rply_buff = nullptr;
    HANDLE icmp_file = nullptr;
    IP_OPTION_INFORMATION optn_info;
    IN_ADDR rply_addr;

    void del_buff(void) { delete send_buff; delete rply_buff; }

};// IcmpTrace

//------------------------------------------------------------------------------
#endif // ICMPTRACE_H
