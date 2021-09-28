/** Modbus CRC16.
 */

#include "modbus_crc16.h"

uint16_t modbus_crc16(void* buf, size_t bytes_length, uint16_t prev_crc) {
  uint16_t crc = 0xFFFF;
  
  for (size_t i = 0; i < bytes_length; i++) {
      crc ^= (uint16_t)((uint16_t*)buf)[i];          // XOR byte into least sig. byte of crc
  
    for (int j = 8; j != 0; j--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return ((crc >> 8) & 0x0f) | ((crc << 8) & 0xf0);  
}
