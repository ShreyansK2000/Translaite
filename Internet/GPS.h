#ifndef GPS_H__
#define GPS_H__

#define RS232_ReceiverFifoOffset                (0x00010210)
#define RS232_TransmitterFifoOffset             (0x00010210)
#define RS232_InterruptEnableRegOffset          (0x00010212)
#define RS232_InterruptIdentificationRegOffset  (0x00010214)
#define RS232_FifoControlRegOffset              (0x00010214)
#define RS232_LineControlRegOffset              (0x00010216)
#define RS232_ModemControlRegOffset             (0x00010218)
#define RS232_LineStatusRegOffset               (0x0001021A)
#define RS232_ModemStatusRegOffset              (0x0001021C)
#define RS232_ScratchRegOffset                  (0x0001021E)
#define RS232_DivisorLatchLSBOffset             (0x00010210)
#define RS232_DivisorLatchMSBOffset             (0x00010212)

float get_longitude(void);
float get_latitude(void);
void collectData(void);
void Init_RS232(void);
void RS232Flush(void);
void init_gps(void);

#endif /* GPS_H__ */
