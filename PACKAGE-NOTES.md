# CIGI HEMU protocol-version architecture package

This package is a Win32 Release build from the
`feature/cigi-version-architecture` branch.

## Run

Start `Hemu4.exe`. The application launches `HemuDrv.exe` automatically.
`DummyIG4.exe` is included for local CIGI 4 smoke testing.

The Setup dialog's CIGI Version list contains:

- 3.0/3.1
- 3.2
- 3.3
- 4.0

CIGI 4 packet I/O remains fully active. CIGI 3 selections are persisted, but
packet I/O safely stays on CIGI 4.0 until CIGI 3 packet definitions and the
corresponding protocol adapter are implemented.
