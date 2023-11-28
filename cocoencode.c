/**
 * @brief TRS-80 Color Computer FSK encoder
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license LICENSE.MD
 * @verbose This encoder outputs raw 8-bit unsigned audio at 11025Hz
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define SILENCE_BYTE 0x40                             /* Silence at zero crossing amplitude */
#define LEADER_SECS 1                                 /* # of seconds of silence */
#define SAMPLE_RATE 11025                             /* Sample rate */
#define HZ_MARK 2400                                  /* Mark Frequency in Hz */
#define HZ_SPACE 1200                                 /* Space Frequency */
#define SIGN_BIAS 128                                 /* DC offset to change to unsigned 8-bit output */
#define SYNC_BYTE 0x3C                                /* Sync byte */
#define NAME_REC_LEN 18                               /* Len of name record */

/**
 * @brief sine table for space tone (1200Hz) at 11025Hz
 */
const uint8_t space_sin[] =
  {
    0x80,0xda,0xff,0xda,0x80,0x25,0x0,0x25,0x80,
  };

/**
 * @brief sine table for mark tone (2400Hz) at 11025Hz
 */
const uint8_t mark_sin[] =
  {
    0x80,0xff,0x80,0x0,0x80,
  };

/**
 * @brief write requested frequency for one bit cell
 * @param freq_hz the requested frequency in Hz
 */
void write_baud(uint16_t freq_hz)
{
  uint8_t c = 0;
  const uint8_t *p = NULL;
  
  switch(freq_hz)
    {
    case HZ_SPACE:
      c = sizeof(space_sin);
      p = space_sin;
      break;
    case HZ_MARK:
      c = sizeof(mark_sin);
      p = mark_sin;
      break;
    }

  for (uint8_t i=0; i<c; i++)
    putchar(p[i]);
}

/**
 * @brief encode and key a byte
 * @param b Byte to encode and key
 */
void write_byte(uint8_t b)
{
  // Shift and key out 8 data bits
  for (uint8_t i=0;i<8;i++)
    {
      if (b & (1<<i))
	write_baud(HZ_MARK);
      else
	write_baud(HZ_SPACE);
    }
}

/**
 * @brief write two seconds of leader
 */
void write_leader(void)
{
  for (int i=0;i<SAMPLE_RATE*LEADER_SECS;i++)
    putchar(SILENCE_BYTE);
}

/**
 * @brief handle header
 */
void header(void)
{
  uint8_t ch = 0;

  // Write the leader 0x55 bytes
  while (read(STDIN_FILENO,&ch,1) > 0)
    {
      write_byte(ch);
      if (ch == SYNC_BYTE) // name record's sync byte.
	break; 
    }

  // Write the name record
  for (uint8_t i=0;i<NAME_REC_LEN;i++)
    {
      read(STDIN_FILENO,&ch,1);
      write_byte(ch);
    }
}

/**
 * @brief Handle rest of tape
 */
void body(void)
{
  uint8_t ch = 0;
  
  while (read(STDIN_FILENO,&ch,1) > 0)
    write_byte(ch);
}

/**
 * @brief Read from stdin, and process
 */
void main(void)
{
  uint8_t ch=0;
  
  write_leader();
  header();
  write_leader();
  body();
}
