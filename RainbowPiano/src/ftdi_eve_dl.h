/*****************
 * ftdi_eve_dl.h *
 *****************/

/**********************************************************************************
 * Adapted from:                                                                  *
 *     https://github.com/RudolphRiedel/FT800-FT813                               *
 *     By Rudolph Riedel                                                          *
 *                                                                                *
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2017                                                             *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 *                                                                                *
 **********************************************************************************/

#ifndef _FTDI_EVE_DL_H_
#define _FTDI_EVE_DL_H_

#ifndef FORCEDINLINE
  #define FORCEDINLINE __attribute__((always_inline)) inline
#endif

namespace FTDI {

   /* FT8xx graphics engine specific macros useful for static display list generation */
   inline uint32_t ALPHA_FUNC(uint8_t func, uint8_t ref)        {return (9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0);}
   inline uint32_t BEGIN(begin_t prim)                          {return DL::BEGIN|(((prim)&15UL)<<0);}

   inline uint32_t BITMAP_SOURCE(uint32_t ram_g_addr)     {return DL::BITMAP_SOURCE | (ram_g_addr & 0x000FFFFF);}
   inline uint32_t BITMAP_HANDLE(uint8_t handle)                {return (5UL<<24)|(((handle)&31UL)<<0);}
   inline uint32_t BITMAP_LAYOUT(uint8_t format, uint8_t linestride, uint8_t height)
                                                                {return (7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0);}
   inline uint32_t BITMAP_SIZE(uint8_t filter, bool wrapx, bool wrapy, uint16_t width, uint16_t height)
                                                                {return (8UL<<24)|(((filter)&1UL)<<20)|((wrapx?1:0)<<19)|((wrapy?1:0)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0);}
   inline uint32_t BITMAP_TRANSFORM_A(uint16_t a)               {return (21UL<<24)|(((a)&131071UL)<<0);}
   inline uint32_t BITMAP_TRANSFORM_B(uint16_t b)               {return (22UL<<24)|(((b)&131071UL)<<0);}
   inline uint32_t BITMAP_TRANSFORM_C(uint16_t c)               {return (23UL<<24)|(((c)&16777215UL)<<0);}
   inline uint32_t BITMAP_TRANSFORM_D(uint16_t d)               {return (24UL<<24)|(((d)&131071UL)<<0);}
   inline uint32_t BITMAP_TRANSFORM_E(uint16_t e)               {return (25UL<<24)|(((e)&131071UL)<<0);}
   inline uint32_t BITMAP_TRANSFORM_F(uint16_t f)               {return (26UL<<24)|(((f)&16777215UL)<<0);}
   inline uint32_t BLEND_FUNC(uint8_t src,uint8_t dst)          {return (11UL<<24)|(((src)&7UL)<<3)|(((dst)&7UL)<<0);}
   inline uint32_t CALL(uint16_t dest)                          {return (29UL<<24)|(((dest)&65535UL)<<0);}
   inline uint32_t CELL(uint8_t cell)                           {return (6UL<<24)|(((cell)&127UL)<<0);}
   inline uint32_t CLEAR(bool c,bool s,bool t)                  {return DL::CLEAR|((c?1:0)<<2)|((s?1:0)<<1)|((t?1:0)<<0);}
   inline uint32_t CLEAR_COLOR_A(uint8_t alpha)                 {return (15UL<<24)|(((alpha)&255UL)<<0);}
   inline uint32_t CLEAR_COLOR_RGB(uint8_t red, uint8_t green, uint8_t blue)
                                                                {return DL::CLEAR_COLOR_RGB|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0);}
   inline uint32_t CLEAR_COLOR_RGB(uint32_t rgb)                {return DL::CLEAR_COLOR_RGB|rgb;}
   inline uint32_t CLEAR_STENCIL(uint8_t s)                     {return (17UL<<24)|(((s)&255UL)<<0);}
   inline uint32_t CLEAR_TAG(uint8_t s)                         {return (18UL<<24)|(((s)&255UL)<<0);}
   inline uint32_t COLOR_A(uint8_t alpha)                       {return (16UL<<24)|(((alpha)&255UL)<<0);}
   inline uint32_t COLOR_MASK(bool r, bool g, bool b, bool a)   {return (32UL<<24)|((r?1:0)<<3)|((g?1:0)<<2)|((b?1:0)<<1)|((a?1:0)<<0);}
   inline uint32_t COLOR_RGB(uint8_t red,uint8_t green,uint8_t blue)
                                                                {return (4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0);}
   inline uint32_t COLOR_RGB(uint32_t rgb)                      {return (4UL<<24)|rgb;}
   /* inline uint32_t DISPLAY()                                 {return (0UL<<24)) */
   inline uint32_t END()                                        {return (33UL<<24);}
   inline uint32_t JUMP(uint16_t dest)                          {return (30UL<<24)|(((dest)&65535UL)<<0);}
   inline uint32_t LINE_WIDTH(uint16_t width)                   {return (14UL<<24)|(((width)&4095UL)<<0);}
   inline uint32_t MACRO(uint16_t m)                             {return (37UL<<24)|(((m)&1UL)<<0);}
   inline uint32_t POINT_SIZE(uint16_t size)                    {return (13UL<<24)|(((size)&8191UL)<<0);}
   inline uint32_t RESTORE_CONTEXT()                            {return (35UL<<24);}
   inline uint32_t RETURN ()                                    {return (36UL<<24);}
   inline uint32_t SAVE_CONTEXT()                               {return (34UL<<24);}
   inline uint32_t STENCIL_FUNC(uint16_t func, uint16_t ref, uint16_t mask)
                                                                {return (10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0);}
   inline uint32_t STENCIL_MASK(uint8_t mask)                   {return (19UL<<24)|(((mask)&255UL)<<0);}
   inline uint32_t STENCIL_OP(uint8_t sfail, uint8_t spass)     {return (12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0);}
   inline uint32_t TAG(uint8_t s)                               {return (3UL<<24)|(((s)&255UL)<<0);}
   inline uint32_t _TAG(uint8_t s)                              {return (3UL<<24)|(((s)&255UL)<<0);}
   inline uint32_t TAG_MASK(bool mask)                          {return (20UL<<24)|((mask?1:0)<<0);}
   inline uint32_t VERTEX2F(uint16_t x, uint16_t y)             {return (1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0);}
   inline uint32_t VERTEX2II(uint16_t x,uint16_t y, uint8_t handle = 0, uint8_t cell = 0)
                                                              {return (2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0);}

    // The following functions *must* be inlined since we are relying on the compiler to do
    // substitution of the constants from the data structure rather than actually storing
    // it in PROGMEM (which would fail, since we are not using pgm_read_near to read them).
    // Plus, by inlining, all the equations are evaluated at compile-time as everything
    // should be a constant.

    typedef struct {
      const uint8_t  format;
      const uint16_t linestride;
      const uint8_t  filter;
      const uint8_t  wrapx;
      const uint8_t  wrapy;
      const uint32_t RAMG_addr;
      const uint16_t width;
      const uint16_t height;
    } bitmap_info_t;

    FORCEDINLINE uint32_t BITMAP_SOURCE (const bitmap_info_t& info) {return BITMAP_SOURCE (info.RAMG_addr);};
    FORCEDINLINE uint32_t BITMAP_LAYOUT (const bitmap_info_t& info) {return BITMAP_LAYOUT (info.format, info.linestride, info.height);};
    FORCEDINLINE uint32_t BITMAP_SIZE   (const bitmap_info_t& info) {return BITMAP_SIZE (info.filter, info.wrapx, info.wrapy, info.width, info.height);}
}

#endif // _FTDI_EVE_DL_MACROS_H_