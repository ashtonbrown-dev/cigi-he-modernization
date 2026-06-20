/** <pre>
 *  Copyright 2004 The Boeing Company
 *  Author: Lance Durham
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

#include "StdAfx.h"
#include "coordcnv.h"
#include "Fly.h"
#include "RTXSharedBufferQ.h"
#include "SharedEntityObj.h"
#include "hemumsg.h"

extern CSharedEntityObj *FindEntity(const int id, unsigned long *handle);

int UpdateEntityPosition(ENTITY *entity, double dt)
{
    if (entity->noncigi.flymode == ENTITY_FLYMODE_FLY) {
        switch (entity->noncigi.entity_class) {
        case ENTITY_CLASS_FIXEDWING:
        case ENTITY_CLASS_TANK:
        case ENTITY_CLASS_CAR:
            if (entity->unsaved.fly_waypoints)
                return FlyWaypoint(entity, dt);
            else
                return FlyFixedWing(entity, dt);

            break;

        case ENTITY_CLASS_ROTORCRAFT:
            if (entity->unsaved.fly_waypoints)
                return FlyWaypoint(entity, dt);
            else
                return FlyRotor(entity, dt);

            break;

        case ENTITY_CLASS_MISSILE:
            return TrackTarget(entity, dt);
            break;

        default:
            return Glide(entity, dt);
            break;
        }
    } else
        return Glide(entity, dt);
}

// Returns 1 if the missile has been modified.
int TrackTarget(ENTITY *missile, double dt)
{
    static double velocity;
    static double yaw, pitch;
    static double sin_yaw, sin_pitch;
    static double cos_yaw, cos_pitch;
    static double dh, dp, dh_max, dp_max;
    static double dx1, dy1, dz1;
    static double dx2, dy2, dz2;
    static double i, j, k;
    double target_x = 0.0;
    double target_y = 0.0;
    double missile_x = 0.0;
    double missile_y = 0.0;
    static CSharedEntityObj *missile_obj = NULL;
    static CSharedEntityObj *target_obj = NULL;
    static ENTITY target;
    static DOF pos_and_att;

    if ((missile->cigi.parent_id >= 0) || !(missile->unsaved.track_target))
        return 0;

    // Allow the missile some ramp-up time before tracking the target.
    if (missile->noncigi.speed < missile->unsaved.speed_final) {
        // Set the current velocity based on our time index.
        missile->noncigi.speed += (missile->unsaved.ramp_up_ctr * missile->unsaved.ramp_up_ctr);

        if (missile->noncigi.speed > missile->unsaved.speed_final)
            missile->noncigi.speed = missile->unsaved.speed_final;

        (missile->unsaved.ramp_up_ctr)++;

        // Allow the missile some ramp-up time before tracking the target.
        if (missile->unsaved.ramp_up_ctr < 10)
            return 1;
    }

    unsigned long target_hdl = NULL;
    target_obj = FindEntity(missile->noncigi.target_id, &target_hdl);
    if (!target_obj) {
        missile->unsaved.track_target = 0;
        return 1;
    }

    // Save the current angles to use in the calculations.
    pos_and_att = missile->cigi.dofs;
    yaw = pos_and_att.yaw;
    pitch = pos_and_att.pitch;

    sin_yaw = sin(DegToRad(yaw));
    sin_pitch = sin(DegToRad(pitch));
    cos_yaw = cos(DegToRad(yaw));
    cos_pitch = cos(DegToRad(pitch));

    while (target_obj->GetAndLock(&target) == FALSE)
        continue;
    target_obj->Unlock();

    GDCtoTM(target.cigi.dofs.latitude, target.cigi.dofs.longitude,
            &target_y, &target_x);
    GDCtoTM(pos_and_att.latitude, pos_and_att.longitude, &missile_y, &missile_x);

    dx1 = target_x - missile_x;
    dy1 = target_y - missile_y;
    dz1 = pos_and_att.altitude - target.cigi.dofs.altitude; // +Z is down

    i = (dy1 * cos_yaw * cos_pitch) + (dx1 * sin_yaw * cos_pitch) + (dz1 * sin_pitch);
    j = (dx1 * cos_yaw) - (dy1 * sin_yaw);
    k = (dy1 * cos_yaw * sin_pitch) + (dx1 * sin_yaw * sin_pitch) - (dz1 * cos_pitch);

    // Calculate the yaw.
    if (i == 0.0f)
        dh = (j < 0.0f ? -90.0 : 90.0);
    else
        dh = RadToDeg(atan2(j, i));

    // Assume a maximum turn rate of 30 degrees per second
    dh_max = 30.0f * dt;
    if (fabs(dh) > dh_max)
        dh = (dh < 0 ? -dh_max : dh_max);

    pos_and_att.yaw += dh;

    // We can assume roll = 0.

    // Calculate the pitch.
    double edp = RadToDeg(asin(-dz1 / (sqrt(dx1 * dx1 + dy1 * dy1 + dz1 * dz1))));
    dp = edp - pitch;

    // Assume a maximum pitch rate of 30 degrees per second
    dp_max = 30.0f * dt;
    if (fabs(dp) > dp_max)
        dp = (dp < 0 ? -dp_max : dp_max);

    pos_and_att.pitch += dp;

    // Adjust for pitching beyond vertical.
    if (pos_and_att.pitch > 90.0f) {
        pos_and_att.pitch = 180.0f - pos_and_att.pitch;
        pos_and_att.yaw += 180.0f;
        //pos_and_att.roll += 180.0f;
    } else if (pos_and_att.pitch < -90.0f) {
        pos_and_att.pitch = -180.0f - pos_and_att.pitch;
        pos_and_att.yaw += 180.0f;
        //pos_and_att.roll += 180.0f;
    }

    if (pos_and_att.yaw > 180.0f)
        pos_and_att.yaw -= 360.0f;
    else if (pos_and_att.yaw <= -180)
        pos_and_att.yaw += 360.0f;

    // Test for a hit.
    velocity = missile->noncigi.speed * dt;

    // Optimization to eliminate one call to sqrt():
    if ((i < velocity) && (sqrt(j * j + k * k) < 10.0f)) {
        // Update the position.
        pos_and_att.latitude = target.cigi.dofs.latitude;
        pos_and_att.longitude = target.cigi.dofs.longitude;
        pos_and_att.altitude = target.cigi.dofs.altitude;

        // Turn off tracking (stops flying) so we don't get multiple hits.
        missile->unsaved.track_target = 0;

        // Hide the missile.
        missile->cigi.alpha = 0;

        // Notify the Win32 process of the hit.
        MESSAGE_MISSILE_HIT msg;
        msg.missile_id = missile->cigi.id;
        SendGuiMessage(&msg);
    } else {
        // Update the position.
        double dxy2 = velocity * cos(DegToRad(pos_and_att.pitch));
        dx2 = dxy2 * sin(DegToRad(pos_and_att.yaw));
        dy2 = dxy2 * cos(DegToRad(pos_and_att.yaw));
        dz2 = velocity * sin(DegToRad(pos_and_att.pitch));
        missile_x += dx2;
        missile_y += dy2;
        pos_and_att.altitude += dz2;
        TMtoGDC(missile_y, missile_x, &(pos_and_att.latitude), &(pos_and_att.longitude));
    }

    // Now update the position and attitude after all have been calculated.
    missile->cigi.dofs = pos_and_att;

    return 1;
}

// Returns 1 if the entity has been modified.
int FlyWaypoint(ENTITY *entity, double dt)
{
    static double velocity, speed;
    static double utm_x, utm_y;
    static double yaw, pitch, roll;
    static double sin_yaw, sin_pitch;
    static double cos_yaw, cos_pitch;
    static double dh, dp, dr, dh_max, dp_max, dr_max;
    static double dx1, dy1, dz1;
    static double dx2, dy2, dz2;
    static double i, j, k;
    static CSharedEntityObj *target_obj = NULL;
    static DOF pos_and_att;

    int haschanged = 0;

    if (entity->cigi.parent_id >= 0)
        return 0;

    // If we have flown through the waypoint and we haven't gotten the next
    // one, fly straight.
    if (!(entity->unsaved.waypoint_valid)) {
        switch (entity->noncigi.entity_class) {
        case ENTITY_CLASS_FIXEDWING:
        case ENTITY_CLASS_TANK:
        case ENTITY_CLASS_CAR:
            return FlyFixedWing(entity, dt);
            break;

        case ENTITY_CLASS_ROTORCRAFT:
            return FlyRotor(entity, dt);
            break;

        default:
            return FlyFixedWing(entity, dt);
            break;
        }
    }

    // Save the current angles to use in the calculations.
    pos_and_att = entity->cigi.dofs;
    yaw = pos_and_att.yaw;
    pitch = pos_and_att.pitch;
    roll = pos_and_att.roll;

    speed = entity->noncigi.speed;

    sin_yaw = sin(DegToRad(yaw));
    sin_pitch = sin(DegToRad(pitch));
    cos_yaw = cos(DegToRad(yaw));
    cos_pitch = cos(DegToRad(pitch));

    GDCtoTM(pos_and_att.latitude, pos_and_att.longitude, &utm_y, &utm_x);

    dx1 = entity->unsaved.waypoint_x - utm_x;
    dy1 = entity->unsaved.waypoint_y - utm_y;
    dz1 = pos_and_att.altitude - entity->unsaved.waypoint_z;        // +Z is down

    // (i, j, k) for ENU coordinate system:
    i = (dy1 * cos_yaw * cos_pitch) + (dx1 * sin_yaw * cos_pitch) + (dz1 * sin_pitch);
    j = (dx1 * cos_yaw) - (dy1 * sin_yaw);
    k = (dy1 * cos_yaw * sin_pitch) + (dx1 * sin_yaw * sin_pitch) - (dz1 * cos_pitch);

    // This version would be for NED:
    //      i = (dx1 * cos_yaw * cos_pitch) + (dy1 * sin_yaw * cos_pitch) - (dz1 * sin_pitch);
    //      j = (dy1 * cos_yaw) - (dx1 * sin_yaw);
    //      k = (dx1 * cos_yaw * sin_pitch)   + (dy1 * sin_yaw * sin_pitch) + (dz1 * cos_pitch);

    // Calculate the pitch first.
    //*********************************************************
    double edp = RadToDeg(asin(-dz1 / (sqrt(dx1 * dx1 + dy1 * dy1 + dz1 * dz1))));
    dp = edp - pitch;

    double ddp = dp;
    double ten_dps = 10.0f * dt;

    dp_max = pow(2.0f, entity->unsaved.pitch_ctr * 0.05f) - 1.0f;

    // Assume a maximum pitch rate of 10 degrees per second.
    if (dp_max > ten_dps) {
        if (fabs(ddp) < 10.0f)
            dp_max = fabs(ddp) * dt;
        else
            dp_max = ten_dps;
    }

    if (fabs(ddp) < 0.00001)
        entity->unsaved.pitch_ctr = 0;

    if (fabs(ddp) > dp_max) {
        ddp = (ddp < 0 ? -dp_max : dp_max);
        entity->unsaved.pitch_ctr++;
    }

    if (((ddp < 0) && (dp < 0) && (ddp < dp)) || ((ddp > 0) && (dp > 0) && (ddp > dp)))
        ddp = dp;

    pos_and_att.pitch += ddp;


    // Once the correct pitch is acquired, perform the turn.
    if (ddp < 0.2f) {
        // Calculate the yaw.
        //*********************************************************
        if (i == 0.0f)
            dh = (j < 0.0f ? -90.0 : 90.0);
        else
            dh = RadToDeg(atan2(j, i));

        double ddh = dh;

        dh_max = pow(2.0f, entity->unsaved.yaw_ctr * 0.05f) - 1.0f;

        // Limit to turn rate specified in script.
        if (dh_max > entity->unsaved.turn_rate * dt) {
            if (fabs(ddh) < 10.0f)
                dh_max = (fabs(ddh) * entity->unsaved.turn_rate / 10.0f) * dt;
            else
                dh_max = entity->unsaved.turn_rate * dt;
        }

        if (fabs(ddh) < 0.00001)
            entity->unsaved.yaw_ctr = 0;

        if (fabs(ddh) > dh_max) {
            ddh = (ddh < 0.0f ? -dh_max : dh_max);
            entity->unsaved.yaw_ctr++;
        }

        pos_and_att.yaw = yaw + ddh;

        // Calculate the instantaneous roll.
        //*********************************************************
        if (!entity->unsaved.roll_disable) {
            double cr = cos(DegToRad(-ddh));
            double sr = sin(DegToRad(-ddh));
            double rCr = 1.0f - cr;
            double ddx = (speed * dt) / sr;
            double ddy = -ddx * rCr;        // turn radius
            double a = 0.0f;
            if (fabs(ddh) > 0.0000000001)
                a = 2.0f * ddy / (dt * dt);

            dr = RadToDeg(atan(a / 9.81f));

            double ddr = dr - roll;
            dr_max = pow(2.0f, entity->unsaved.roll_ctr * 0.05f) - 1.0f;
            double thirty_dps = 30.0f * dt;
            if (dr_max > thirty_dps) {
                if (fabs(ddr) < 10.0f)
                    dr_max = fabs(ddr) / 10.0f;
                else
                    dr_max = thirty_dps;
            }

            if (fabs(ddr) < 0.00001)
                entity->unsaved.roll_ctr = 0;

            if (fabs(ddr) > dr_max) {
                ddr = (ddr < 0.0f ? -dr_max : dr_max);
                entity->unsaved.roll_ctr++;
            }

            pos_and_att.roll = roll + ddr;
        }
    }
    //*********************************************************

    // Adjust for pitching beyond vertical.
    if (pos_and_att.pitch > 90.0f) {
        pos_and_att.pitch = 180.0f - pos_and_att.pitch;
        pos_and_att.yaw += 180.0f;
        pos_and_att.roll += 180.0f;
    } else if (pos_and_att.pitch < -90.0f) {
        pos_and_att.pitch = -180.0f - pos_and_att.pitch;
        pos_and_att.yaw += 180.0f;
        pos_and_att.roll += 180.0f;
    }

    if (pos_and_att.yaw > 180.0f)
        pos_and_att.yaw -= 360.0f;
    else if (pos_and_att.yaw <= -180)
        pos_and_att.yaw += 360.0f;

    // Calculate the airspeed.  The GUI ensures accel >= 0.
    double final_speed = entity->unsaved.speed_final;
    if (speed != final_speed) {
        if (speed < final_speed) {
            speed += entity->unsaved.accel * dt;
            if (speed > final_speed)
                speed = final_speed;
        } else if (speed > entity->unsaved.speed_final) {
            speed -= entity->unsaved.accel * dt;
            if (speed < final_speed)
                speed = final_speed;
        }

        if (entity->noncigi.speed != speed) {
            entity->noncigi.speed = speed;
            haschanged = 1;
        }
    }

    // Test for a hit.  Also register a hit if our airspeed is zero.
    velocity = entity->noncigi.speed * dt;
    if (((i < velocity) && (sqrt(j * j + k * k) < 10.0f)) || (speed == 0)) {
        entity->unsaved.waypoint_reached = 1;
        entity->unsaved.waypoint_valid = 0;

        MESSAGE_WAYPOINT_REACHED msg_reached;
        msg_reached.entity_id = entity->cigi.id;
        msg_reached.waypoint_id = entity->unsaved.waypoint_id;
        SendGuiMessage(&msg_reached);
    } else {
        // Update the position.
        double dxy2 = velocity * cos(DegToRad(pos_and_att.pitch));
        dx2 = dxy2 * sin(DegToRad(pos_and_att.yaw));
        dy2 = dxy2 * cos(DegToRad(pos_and_att.yaw));
        dz2 = velocity * sin(DegToRad(pos_and_att.pitch));
        utm_x += dx2;
        utm_y += dy2;
        TMtoGDC(utm_y, utm_x, &(pos_and_att.latitude), &(pos_and_att.longitude));
        pos_and_att.altitude += dz2;
    }

    // Now update the position and attitude after all have been calculated.
    if ((speed != 0.0) || (dp != 0.0) || (dr != 0.0) || (dh != 0.0)) {
        entity->cigi.dofs = pos_and_att;

        haschanged = 1;
    }

    return haschanged;
}

// Return 1 if the entity has been modified.
int FlyFixedWing(ENTITY *entity, double dt)
{
    static double yaw, pitch, roll;
    static double sin_roll, cos_pitch, cos_roll;
    static RATES rates;
    static DOF pos_and_att;

    if (entity->cigi.parent_id >= 0)
        return 0;

    // Get the position, attitude, and rate information so we don't have
    // to touch it repeatedly.  We will set the final values in one step
    // later.
    rates = entity->noncigi.rates;
    pos_and_att = entity->cigi.dofs;

    // Save the current angles to use in the calculations.
    yaw = pos_and_att.yaw;
    pitch = pos_and_att.pitch;
    roll = pos_and_att.roll;

    sin_roll = sin(DegToRad(roll));
    cos_pitch = cos(DegToRad(pitch));
    cos_roll = cos(DegToRad(roll));

    if (entity->cigi.clamp_mode == ENTITY_CLAMP_NONE) { // If not taxiing (clamped to ground).
        // Calculate the yaw.
        pos_and_att.yaw += (rates.dpitch * sin_roll * dt) +
                           ((rates.dyaw * dt) * cos_roll * cos_pitch);
        if (pos_and_att.yaw > 180.0f)
            pos_and_att.yaw -= 360.0f;
        else if (pos_and_att.yaw <= -180)
            pos_and_att.yaw += 360.0f;

        // Calculate the roll.
        pos_and_att.roll += rates.droll * dt;
        if (pos_and_att.roll > 180.0f)
            pos_and_att.roll -= 360.0f;
        else if (pos_and_att.roll <= -180.0f)
            pos_and_att.roll += 360.0f;

        // Calculate the pitch.
        pos_and_att.pitch += (rates.dpitch * cos(DegToRad(roll)) * dt) +
                             ((rates.dyaw * dt) * (1 - cos_roll * cos_pitch));
        if (pos_and_att.pitch > 90.0f) {
            pos_and_att.pitch = 180.0f - pos_and_att.pitch;
            pos_and_att.yaw += 180.0f;
            pos_and_att.roll += 180.0f;
        } else if (pos_and_att.pitch < -90.0f) {
            pos_and_att.pitch = -180.0f - pos_and_att.pitch;
            pos_and_att.yaw += 180.0f;
            pos_and_att.roll += 180.0f;
        }

        // Fudge the yaw based on roll to simulate lift.
        pos_and_att.yaw += sin(DegToRad(pos_and_att.roll)) * (10.0f * dt);

        // Adjust the yaw again to range from 0 to <360 degrees.
        if (pos_and_att.yaw > 180.0f)
            pos_and_att.yaw -= 360.0f;
        else if (pos_and_att.yaw <= -180)
            pos_and_att.yaw += 360.0f;

        // Adjust the roll again to range from >-180 to <=180 degrees.
        if (pos_and_att.roll > 180.0f)
            pos_and_att.roll -= 360.0f;
        else if (pos_and_att.roll <= -180.0f)
            pos_and_att.roll += 360.0f;
    } else {
        // If we are clamped to the ground, we want to be level.
        pos_and_att.pitch = 0;
        pos_and_att.roll = 0;

        // Calculate the yaw.
        pos_and_att.yaw += rates.dyaw * dt;
        if (pos_and_att.yaw > 180.0f)
            pos_and_att.yaw -= 360.0f;
        else if (pos_and_att.yaw <= -180)
            pos_and_att.yaw += 360.0f;
    }

    // Update the position.  Note that we are only updating the altitude if
    // we are not clamping to the ground in response to a HOT response.
    double lat, lon;
    double distance = entity->noncigi.speed * dt;
    if (distance != 0.0) {
        waypoint(&g_ERM, pos_and_att.latitude, pos_and_att.longitude,
                 distance, pos_and_att.yaw, &lat, &lon);
        pos_and_att.latitude = lat;
        pos_and_att.longitude = lon;

        if (entity->cigi.clamp_mode == ENTITY_CLAMP_NONE)
            pos_and_att.altitude += distance * sin(DegToRad(pos_and_att.pitch));
    }

    // Now set the position and attitude in one step.
    if ((distance != 0.0) || (rates.dyaw != 0.0) || (rates.dpitch != 0.0)
        || (rates.droll != 0.0)) {
        if (memcmp(&pos_and_att, &(entity->cigi.dofs), sizeof(DOF)) != 0) {
            entity->cigi.dofs = pos_and_att;
            return 1;
        }
    }

    return 0;
}

// Returns 1 if the entity has been modified.
int Glide(ENTITY *entity, double dt)
{
    static RATES rates;
    static DOF pos_and_att;

    if (entity->cigi.parent_id >= 0)
        return 0;

    // Get the position, attitude, and rate information so we don't have
    // to touch it repeatedly.  We will set the final values in one step
    // later.
    rates = entity->noncigi.rates;
    pos_and_att = entity->cigi.dofs;

    // Calculate the yaw.
    pos_and_att.yaw += rates.dyaw * dt;

    // Calculate the roll.
    pos_and_att.roll += rates.droll * dt;

    // Calculate the pitch and adjust to range from -90 to +90 degrees.
    pos_and_att.pitch += rates.dpitch * dt;
    if (pos_and_att.pitch > 90.0f) {
        pos_and_att.pitch = 180.0f - pos_and_att.pitch;
        pos_and_att.yaw += 180.0f;
        pos_and_att.roll += 180.0f;
    } else if (pos_and_att.pitch < -90.0f) {
        pos_and_att.pitch = -180.0f - pos_and_att.pitch;
        pos_and_att.yaw += 180.0f;
        pos_and_att.roll += 180.0f;
    }

    // Adjust the yaw to range from 0 to <360 degrees.
    if (pos_and_att.yaw > 180.0f)
        pos_and_att.yaw -= 360.0f;
    else if (pos_and_att.yaw <= -180)
        pos_and_att.yaw += 360.0f;

    // Adjust the roll to range from >-180 to <=180 degrees.
    if (pos_and_att.roll > 180.0f)
        pos_and_att.roll -= 360.0f;
    else if (pos_and_att.roll <= -180.0f)
        pos_and_att.roll += 360.0f;

    // Update the coordinates and altitude.
    if ((rates.dx != 0.0) || (rates.dy != 0.0) || (rates.dz != 0.0)
        || (rates.dyaw != 0.0) || (rates.dpitch != 0.0) || (rates.droll != 0.0)) {
        double range = sqrt(rates.dx * rates.dx + rates.dy * rates.dy) * dt;
        double bearing;
        if (rates.dx > 0.0)
            bearing = RadToDeg(atan(rates.dy / rates.dx));
        else if (rates.dx < 0.0)
            bearing = 180.0 + RadToDeg(atan(rates.dy / rates.dx));
        else {
            if (rates.dy > 0.0)
                bearing = 90.0f;
            else
                bearing = 270.0f;
        }

        GetRelativePos(pos_and_att.latitude, pos_and_att.longitude,
                       0, bearing, range,
                       &pos_and_att.latitude, &pos_and_att.longitude);
        pos_and_att.altitude += rates.dz * dt;

        // Now set the position and attitude in one step.
        if (memcmp(&pos_and_att, &(entity->cigi.dofs), sizeof(DOF)) != 0) {
            entity->cigi.dofs = pos_and_att;
            return 1;
        }
    }

    return 0;
}

// Returns 1 if the entity has been modified.
int FlyRotor(ENTITY *entity, double dt)
{
    static double velocity;
    static double yaw, pitch, roll;
    static double sin_roll, cos_pitch, cos_roll;
    static RATES rates;
    static DOF pos_and_att;

    if (entity->cigi.parent_id >= 0)
        return 0;

    // Get the position, attitude, and rate information so we don't have
    // to touch it repeatedly.  We will set the final values in one step
    // later.
    rates = entity->noncigi.rates;
    pos_and_att = entity->cigi.dofs;

    // Save the current angles to use in the calculations.
    yaw = pos_and_att.yaw;
    pitch = pos_and_att.pitch;
    roll = pos_and_att.roll;

    sin_roll = sin(DegToRad(roll));
    cos_pitch = cos(DegToRad(pitch));
    cos_roll = cos(DegToRad(roll));

    //////////////////////////////////////////////////////////

    // Calculate the yaw.
    double ddy = entity->unsaved.ddy;
    double dyaw = entity->noncigi.rates.dyaw;

    if (fabs(dyaw) < 0.0001f) {
        if (ddy > dyaw) {
            ddy -= 100.0f;      // Tweak these numbers to adjust yaw rate.

            if (ddy < dyaw)
                ddy = dyaw;
        } else if (ddy < dyaw) {
            ddy += 100.0f;

            if (ddy > dyaw)
                ddy = dyaw;
        } else
            ddy = 0.0f;
    } else if (dyaw < 0) {
        if (ddy > dyaw) {
            ddy -= 100.0f;
            if (ddy < dyaw)
                ddy = dyaw;
        }
    } else {
        if (ddy < dyaw) {
            ddy += 100.0f;
            if (ddy > dyaw)
                ddy = dyaw;
        }
    }

    entity->unsaved.ddy = ddy;
    pos_and_att.yaw += ddy * dt;

    if (pos_and_att.yaw > 180.0f)
        pos_and_att.yaw -= 360.0f;
    else if (pos_and_att.yaw <= -180.0f)
        pos_and_att.yaw += 360.0f;

    //////////////////////////////////////////////////////////

    // Calculate the roll.
    double phi_max = rates.dx;
    double dr = rates.droll;    //entity->ext.dr;

    if (fabs(pos_and_att.roll - phi_max) < 0.0001f) { // phi == phi_max
        rates.droll = 0.0f;
    } else {
        double delta_roll = phi_max - pos_and_att.roll;

        if (delta_roll < 0) {
            dr -= 400.0f;       // Tweak to adjust roll.

            double min_val = -15.0f;
            if (min_val < delta_roll)
                min_val = delta_roll;
            if (dr < min_val)
                dr = min_val;
        } else {
            dr += 400.0f;       // Tweak to adjust roll.

            double max_val = 15.0f;
            if (max_val > delta_roll)
                max_val = delta_roll;
            if (dr > max_val)
                dr = max_val;
        }

        rates.droll = dr;
        pos_and_att.roll += rates.droll * dt;
    }

    ////////////////////////////////////////////////////////

    // Calculate the pitch.
    double theta_max = -rates.dy;
    double dp = rates.dpitch;

    if (fabs(pos_and_att.pitch - theta_max) < 0.0001f) { // theta == theta_max
        rates.dpitch = 0.0f;
    } else {
        double delta_pitch = theta_max - pos_and_att.pitch;

        if (delta_pitch < 0) {
            dp -= 400.0f;       // Tweak to adjust pitch.

            double min_val = -15.0f;
            if (min_val < delta_pitch)
                min_val = delta_pitch;
            if (dp < min_val)
                dp = min_val;
        } else {
            dp += 400.0f;       // Tweak to adjust pitch.

            double max_val = 15.0f;
            if (max_val > delta_pitch)
                max_val = delta_pitch;
            if (dp > max_val)
                dp = max_val;
        }

        rates.dpitch = dp;
        pos_and_att.pitch += rates.dpitch * dt;
    }

    ////////////////////////////////////////////////////////

    // Set the airspeed.
    const double accel = 0.5f;
    double sin_new_pitch = sin(DegToRad(pitch));
    double sin_new_roll = sin(DegToRad(roll));
    double vi_max = -sin_new_pitch * KnotsToMPS(200);   // Max 100 knots (100 / sin(30) = 200).
    double vj_max = sin_new_roll * KnotsToMPS(88);      // Max 30 knots (30 / sin(20) = 88)
    double vi = entity->unsaved.vi;
    double vj = entity->unsaved.vj;
    double delta_vi = vi_max - vi;
    double delta_vj = vj_max - vj;
    double a = entity->noncigi.collective * accel / 50;
    double aix = a * -sin_new_pitch;
    double ajx = a * sin_new_roll;
    double ay = a * cos(DegToRad(-pitch)) * cos(DegToRad(roll)) - accel;

    vi += aix;
    if (vi > 0) {
        if (vi > vi_max) {
            vi = vi_max;    // * 0.4
        }
    } else {
        if (vi < vi_max) {
            vi = vi_max;
        }
    }

    vj += ajx;
    if (vj > 0) {
        if (vj > vj_max) {
            vj = vj_max;
        }
    } else {
        if (vj < vj_max) {
            vj = vj_max;
        }
    }

    entity->unsaved.vi = vi;
    entity->unsaved.vj = vj;

    velocity = sqrt(vi * vi + vj * vj);

    vi *= dt;
    vj *= dt;

    /////////////////////////////////////////////////////////

    // Update the coordinates and altitude.

    // Calculate the total dx and dy from vi and vj.  Do this BEFORE we
    // change the yaw.
    double yaw_deg = DegToRad(pos_and_att.yaw);
    double dx = (vi * sin(yaw_deg)) + (vj * cos(yaw_deg));
    double dy = (vi * cos(yaw_deg)) - (vj * sin(yaw_deg));

    // Update the position.
    double utmx = 0.0;
    double utmy = 0.0;
    GDCtoTM(pos_and_att.latitude, pos_and_att.longitude, &utmy, &utmx);
    TMtoGDC(utmy + dy, utmx + dx, &(pos_and_att.latitude), &(pos_and_att.longitude));

    // Fudge the yaw based on roll to simulate lift.
    pos_and_att.yaw += sin(DegToRad(pos_and_att.roll)) * vi;

    // Note that we are only updating the altitude if we are not clamping to
    // the ground in response to a HOT response.
    if (entity->cigi.clamp_mode == ENTITY_CLAMP_NONE)
        pos_and_att.altitude -= (50.0f - entity->noncigi.collective) * fabs(ay) * dt;

    // Now set the position and attitude in one step.
    if ((velocity != 0.0) ||
        (memcmp(&pos_and_att, &(entity->cigi.dofs), sizeof(DOF)) != 0) ||
        (memcmp(&rates, &(entity->noncigi.rates), sizeof(RATES)) != 0)) {
        entity->cigi.dofs = pos_and_att;
        entity->noncigi.rates = rates;
        entity->noncigi.speed = MPSToKnots(velocity);

        return 1;
    } else {
        return 0;
    }
}