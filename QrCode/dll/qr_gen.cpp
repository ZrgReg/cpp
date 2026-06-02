#include "_unt/QrMng.h"
#include "qr_gen.h"

#define VERSION      5

#define NOT_MARGIN   0
#define YES_MARGIN   1
#define TRP_MARGIN   2

#define MIN_QUALITY  100
#define MAX_QUALITY  0
#define DFT_QUALITY -1

#define ERR_LOW      0
#define ERR_MEDIUM   1
#define ERR_QUARTILE 2
#define ERR_HIGH     3

#define FMT_MONO     1
#define FMT_RGB32    4
#define FMT_ARGB32   5
#define FMT_RGB16    7
#define FMT_ARGB24   12
#define FMT_RGB24    13
#define FMT_ARGB16   15
#define FMT_GRAY     24

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

QrMng *mng = nullptr;

EXTERN_DLL_EXPORT int  version(void) { return VERSION; }
EXTERN_DLL_EXPORT void initial(void) { if(!mng) { mng = new QrMng; }}
EXTERN_DLL_EXPORT void destroy(void) { delete mng; mng = nullptr; }
EXTERN_DLL_EXPORT int  execute(
    const char *xml, int size, int threads, int mode )
{
    initial();
    return mng->GenerateQR(
        xml, threads, mode, size, YES_MARGIN
      , FMT_MONO, DFT_QUALITY, ERR_LOW );
}// execute

EXTERN_DLL_EXPORT int  perform(
    const char *xml, int size, int margin, int threads, int mode )
{
    if(mng == nullptr) { initial(); }
    return mng->GenerateQR(
        xml, threads, mode, size, margin
      , FMT_MONO, DFT_QUALITY, ERR_LOW );
}// perform

EXTERN_DLL_EXPORT int  advance(
    const char *xml, int size, int margin, int threads, int mode
  , int img_format, int img_quality, int err_correction )
{
    if(mng == nullptr) { initial(); }
    return mng->GenerateQR(
        xml, threads, mode, size, margin
      , img_format, img_quality, err_correction );
}// perform

EXTERN_DLL_EXPORT int  err_cnt(void)
    { return mng == nullptr ? 0 : mng->numErr(); }

EXTERN_DLL_EXPORT const void* err_txt(const char *fmt)
    { return mng == nullptr ? nullptr : mng->lstErr(fmt); }

EXTERN_DLL_EXPORT const void* err_txt_1251(void)
    { return mng == nullptr ? nullptr : mng->lstErr("Windows-1251"); }
