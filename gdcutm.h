#ifndef __GDCUMTM_H__908322654365__
#define __GDCUMTM_H__908322654365__

extern void GDCtoUTM(
    double lat,   /* In radians, north is positive, south is negative */
    double lon,   /* In radians, east is positive, west is negative */
    double *northing,
    double *easting,
    char *UTMZone);

void UTMtoGDC(
    const double northing,
    const double easting,
    const char *UTMZone,
    double *lat,
    double *lon);

#define M_PI  3.14159265359


#endif