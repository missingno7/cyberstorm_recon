// CRC object ends at 0047f114; the following scaling blitters are hand-written assembly.
extern "C" void Crc16Byte(unsigned short *crc, unsigned char value)
{
    for (unsigned char mask = 0x80; mask; mask >>= 1) {
        unsigned short high = *crc & 0x8000;
        *crc <<= 1;
        *crc |= (unsigned char)((mask & value) != 0);
        if (high) *crc ^= 0x1021;
    }
}
extern "C" void Crc16(unsigned short *crc, const unsigned char *data, unsigned int count)
{
    if (data) {
        while (count--) {
            unsigned char value = *data++;
            for (unsigned char mask = 0x80; mask; mask >>= 1) {
                unsigned short high = *crc & 0x8000;
                *crc <<= 1;
                *crc |= (unsigned char)((value & mask) != 0);
                if (high) *crc ^= 0x1021;
            }
        }
    }
}
