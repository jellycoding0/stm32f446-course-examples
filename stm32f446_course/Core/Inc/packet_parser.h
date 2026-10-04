#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H

#include <stdint.h>

void Parser_Reset(void);
void Parser_Feed(uint8_t byte);
/* main에서 즉시 소비하거나 복사함. data 포인터를 보관하지 않음. */
void Packet_OnValid(const uint8_t *data, uint8_t length);

extern volatile uint32_t parser_length_errors;
extern volatile uint32_t parser_check_errors;

#endif
