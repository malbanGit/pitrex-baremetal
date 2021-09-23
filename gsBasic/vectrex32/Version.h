/* 
 * File:   Version.h
 * Author: Bob
 *
 * Created on December 12, 2015, 2:46 PM
 */

#ifndef VERSION_H
#define	VERSION_H

#ifdef	__cplusplus
extern "C" {
#endif

extern uint32_t MAJOR_VERSION, MINOR_VERSION;
    
#define VERSIONTEMPLATE "Vectrex32 %s, firmware %d.%02d " __DATE__
#define COPYRIGHT "Copyright (c)2020 by Robert E. Alexander"

extern int hwversion;
extern const char *hwversions[];
extern const int firstHWVersionUsing6522Signal;

#ifdef	__cplusplus
}
#endif

#endif	/* VERSION_H */

