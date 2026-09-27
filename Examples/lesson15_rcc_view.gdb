# Source this file in CubeIDE Debugger Console, then run lesson15_rcc
# while the target is suspended. This defines a read-only observation command.
# It does not reset, resume, set breakpoints, or write any target registers.
define lesson15_rcc
  printf "SystemCoreClock=%u Hz (software value, not a measurement)\n", SystemCoreClock
  set $l15_ahb1enr = *(unsigned int *)0x40023830
  set $l15_ahb1rstr = *(unsigned int *)0x40023810
  set $l15_apb1enr = *(unsigned int *)0x40023840
  set $l15_apb1rstr = *(unsigned int *)0x40023820
  printf "GPIOA: EN=%u RESET=%u\n", $l15_ahb1enr & 1, $l15_ahb1rstr & 1
  printf "TIM6: EN=%u RESET=%u\n", ($l15_apb1enr >> 4) & 1, ($l15_apb1rstr >> 4) & 1
  printf "USART2: EN=%u RESET=%u\n", ($l15_apb1enr >> 17) & 1, ($l15_apb1rstr >> 17) & 1
  if ($l15_ahb1enr & 1) && !($l15_ahb1rstr & 1)
    printf "GPIOA PA5 MODER=%u (0=input, 1=output)\n", ((*(unsigned int *)0x40020000) >> 10) & 3
  else
    printf "GPIOA MODER read skipped: clock disabled or reset asserted\n"
  end
end
document lesson15_rcc
Read RCC enable/reset bits and, only when GPIOA is enabled and not in reset,
read PA5 mode. Use before GPIO init, after clock enable, and after pin init.
end
