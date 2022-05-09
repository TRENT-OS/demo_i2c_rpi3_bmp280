/*
* HENDSOLDT Cyber GmbH
*/

#define RPi3
#define I2C1

/*! Base Address of the BSC0 registers */
#define BCM2835_BSC0_BASE 				0x205000
/*! Base Address of the BSC1 registers */
#define BCM2835_BSC1_BASE				0x804000

#ifdef RPi3
#ifdef I2C1
/*SDA*/
#define SDAPIN RPI_GPIO_P1_03
#define SDAMODE BCM2835_GPIO_FSEL_ALT0
#define SCLPIN RPI_GPIO_P1_05
#define SCLMODE BCM2835_GPIO_FSEL_ALT0
#define I2C_BASE_ADDRESS BCM2835_BSC0_BASE  
/*SDL*/
#elif I2C2
#define SDAPIN RPI_V2_GPIO_P1_03
#define SDAMODE BCM2835_GPIO_FSEL_ALT0
#define SCLPIN RPI_GPIO_P1_05
#define SCLMODE BCM2835_GPIO_FSEL_ALT0
#define I2C_BASE_ADDRESS BCM2835_BSC1_BASE
#endif
#endif

/* Defines for I2C
   GPIO register offsets from BCM2835_BSC*_BASE.
   Offsets into the BSC Peripheral block in bytes per 3.1 BSC Register Map
*/
#define BCM2835_BSC_C 			0x0000 /*!< BSC Master Control */
#define BCM2835_BSC_S 			0x0004 /*!< BSC Master Status */
#define BCM2835_BSC_DLEN		0x0008 /*!< BSC Master Data Length */
#define BCM2835_BSC_A 			0x000c /*!< BSC Master Slave Address */
#define BCM2835_BSC_FIFO		0x0010 /*!< BSC Master Data FIFO */
#define BCM2835_BSC_DIV			0x0014 /*!< BSC Master Clock Divider */
#define BCM2835_BSC_DEL			0x0018 /*!< BSC Master Data Delay */
#define BCM2835_BSC_CLKT		0x001c /*!< BSC Master Clock Stretch Timeout */

/* Register masks for BSC_C */
#define BCM2835_BSC_C_I2CEN 	0x00008000 /*!< I2C Enable, 0 = disabled, 1 = enabled */
#define BCM2835_BSC_C_INTR 		0x00000400 /*!< Interrupt on RX */
#define BCM2835_BSC_C_INTT 		0x00000200 /*!< Interrupt on TX */
#define BCM2835_BSC_C_INTD 		0x00000100 /*!< Interrupt on DONE */
#define BCM2835_BSC_C_ST 		0x00000080 /*!< Start transfer, 1 = Start a new transfer */
#define BCM2835_BSC_C_CLEAR_1 	0x00000020 /*!< Clear FIFO Clear */
#define BCM2835_BSC_C_CLEAR_2 	0x00000010 /*!< Clear FIFO Clear */
#define BCM2835_BSC_C_READ 		0x00000001 /*!<	Read transfer */

/* Register masks for BSC_S */
#define BCM2835_BSC_S_CLKT 		0x00000200 /*!< Clock stretch timeout */
#define BCM2835_BSC_S_ERR 		0x00000100 /*!< ACK error */
#define BCM2835_BSC_S_RXF 		0x00000080 /*!< RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full */
#define BCM2835_BSC_S_TXE 		0x00000040 /*!< TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full */
#define BCM2835_BSC_S_RXD 		0x00000020 /*!< RXD FIFO contains data */
#define BCM2835_BSC_S_TXD 		0x00000010 /*!< TXD FIFO can accept data */
#define BCM2835_BSC_S_RXR 		0x00000008 /*!< RXR FIFO needs reading (full) */
#define BCM2835_BSC_S_TXW 		0x00000004 /*!< TXW FIFO needs writing (full) */
#define BCM2835_BSC_S_DONE 		0x00000002 /*!< Transfer DONE */
#define BCM2835_BSC_S_TA 		0x00000001 /*!< Transfer Active */

#define BCM2835_BSC_FIFO_SIZE   	16 /*!< BSC FIFO size */

/*! \brief bcm2835I2CClockDivider
  Specifies the divider used to generate the I2C clock from the system clock.
  Clock divided is based on nominal base clock rate of 250MHz
*/
typedef enum
{
    BCM2835_I2C_CLOCK_DIVIDER_2500   = 2500,      /*!< 2500 = 10us = 100 kHz */
    BCM2835_I2C_CLOCK_DIVIDER_626    = 626,       /*!< 622 = 2.504us = 399.3610 kHz */
    BCM2835_I2C_CLOCK_DIVIDER_150    = 150,       /*!< 150 = 60ns = 1.666 MHz (default at reset) */
    BCM2835_I2C_CLOCK_DIVIDER_148    = 148        /*!< 148 = 59ns = 1.689 MHz */
} bcm2835I2CClockDivider;

/*! \brief bcm2835I2CReasonCodes
  Specifies the reason codes for the bcm2835_i2c_write and bcm2835_i2c_read functions.
*/
typedef enum
{
    BCM2835_I2C_REASON_OK   	     = 0x00,      /*!< Success */
    BCM2835_I2C_REASON_ERROR_NACK    = 0x01,      /*!< Received a NACK */
    BCM2835_I2C_REASON_ERROR_CLKT    = 0x02,      /*!< Received Clock Stretch Timeout */
    BCM2835_I2C_REASON_ERROR_DATA    = 0x04       /*!< Not all data is sent / received */
} bcm2835I2CReasonCodes;