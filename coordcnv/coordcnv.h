/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Bob Murray
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * </pre>
 */

/* Coordinate Conversion routines */

#ifndef COORD_CNV_H
#define COORD_CNV_H

#  ifdef __cplusplus
extern "C" {
#  endif


    /*---------------------------------------------------------------------------*\
    *  Global definitions (#defines, typedefs, struct defines, global variables)
    \*---------------------------------------------------------------------------*/

    /* Earth Reference Model (ERM) constants.  The ERM ellipsoid is
       defined by equatorial radius (a) and flattening (f).  All other
       constants are derived from a and f. */
#define CC_WGS84_a (6378137.0)
#define CC_WGS84_f (1.0 / 298.257223563)

#define CC_GRS80_a (6378137.0)
#define CC_GRS80_f (1.0 / 298.257222101)

#define CC_CLARKE66_a (6378206.4)
#define CC_CLARKE66_f (1.0 - 6356583.8/6378206.4)

    /* North American Datum of 1927 (NAD 27) uses the Clarke 66 ellipsoid
     *   North American Datum of 1983 (NAD 83) uses the GRS 80 ellipsoid, which is
     *  virtually identical to the WGS 84 ellipsoid. */

    /** Standard UTM scale at central meridian */
#define CC_UTM_CENTRAL_SCALE (0.9996)


    /* The same pi as everyone else's pi */
#define CC_PI (3.14159265358979323846264338327)

    /* Degree/Radian conversion */
#define CC_DTOR(d) ((d)*(CC_PI) / 180.0)
#define CC_RTOD(r) ((r)*180.0 / (CC_PI))

    typedef struct {
        double a;     /* Semi-major axis (ellipsoid equatorial radius) */
        double f;     /* flattening f = (a-b)/a or f = 1-sqrt(1-esqu) */
        double b;     /* Semi-minor axis (ellipsoid polar radius) */
        double esqu;  /* eccentricity squared = 2f-f^2 */
        double e4th;  /* eccentricity ^ 4th */
        double e6th;  /* eccentricity ^ 6th */
        double e;     /* eccentricity = sqrt(esqu) */
        double e1;    /* (1-sqrt(1-esqu)) / (1+sqrt(1-esqu)), used in TM */
        double eprimesqu;    /* (a^2 - b^2) / b^2 */

        /* Transverse Mercator map parameters */
        double TMcentMeridian;     /* degrees */
        double TMfalseEasting;     /* offset of x=0 from central meridian, meters */
        double TMfalseNorthing;    /* offset of y=0 from equator */
        double TM_k0;              /* Scale at central meridian */
        double C1, C2, C3, C4;     /* Constants used in GDC to TM */

        /* TM grid declination, degrees */
        double TMgridDeclination;

    } EarthRefModelStruct;


    extern void initEarthRefModel(
        EarthRefModelStruct *erm,
        double a,   /* Equitorial radius, meters */
        double f);  /* Flattening */

    extern void initTM(
        EarthRefModelStruct *erm,
        double centralMeridian,    /* Central longitude of map, degrees */
        double falseEasting,       /* offset of x=0 from central meridian, meters */
        double falseNorthing,      /* offset of y=0 from equator */
        double k0);                /* Scale at central meridian */

    extern void convertGDCtoTM(
        EarthRefModelStruct *erm,
        const double LLA[3],  /* Input Lat, Lon, Alt, in degrees and meters */
        double xyup[3]);      /* Output x, y, up in meters */

    extern void convertTMtoGDC(
        EarthRefModelStruct *erm,
        const double xyup[3],      /* input in meters */
        double LLA[3]);            /* output in degrees, altitude in meters */

    extern double distAzimuth(
        EarthRefModelStruct *erm,
        double lat1,       /* First point, Lat/Lon in degrees */
        double lon1,
        double lat2,       /* The second point */
        double lon2,
        double *pAzimuth); /* Azimuth in degrees written here.
                         Distance in meters is returned. */
    extern void waypoint(
        EarthRefModelStruct *erm,
        double lat1,         /* Initial point, in degrees */
        double lon0,
        double dist,         /* Distance to waypoint, meters */
        double bearing,      /* Azimuth to waypoint, degrees from true North */
        double *pLat,        /* Resulting lat/lon in degrees */
        double *pLon);


#  ifdef __cplusplus
}
#  endif

#endif

/* End of File */
