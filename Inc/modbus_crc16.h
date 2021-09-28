/** Modbus CRC16 generator. 
 */

#ifndef MODBUS_CRC16_H
#define MODBUS_CRC16_H

#include <stdint.h>
#include <stddef.h>

/** Calculate Modbus CRC16. 
 */

uint16_t modbus_crc16(void* buf, size_t bytes_length, uint16_t prev_crc16 = 0xffff);

#endif
