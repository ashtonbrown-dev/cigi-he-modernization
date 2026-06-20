// shared.h: interface for the shared class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHARED_H__E395927D_F381_11D3_81AD_006094B94A48__INCLUDED_)
#define AFX_SHARED_H__E395927D_F381_11D3_81AD_006094B94A48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "Body.h"
#include "gdcutm.h"
#include <stdlib.h>
void SinCos(float ANGLE, float *SINA, float *COSA);
void reset_fly();
void read_controls(Body_Struct *body);
void body_rates(Body_Struct *body);
void normalize(Body_Struct *body);
void bam_to_euler(Body_Struct *body, float *theta, float *phi, float *psi);
void euler_to_bam(float pitch, float roll, float heading, float bam[3][3]);
void init_position(double x, double y, float z, float pitch, float roll, float heading,  float aspd, Body_Struct *body);
void sendUMPMessage(Body_Struct *Body, int sensortoggle);


class CCPWayPointObject
{
public:
    CCPWayPointObject() {
        lat = 0.0;
        lon = 0.0;
        alt = 0.0;
        id = 0;
        next = NULL;
    };
    double lat, lon;
    float alt;
    int id;
    CCPWayPointObject *next;
};

class LatLon2XYConverter
{
public:
    virtual void latLon2xy(double lat, double lon, double *x, double *y) = 0;
};

class UTMConverter : public LatLon2XYConverter
{
public:
    virtual void latLon2xy(double lat, double lon, double *x, double *y) {
        int utmzone;
        double easting, northing;
        GDCtoUTM(lat, lon, &northing, &easting, &utmzone);
        *x = (float) easting;
        *y = (float) northing;
    };
};
class FlatEarthConverter : public LatLon2XYConverter
{
public:
    FlatEarthConverter(double zeroLat, double zeroLon) {
        //      static double M_PI = acos(-1.0);
        initLon = zeroLon;
        initLat = zeroLat;
        convergance = cos(initLat * M_PI / 180.0);
    }

    virtual void latLon2xy(double lat, double lon, double *x, double *y) {
        static double MetersPerDegree = 111120.0;

        *x = (lon - initLon) * MetersPerDegree * convergance;
        *y = (lat - initLat) * MetersPerDegree;
    };
protected:
    double initLon, initLat, convergance;
};

/*
class shared
{
public:
    shared();
    virtual ~shared();

};
*/
#endif // !defined(AFX_SHARED_H__E395927D_F381_11D3_81AD_006094B94A48__INCLUDED_)
