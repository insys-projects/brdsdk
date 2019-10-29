//********************************************************
//
// exam_edac.h
// РџСЂРѕРіСЂР°РјРјР° EXAM_EDAC РґР»СЏ СѓРїСЂР°РІР»РµРЅРёСЏ Р¦РђРџР°РјРё РЅР° СЃСѓР±РјРѕРґСѓР»СЏС… 
//
// (C) РРЅРЎРёСЃ, Р­РєРєРѕСЂРµ, РњР°СЂС‚ 2014
//
//=********************************************************

#ifndef _EXAM_EDAC_H
#define _EXAM_EDAC_H

#include	"brd.h"
#include	"ctrlstrm.h"
#include	"ctrlcmpsc.h"
#include	"ctrldac.h"
//#include	"CtrlDac216x400m.h"
#include	"ctrlsdram.h"
//#include	"ctrlreg.h"
#include	"ctrlbasef.h"

//
// Macros
//

#define PI2   6.2831853071795864 
#define MAX_DEVICE   17 
#define MAX_DAC      (MAX_DEVICE*2)  
#define MAX_CHAN     32  
#define	MAX_SECTNAME 32
#define MAX_SERVICE_ON_DEVICE   20 

typedef		S64		SBIG;

typedef struct  
{
	BRD_Handle		handle;
	BRDCHAR			sSection[MAX_SECTNAME];
	S32				sampleSizeb;
	SBIG			samplesPerChannel;
	S32				chanMask;
	S32				chanNum;
	S32				chanMaxNum;
	double			aThdac[2];
	double			aAmpl[MAX_CHAN];
	double			aPhase[MAX_CHAN];
	double			aPhaseKee[MAX_CHAN];
	S32				signalType;
	SBIG			outBufSizeb;

	S32				switchIn;
	S32				switchOut;

	U32				nFifoSizeb;
	double			dSamplingRate;	// На самом деле это Data Rate
	double			dSignalFreq;
	BRDCHAR			sRegRwSpdFilename[500];

	void					*pBuf;
	BRDctrl_StreamCBufAlloc rBufAlloc;
} TDacParam;

typedef struct 
{
	U32  WorkMode;
	U32  IsAlwaysWriteSdram;
	U32  SamplesPerChannel;
	U32  IsSystemMemory;
	U32  IsAlwaysLoadPld;
	U32	 MasterSlave;
	U32	 StartSource;
	U32	 StartInverse;
	U32  StartTrigger; 
	U32	 StopSource;
	U32	 StopInverse;
	float ThresholdComp1;
	TCHAR PldFileName[255];
	TCHAR ServiceName[100];
	TCHAR IniFileName[100];
}EXAM_PARAM;

typedef struct 
{
    U32  ClockSource;
    U32  ExtClockValue;
    U32  RefSource;
    U32  ExtRefValue;
	U32  MaskChannels;
	U32  DividerClk;
	U32	 AmpChan0;
	U32	 AmpChan1;
	U32	 OutFreq;
	U32	 SignalType;
    U32  SlaveClockSource;
    U32  Filter;
    U32  EnableAnalogQm;
    U32  SamplesPerString;
    U32  StringsPerFrame;
    U32  SamplesPerStringReverse;
} DAC_PARAM;				// ADMDAC216x400M 

//
// Globals
//
extern BRDCHAR		g_iniFileName[MAX_PATH];
extern BRDCHAR		g_sFullName[MAX_PATH];

extern int			g_lid;
extern int			g_exitRightAway;
extern S32			g_nDevNum;
extern BRD_Handle	g_hDev[MAX_DEVICE];
extern S32			g_nDacNum;
extern TDacParam	g_aDac[MAX_DAC];
extern S32			g_idx[MAX_DAC];


//extern BRD_Handle		g_hCaptSrv[10];
//extern S32				g_nCaptSrv;
//extern double			g_OutRate;

//extern U32				g_OutBufSize;
//extern U32				g_DacFifoSize;
//extern int				g_NumChans;
//extern U32				g_SdramWriteBufSize;
//extern S32				*g_pbufSdram[6];

//extern EXAM_PARAM		g_ExamParam;
//extern DAC_PARAM		g_DacParam;

//
// Params from ini-file
//
extern BRDCHAR		g_sServiceName[256];	// Р±РµР· РЅРѕРјРµСЂР° СЃР»СѓР¶Р±С‹
extern BRDCHAR		g_sPldFileName[MAX_PATH];
extern int			g_nWorkMode;
extern int			g_nIsAlwaysWriteSdram;
extern SBIG			g_nSamplesPerChannel;
extern int			g_nIsSystemMemory;
extern int			g_nSdramWriteBufSize;
extern int			g_nIsAlwaysLoadPld;
extern int			g_nMasterSlave;
extern int			g_nDmaBufFactor;
extern int			g_nCycle;
extern int			g_nQuickQuit;
extern S32			g_nMsTimeout;

//
// EXAM_EDAC.CPP
//
S32 ParseCommandLine(int argc, BRDCHAR *argv[]);
S32 DisplayError( S32 status, const char *func_name, const BRDCHAR *cmd_str );
S32 CaptureAllDac( void );
S32 ReleaseAllDac( void );
S32	SetAllDac( void );
S32 SetSwitch( int idxDac );

S32	SetMasterSlave( void );
S32 CalcSignal( void *pvBuf, SBIG nSamples, int idxDac, int cnt );
S32 CalcSignalToBuf( void *pvBuf, SBIG nSamples, S32 signalType,
					S32 sampleWidth, S32 chanMask, S32 chanMaxNum, 
					double twiddle, double *aAmpl, double *aPhase );
S32 CalcSignalToChan( void *pvBuf, SBIG nSamples, S32 signalType,
					 S32 sampleWidth, S32 chanIdx, S32 chanNum, 
					 double twiddle, double ampl, double *pPhase );
S32 FillSignalFromFile( void *pvBuf, SBIG nSamples, int idxDac, int cnt );

S32 CorrectOutFreq( int idxDac );
S32 ReadIniFile2( TCHAR *pFileName );
S32 GetInifileString( const BRDCHAR *FileName, const BRDCHAR *SectionName, 
					  const BRDCHAR *ParamName, BRDCHAR *defValue, 
					  BRDCHAR *strValue, int strSize);
S32  ReadIniFileOption( void );
S32  ReadIniFileDevice( void );
S32 DisplayDacTraceText( int loop, int dacIdx );

//
// WORKMODE.CPP
//
S32 WorkMode( void );
S32 WorkMode0( void );
S32 WorkMode1( void );
S32 WorkMode2( void );
S32 WorkMode3( void );
S32 WorkMode4( void );
S32 WorkMode5( void );
S32 WorkMode6( void );
S32 WorkMode7( void );
S32 WorkMode8( void );

S32	FifoOutputCPU( void );
S32 FifoOutputCPUStart( S32 isCycle );
S32	FifoOutputDMA( void );
S32	FifoOutputCycleDMA( void );
S32	SdramLikeFifoOutputCycleDMA( void );

S32 SdramSetParam( int idxDac );
S32	SdramWriteDMA( void );
S32 SdramModulWriteDMA( int idxDac );
S32	SdramOutToDAC( void );
S32 SdramCycleOutput( S32 cycle );
S32	PrepareStart( void );

//
// TMP.CPP
//

S32	CaptureSrv(BRD_Handle handle,TCHAR* serviceName );
S32	SetParamSrv(void);
S32 DacSettings(BRD_Handle hDAC, int idx, BRDCHAR* srvName, BRDCHAR* iniFileName);
S32	WorkSrv(void);
S32 CalcSignalOld(S32 *samples, S32 nsamples);
S32 CalcSignalSDRAM(S32 *samples, S32 nsamples,double *phase, S32 *iphase);
S32 OldCorrectOutFreq(void);

S32	OldFifoOutputCPU( PVOID pSig, ULONG bBufSize);
S32 OldFifoOutputCPUCycleFIFO( PVOID pSig, ULONG bBufSize,S32 cycle);
S32	OldFifoOutputDMA(void);
S32	OldSdramWriteDMA(void);
S32 OldSdramModulWriteDMA(int nmod);
S32	OldSdramOutToDAC(void);
S32 OldCycleSdramOutput(S32 cycle);
S32	OldFifoOutputCycleDMA(void);
bool OldSetParamSDRAM(BRD_Handle hSrv);


#endif // _EXAM_EDAC_H

//
// End of file
//

