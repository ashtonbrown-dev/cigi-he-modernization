#include "windows.h"

#ifndef NO_RTX
#include "rtapi.h"
#else
#include "stdio.h"
#endif

HANDLE OpenCommPort(const unsigned int portnum)
{
#ifndef NO_RTX
    // Figure out the port address from the port number.
    int portaddr = 0;

    switch (portnum) {
    case 1:
        portaddr = 0x03F8;
        break;

    case 2:
        portaddr = 0x02F8;
        break;

    case 3:
        portaddr = 0x03E8;
        break;

    case 4:
        portaddr = 0x02E8;
        break;

    default:
        return NULL;
    }

    if (RtEnablePortIo((unsigned char *)portaddr, 1))
        return (HANDLE)portaddr;
    else
        return NULL;

#else
    // Generate the port name from the port number.
    HANDLE hdl = NULL;
    char portname[16] = "";
    sprintf(portname, "COM%d", portnum);

    printf("COMM port name = %s\n", portname);

    hdl = ::CreateFile(portname, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hdl == INVALID_HANDLE_VALUE) {
        int error = GetLastError();
        printf("error = %d\n", error);
        return NULL;
    } else {
        return hdl;
    }

#endif
}

void CloseCommPort(HANDLE hPort)
{
#ifndef NO_RTX
    // Do nothing under RTX.
#else
    // Just close the file.
    ::CloseHandle(hPort);
#endif
}

void WriteByteToCommPort(HANDLE hPort, unsigned char value)
{
    if (!hPort)
        return;

#ifndef NO_RTX
    RtWritePortUchar((PUCHAR)hPort, value);
#else
    TransmitCommChar(hPort, (char)value);
#endif
}
