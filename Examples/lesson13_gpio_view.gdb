# Lesson 13: run in the CubeIDE Debugger Console with the target suspended
# after MX_GPIO_Init, using the matching Debug ELF and its GPIO_TypeDef symbols.
# This script reads registers only. It does not resume, reset, or write the MCU.
# GPIOA base is taken from the project's STM32F446 device header.
# Do not read the write-only BSRR register to infer output state; read ODR.

set $lesson13_gpioa = (GPIO_TypeDef *)0x40020000
printf "GPIO_TypeDef layout (member offsets in bytes):\n"
ptype /o GPIO_TypeDef
printf "GPIO_TypeDef size:\n"
print sizeof(GPIO_TypeDef)
printf "GPIOA ODR address and byte offset:\n"
print /x &($lesson13_gpioa->ODR)
print /x (char *)&($lesson13_gpioa->ODR) - (char *)$lesson13_gpioa
printf "GPIOA BSRR address only (no register read):\n"
print /x &($lesson13_gpioa->BSRR)
printf "RCC AHB1ENR GPIOAEN bit:\n"
print (*(unsigned int *)0x40023830) & 1
printf "GPIOA MODER PA5 field (1 = general-purpose output):\n"
print ($lesson13_gpioa->MODER >> 10) & 3
printf "GPIOA ODR by typed access and raw address (must match while halted):\n"
print /x $lesson13_gpioa->ODR
x/wx 0x40020014
printf "PA5 output latch (0 or 1):\n"
print ($lesson13_gpioa->ODR >> 5) & 1
