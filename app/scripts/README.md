# CIGI HEMU startup script examples

These small scripts provide quick demo/playground starting points for CIGI
HEMU. They create active aircraft, configure their initial positions and
speeds, switch them to Fly mode, and start the simulation.

## Running a script

1. Start HEMU and open its existing Script/Test dialog.
2. Browse to `app/scripts` and select one of the `.scp` files.
3. Start the script using the dialog's run control.

`ownship_startup.scp` starts only entity 0, the Ownship.
`squadron_startup.scp` starts entity 0 plus wingmen 1 through 3 in a small
formation.

Both scripts end with `RUN 0`, which keeps the simulation running until the
user freezes or stops it.

The examples use aircraft type 118. In this repository's active default entity
definitions (`app/config/default/Entities.def`), type 118 is the fixed-wing
`f18c`.

## Editing tips

- Change `ENTITY_POS` to choose the Ownship's starting latitude, longitude,
  and altitude.
- Change `ENTITY_YPR` to set heading (yaw), pitch, and roll.
- Change `ENTITY_AIRSPEED` to set speed in knots.
- Change `ENTITY_POS_RELATIVE` to adjust each wingman's bearing and range from
  the Ownship.
