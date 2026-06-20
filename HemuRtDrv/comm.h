

HANDLE OpenCommPort(const unsigned int portnum);
void CloseCommPort(HANDLE hPort);
void WriteByteToCommPort(HANDLE hPort, unsigned char value);
