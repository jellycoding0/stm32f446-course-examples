/* Lesson 10: compile this file separately with -O0 and -Os, then inspect .s/.o.
 * This observation sample is outside the CubeIDE firmware source folders.
 * It is not linked into the LED firmware and must not be downloaded as firmware.
 */
static inline unsigned clamp100(unsigned value)
{
    return value > 100u ? 100u : value;
}

unsigned lesson10_clamp_probe(unsigned value)
{
    return clamp100(value);
}
