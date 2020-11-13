//header file for the thread where CAN and GPS data are stored

#ifndef __storedat
#define __storedat

void storeData(void);

extern char SD_Path[];
extern char CANdataFile[];

extern char GPSdataFile[];
extern char configFile[];

#endif
