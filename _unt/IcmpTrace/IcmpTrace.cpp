// INCLUDE. --------------------------------------------------------------------
//------------------------------------------------------------------------------
#include "IcmpTrace.h"


// Деструктор. -----------------------------------------------------------------
//------------------------------------------------------------------------------
IcmpTrace::~IcmpTrace() {
    del_buff();
    IcmpCloseHandle(this->icmp_file);
}// ~IcmpTrace

// Конструктор. ----------------------------------------------------------------
//------------------------------------------------------------------------------
IcmpTrace::IcmpTrace() {
    this->optn_info.Flags = 0;
    this->optn_info.OptionsData = nullptr;
    this->optn_info.OptionsSize = 0;
    this->optn_info.Tos = 0;

    this->icmp_file = IcmpCreateFile();

    message(32, "### SEND ###");
}// IcmpTrace

// Изменить размер сообщения. --------------------------------------------------
//------------------------------------------------------------------------------
IcmpTrace* IcmpTrace::message(int size, const QString &text) {
    del_buff();
    this->send_size = size;
    this->send_buff = new char[size];

    for(int i=0; i<size && i<text.size(); i++)
        { this->send_buff[i] = text[i].toLatin1(); }

    this->rply_size = sizeof(ICMP_ECHO_REPLY) + this->send_size;
    this->rply_buff = new char[this->rply_size];

    return this;
}// message_size

// Пинг. -----------------------------------------------------------------------
//------------------------------------------------------------------------------
PingRes IcmpTrace::ping(const QString &addr, int deep, int tout) {
    PingRes res;
    unsigned long ipaddr = inet_addr(addr.toStdString().c_str());
    this->optn_info.Ttl = deep;

    if(IcmpSendEcho(
        this->icmp_file, ipaddr
      , this->send_buff, this->send_size
      ,&this->optn_info
      , this->rply_buff, this->rply_size, tout ))
    {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)this->rply_buff;
        this->rply_addr.S_un.S_addr = pEchoReply->Address;

        res.cerr = 0;
        res.term = pEchoReply->RoundTripTime;
        res.addr = inet_ntoa(this->rply_addr);
    } else {
        res.cerr = GetLastError();
        res.term = -1; res.addr = "";
    }// else // if(if(IcmpSendEcho( ...

    return res;;
}// ping

// Расчитать количество посредников. -------------------------------------------
//------------------------------------------------------------------------------
int IcmpTrace::hops(const QString &addr, int deep, int tout) {
    for(int i=1; i<=deep; i++)
        { if(ping(addr, i, tout).addr == addr) { return i; }}
    return -1;
}// hops

//------------------------------------------------------------------------------


