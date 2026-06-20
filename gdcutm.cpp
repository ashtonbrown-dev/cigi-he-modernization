/* Conversion of Geodetic (Latitude/Longitude) to/from
   Universal Transverse Mercator
   Taken directly from USGS Technical Bulletin 1532, 1984

   Special zones are not handled in this code:

   Zone 32V (Norway?)
   Lat 56 to 64 N, Lon 3 to 12 E instead of the normal 6 to 12 East

   Zones 31X - 37X (Svalbard), only odd numbered zones are used,
      9 and 12 deg wide,  Lat 72 to 84 N
     Lon  0 to  9 E - Zone 31X
     Lon  9 to 21 E - Zone 33X
     Lon 21 to 33 E - Zone 35X
     Lon 33 to 42 E - Zone 37X

   The so-called Swiss Zone (apparently used in Switzerland) is not supported

   All the series expansion terms shown in Bulletin 1532 are used.  I
   don't know how much accuracy that gives.  I'm assuming it's better
   than one meter, maybe even centimeter or millimeter.

   Bob Murray
   234-0178
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gdcutm.h"

/* Degrees/Radians conversions */
#define DTOR(d) ((d)*(2.0*M_PI)/360.0)
#define RTOD(r) ((r)*360.0 / (2.0*M_PI))


/* WGS 84 constants */

/* Semi-major axis (ellipsoid equitorial radius) in meters */
#define a ((double)6378137.0)

/* flattening = (a-b)/a */
#define f ((double)1.0 / (double)298.257223563)

/* Semi-minor axis (ellipsoid polar radius) in meters */
#define b (a * (1.0 - f))

/* eccentricity squared = 2f-f^2 */
#define esqu (double)(2*f - f*f)
#define ePrimeSqu (esqu / (1. - esqu))
#define e (sqrt(esqu))


/* UTM constants */
const double k0 = 0.9996;       /* Central meridian scale factor */

/* Constants used in calculating M, the TRUE distance along the
   central meridian */
const double C1 = 1. - esqu / 4. - esqu * esqu * 3. / 64. - esqu * esqu * esqu * 5. / 256.;
const double C2 = esqu * 3. / 8. + esqu * esqu * 3. / 32. +
                  esqu * esqu * esqu * 45. / 1024.;
const double C3 = esqu * esqu * 15. / 256. + esqu * esqu * esqu * 45. / 1024.;
const double C4 = esqu * esqu * esqu * 35. / 3072.;


void GDCtoUTM(
    double lat,   /* In radians, north is positive, south is negative */
    double lon,   /* In radians, east is positive, west is negative */
    double *northing,
    double *easting,
    char *UTMZone)
{
    double lonOrigin;
    double N, T, C, A, M;

    double lonDeg = RTOD(lon);

    //*UTMZone = (int)(lonDeg + 180.) / 6 + 1;
    int zone = atoi(UTMZone);

    /* Longitude origin is in middle of zone, so add 3 degrees */
    lonOrigin = DTOR((zone - 1) * 6 - 180 + 3);

    N = a / sqrt(1 - esqu * sin(lat) * sin(lat));
    T = tan(lat) * tan(lat);
    C = ePrimeSqu * cos(lat) * cos(lat);
    A = cos(lat) * (lon - lonOrigin);

    M = a * (C1 * lat - C2 * sin(2 * lat) + C3 * sin(4 * lat) - C4 * sin(6 * lat));

    *easting = k0 * N *
               (A + (1 - T + C) * A * A * A / 6.
                + (5. - 18.*T + T * T + 72.*C - 58 * ePrimeSqu) * A * A * A * A * A / 120.);

    /* Add FALSE Easting of 500 km */
    *easting += 500000.0;

    *northing = k0 *
                (M + N * tan(lat) * (A * A / 2. + (5. - T + 9 * C + 4 * C * C) * A * A * A * A / 24.
                                     + (61. - 58.*T + T * T + 600.*C - 330.*ePrimeSqu) *
                                     A * A * A * A * A * A / 720.));

    if (lat < 0) {
        /* False northing of 10,000 km southern hemisphere */
        *northing += 10000000.0;
    }
}

/* Screwy version for a particular visual database
   The central meridian is fixed 117 degrees West.  This is the normal
     central meridian for UTM Zone 11 which extends from 114 to 120
     degrees West longitude.  However, the database actually extends
     from 113 to 119 degrees West.  By correct UTM usage, that would
     put part of the database in zone 12 and that part would need a
     different central meridian and zero Easting point.  For simplicity
     they don't do that.  Instead, eastings are all relative to
     117 degrees West.
   There is no FALSE Easting - 117W is 0, Easting can go negative
     unlike real UTM
   Northern hemisphere only, so no FALSE Northing
     Northing is meters up from the equator just like real UTM in
     the northern hemisphere.
   The standard UTM 0.9996 scale factor is used
*/
void GDCtoUTMdb(
    double latDeg,   /* Degrees, north is positive, south is negative */
    double lonDeg,   /* Degrees, east is positive, west is negative */
    double *northing,
    double *easting)
{
    double lonOrigin = DTOR(-117.0);    /* Central meridian */
    double N, T, C, A, M;
    double sinLat, cosLat, tanLat;
    double lat = DTOR(latDeg);
    double lon = DTOR(lonDeg);

    sinLat = sin(lat);
    cosLat = cos(lat);
    tanLat = tan(lat);

    N = a / sqrt(1 - esqu * sinLat * sinLat);
    T = tanLat * tanLat;
    C = ePrimeSqu * cosLat * cosLat;
    A = cosLat * (lon - lonOrigin);

    M = a * (C1 * lat - C2 * sin(2 * lat) + C3 * sin(4 * lat) - C4 * sin(6 * lat));

    *easting = k0 * N *
               (A + (1 - T + C) * A * A * A / 6.
                + (5. - 18.*T + T * T + 72.*C - 58 * ePrimeSqu) * A * A * A * A * A / 120.);

    *northing = k0 *
                (M + N * tan(lat) * (A * A / 2. + (5. - T + 9 * C + 4 * C * C) * A * A * A * A / 24.
                                     + (61. - 58.*T + T * T + 600.*C - 330.*ePrimeSqu) *
                                     A * A * A * A * A * A / 720.));
}

void UTMtoGDC(
    const double northing,
    const double easting,
    const char *UTMZone,
    double *lat,
    double *lon)
{
    double e1 = (1 - sqrt(1 - esqu)) / (1 + sqrt(1 - esqu));
    double N1, T1, C1, R1, D, M;
    double lonOrigin;
    double mu, phi1, phi1Rad;
    double x, y;
    int ZoneNumber;
    char *ZoneLetter;
    int NorthernHemisphere; // 1 for northern hemispher, 0 for southern

    x = easting - 500000.0; // remove 500 km offset for longitude
    y = northing;

    ZoneNumber = strtoul(UTMZone, &ZoneLetter, 10);
    if ((*ZoneLetter - 'N') >= 0)
        NorthernHemisphere = 1;//point is in northern hemisphere
    else {
        NorthernHemisphere = 0;//point is in southern hemisphere
        //remove 10,000,000 meter offset used for southern hemisphere
        y -= 10000000.0;
    }

    //+3 puts origin in middle of zone
    lonOrigin = (ZoneNumber - 1) * 6 - 180 + 3;

    M = y / k0;
    mu = M / (a * (1 - esqu / 4 - 3 * esqu * esqu / 64 -
                   5 * esqu * esqu * esqu / 256));

    phi1Rad = mu + (3 * e1 / 2 - 27 * e1 * e1 * e1 / 32) * sin(2 * mu)
              + (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32) * sin(4 * mu)
              + (151 * e1 * e1 * e1 / 96) * sin(6 * mu);
    phi1 = RTOD(phi1Rad);

    N1 = a / sqrt(1 - esqu * sin(phi1Rad) * sin(phi1Rad));
    T1 = tan(phi1Rad) * tan(phi1Rad);
    C1 = ePrimeSqu * cos(phi1Rad) * cos(phi1Rad);
    R1 = a * (1 - esqu) / pow(1 - esqu * sin(phi1Rad) * sin(phi1Rad), 1.5);
    D = x / (N1 * k0);

    *lat = phi1Rad - (N1 * tan(phi1Rad) / R1) * (D * D / 2 - (5 + 3 * T1 + 10 * C1 - 4 * C1 * C1 - 9 * ePrimeSqu) * D * D * D * D / 24
                                                 + (61 + 90 * T1 + 298 * C1 + 45 * T1 * T1 - 252 * ePrimeSqu - 3 * C1 * C1) * D * D * D * D * D * D / 720);
    *lat = RTOD(*lat);

    *lon = (D - (1 + 2 * T1 + C1) * D * D * D / 6 + (5 - 2 * C1 + 28 * T1 - 3 * C1 * C1 + 8 * ePrimeSqu + 24 * T1 * T1)
            * D * D * D * D * D / 120) / cos(phi1Rad);
    *lon = lonOrigin + RTOD(*lon);

}

