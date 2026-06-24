# CIGI protocol-version architecture plan

## Current assessment

The best CIGI 3 reference is `C:\Dev\ItWorks\CIGI\upstream\cigi-master`.
It contains `cigi3.h`, `cigi3.cpp`, `cigi3types.h`, `hemu3`, `HemuRtDrv`,
and `DummyIG3`. The other upstream trees inspected are CIGI 4 references.

The current application is still primarily CIGI 4. `cigi4.*` owns message
framing, byte swapping, packet replacement, and callback dispatch.
`HemuDrv/HemuDrv.cpp` owns network send/receive, callback registration,
driver-side packet construction, and callback-to-GUI queueing. The GUI/model
also constructs CIGI 4 raw packet buffers directly in packet dialogs and model
objects such as entities, components, articulated parts, and weather.

The branch already added useful architecture in:

- `CigiProtocolVersion.*`: central version model/catalog.
- `hemu4/SetupDlg.*`: version selection UI.
- `hemu4/globals.*` and `hemu4/Hemu4.cpp`: backward-compatible persistence and
  GUI-to-driver protocol-option messages.
- `CigiProtocolAdapter.*`: initial adapter/factory seam around driver message
  framing.

## Design options

1. Compile the old CIGI 3 HEMU beside CIGI 4 as mostly duplicated code.
   This is fast to bootstrap but hard to maintain and leaves two applications
   inside one process.

2. Add scattered `if CIGI 3 else CIGI 4` checks where packets are built and
   processed. This keeps files small initially but spreads protocol rules across
   UI, model, driver, and networking code.

3. Keep one scenario/UI model and isolate protocol-specific packet work behind
   a versioned adapter. The CIGI 4 adapter keeps current behavior. A future
   CIGI 3 adapter maps the shared model/actions to CIGI 3.0/3.1, 3.2, or 3.3
   packets and callbacks.

The recommended path is option 3. It gives one application and one scenario
model while keeping packet layout, opcode, message-framing, byte-order, and
callback differences inside protocol-owned code.

## Target boundary

The protocol boundary should own:

- session initialization and callback registration;
- message start/end, send-buffer access, receive-buffer processing, and byte
  swapping;
- driver-generated frame packets for entities, views, HAT/HOT, LOS, and
  transport-delay testing;
- queued/immediate packet dispatch from legacy raw packet buffers;
- incoming packet translation into the existing GUI packet queues or a future
  protocol-neutral event queue.

The UI and scenario model should eventually send semantic commands or shared
model updates. They should not need to know CIGI 3 vs CIGI 4 packet structs.

## Safe sequence

1. Keep current CIGI 4 behavior as the active implementation.
2. Move version-specific dispatch code out of `HemuDrv.cpp` and into the
   protocol adapter without changing packet bytes.
3. Move CIGI session initialization and callback registration behind the
   adapter.
4. Add a CIGI 3 API wrapper with renamed or namespaced symbols so `cigi3.*` and
   `cigi4.*` can coexist.
5. Port CIGI 3 callback decoding behind the adapter and translate responses
   into the current GUI queue format.
6. Introduce protocol-neutral builders for driver-generated packets, starting
   with entity/view control because they are the core frame path.
7. Convert immediate packet dialogs and script packet creation from raw CIGI 4
   buffers to semantic requests incrementally.
8. Enable real CIGI 3 packet I/O version by version: 3.2/3.3 first, then
   3.0/3.1 compatibility details.

## First implemented step

This branch now routes legacy queued-packet opcode dispatch through
`ICigiProtocolAdapter`. The CIGI 4 adapter contains the existing CIGI 4 dispatch
logic; `HemuDrv.cpp` only asks the adapter whether a queued buffer is a frame
boundary and whether the packet was handled. This is a no-format-change seam for
future CIGI 3 queue handling.

## Second implemented step

The driver host-session initialization seam is now adapter-owned.
`HemuDrv.cpp` builds a semantic `CigiHostCallbacks` table, then asks the active
`ICigiProtocolAdapter` to initialize the host session. The CIGI 4 adapter still
calls the same CIGI 4 API functions with the same startup values:

- `CigiInit(1, CIGI_VERSION)`
- `CigiCreateSession(CIGI_HOST_SESSION, 8, MAX_ETHERNET_PACKET_SIZE)`
- the existing CIGI 4 callback registrations for SOF, HAT/HOT, LOS, sensor,
  position, weather, collision, animation, event, and IG message packets

This keeps current CIGI 4 behavior as the default while making future CIGI 3
host-session callback registration protocol-specific.

## Open decisions

- Whether the combined app must support loading old `.sf3` scenario files or
  only CIGI 3 packet I/O from the current scenario model.
- Whether CIGI 3 immediate packet dialogs should be fully exposed, hidden until
  implemented, or mapped from existing CIGI 4-oriented dialogs.
- Whether CIGI 3.0 and 3.1 should remain one compatibility selection or become
  two explicit selections.
- How strict the app should be when a selected protocol cannot represent a
  current scenario feature.
- The GUI packet-watch/parser path in `hemu4/Hemu4.cpp` still initializes a
  CIGI 4 parser session and CIGI 4 callbacks directly. Moving that path should
  use a separate adapter-owned parser callback table so this driver host-session
  seam stays small and low risk.
