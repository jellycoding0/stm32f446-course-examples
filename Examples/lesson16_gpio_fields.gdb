# Source in CubeIDE Debugger Console, then run lesson16_gpio while suspended
# after MX_GPIO_Init. Reads PA5 fields only; no target writes or execution control.
# BSRR is a command register and is deliberately not read.
define lesson16_gpio
  if ((*(unsigned int *)0x40023830) & 1) && !((*(unsigned int *)0x40023810) & 1)
    set $l16_gpio = (GPIO_TypeDef *)0x40020000
    printf "PA5 MODER=%u (1=output)\n", ($l16_gpio->MODER >> 10) & 3
    printf "PA5 OTYPER=%u (0=push-pull)\n", ($l16_gpio->OTYPER >> 5) & 1
    printf "PA5 OSPEEDR=%u (0=low speed)\n", ($l16_gpio->OSPEEDR >> 10) & 3
    printf "PA5 PUPDR=%u (0=no pull)\n", ($l16_gpio->PUPDR >> 10) & 3
    printf "PA5 AFRL=%u (not selected in output mode)\n", ($l16_gpio->AFR[0] >> 20) & 15
    set $l16_odr = ($l16_gpio->ODR >> 5) & 1
    set $l16_idr = ($l16_gpio->IDR >> 5) & 1
    printf "PA5 ODR=%u IDR=%u (separate sequential reads)\n", $l16_odr, $l16_idr
  else
    printf "GPIO reads skipped: GPIOA clock off or reset asserted\n"
  end
end
document lesson16_gpio
Read PA5 mode, output type, speed, pull, alternate function and output/input bits.
ODR is the latch; IDR is the digital input sample, not an analog measurement.
end
