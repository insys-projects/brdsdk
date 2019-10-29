//
//  Copyright (c) 1999, InSys Corp.
//
//  ===== pcidef.h =====
//
//	PCI definition structures
//
//

#ifndef __PCIDEF_H
#define __PCIDEF_H

//typedef	int	BOOL;

enum PciErrorCodes {
  PCI_errOK           = 0,
  PCI_errNOMEM        = 8,
  PCI_errNOTSUPPORTED = 0x00000081, /* BIOS function not supported */
  PCI_errBADVENDORID  = 0x00000083, /* Bad vendor ID */
  PCI_errNOTFOUND     = 0x00000086, /* Device not found */
  PCI_errBADREGNUMBER,              /* Bad register number */
  PCI_errSETFAILED,                 /* Set failed */
  PCI_errBUFTOOSMALL                /* Buffer is too small */
};

/*
 * Hardware Mechanism type to access PCI configuration space
 */
typedef union PciBiosCfgMech {
//  UCHAR AsWhole;
  ULONG AsWhole;
  struct {
    USHORT mechOneSup : 1,
         mechTwoSup : 1;
    USHORT            : 2;
    USHORT spcyOneSup : 1,
         spcyTwoSup : 1;
  } ByBits;
} PCI_CFGMECH;

typedef struct PciBiosStatus { /* PCI BIOS Present Status */
  PCI_CFGMECH cfg; /* Hardware cfg mechanism supported */
//  UCHAR        cfg; /* Hardware cfg mechanism supported */
  USHORT        ver; /* Version major/minor */
  UCHAR        lst; /* Last bus no (0...N-1) */
} PCI_STATUS;

typedef union PciDeviceFxn { /* PCI Device and Function Numbers */
  ULONG AsWhole;
//  UCHAR AsWhole;
  struct {
    USHORT fno : 3, /* Function Number */
		   dno : 5; /* Device Number ( Slot) */
  } ByBits;
} PCI_DEVFXN;

typedef struct PciDevice { /* PCI Device */
  PCI_DEVFXN tar;
  UCHAR      bus;		// Bus number 
} PCI_DEVICE;

# define FxnNo tar.ByBits.fno
# define DevNo tar.ByBits.dno

typedef struct PciDeviceId {
  USHORT DID; /* Device Identificator */
  USHORT VID; /* Vendor Identificator */
  USHORT Cnt; /* Instances counter */
} PCI_DEVID;

typedef struct PciClassCode {
  UCHAR intf; /* Programming Interface Class Code */
  UCHAR sub;  /* Secondary Sub-Class Code */
  UCHAR base; /* Base Class Code */
} PCI_CLASS;

typedef struct PciClassId {
  PCI_CLASS  *CLCD; /* Class Code */
  USHORT       Cnt;  /* Instances counter */
} PCI_CLSID;

typedef struct PciRegId {
  PCI_DEVICE  Dev;
  ULONG       Reg;  /* Register offset in header */
  ULONG       Data; /* Data or pointer to data */
  ULONG		  Width; 	
} PCI_REGID;

typedef struct PciData {
  PCI_DEVICE  *Dev;
  ULONG       Offset;		/* Register offset in header */
  void        *Data;		/* Data or pointer to data */
  ULONG		  SizeOfBytes; 	
} PCI_DATA;

/*
 * PCI Configuration Space Header Region
 */
typedef struct PciCfgSpaceHdrRgn {
  USHORT      VID;    /* Vendor Identification Register */
  USHORT      DID;    /* Device Identification Register */
  USHORT      PCICMD; /* PCI Command Register */
  USHORT      PCISTS; /* PCI Status Register */
  UCHAR      RID;    /* Revision Identification Register */
  PCI_CLASS CLCD;   /* Class Code */
  UCHAR      CALN;   /* Cache Line Size Register */
  UCHAR      LAT;    /* Master Latency Timer */
  UCHAR      HDR;    /* Header Type */
  UCHAR      BIST;   /* Built-in Self-test */
  union PciHeaderTypeRgn {
    struct PciHeaderTypeOne {
      ULONG BADR[6]; /* Base Address Registers */
      ULONG CISP;    /* CardBus Card-Information-Structure Pointer */
      USHORT  SID;     /* Subsystem ID */
      USHORT  SVID;    /* Subsystem Vendor ID */
      ULONG EXROM;   /* Expansion ROM Base Address */
      ULONG RESV[2]; /* Reserved */
      UCHAR  INTLN;   /* Interrupt Line */
      UCHAR  INTPIN;  /* Interrupt Pin */
      UCHAR  MINGNT;  /* Minimum Grant */
      UCHAR  MAXLAT;  /* Maximum Latency */
    } Type00h;
    struct PciHeaderTypeTwo {
      ULONG BADR[2]; /* Base Address Registers */
      UCHAR  PRINUM;  /* Primary Bus Number */
      UCHAR  SECNUM;  /* Secondary Bus Number */
      UCHAR  SUBNUM;  /* Subordinate Bus Number */
      UCHAR  SECLAT;  /* Secondary Latency Timer */
      UCHAR  IOBASE;  /* I/O Base */
      UCHAR  IOLIM;   /* I/O Limit */
      USHORT  SECSTS;  /* Secondary Status */
      USHORT  MEMBASE; /* Memory Base */
      USHORT  MEMLIM;  /* Memory Limit */
      USHORT  PFMBASE; /* Prefetchable Memory Base */
      USHORT  PFMLIM;  /* Prefetchable Memory Limit */
      ULONG PBU32B;  /* Prefetchable Base Upper 32 Bits */
      ULONG PLU32B;  /* Prefetchable Limit Upper 32 Bits */
      USHORT  IOBU16B; /* I/O Base Upper 16 Bits */
      USHORT  IOLU16B; /* I/O Limit Upper 16 Bits */
      ULONG RESV;    /* Reserved */
      ULONG EXROM;   /* Expansion ROM Base Address */
      UCHAR  INTLN;   /* Interrupt Line */
      UCHAR  INTPIN;  /* Interrupt Pin */
      USHORT  BRGCTRL; /* Bridge Control */
    } Type01h;
  } Hdr;

  UCHAR	vendorSpecific[192];

} PCI_HEADER;

typedef PCI_HEADER *PCI_HDRPTR;
typedef PCI_HDRPTR PCI_DEVTABLE[];

typedef struct PCI_CONFIG_HEADER_0 {
    USHORT  VendorID;
    USHORT  DeviceID;
    USHORT  Command; 
    USHORT  Status;
    UCHAR   RevisionID;
    UCHAR   ProgIf;
    UCHAR   SubClass;  
    UCHAR   BaseClass; 
    UCHAR   CacheLineSize;
    UCHAR   LatencyTimer; 
    UCHAR   HeaderType;   
    UCHAR   BIST;          
    ULONG   BaseAddresses[6];
    ULONG   CardBusCISPtr;
    USHORT  SubsystemVendorID;
    USHORT  SubsystemID;
    ULONG   ROMBaseAddress;
    ULONG   Reserved2[2];
    UCHAR   InterruptLine; 
    UCHAR   InterruptPin;  
    UCHAR   MinimumGrant;
    UCHAR   MaximumLatency;

	UCHAR	vendorSpecific[192];

} PCI_CONFIG_HEADER_0;

typedef struct PciInterruptDescriptor {
  UCHAR link; /* INT link value */
  USHORT map;  /* INT Bit-Map */
} PCI_INTDESC;

typedef struct PCI_SPACE {
  PCI_DEVICE  Dev;
  PCI_HEADER  Hdr;
} PCI_SPACE;

typedef struct PciIrqRoutingTableEntry {
  PCI_DEVICE  dev;
  PCI_INTDESC desc[4];
}PCI_INTROUTE;

typedef PCI_INTROUTE *PCI_INTROUTETAB;

typedef struct PciInterruptRoutingData {
  USHORT size;
  PCI_INTROUTETAB tab;
} PCI_INTROUTEDATA;

#endif	// __PCIDEF_H
