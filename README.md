# CIGI Host Emulator Modernization

This repository starts with a clean baseline import of the public CIGI Host
Emulator source. The working goal is to preserve the original behavior while
modernizing the project for current Windows development tools.

## Upstream

- Project: CIGI Host Emulator
- Source: `https://svn.code.sf.net/p/cigi/code/trunk/he`
- Imported area: SourceForge SVN `trunk/he`

The untouched downloaded references are maintained outside this Git repository.

## Build and runtime layout

The Visual Studio solution contains three Win32 applications and supports only
`Debug` and `Release` configurations:

- `Hemu4`: the MFC host emulator GUI
- `HemuDrv`: the Windows driver process
- `DummyIG4`: a companion image-generator simulator

Both configurations deploy runnable files to `app`. Runtime definitions are
kept in `app/config/default`, beside the deployed applications but separate
from source and compiler output.

When a Debug build of Hemu4 is started under the Visual Studio debugger, Hemu4
also starts `DummyIG4.exe` with the local-loopback defaults before launching
HemuDrv.

## License

The imported source is distributed under the GNU General Public License,
version 2. See [license.txt](license.txt) for the original license text.
