/**
 * @file neopixel.c
 * @brief Implementations of interfaces for neopixel support.
 * @author Arthur Beck/@ave (averse.abfun@gmail.com)
 * @note Written ad-hoc for Forge by Arthur Beck
 * @version 1.0
 * @copyright 2024
 */

#include "neopixel.h"
#include <stdbool.h>
#include "../HAL/stm32f4xx_hal.h"
#include "../CMSIS-Core/cmsis_compiler.h"

uint8_t PIXELS_MEM[MAX_PIXELS];

/*!
  @note    Copied from the official adafruit repo
  @brief   "Empty" NeoPixel constructor when length, pin and/or pixel type
           are not known at compile-time, and must be initialized later with
           updateType(), updateLength() and setPin().
  @return  Adafruit_NeoPixel object. Call the begin() function before use.
  @note    This function is deprecated(according to the actual adafruit thing)
*/
NeoPixelString createNPSvoid(void)
{
    NeoPixelString out;
#if defined(NEO_KHZ400)
    out.is800KHz = true;
#endif
    out.begun = false;
    out.numLEDs = 0;
    out.numBytes = 0;
    out.pin = -1;
    out.brightness = 0;
    out.pixels = NULL;
    out.rOffset = 1;
    out.gOffset = 0;
    out.bOffset = 2;
    out.wOffset = 1;
    out.endTime = 0;
    return out;
}

/*!
  @note    Copied from the official adafruit repo
  @brief   NeoPixel constructor when length, pin and pixel type are known
           at compile-time.
  @param   n  Number of NeoPixels in strand.
  @param   p  Arduino pin number which will drive the NeoPixel data in.
  @param   t  Pixel type -- add together NEO_* constants defined in
              Adafruit_NeoPixel.h, for example NEO_GRB+NEO_KHZ800 for
              NeoPixels expecting an 800 KHz (vs 400 KHz) data stream
              with color bytes expressed in green, red, blue order per
              pixel.
  @return  Adafruit_NeoPixel object. Call the begin() function before use.
*/
NeoPixelString createNPS(uint16_t n, GPIO_TypeDef *gpioPort, uint32_t gpioPin,
                         neoPixelType type)
{
    NeoPixelString out;
    out.begun = false;
    out.brightness = 0;
    out.pixels = NULL;
    out.endTime = 0;

    updateType(&out, type);
    updateLength(&out, n);
    setPin(&out, gpioPort, gpioPin);

    return out;
}

void destroyNPS(NeoPixelString *nps)
{
    nps->pixels = NULL;
    if (nps->pin >= 0)
    {
        HAL_GPIO_DeInit(nps->gpioPort, nps->gpioPin);
    }
}

static GPIO_InitTypeDef GPIO_InitStruct;

/*!
  @brief   Configure NeoPixel pin for output.
  @param   nps         The NeoPixelString structure.
*/
void NPbegin(NeoPixelString *nps)
{
    if (nps->pin >= 0)
    {
        GPIO_InitStruct.Pin = nps->gpioPin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(nps->gpioPort, &GPIO_InitStruct);
        HAL_GPIO_WritePin(nps->gpioPort, nps->gpioPin, GPIO_PIN_RESET);
    }
    nps->begun = true;
}

void updateLength(NeoPixelString *nps, uint16_t n)
{
    nps->pixels = NULL;
    nps->numBytes = n * ((nps->wOffset == nps->rOffset) ? 3 : 4);
    nps->pixels = (uint8_t *)PIXELS_MEM;
    for (uint16_t i = 0; i < nps->numBytes; i++)
    {
        PIXELS_MEM[i] = 0U;
    }

    nps->numLEDs = n;
}

void updateType(NeoPixelString *nps, neoPixelType t)
{
    bool oldThreeBytesPerPixel = (nps->wOffset == nps->rOffset); // false if RGBW

    nps->wOffset = (t >> 6) & 0b11; // See notes in header file
    nps->rOffset = (t >> 4) & 0b11; // regarding R/G/B/W offsets
    nps->gOffset = (t >> 2) & 0b11;
    nps->bOffset = t & 0b11;
#if defined(NEO_KHZ400)
    nps->is800KHz = (t < 256); // 400 KHz flag is 1<<8
#endif

    // If bytes-per-pixel has changed (and pixel data was previously
    // allocated), re-allocate to new size. Will clear any data.
    if (nps->pixels)
    {
        bool newThreeBytesPerPixel = (nps->wOffset == nps->rOffset);
        if (newThreeBytesPerPixel != oldThreeBytesPerPixel)
            updateLength(nps, nps->numLEDs);
    }
}

#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004) // Cycle count register
#define CYCLES_800_T0H (SystemCoreClock / 4000000)
#define CYCLES_800_T1H (SystemCoreClock / 1250000)
#define CYCLES_800 (SystemCoreClock / 800000)
#define CYCLES_400_T0H (SystemCoreClock / 2000000)
#define CYCLES_400_T1H (SystemCoreClock / 833333)
#define CYCLES_400 (SystemCoreClock / 400000)

/*!
  @brief   Transmit pixel data in RAM to NeoPixels.
  @note    Interrupts are temporarily disabled in order to achieve the correct NeoPixel signal timing.
  @param   nps         The NeoPixelString structure.
*/
void NPshow(NeoPixelString *nps)
{
    SystemCoreClockUpdate();
    if (!nps->pixels)
        return;

    while (!NPcanShow(nps))
        ;

    uint32_t primask = __get_PRIMASK();
    bool backup_interrupts_enabled = (primask == 0);
    __disable_irq();

    uint8_t *p = nps->pixels, *end = p + nps->numBytes, pix, mask;
    uint32_t cyc;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

#if defined(NEO_KHZ400) // 800 KHz check needed only if 400 KHz support enabled
    if (is800KHz)
    {
#endif
        cyc = DWT_CYCCNT + CYCLES_800;
        while (p < end)
        {
            pix = *p++;
            for (mask = 0x80; mask; mask >>= 1)
            {
                while (DWT_CYCCNT - cyc < CYCLES_800)
                    ;
                cyc = DWT_CYCCNT;
                HAL_GPIO_WritePin(nps->gpioPort, nps->gpioPin, 1);
                if (pix & mask)
                {
                    while (DWT_CYCCNT - cyc < CYCLES_800_T1H)
                        ;
                }
                else
                {
                    while (DWT_CYCCNT - cyc < CYCLES_800_T0H)
                        ;
                }
                HAL_GPIO_WritePin(nps->gpioPort, nps->gpioPin, 0);
            }
        }
        while (DWT_CYCCNT - cyc < CYCLES_800)
            ;
#if defined(NEO_KHZ400)
    }
    else
    {
        cyc = DWT_CYCCNT + CYCLES_400;
        while (p < end)
        {
            pix = *p++;
            for (mask = 0x80; mask; mask >>= 1)
            {
                while (DWT_CYCCNT - cyc < CYCLES_400)
                    ;
                cyc = DWT_CYCCNT;
                HAL_GPIO_WritePin(nps->gpioPort, nps->gpioPin, 1);
                if (pix & mask)
                {
                    while (DWT_CYCCNT - cyc < CYCLES_400_T1H)
                        ;
                }
                else
                {
                    while (DWT_CYCCNT - cyc < CYCLES_400_T0H)
                        ;
                }
                HAL_GPIO_WritePin(nps->gpioPort, nps->gpioPin, 0);
            }
        }
        while (DWT_CYCCNT - cyc < CYCLES_400)
            ;
    }
#endif
    if (backup_interrupts_enabled)
    {
        __enable_irq();
    }
    else
    {
        __set_PRIMASK(primask);
    }
    _setupTime();
    nps->endTime = _getTime();
}

void setPin(NeoPixelString *nps, GPIO_TypeDef *gpioPort, uint32_t gpioPin)
{
    if (nps->begun && nps->pin > 0)
    {
        GPIO_InitStruct.Pin = nps->gpioPin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(nps->gpioPort, &GPIO_InitStruct);
    }
    nps->pin = gpioPin;
    nps->gpioPort = gpioPort;
    nps->gpioPin = gpioPin;
}

/*!
  @brief   Set a pixel's color using separate red, green and blue
           components. If using RGBW pixels, white will be set to 0.
  @param   nps         The NeoPixelString structure.
  @param   n  Pixel index, starting from 0.
  @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
  @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
  @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
*/
void NPsetPixelColorRGB(NeoPixelString *nps, uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    if (n < nps->numLEDs)
    {
        if (nps->brightness)
        {
            r = (r * nps->brightness) >> 8;
            g = (g * nps->brightness) >> 8;
            b = (b * nps->brightness) >> 8;
        }
        uint8_t *p;
        if (nps->wOffset == nps->rOffset)
        {                            // Is an RGB-type strip
            p = &nps->pixels[n * 3]; // 3 bytes per pixel
        }
        else
        {                            // Is a WRGB-type strip
            p = &nps->pixels[n * 4]; // 4 bytes per pixel
            p[nps->wOffset] = 0;     // But only R,G,B passed -- set W to 0
        }
        p[nps->rOffset] = r; // R,G,B always stored
        p[nps->gOffset] = g;
        p[nps->bOffset] = b;
    }
}

/*!
  @brief   Set a pixel's color using separate red, green, blue and white
           components (for RGBW NeoPixels only).
  @param   nps         The NeoPixelString structure.
  @param   n  Pixel index, starting from 0.
  @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
  @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
  @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
  @param   w  White brightness, 0 = minimum (off), 255 = maximum, ignored
              if using RGB pixels.
*/
void NPsetPixelColorRGBW(NeoPixelString *nps, uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    if (n < nps->numLEDs)
    {
        if (nps->brightness)
        {
            r = (r * nps->brightness) >> 8;
            g = (g * nps->brightness) >> 8;
            b = (b * nps->brightness) >> 8;
            w = (w * nps->brightness) >> 8;
        }
        uint8_t *p;
        if (nps->wOffset == nps->rOffset)
        {                            // Is an RGB-type strip
            p = &nps->pixels[n * 3]; // 3 bytes per pixel (ignore W)
        }
        else
        {                            // Is a WRGB-type strip
            p = &nps->pixels[n * 4]; // 4 bytes per pixel
            p[nps->wOffset] = w;     // Store W
        }
        p[nps->rOffset] = r; // R,G,B always stored
        p[nps->gOffset] = g;
        p[nps->bOffset] = b;
    }
}

/*!
  @brief   Set a pixel's color using a 32-bit 'packed' RGB or RGBW value.
  @param   nps         The NeoPixelString structure.
  @param   n  Pixel index, starting from 0.
  @param   c  32-bit color value. Most significant byte is white (for RGBW
              pixels) or ignored (for RGB pixels), next is red, then green,
              and least significant byte is blue.
*/
void NPsetPixelColorC(NeoPixelString *nps, uint16_t n, uint32_t c)
{
    if (n < nps->numLEDs)
    {
        uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
        if (nps->brightness)
        { // See notes in setBrightness()
            r = (r * nps->brightness) >> 8;
            g = (g * nps->brightness) >> 8;
            b = (b * nps->brightness) >> 8;
        }
        if (nps->wOffset == nps->rOffset)
        {
            p = &nps->pixels[n * 3];
        }
        else
        {
            p = &nps->pixels[n * 4];
            uint8_t w = (uint8_t)(c >> 24);
            p[nps->wOffset] = nps->brightness ? ((w * nps->brightness) >> 8) : w;
        }
        p[nps->rOffset] = r;
        p[nps->gOffset] = g;
        p[nps->bOffset] = b;
    }
}

/*!
  @brief   Fill all or part of the NeoPixel strip with a color.
  @param   nps         The NeoPixelString structure.
  @param   c      32-bit color value. Most significant byte is white (for
                  RGBW pixels) or ignored (for RGB pixels), next is red,
                  then green, and least significant byte is blue. If all
                  arguments are unspecified, this will be 0 (off).
  @param   first  Index of first pixel to fill, starting from 0. Must be
                  in-bounds, no clipping is performed. 0 if unspecified.
  @param   count  Number of pixels to fill, as a positive value. Passing
                  0 or leaving unspecified will fill to end of strip.
*/
void NPfill(NeoPixelString *nps, uint32_t c, uint16_t first, uint16_t count)
{
    uint16_t i, end;

    if (first >= nps->numLEDs)
    {
        return; // If first LED is past end of strip, nothing to do
    }

    // Calculate the index ONE AFTER the last pixel to fill
    if (count == 0)
    {
        // Fill to end of strip
        end = nps->numLEDs;
    }
    else
    {
        // Ensure that the loop won't go past the last pixel
        end = first + count;
        if (end > nps->numLEDs)
            end = nps->numLEDs;
    }

    for (i = first; i < end; i++)
    {
        setPixelColor(nps, i, c);
    }
}

/*!
  @brief   Convert hue, saturation and value into a packed 32-bit RGB color
           that can be passed to setPixelColor() or other RGB-compatible
           functions.
  @param   nps         The NeoPixelString structure.
  @param   hue  An unsigned 16-bit value, 0 to 65535, representing one full
                loop of the color wheel, which allows 16-bit hues to "roll
                over" while still doing the expected thing (and allowing
                more precision than the wheel() function that was common to
                prior NeoPixel examples).
  @param   sat  Saturation, 8-bit value, 0 (min or pure grayscale) to 255
                (max or pure hue). Default of 255 if unspecified.
  @param   val  Value (brightness), 8-bit value, 0 (min / black / off) to
                255 (max or full brightness). Default of 255 if unspecified.
  @return  Packed 32-bit RGB with the most significant byte set to 0 -- the
           white element of WRGB pixels is NOT utilized. Result is linearly
           but not perceptually correct, so you may want to pass the result
           through the gamma32() function (or your own gamma-correction
           operation) else colors may appear washed out. This is not done
           automatically by this function because coders may desire a more
           refined gamma-correction function than the simplified
           one-size-fits-all operation of gamma32(). Diffusing the LEDs also
           really seems to help when using low-saturation colors.
*/
static uint32_t NPColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
{
    uint8_t r, g, b;

    // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
    // 0 is not the start of pure red, but the midpoint...a few values above
    // zero and a few below 65536 all yield pure red (similarly, 32768 is the
    // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
    // each for red, green, blue) really only allows for 1530 distinct hues
    // (not 1536, more on that below), but the full unsigned 16-bit type was
    // chosen for hue so that one's code can easily handle a contiguous color
    // wheel by allowing hue to roll over in either direction.
    hue = (hue * 1530L + 32768) / 65536;
    // Because red is centered on the rollover point (the +32768 above,
    // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
    // where 0 and 1530 would yield the same thing. Rather than apply a
    // costly modulo operator, 1530 is handled as a special case below.

    // So you'd think that the color "hexcone" (the thing that ramps from
    // pure red, to pure yellow, to pure green and so forth back to red,
    // yielding six slices), and with each color component having 256
    // possible values (0-255), might have 1536 possible items (6*256),
    // but in reality there's 1530. This is because the last element in
    // each 256-element slice is equal to the first element of the next
    // slice, and keeping those in there this would create small
    // discontinuities in the color wheel. So the last element of each
    // slice is dropped...we regard only elements 0-254, with item 255
    // being picked up as element 0 of the next slice. Like this:
    // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
    // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
    // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
    // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
    // the constants below are not the multiples of 256 you might expect.

    // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
    if (hue < 510)
    { // Red to Green-1
        b = 0;
        if (hue < 255)
        { //   Red to Yellow-1
            r = 255;
            g = hue; //     g = 0 to 254
        }
        else
        {                  //   Yellow to Green-1
            r = 510 - hue; //     r = 255 to 1
            g = 255;
        }
    }
    else if (hue < 1020)
    { // Green to Blue-1
        r = 0;
        if (hue < 765)
        { //   Green to Cyan-1
            g = 255;
            b = hue - 510; //     b = 0 to 254
        }
        else
        {                   //   Cyan to Blue-1
            g = 1020 - hue; //     g = 255 to 1
            b = 255;
        }
    }
    else if (hue < 1530)
    { // Blue to Red-1
        g = 0;
        if (hue < 1275)
        {                   //   Blue to Magenta-1
            r = hue - 1020; //     r = 0 to 254
            b = 255;
        }
        else
        { //   Magenta to Red-1
            r = 255;
            b = 1530 - hue; //     b = 255 to 1
        }
    }
    else
    { // Last 0.5 Red (quicker than % operator)
        r = 255;
        g = b = 0;
    }

    // Apply saturation and value to R,G,B, pack into 32-bit result:
    uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
    uint16_t s1 = 1 + sat;  // 1 to 256; same reason
    uint8_t s2 = 255 - sat; // 255 to 0
    return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
           (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
           (((((b * s1) >> 8) + s2) * v1) >> 8);
}

/*!
  @brief   Query the color of a previously-set pixel.
  @param   nps         The NeoPixelString structure.
  @param   n  Index of pixel to read (0 = first).
  @return  'Packed' 32-bit RGB or WRGB value. Most significant byte is white
           (for RGBW pixels) or 0 (for RGB pixels), next is red, then green,
           and least significant byte is blue.
  @note    If the strip brightness has been changed from the default value
           of 255, the color read from a pixel may not exactly match what
           was previously written with one of the setPixelColor() functions.
           This gets more pronounced at lower brightness levels.
*/
uint32_t NPgetPixelColor(NeoPixelString *nps, uint16_t n)
{
    if (n >= nps->numLEDs)
        return 0; // Out of bounds, return no color.

    uint8_t *p;

    if (nps->wOffset == nps->rOffset)
    { // Is RGB-type device
        p = &nps->pixels[n * 3];
        if (nps->brightness)
        {
            // Stored color was decimated by setBrightness(). Returned value
            // attempts to scale back to an approximation of the original 24-bit
            // value used when setting the pixel color, but there will always be
            // some error -- those bits are simply gone. Issue is most
            // pronounced at low brightness levels.
            return (((uint32_t)(p[nps->rOffset] << 8) / nps->brightness) << 16) |
                   (((uint32_t)(p[nps->gOffset] << 8) / nps->brightness) << 8) |
                   ((uint32_t)(p[nps->bOffset] << 8) / nps->brightness);
        }
        else
        {
            // No brightness adjustment has been made -- return 'raw' color
            return ((uint32_t)p[nps->rOffset] << 16) | ((uint32_t)p[nps->gOffset] << 8) |
                   (uint32_t)p[nps->bOffset];
        }
    }
    else
    { // Is RGBW-type device
        p = &nps->pixels[n * 4];
        if (nps->brightness)
        { // Return scaled color
            return (((uint32_t)(p[nps->wOffset] << 8) / nps->brightness) << 24) |
                   (((uint32_t)(p[nps->rOffset] << 8) / nps->brightness) << 16) |
                   (((uint32_t)(p[nps->gOffset] << 8) / nps->brightness) << 8) |
                   ((uint32_t)(p[nps->bOffset] << 8) / nps->brightness);
        }
        else
        { // Return raw color
            return ((uint32_t)p[nps->wOffset] << 24) | ((uint32_t)p[nps->rOffset] << 16) |
                   ((uint32_t)p[nps->gOffset] << 8) | (uint32_t)p[nps->bOffset];
        }
    }
}

/*!
  @brief   Adjust output brightness. Does not immediately affect what's
           currently displayed on the LEDs. The next call to show() will
           refresh the LEDs at this level.
  @param   nps         The NeoPixelString structure.
  @param   b  Brightness setting, 0=minimum (off), 255=brightest.
  @note    This was intended for one-time use in one's setup() function,
           not as an animation effect in itself. Because of the way this
           library "pre-multiplies" LED colors in RAM, changing the
           brightness is often a "lossy" operation -- what you write to
           pixels isn't necessary the same as what you'll read back.
           Repeated brightness changes using this function exacerbate the
           problem. Smart programs therefore treat the strip as a
           write-only resource, maintaining their own state to render each
           frame of an animation, not relying on read-modify-write.
*/
void NPsetBrightness(NeoPixelString *nps, uint8_t b)
{
    // Stored brightness value is different than what's passed.
    // This simplifies the actual scaling math later, allowing a fast
    // 8x8-bit multiply and taking the MSB. 'brightness' is a uint8_t,
    // adding 1 here may (intentionally) roll over...so 0 = max brightness
    // (color values are interpreted literally; no scaling), 1 = min
    // brightness (off), 255 = just below max brightness.
    uint8_t newBrightness = b + 1;
    if (newBrightness != nps->brightness)
    { // Compare against prior value
        // Brightness has changed -- re-scale existing data in RAM,
        // This process is potentially "lossy," especially when increasing
        // brightness. The tight timing in the WS2811/WS2812 code means there
        // aren't enough free cycles to perform this scaling on the fly as data
        // is issued. So we make a pass through the existing color data in RAM
        // and scale it (subsequent graphics commands also work at this
        // brightness level). If there's a significant step up in brightness,
        // the limited number of steps (quantization) in the old data will be
        // quite visible in the re-scaled version. For a non-destructive
        // change, you'll need to re-render the full strip data. C'est la vie.
        uint8_t c, *ptr = nps->pixels,
                   oldBrightness = nps->brightness - 1; // De-wrap old brightness value
        uint16_t scale;
        if (oldBrightness == 0)
            scale = 0; // Avoid /0
        else if (b == 255)
            scale = 65535 / oldBrightness;
        else
            scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
        for (uint16_t i = 0; i < nps->numBytes; i++)
        {
            c = *ptr;
            *ptr++ = (c * scale) >> 8;
        }
        nps->brightness = newBrightness;
    }
}

/*!
  @brief   Fill the whole NeoPixel strip with 0 / black / off.
  @param   nps         The NeoPixelString structure.
*/
void NPclear(NeoPixelString *nps)
{
    NPfill(nps, 0, 0, 0);
}

// A 32-bit variant of gamma8() that applies the same function
// to all components of a packed RGB or WRGB value.
uint32_t gamma32(uint32_t x)
{
    uint8_t *y = (uint8_t *)&x;
    // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
    // to avoid a bunch of shifting and masking that would be necessary for
    // properly handling different endianisms (and each byte is a fairly
    // trivial operation, so it might not even be wasting cycles vs a check
    // and branch for the RGB case). In theory this might cause trouble *if*
    // someone's storing information in the unused most significant byte
    // of an RGB value, but this seems exceedingly rare and if it's
    // encountered in reality they can mask values going in or coming out.
    for (uint8_t i = 0; i < 4; i++)
        y[i] = gamma8(y[i]);
    return x; // Packed 32-bit return
}

/*!
  @brief   Fill NeoPixel strip with one or more cycles of hues.
           Everyone loves the rainbow swirl so much, now it's canon!
  @param   nps         The NeoPixelString structure.
  @param   first_hue   Hue of first pixel, 0-65535, representing one full
                       cycle of the color wheel. Each subsequent pixel will
                       be offset to complete one or more cycles over the
                       length of the strip.
  @param   reps        Number of cycles of the color wheel over the length
                       of the strip. Default is 1. Negative values can be
                       used to reverse the hue order.
  @param   saturation  Saturation (optional), 0-255 = gray to pure hue,
                       default = 255.
  @param   brightness  Brightness/value (optional), 0-255 = off to max,
                       default = 255. This is distinct and in combination
                       with any configured global strip brightness.
  @param   gammify     If true (default), apply gamma correction to colors
                       for better appearance.
*/
void NPrainbow(NeoPixelString *nps, uint16_t first_hue, int8_t reps,
             uint8_t saturation, uint8_t brightness, bool gammify)
{
    for (uint16_t i = 0; i < nps->numLEDs; i++)
    {
        uint16_t hue = first_hue + (i * reps * 65536) / nps->numLEDs;
        uint32_t color = ColorHSV(hue, saturation, brightness);
        if (gammify)
            color = gamma32(color);
        setPixelColor(i, color);
    }
}

/*!
  @brief  Convert pixel color order from string (e.g. "BGR") to NeoPixel
          color order constant (e.g. NEO_BGR). This may be helpful for code
          that initializes from text configuration rather than compile-time
          constants.
  @param   v  Input string. Should be reasonably sanitized (a 3- or 4-
              character NUL-terminated string) or undefined behavior may
              result (output is still a valid NeoPixel order constant, but
              might not present as expected). Garbage in, garbage out.
  @return  One of the NeoPixel color order constants (e.g. NEO_BGR).
           NEO_KHZ400 or NEO_KHZ800 bits are not included, nor needed (all
           NeoPixels actually support 800 KHz it's been found, and this is
           the default state if no KHZ bits set).
  @note    This function is declared static in the class so it can be called
           without a NeoPixel object (since it's not likely been declared
           in the code yet). Use Adafruit_NeoPixel::str2order().
*/
neoPixelType str2order(const char *v) {
  int8_t r = 0, g = 0, b = 0, w = -1;
  if (v) {
    char c;
    for (uint8_t i=0; ((c = tolower(v[i]))); i++) {
      if (c == 'r') r = i;
      else if (c == 'g') g = i;
      else if (c == 'b') b = i;
      else if (c == 'w') w = i;
    }
    r &= 3;
  }
  if (w < 0) w = r; // If 'w' not specified, duplicate r bits
  return (w << 6) | (r << 4) | ((g & 3) << 2) | (b & 3);
}