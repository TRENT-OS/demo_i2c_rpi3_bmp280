
#include "lib_debug/Debug.h"
#include "OS_Dataport.h"

#include <stdint.h>
#include <string.h>

#include "i2c_lib.h"

#include <camkes.h>


OS_Dataport_t port_storage = OS_DATAPORT_ASSIGN(i2c_port);

I2C_Error_t i2c_write(int dev, size_t len, size_t *written, const uint8_t* buf)
{
    return i2c_write_reg(dev, -1, len, written, buf);
}

I2C_Error_t i2c_write_reg(int dev, int reg, size_t len, size_t *written, const uint8_t* buf)
{
    uint8_t* i2c_buf = OS_Dataport_getBuf(port_storage);
    size_t i2c_buf_size = OS_Dataport_getSize(port_storage);
    size_t i2c_buf_offset = 0;
    *written = 0;

    // On the I2C bus the LSB bit is the read/write flag. We expect
    // the last bit to be a zero
    if((dev & 1) != 0)
    {
        Debug_LOG_ERROR("i2c_write_reg() expected an even address");
        return I2C_ERROR_INVALID_PARAMETER;
    }
    
    // Test if Address is bigger than 10 Bits. This is unsupported
    if((dev  >> 11) != 0)
    {
        Debug_LOG_ERROR("i2c_write_reg() Address can not be bigger than 10 bits");
        return I2C_ERROR_INVALID_PARAMETER;
    }
   
    //See if Address is more than 7 bits
 /*   if((dev >> 8) != 0)
    {
        i2c_buf[i2c_buf_offset] = (dev >> 1) & 0xff;
        i2c_buf_offset++;
        dev = ((dev >> 8) & 6 ) | 0b11110000;
    }*/
    
    // I2C Protocoll specifies the first byte after the address can be an
    // internal register offset
    if(reg != -1)
    {
        i2c_buf[i2c_buf_offset] = (uint8_t) reg;
        i2c_buf_offset++;
    }
    
    // Check if port Storage is still big enough for i2c data
    if(len > (i2c_buf_size - i2c_buf_offset))
    {
        Debug_LOG_ERROR("i2c_write_reg() write buffer not big enough");
        return I2C_ERROR_INVALID_PARAMETER;
    }
  /*  for(int i = 0; i < len; i++)
    {
        i2c_buf[i+i2c_buf_offset] = buf[i];
    }*/

    memcpy(&(i2c_buf[i2c_buf_offset]),buf, len);
    I2C_Error_t ret = i2c_rpc_write(dev, len + i2c_buf_offset, written);
    if (ret != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("i2c_write_reg() returned error %d", ret);
    }
    *written = *written - i2c_buf_offset;
    return ret;
}

I2C_Error_t i2c_read(int dev, size_t len, size_t *read, uint8_t* buf)
{
    uint8_t* i2c_buf = OS_Dataport_getBuf(port_storage);
    size_t i2c_buf_size = OS_Dataport_getSize(port_storage);
    *read = 0;

    // On the I2C bus the LSB bit is the read/write flag. We expect
    // the last bit to be a zero
    if((dev & 1) != 0)
    {
        Debug_LOG_ERROR("i2c_read_reg() expected an even address");
        return I2C_ERROR_INVALID_PARAMETER;
    }
    
    // Test if Address is bigger than 10 Bits. This is unsupported
    if((dev  >> 11) != 0)
    {
        Debug_LOG_ERROR("i2c_read_reg() Address can not be bigger than 10 bits");
        return I2C_ERROR_INVALID_PARAMETER;
    }

    if(len > i2c_buf_size)
    {
        Debug_LOG_ERROR("i2c_read_reg() write buffer not big enough");
        return I2C_ERROR_INVALID_PARAMETER;
    }

    I2C_Error_t ret = i2c_rpc_read(dev, len, read);
    if( ret != I2C_SUCCESS)
    {
        Debug_LOG_ERROR("I2C_read_reg() returned %d", ret);
        *read = 0;
        return ret;
    }
    if(*read != len)
    {
        Debug_LOG_ERROR("I2C_read_reg() returned length %d not equal to requested %d", *read, len);
    }
    // Even if error occured, read is set if data was read out.
/*    for(int i = 0; i < len; i++)
    {
        buf[i] = i2c_buf[i];
    }*/
    memcpy(buf, i2c_buf, len);
    return ret;
}