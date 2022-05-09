
//TODO Work in Progress!!

//HElPER stuff

#include "i2c_refactored.h"

/* This variable allows us to test on hardware other than RPi.
// It prevents access to the kernel memory, and does not do any peripheral access
// Instead it prints out what it _would_ do if debug were 0
 */


/* I2C The time needed to transmit one byte. In microseconds.
 */
static int i2c_byte_wait_us = 0;

static volatile uint32_t bcm2835_bsc = NULL;


int bcm2835_i2c_setup(void* vaddr)
{
//TODO Allow for both I2C to be used with outside Define!
    uint16_t cdiv = 0;
    volatile uint32_t* paddr = NULL;

    bcm2837_gpio_init(vaddr);
    bcm2835_bsc = (uint32_t*)(vaddr + (I2C_BASE_ADDRESS - BCM2835_GPIO_BASE)); //TODO look up if correct
    
    /* Set the I2C/BSC pins to enable I2C access on them */
    bcm2835_gpio_fsel(SDAPIN, SDAMODE); /* SDA */
    bcm2835_gpio_fsel(SCLPIN, SCLMODE); /* SCL */ 
    paddr = bcm2835_bsc + BCM2835_BSC_DIV/4;
    /* Read the clock divider register */
    cdiv = bcm2835_peri_read(paddr);
    /* Calculate time for transmitting one byte
    // 1000000 = micros seconds in a second
    // 9 = Clocks per byte : 8 bits + ACK
    */
    i2c_byte_wait_us = ((float)cdiv / BCM2835_CORE_CLK_HZ) * 1000000 * 9;

    return 1;
}

void bcm2835_i2c_end(void)
{

//TODO allow generic define for GPIO with outside defines!
    bcm2835_gpio_fsel(SDAPIN, BCM2835_GPIO_FSEL_INPT); /* SDA */
    bcm2835_gpio_fsel(SCLPIN, BCM2835_GPIO_FSEL_INPT); /* SCL */

}


void bcm2835_i2c_setSlaveAddress(uint8_t addr)
{
    /* Set I2C Device Address */
    volatile uint32_t* paddr = bcm2835_bsc + BCM2835_BSC_A/4;
    bcm2835_peri_write(paddr, addr);
}

/* defaults to 0x5dc, should result in a 166.666 kHz I2C clock frequency.
// The divisor must be a power of 2. Odd numbers
// rounded down.
*/
void bcm2835_i2c_setClockDivider(uint16_t divider)
{
    volatile uint32_t* paddr = bcm2835_bsc + BCM2835_BSC_DIV/4;

    bcm2835_peri_write(paddr, divider);
    /* Calculate time for transmitting one byte
    // 1000000 = micros seconds in a second
    // 9 = Clocks per byte : 8 bits + ACK
    */
    i2c_byte_wait_us = ((float)divider / BCM2835_CORE_CLK_HZ) * 1000000 * 9;
}

/* set I2C clock divider by means of a baudrate number */
void bcm2835_i2c_set_baudrate(uint32_t baudrate)
{
	uint32_t divider;
	/* use 0xFFFE mask to limit a max value and round down any odd number */
	divider = (BCM2835_CORE_CLK_HZ / baudrate) & 0xFFFE;
	bcm2835_i2c_setClockDivider( (uint16_t)divider );
}

/* Writes an number of bytes to I2C */
uint8_t bcm2835_i2c_write(const char * buf, uint32_t len)
{
    volatile uint32_t* dlen    = bcm2835_bsc + BCM2835_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm2835_bsc + BCM2835_BSC_FIFO/4;
    volatile uint32_t* status  = bcm2835_bsc + BCM2835_BSC_S/4;
    volatile uint32_t* control = bcm2835_bsc + BCM2835_BSC_C/4;  

    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM2835_I2C_REASON_OK;

    /* Clear FIFO */
    bcm2835_peri_set_bits(control, BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );
    /* Clear Status */
    bcm2835_peri_write(status, BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
    /* Set Data Length */
    bcm2835_peri_write(dlen, len);
    /* pre populate FIFO with max buffer */
    while( remaining && ( i < BCM2835_BSC_FIFO_SIZE ) )
    {
        bcm2835_peri_write_nb(fifo, buf[i]);
        i++;
        remaining--;
    }
    
    /* Enable device and start transfer */
    bcm2835_peri_write(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);
    
    /* Transfer is over when BCM2835_BSC_S_DONE */
    while(!(bcm2835_peri_read(status) & BCM2835_BSC_S_DONE ))
    {
        while ( remaining && (bcm2835_peri_read(status) & BCM2835_BSC_S_TXD ))
    	{
	    /* Write to FIFO */
	    bcm2835_peri_write(fifo, buf[i]);
	    i++;
	    remaining--;
    	}
    }

    /* Received a NACK */
    if (bcm2835_peri_read(status) & BCM2835_BSC_S_ERR)
    {
	reason = BCM2835_I2C_REASON_ERROR_NACK;
    }

    /* Received Clock Stretch Timeout */
    else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
    {
	reason = BCM2835_I2C_REASON_ERROR_CLKT;
    }

    /* Not all data is sent */
    else if (remaining)
    {
	reason = BCM2835_I2C_REASON_ERROR_DATA;
    }

    bcm2835_peri_set_bits(control, BCM2835_BSC_S_DONE , BCM2835_BSC_S_DONE);

    return reason;
}

/* Read an number of bytes from I2C */
uint8_t bcm2835_i2c_read(char* buf, uint32_t len)
{
    volatile uint32_t* dlen    = bcm2835_bsc + BCM2835_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm2835_bsc + BCM2835_BSC_FIFO/4;
    volatile uint32_t* status  = bcm2835_bsc + BCM2835_BSC_S/4;
    volatile uint32_t* control = bcm2835_bsc + BCM2835_BSC_C/4;

    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM2835_I2C_REASON_OK;

    /* Clear FIFO */
    bcm2835_peri_set_bits(control, BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );
    /* Clear Status */
    bcm2835_peri_write_nb(status, BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
    /* Set Data Length */
    bcm2835_peri_write_nb(dlen, len);
    /* Start read */
    bcm2835_peri_write_nb(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST | BCM2835_BSC_C_READ);
    
    /* wait for transfer to complete */
    while (!(bcm2835_peri_read_nb(status) & BCM2835_BSC_S_DONE))
    {
        /* we must empty the FIFO as it is populated and not use any delay */
        while (remaining && bcm2835_peri_read_nb(status) & BCM2835_BSC_S_RXD)
    	{
	    /* Read from FIFO, no barrier */
	    buf[i] = bcm2835_peri_read_nb(fifo);
	    i++;
	    remaining--;
    	}
    }
    
    /* transfer has finished - grab any remaining stuff in FIFO */
    while (remaining && (bcm2835_peri_read_nb(status) & BCM2835_BSC_S_RXD))
    {
        /* Read from FIFO, no barrier */
        buf[i] = bcm2835_peri_read_nb(fifo);
        i++;
        remaining--;
    }
    
    /* Received a NACK */
    if (bcm2835_peri_read(status) & BCM2835_BSC_S_ERR)
    {
	reason = BCM2835_I2C_REASON_ERROR_NACK;
    }

    /* Received Clock Stretch Timeout */
    else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
    {
	reason = BCM2835_I2C_REASON_ERROR_CLKT;
    }

    /* Not all data is received */
    else if (remaining)
    {
	reason = BCM2835_I2C_REASON_ERROR_DATA;
    }

    bcm2835_peri_set_bits(status, BCM2835_BSC_S_DONE , BCM2835_BSC_S_DONE);

    return reason;
}

/* Read an number of bytes from I2C sending a repeated start after writing
// the required register. Only works if your device supports this mode
*/
uint8_t bcm2835_i2c_read_register_rs(char* regaddr, char* buf, uint32_t len)
{   
    volatile uint32_t* dlen    = bcm2835_bsc + BCM2835_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm2835_bsc + BCM2835_BSC_FIFO/4;
    volatile uint32_t* status  = bcm2835_bsc + BCM2835_BSC_S/4;
    volatile uint32_t* control = bcm2835_bsc + BCM2835_BSC_C/4;
	uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM2835_I2C_REASON_OK;
    
    /* Clear FIFO */
    bcm2835_peri_set_bits(control, BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );
    /* Clear Status */
    bcm2835_peri_write(status, BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
    /* Set Data Length */
    bcm2835_peri_write(dlen, 1);
    /* Enable device and start transfer */
    bcm2835_peri_write(control, BCM2835_BSC_C_I2CEN);
    bcm2835_peri_write(fifo, regaddr[0]);
    bcm2835_peri_write(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);
    
    /* poll for transfer has started */
    while ( !( bcm2835_peri_read(status) & BCM2835_BSC_S_TA ) )
    {
        /* Linux may cause us to miss entire transfer stage */
        if(bcm2835_peri_read(status) & BCM2835_BSC_S_DONE)
            break;
    }
    
    /* Send a repeated start with read bit set in address */
    bcm2835_peri_write(dlen, len);
    bcm2835_peri_write(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST  | BCM2835_BSC_C_READ );
    
    /* Wait for write to complete and first byte back. */
    bcm2835_delayMicroseconds(i2c_byte_wait_us * 3);
    
    /* wait for transfer to complete */
    while (!(bcm2835_peri_read(status) & BCM2835_BSC_S_DONE))
    {
        /* we must empty the FIFO as it is populated and not use any delay */
        while (remaining && bcm2835_peri_read(status) & BCM2835_BSC_S_RXD)
    	{
	    /* Read from FIFO */
	    buf[i] = bcm2835_peri_read(fifo);
	    i++;
	    remaining--;
    	}
    }
    
    /* transfer has finished - grab any remaining stuff in FIFO */
    while (remaining && (bcm2835_peri_read(status) & BCM2835_BSC_S_RXD))
    {
        /* Read from FIFO */
        buf[i] = bcm2835_peri_read(fifo);
        i++;
        remaining--;
    }
    
    /* Received a NACK */
    if (bcm2835_peri_read(status) & BCM2835_BSC_S_ERR)
    {
		reason = BCM2835_I2C_REASON_ERROR_NACK;
    }

    /* Received Clock Stretch Timeout */
    else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
    {
	reason = BCM2835_I2C_REASON_ERROR_CLKT;
    }

    /* Not all data is sent */
    else if (remaining)
    {
	reason = BCM2835_I2C_REASON_ERROR_DATA;
    }

    bcm2835_peri_set_bits(control, BCM2835_BSC_S_DONE , BCM2835_BSC_S_DONE);

    return reason;
}

/* Sending an arbitrary number of bytes before issuing a repeated start 
// (with no prior stop) and reading a response. Some devices require this behavior.
*/
uint8_t bcm2835_i2c_write_read_rs(char* cmds, uint32_t cmds_len, char* buf, uint32_t buf_len)
{   
    volatile uint32_t* dlen    = bcm2835_bsc + BCM2835_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm2835_bsc + BCM2835_BSC_FIFO/4;
    volatile uint32_t* status  = bcm2835_bsc + BCM2835_BSC_S/4;
    volatile uint32_t* control = bcm2835_bsc + BCM2835_BSC_C/4;
    uint32_t remaining = cmds_len;
    uint32_t i = 0;
    uint8_t reason = BCM2835_I2C_REASON_OK;
    
    /* Clear FIFO */
    bcm2835_peri_set_bits(control, BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );

    /* Clear Status */
    bcm2835_peri_write(status, BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);

    /* Set Data Length */
    bcm2835_peri_write(dlen, cmds_len);
 
    /* pre populate FIFO with max buffer */
    while( remaining && ( i < BCM2835_BSC_FIFO_SIZE ) )
    {
        bcm2835_peri_write_nb(fifo, cmds[i]);
        i++;
        remaining--;
    }

    /* Enable device and start transfer */
    bcm2835_peri_write(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);
    
    /* poll for transfer has started (way to do repeated start, from BCM2835 datasheet) */
    while ( !( bcm2835_peri_read(status) & BCM2835_BSC_S_TA ) )
    {
        /* Linux may cause us to miss entire transfer stage */
        if(bcm2835_peri_read_nb(status) & BCM2835_BSC_S_DONE)
            break;
    }
    
    remaining = buf_len;
    i = 0;

    /* Send a repeated start with read bit set in address */
    bcm2835_peri_write(dlen, buf_len);
    bcm2835_peri_write(control, BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST  | BCM2835_BSC_C_READ );
    
    /* Wait for write to complete and first byte back. */
    bcm2835_delayMicroseconds(i2c_byte_wait_us * (cmds_len + 1));
    
    /* wait for transfer to complete */
    while (!(bcm2835_peri_read_nb(status) & BCM2835_BSC_S_DONE))
    {
        /* we must empty the FIFO as it is populated and not use any delay */
        while (remaining && bcm2835_peri_read(status) & BCM2835_BSC_S_RXD)
    	{
	    /* Read from FIFO, no barrier */
	    buf[i] = bcm2835_peri_read_nb(fifo);
	    i++;
	    remaining--;
    	}
    }
    
    /* transfer has finished - grab any remaining stuff in FIFO */
    while (remaining && (bcm2835_peri_read(status) & BCM2835_BSC_S_RXD))
    {
        /* Read from FIFO */
        buf[i] = bcm2835_peri_read(fifo);
        i++;
        remaining--;
    }
    
    /* Received a NACK */
    if (bcm2835_peri_read(status) & BCM2835_BSC_S_ERR)
    {
	reason = BCM2835_I2C_REASON_ERROR_NACK;
    }

    /* Received Clock Stretch Timeout */
    else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
    {
	reason = BCM2835_I2C_REASON_ERROR_CLKT;
    }

    /* Not all data is sent */
    else if (remaining)
    {
	reason = BCM2835_I2C_REASON_ERROR_DATA;
    }

    bcm2835_peri_set_bits(control, BCM2835_BSC_S_DONE , BCM2835_BSC_S_DONE);

    return reason;
}