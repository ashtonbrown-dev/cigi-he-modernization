/*---------------------------------------------------------------------------*\
*  This module contains functions to convert between Geodetic (Lat/Lon)
*  and Transverse Mercator coordinates.
*
*  Additional functions are included to calcualte distance and
*  bearing between two points on the Earth.  The first gives distance
*  and azimuth (bearing) between two lat/lon points.  The second gives
*  the lat/lon of a 'waypoint' which is a given distance and bearing
*  from an initial lat/lon point.
*
*  Copyright (c) 2004, The Boeing Company.  All rights reserved.
*
*  AUTHOR: Bob Murray
\*---------------------------------------------------------------------------*/


#include <math.h>
#include <string.h>
#include "coordcnv.h"


/*---------------------------------------------------------------------------*\
*  FUNCTION NAME: initEarthRefModel
*  DESCRIPTION:   Sets up the Earth ellipsoid parameters
\*---------------------------------------------------------------------------*/
void initEarthRefModel(
   EarthRefModelStruct *erm,
   double a,   /* Equitorial radius, meters */
   double f)   /* Flattening */
{
   memset(erm, 0, sizeof(EarthRefModelStruct));

   erm->a = a;
   erm->f = f;
   erm->b = a * (1.0 - f);
   erm->esqu = 2*f - f*f;
   erm->e4th = erm->esqu * erm->esqu;
   erm->e6th = erm->e4th * erm->esqu;
   erm->e    = sqrt(erm->esqu);
   erm->e1   = (1-sqrt(1-erm->esqu))/(1+sqrt(1-erm->esqu));
   erm->eprimesqu = ((a*a - erm->b*erm->b) / (erm->b*erm->b));
}


/*---------------------------------------------------------------------------*\
*  FUNCTION NAME: initTM
*  DESCRIPTION:
*   Sets up constants needed for GDC to TM conversion.
\*---------------------------------------------------------------------------*/
void initTM(
   EarthRefModelStruct *erm,
   double centralMeridian,    /* Central longitude of map, degrees */
   double falseEasting,       /* offset of x=0 from central meridian, meters */
   double falseNorthing,      /* offset of y=0 from equator */
   double k0)                 /* Scale at central meridian */
{
   /* Normalize the input central longitude */
   while (centralMeridian > 180.)
      centralMeridian -= 360.;
   while (centralMeridian < -180.)
      centralMeridian += 360.;
   erm->TMcentMeridian = centralMeridian;

   erm->TMfalseEasting = falseEasting;
   erm->TMfalseNorthing = falseNorthing;
   erm->TM_k0 = k0;
   erm->TMgridDeclination = 0.0;

   erm->C1 = 1. - erm->esqu/4. - erm->e4th * 3./64. - erm->e6th * 5./256.;
   erm->C2 = erm->esqu * 3./8. + erm->e4th * 3./32. + erm->e6th * 45./1024.;
   erm->C3 = erm->e4th * 15./256. + erm->e6th * 45./1024.;
   erm->C4 = erm->e6th * 35./3072.;
}


/*---------------------------------------------------------------------------*\
*  FUNCTION NAME: convertGDCtoTM
*  DESCRIPTION:
*   Converts Geodetic Lat/Lon/Altitude to Transverse Mercator x-y-up
*   All units in must be in degrees and meters
*   Input LLA array and output xyup array can be co-located in memory
*
*   Writes Grid Declination to the ERM structure
\*---------------------------------------------------------------------------*/
void convertGDCtoTM(
   EarthRefModelStruct *erm,
   const double LLA[3],  /* Input Lat, Lon, Alt, in degrees and meters */
   double xyup[3])       /* Output x, y, up in meters */
{
   double lat = CC_DTOR(LLA[0]);
   double lon = CC_DTOR(LLA[1]);
   double N, T, C, A, M;
   double sinLat = sin(lat);
   double cosLat = cos(lat);
   double tanLat = sinLat / cosLat;

   N = erm->a / sqrt(1 - erm->esqu * sinLat * sinLat);
   T = tanLat * tanLat;
   C = erm->eprimesqu * cosLat * cosLat;
   A = cosLat * (lon - CC_DTOR(erm->TMcentMeridian));
   M = erm->a * (erm->C1*lat - erm->C2*sin(2*lat) + erm->C3*sin(4*lat) -
                erm->C4*sin(6*lat));

   /* Easting (x) */
   xyup[0] =  erm->TMfalseEasting +
      erm->TM_k0 * N * (A + (1-T+C)*A*A*A/6. +
       (5. - 18.*T + T*T + 72.*C - 58*erm->eprimesqu) * A*A*A*A*A/120.);

   /* Northing (y) */
   xyup[1] = erm->TMfalseNorthing +
      erm->TM_k0 * (M + N*tanLat * (A*A/2. + (5. - T + 9*C + 4*C*C) *
          A*A*A*A/24. + (61. - 58.*T + T*T + 600.*C - 330.*erm->eprimesqu) *
          A*A*A*A*A*A/720.));

   xyup[2] = LLA[2];    /* Altitude */

   /* North correction angle, in degrees */
   erm->TMgridDeclination = sinLat * (LLA[1] - erm->TMcentMeridian);
}


/*---------------------------------------------------------------------------*\
*  FUNCTION NAME: convertTMtoGDC
*  DESCRIPTION:
*   Converts Transverse Mercator x-y-up to Geodetic Lat/Lon/Altitude
*   All units must be in degrees and meters
*   Input xyup array and output LLA array can be co-located in memory
*
*   Writes Grid Declination to the ERM structure
\*---------------------------------------------------------------------------*/
void convertTMtoGDC(
   EarthRefModelStruct *erm,
   const double xyup[3],      /* input in meters */
   double LLA[3])             /* output in degrees, altitude in meters */
{
   double e1sq = erm->e1*erm->e1;
   double e1_3rd = e1sq * erm->e1;
   double e1_4th = e1sq * e1sq;

   double N1, T1, T1sq, C1, C1sq, R1, D, Dsq, D4th, D6th, M;
   double mu, phi1, sinPhi1, cosPhi1, tanPhi1;
   double x, y;

   x = xyup[0] - erm->TMfalseEasting;
   y = xyup[1] - erm->TMfalseNorthing;

   M = y / erm->TM_k0;
   mu = M/(erm->a*(1 - erm->esqu/4 - 3*erm->e4th/64 - 5*erm->e6th/256));

   phi1 = mu + (   3.*erm->e1 /   2. - 27.*e1_3rd/32.) * sin(2*mu) 
      + (  21.*e1sq   /  16. - 55.*e1_4th/32.) * sin(4*mu)
      + ( 151.*e1_3rd /  96.) * sin(6*mu)
      + (1097.*e1_4th / 512.) * sin(8*mu);
   sinPhi1 = sin(phi1);
   cosPhi1 = cos(phi1);
   tanPhi1 = sinPhi1 / cosPhi1;

   N1 = erm->a / sqrt(1 - erm->esqu * sinPhi1 * sinPhi1);
   T1 = tanPhi1 * tanPhi1;
   T1sq = T1 * T1;
   C1 = erm->eprimesqu * cosPhi1 * cosPhi1;
   C1sq = C1 * C1;
   R1 = erm->a * (1 - erm->esqu) /
      pow(1 - erm->esqu * sinPhi1 * sinPhi1, 1.5);
   D = x / (N1*erm->TM_k0);
   Dsq = D * D;
   D4th = Dsq * Dsq;
   D6th = D4th * Dsq;

   LLA[0] = CC_RTOD(phi1 - (N1 * tanPhi1 / R1) *
      (Dsq/2. - (5. + 3.*T1 + 10.*C1 - 4.*C1sq - 9.*erm->eprimesqu)*D4th/24. +
       (61. + 90.*T1 + 298.*C1 + 45.*T1sq - 252.*erm->eprimesqu - 3.*C1sq) *
       D6th/720.));

   LLA[1] = erm->TMcentMeridian +
      CC_RTOD((D - (1. + 2.*T1 + C1)*Dsq*D/6 +
       (5. - 2.*C1 + 28.*T1 - 3.*C1sq + 8.*erm->eprimesqu + 24.*T1sq) *
       D4th*D/120.)/cosPhi1);

   LLA[2] = xyup[2];

   /* To avoid an extra sine function call, Grid Declination is
      calculated using the "footprint" latitude (phi1) instead of the
      final latitude.  The worst-case error at 3 degrees off the
      central meridian is only 0.0016 degrees (occurs between 30 and 40
      degrees latitude). */
   erm->TMgridDeclination = sinPhi1 * (LLA[1] - erm->TMcentMeridian);
}


/*---------------------------------------------------------------------------*\
*  FUNCTION NAME: distAzimuth
*  DESCRIPTION:  Returns distance and azimuth between to Lat/Lon points.
*
*   Algorithm used is the forward solution for an Azumuthal
*   Equidistance map projections, developed for a map of Micronesia.
*   From "Map Projections, a Working Manual", U.S. Geological Survey
*   Professional Paper 1395, by John P. Snyder. (Pg. 199)  This algorithm is
*   referred to as "Clarke's best formula".  It is considered usable up
*   to 800 km.
\*---------------------------------------------------------------------------*/
double distAzimuth(
   EarthRefModelStruct *erm,
   double lat1,       /* First point, Lat/Lon in degrees */
   double lon1,
   double lat2,       /* The second point */
   double lon2,
   double *pAzimuth)  /* Azimuth in degrees written here.
                         Distance in meters is returned. */
{
   double sinLat, cosLat, tanLat;
   double sinLat1, cosLat1;
   double N, N1;
   double Az, sinAz, cosAz, Psi;
   double sinLonMinusLon0, cosLonMinusLon0;
   double sinPsi, cosPsi, tanPsi;

   double s, sSq, H, Hsq, G, c;

   /* In map terminology, lat1 is the center latitude, lon0 is the
      center longitude, and lat/lon with no subscript is the point in
      question (here, the waypoint, or lat2/lon2) */
   sinLat1 = sin(CC_DTOR(lat1));
   cosLat1 = cos(CC_DTOR(lat1));

   sinLat = sin(CC_DTOR(lat2));
   cosLat = cos(CC_DTOR(lat2));
   tanLat = sinLat/cosLat;

   sinLonMinusLon0 = sin(CC_DTOR(lon2 - lon1));
   cosLonMinusLon0 = cos(CC_DTOR(lon2 - lon1));

   N1 = erm->a / sqrt(1 - erm->esqu * sinLat1 * sinLat1);
   N = erm->a / sqrt(1 - erm->esqu * sinLat * sinLat);

   Psi = atan((1 - erm->esqu)*tanLat +
              erm->esqu * N1 * sinLat1 / (N * cosLat));
   sinPsi = sin(Psi);
   cosPsi = cos(Psi);
   tanPsi = sinPsi/cosPsi;

   Az = atan2(sinLonMinusLon0, cosLat1 * tanPsi - sinLat1 * cosLonMinusLon0);
   sinAz = sin(Az);
   cosAz = cos(Az);

   /* Note, "if (sinAz == 0.0)" doesn't work.  sinAz very close to zero
      gives the wrong result.  1e-12 was found experimentally to be a
      good threshold. */
   if (fabs(sinAz)< 1.e-12) {
      s = asin(cosLat1 * sinPsi - sinLat1 * cosPsi);
      if (cosAz < 0.0) {
         s = -s;
      }
   }
   else {
      s = asin(sinLonMinusLon0 * cosPsi/sinAz);
   }
   sSq = s*s;

   G = erm->e * sinLat1 / sqrt(1 - erm->esqu);

   H = erm->e * cosLat1 * cosAz / sqrt(1 - erm->esqu);
   Hsq = H*H; 

   /* c is the distance between the two points */
   c = N1 * s*(1. - sSq*Hsq*(1 - Hsq)/6. + (sSq*s/8.)*G*H*(1. - 2.*Hsq) +
               (sSq*sSq/120.) * (Hsq * (4. - 7.*Hsq) - 3*G*G*(1. - 7.*Hsq)) -
               (sSq*sSq*s/48.)*G*H);

   if (pAzimuth != 0) {
      *pAzimuth = CC_RTOD(Az);
   }

   return c;
}


/*---------------------------------------------------------------------------*\
*  FUNCTION NAME: waypoint
*  DESCRIPTION:
*   Algorithm used is the inverse formula for Azumuthal Equidistance
*   map projections.  From "Map Projections, a Working Manual",
*   U.S. Geological Survey Professional Paper 1395, by John P. Snyder. (Pg. 202)
*   This algorithm is referred to as "Robbins' inverse".  It is
*   considered usable up to 800 km.
*   The algorithm uses the c and Az values from pg. 199 as directed by the document,
*   since we are calculating latitude and longitude. (It says to "delete" equations
*   25-32 and 25-33.)
*   Note: Does not work for waypoint longitudes more than 90 degrees
*    from initial point.
\*---------------------------------------------------------------------------*/
void waypoint(
   EarthRefModelStruct *erm,
   double lat1,         /* Initial point, in degrees (phi1 and lambda0) */
   double lon0,
   double dist,         /* Distance to waypoint, meters */
   double bearing,      /* Azimuth to waypoint, degrees from true North */
   double *pLat,        /* Resulting lat/lon in degrees */
   double *pLon)
{
   double N1, A, B, D, E, F;
   double Psi, sinPsi, cosPsi, tanPsi;
   double sinLat1, cosLat1;
   double sinAz, cosAz;

   sinLat1 = sin(CC_DTOR(lat1));
   cosLat1 = cos(CC_DTOR(lat1));

   sinAz = sin(CC_DTOR(bearing));
   cosAz = cos(CC_DTOR(bearing));

   N1 = erm->a / sqrt(1 - erm->esqu * sinLat1 * sinLat1);

   A = -erm->esqu * cosLat1*cosLat1 * cosAz*cosAz / (1 - erm->esqu);
   B = 3. * erm->esqu * (1-A) * sinLat1 * cosLat1 * cosAz / (1 - erm->esqu);
   D = dist / N1;
   E = D - A * (1+A) * D*D*D / 6. - B * (1. + 3.*A) * D*D*D*D/24.;
   F = 1 - A * E*E / 2. - B * E*E*E / 6.;
   Psi = asin(sinLat1 * cos(E) + cosLat1 * sin(E) * cosAz);
   sinPsi = sin(Psi);
   cosPsi = cos(Psi);
   tanPsi = sinPsi/cosPsi;
   *pLon = lon0 + CC_RTOD(asin(sinAz * sin(E) / cosPsi));
   *pLat = CC_RTOD(atan((1 - erm->esqu * F * sinLat1 / sinPsi) *
                tanPsi / (1 - erm->esqu)));
}
