# Lesson 14: suspend after MX_GPIO_Init and source in CubeIDE Debugger Console.
# Only RCC AHB1ENR and GPIOA MODER are read from the MCU. No target writes.
# All assignments below change host-side GDB convenience variables only.
# W1C and interleaving examples are arithmetic models, not hardware tests.

printf "BOARD: GPIOA clock enable = %u\n", (*(unsigned int *)0x40023830) & 1
set $l14_before = *(unsigned int *)0x40020000
set $l14_mask = (unsigned int)(3u << 10)
set $l14_after = ($l14_before & ~$l14_mask) | (1u << 10)
printf "BOARD read: MODER=%#x, PA5 mode=%u\n", $l14_before, ($l14_before >> 10) & 3
printf "MODEL only: output mode candidate=%#x, other bits changed=%#x\n", $l14_after, ($l14_before ^ $l14_after) & ~$l14_mask

# Artificial mode 10: OR with 01 incorrectly gives 11; clear then set gives 01.
set $l14_mode = (unsigned int)(2u << 10)
printf "MODEL field: OR-only=%u, mask-then-set=%u\n", (($l14_mode | (1u << 10)) >> 10) & 3, ((($l14_mode & ~$l14_mask) | (1u << 10)) >> 10) & 3

# Simplified two-event W1C model: pending bits 5 and 13, clear only bit 13.
# remaining = pending & ~write_value (assumes no new event during the model).
set $l14_pending = (unsigned int)((1u << 5) | (1u << 13))
set $l14_clear = (unsigned int)(1u << 13)
printf "MODEL W1C: initial=%#x, direct-mask remaining=%#x, RMW remaining=%#x\n", $l14_pending, $l14_pending & ~$l14_clear, $l14_pending & ~($l14_pending | $l14_clear)

# Simplified ordinary RW register: A reads 0; B sets bit 1; A writes stale | bit 0.
set $l14_a_read = (unsigned int)0
set $l14_b_write = (unsigned int)2
set $l14_a_write = $l14_a_read | 1u
printf "MODEL interleave: after B=%#x, stale A write=%#x (B bit lost)\n", $l14_b_write, $l14_a_write
printf "No target register was written. Resume manually when finished.\n"
