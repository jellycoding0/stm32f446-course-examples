#include "packet_parser.h"

/* 30강: A5 | Length(0~32) | Payload | XOR(Length, Payload). */
enum ParseState { WAIT_HEADER, READ_LENGTH, READ_DATA, READ_CHECK };
static enum ParseState state;
static uint8_t payload[32];
static uint8_t length, used, check;
volatile uint32_t parser_length_errors;
volatile uint32_t parser_check_errors;

void Parser_Reset(void)
{
  state = WAIT_HEADER;
}

void Parser_Feed(uint8_t byte)
{
  /* main만 호출함. DMA 알림/IDLE 경계와 무관하게 상태를 유지함. */
  switch (state)
  {
    case WAIT_HEADER:
      if (byte == 0xA5u)
      {
        state = READ_LENGTH;
      }
      break;
    case READ_LENGTH:
      if (byte > sizeof payload)
      {
        parser_length_errors++;
        Parser_Reset();
        break;
      }
      length = byte;
      used = 0u;
      check = byte;
      state = length ? READ_DATA : READ_CHECK;
      break;
    case READ_DATA:
      /* Payload 안의 A5도 데이터임. 검증한 길이까지만 기록함. */
      payload[used++] = byte;
      check ^= byte;
      if (used == length)
      {
        state = READ_CHECK;
      }
      break;
    case READ_CHECK:
      if (byte == check)
      {
        Packet_OnValid(payload, length);
      }
      else
      {
        parser_check_errors++;
      }
      Parser_Reset();
      break;
  }
  /* 교육용 XOR 검사: 인증 기능이나 모든 손상에서의 즉시 재동기화는 아님. */
}
