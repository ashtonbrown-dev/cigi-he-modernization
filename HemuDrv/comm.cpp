#include "windows.h"
#include "stdio.h"

HANDLE OpenCommPort(const unsigned int portnum)
{
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

}

void CloseCommPort(HANDLE hPort)
{
    if (hPort)
        ::CloseHandle(hPort);
}

void WriteByteToCommPort(HANDLE hPort, unsigned char value)
{
    if (!hPort)
        return;

    TransmitCommChar(hPort, (char)value);
}
