#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2_rotozoom.h>
#include "GfxExtensions.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const Uint32 rmask = 0xff000000;
const Uint32 gmask = 0x00ff0000;
const Uint32 bmask = 0x0000ff00;
const Uint32 amask = 0x000000ff;
#else
const Uint32 rmask = 0x000000ff;
const Uint32 gmask = 0x0000ff00;
const Uint32 bmask = 0x00ff0000;
const Uint32 amask = 0xff000000;
#endif

#define renderdrawline SDL_RenderDrawLine
/*!
\brief Draw thick ellipse with blending.
\param renderer The renderer to draw on.
\param xc X coordinate of the center of the ellipse.
\param yc Y coordinate of the center of the ellipse.
\param xr Horizontal radius in pixels of the ellipse.
\param yr Vertical radius in pixels of the ellipse.
\param r The red value of the ellipse to draw.
\param g The green value of the ellipse to draw.
\param b The blue value of the ellipse to draw.
\param a The alpha value of the ellipse to draw.
\param thick The line thickness in pixels
\returns Returns 0 on success, -1 on failure.
*/
int thickEllipseRGBA(SDL_Renderer * renderer, Sint16 xc, Sint16 yc, Sint16 xr, Sint16 yr, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
    int result = 0 ;
    int xi, yi, xo, yo, x, y, z ;
    double xi2, yi2, xo2, yo2 ;

    if (thick <= 1)
        return ellipseRGBA(renderer, xc, yc, xr, yr, r, g, b, a) ;

    xi = xr - thick / 2 ;
    xo = xi + thick - 1 ;
    yi = yr - thick / 2 ;
    yo = yi + thick - 1 ;

    if ((xi <= 0) || (yi <= 0))
        return -1 ;

    xi2 = xi * xi ;
    yi2 = yi * yi ;
    xo2 = xo * xo ;
    yo2 = yo * yo ;

    if (a != 255) result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

    if (xr < yr)
    {
        for (x = -xo; x <= -xi; x++)
        {
            y = sqrt(yo2 * (1.0 - x*x/xo2)) + 0.5 ;
            result |= renderdrawline(renderer, xc+x, yc-y, xc+x, yc+y) ;
        }
        for (x = -xi + 1; x <= xi - 1; x++)
        {
            y = sqrt(yo2 * (1.0 - x*x/xo2)) + 0.5 ;
            z = sqrt(yi2 * (1.0 - x*x/xi2)) + 0.5 ;
            result |= renderdrawline(renderer, xc+x, yc+z, xc+x, yc+y) ;
            result |= renderdrawline(renderer, xc+x, yc-z, xc+x, yc-y) ;
        }
        for (x = xo; x >= xi; x--)
        {
            y = sqrt(yo2 * (1.0 - x*x/xo2)) + 0.5 ;
            result |= renderdrawline(renderer, xc+x, yc-y, xc+x, yc+y) ;
        }
    }
    else
    {
        for (y = -yo; y <= -yi; y++)
        {
            x = sqrt(xo2 * (1.0 - y*y/yo2)) + 0.5 ;
            result |= renderdrawline(renderer, xc-x, yc+y, xc+x, yc+y) ;
        }
        for (y = -yi + 1; y <= yi - 1; y++)
        {
            x = sqrt(xo2 * (1.0 - y*y/yo2)) + 0.5 ;
            z = sqrt(xi2 * (1.0 - y*y/yi2)) + 0.5 ;
            result |= renderdrawline(renderer, xc+z, yc+y, xc+x, yc+y) ;
            result |= renderdrawline(renderer, xc-z, yc+y, xc-x, yc+y) ;
        }
        for (y = yo; y >= yi; y--)
        {
            x = sqrt(xo2 * (1.0 - y*y/yo2)) + 0.5 ;
            result |= renderdrawline(renderer, xc-x, yc+y, xc+x, yc+y) ;
        }
    }
    return result ;
}

// returns Returns 0 on success, -1 on failure.
int thickCircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
    return thickEllipseRGBA(renderer, x, y, rad, rad, r, g, b, a, thick);
}

// returns Returns 0 on success, -1 on failure.
int thickEllipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color, Uint8 thick)
{
    Uint8 *c = (Uint8 *)&color;
    return thickEllipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3], thick);
}

// returns Returns 0 on success, -1 on failure.
int thickCircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint32 color, Uint8 thick)
{
    Uint8 *c = (Uint8 *)&color;
    return thickEllipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3], thick);
}

// Extensions for anti-aliased filled ellipses and polygons by Richard Russell 20-Aug-2019

/*!
\brief Draw anti-aliased filled ellipse with blending.
\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled ellipse.
\param cy Y coordinate of the center of the filled ellipse.
\param rx Horizontal radius in pixels of the filled ellipse.
\param ry Vertical radius in pixels of the filled ellipse.
\param r The red value of the filled ellipse to draw.
\param g The green value of the filled ellipse to draw.
\param b The blue value of the filled ellipse to draw.
\param a The alpha value of the filled ellipse to draw.
\returns Returns 0 on success, -1 on failure.
*/
int aaFilledEllipseRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int n, xi, yi, result = 0 ;
    double s, v, x, y, dx, dy ;

    if ((rx <= 0.0) || (ry <= 0.0))
        return -1 ;

    result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) ;
    if (rx >= ry)
    {
        n = ry + 1 ;
        for (yi = cy - n - 1; yi <= cy + n + 1; yi++)
        {
            if (yi < (cy - 0.5))
                y = yi ;
            else
                y = yi + 1 ;
            s = (y - cy) / ry ;
            s = s * s ;
            x = 0.5 ;
            if (s < 1.0)
            {
                x = rx * sqrt(1.0 - s) ;
                if (x >= 0.5)
                {
                    result |= SDL_SetRenderDrawColor (renderer, r, g, b, a ) ;
                    result |= renderdrawline (renderer, cx - x + 1, yi, cx + x - 1, yi) ;
                }
            }
            s = 8 * ry * ry ;
            dy = fabs(y - cy) - 1.0 ;
            xi = cx - x ; // left
            while (1)
            {
                dx = (cx - xi - 1) * ry / rx ;
                v = s - 4 * (dx - dy) * (dx - dy) ;
                if (v < 0) break ;
                v = (sqrt(v) - 2 * (dx + dy)) / 4 ;
                if (v < 0) break ;
                if (v > 1.0) v = 1.0 ;
                result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
                result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
                xi -= 1 ;
            }
            xi = cx + x ; // right
            while (1)
            {
                dx = (xi - cx) * ry / rx ;
                v = s - 4 * (dx - dy) * (dx - dy) ;
                if (v < 0) break ;
                v = (sqrt(v) - 2 * (dx + dy)) / 4 ;
                if (v < 0) break ;
                if (v > 1.0) v = 1.0 ;
                result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
                result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
                xi += 1 ;
            }
        }
    }
    else
    {
        n = rx + 1 ;
        for (xi = cx - n - 1; xi <= cx + n + 1; xi++)
        {
            if (xi < (cx - 0.5))
                x = xi ;
            else
                x = xi + 1 ;
            s = (x - cx) / rx ;
            s = s * s ;
            y = 0.5 ;
            if (s < 1.0)
            {
                y = ry * sqrt(1.0 - s) ;
                if (y >= 0.5)
                {
                    result |= SDL_SetRenderDrawColor (renderer, r, g, b, a ) ;
                    result |= renderdrawline (renderer, xi, cy - y + 1, xi, cy + y - 1) ;
                }
            }
            s = 8 * rx * rx ;
            dx = fabs(x - cx) - 1.0 ;
            yi = cy - y ; // top
            while (1)
            {
                dy = (cy - yi - 1) * rx / ry ;
                v = s - 4 * (dy - dx) * (dy - dx) ;
                if (v < 0) break ;
                v = (sqrt(v) - 2 * (dy + dx)) / 4 ;
                if (v < 0) break ;
                if (v > 1.0) v = 1.0 ;
                result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
                result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
                yi -= 1 ;
            }
            yi = cy + y ; // bottom
            while (1)
            {
                dy = (yi - cy) * rx / ry ;
                v = s - 4 * (dy - dx) * (dy - dx) ;
                if (v < 0) break ;
                v = (sqrt(v) - 2 * (dy + dx)) / 4 ;
                if (v < 0) break ;
                if (v > 1.0) v = 1.0 ;
                result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
                result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
                yi += 1 ;
            }
        }
    }
    return result ;
}

// returns Returns 0 on success, -1 on failure.
int aaFilledEllipseColor(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint32 color)
{
    Uint8 *c = (Uint8 *)&color;
    return aaFilledEllipseRGBA(renderer, cx, cy, rx, ry, c[0], c[1], c[2], c[3]);
}


Uint32 ColorToUint(int r, int g, int b, int a)
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return (Uint32)((r << 24) + (g << 16) + (b << 8) + (a << 0));
    #else
    return (Uint32)((a << 24) + (b << 16) + (g << 8) + (r << 0));
    #endif
}

Uint32 ColorToUint(SDL_Color color){
    return ColorToUint(color.r, color.g, color.b, color.a);
}

SDL_Color UintToColor(Uint32 color)
{
    SDL_Color tempcol = {};
    tempcol.a = color & amask;
    tempcol.r = color & rmask;
    tempcol.g = color & gmask;
    tempcol.b = color & bmask;
    return tempcol;
}
