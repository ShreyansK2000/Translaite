#ifndef GPS_H__
#define GPS_H__

#define GPS_ReceiverFifoOffset                (0x00010210)
#define GPS_TransmitterFifoOffset             (0x00010210)
#define GPS_InterruptEnableRegOffset          (0x00010212)
#define GPS_InterruptIdentificationRegOffset  (0x00010214)
#define GPS_FifoControlRegOffset              (0x00010214)
#define GPS_LineControlRegOffset              (0x00010216)
#define GPS_ModemControlRegOffset             (0x00010218)
#define GPS_LineStatusRegOffset               (0x0001021A)
#define GPS_ModemStatusRegOffset              (0x0001021C)
#define GPS_ScratchRegOffset                  (0x0001021E)
#define GPS_DivisorLatchLSBOffset             (0x00010210)
#define GPS_DivisorLatchMSBOffset             (0x00010212)

/**
 * Subroutine to initialise the gps serial port by writing some data
 * to the internal registers.
 *
 * Call this function at the start of the program before you attempt
 * to read or write to data via the gps serial port.
 *
 * Refer to UART data sheet for details of registers and programming.
 */
int init_gps(void);

/**
 * Remove/flush the UART receiver buffer by removing any unread characters.
 */
void flush_gps(void);

/**
 * Returns the latest read longitude from the GPS module.
 */
float get_longitude(void);

/**
 * Returns the latest read latitude from the GPS module.
 */
float get_latitude(void);

#endif /* GPS_H__ */
