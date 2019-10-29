
#include	"brd.h"
#include	"extn.h"
//#include	"extn_andor.h"
#include	"ctrlstrm.h"
#include	"ctrladc.h"
#include	"ctrlsdram.h" // only for base module with SDRAM on board (e.g. AMBPCD)
#include	"ctrlcmpsc.h"

//#include	"ctrldspnode.h" // only for DSP PLD on AMBPCD

#define		MAX_DEV		12		// считаем, что модулей может быть не больше MAX_DEV
#define		MAX_SRV		16		// считаем, что служб на одном модуле может быть не больше MAX_SRV
#define		MAX_PU		8		// считаем, что PU-устройств (ПЛИС, ППЗУ) на одном модуле может быть не больше MAX_PU
#define		MAX_CHAN	64		// считаем, что каналов может быть не больше MAX_CHAN

#define		FORMAT_MASK	0xF		// маска регистра формат, определяющая разрядность отсчетов

const U16 PLD_CFG_TAG	= 0x0500; // tag of PLD

//void DisplayError(S32 status, BRDCHAR* func_name, BRDCHAR* cmd_str);
void DisplayError(S32 status, const char* func_name, const BRDCHAR* cmd_str);

S32 AdcSettings(BRD_Handle hADC, int idx, int isx, BRDCHAR* srvName, BRDCHAR* iniFileName);
S32 SdramSettings(ULONG mem_mode, BRD_Handle hADC, unsigned long long& bBuf_size);

//S32 DaqIntoSdram(BRD_Handle hADC, PVOID pSig, ULONG bBufSize, int num_chan);
//S32 DaqIntoSdramDMA(BRD_Handle hADC, int idx, ULONG bBufSize, int num_chan);
S32 DaqIntoSdram(BRD_Handle hADC);
S32 DaqIntoSdramDMA(BRD_Handle hADC);
S32 DataFromMem(BRD_Handle hADC, PVOID pBuf, ULONG bBufSize, ULONG DmaOn);
S32 DaqIntoFifo(BRD_Handle hADC, PVOID pSig, ULONG bBufSize, int DspMode);
//S32 DaqIntoFifoDMA(BRD_Handle hADC, int idx, ULONG bBufSize, int DspMode);
S32 DaqIntoFifoDMA(BRD_Handle hADC);

S32 AllocDaqBuf(BRD_Handle hADC, PVOID* &pSig, unsigned long long* pbytesBufSize, ULONG bufType, ULONG* pBlkNum);
S32 FreeDaqBuf(BRD_Handle hADC, ULONG blkNum);
//S32 AllocDaqBuf(BRD_Handle hADC, PVOID* pSig, unsigned long long* pbytesBufSize, ULONG bufType);
//S32 FreeDaqBuf(BRD_Handle hADC);

S32 StartDaqIntoSdramDMA(BRD_Handle hADC, int idx);
void BreakDaqIntoSdramDMA();
S32 CheckDaqIntoSdramDMA();
S32 EndDaqIntoSdramDMA();

void MapWriteData(PVOID* pBuf, unsigned long long nNumberOfBytes);
void MapWrFlagSinc(int flg, int isNewParam);
int MapRdFlagSinc();
S32	MapDataFromMemWriteData(BRD_Handle hADC, PVOID* pBuf, unsigned long long bBufSize, unsigned long long bMemBufSize, ULONG DmaOn);
