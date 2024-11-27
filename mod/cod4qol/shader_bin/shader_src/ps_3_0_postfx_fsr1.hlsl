// * COD4-SHADERGEN - xoxor4d.github.io
// * Template used : [ps_3_0_shadergen_2d_no_image.hlsl]
// * Mat. Template : [shadergen_2d_no_image.template]

#define PC
#define IS_VERTEX_SHADER 0
#define IS_PIXEL_SHADER 1
#include <shader_vars.h>

// our input struct ( same as in vs obv. )
struct PixelInput
{
    float4 position     : POSITION;
	float2 texcoord 	: TEXCOORD0;
};

// output struct
struct PixelOutput
{
	float4 color        : COLOR;
};

float3 FsrEasuCF(float2 p) {
    return tex2D(colorMapPostSunSampler, p).rgb;
}

/**** EASU ****/
void FsrEasuCon(out float4 con0, out float4 con1, out float4 con2, out float4 con3, float2 inputViewportInPixels, float2 inputSizeInPixels, float2 outputSizeInPixels)
{
    // Output integer position to a pixel position in viewport.
    con0 = float4( inputViewportInPixels.x / outputSizeInPixels.x, inputViewportInPixels.y / outputSizeInPixels.y, 0.5 * inputViewportInPixels.x / outputSizeInPixels.x-0.5, 0.5 * inputViewportInPixels.y / outputSizeInPixels.y-0.5);
    // Viewport pixel position to normalized image space.
    // This is used to get upper - left of 'F' tap.
    con1 = float4(1, 1, 1, -1) / inputSizeInPixels.xyxy;
    // Centers of gather4, first offset from upper - left of 'F'.
    //      +---+---+
    //      |   |   |
    //      +-- (0)--+
    //      | b | c |
    //  +---F---+---+---+
    //  | e | f | g | h |
    //  +-- (1)--+-- (2)--+
    //  | i | j | k | l |
    //  +---+---+---+---+
    //      | n | o |
    //      +-- (3)--+
    //      |   |   |
    //      +---+---+
    // These are from (0) instead of 'F'.
    con2 = float4(-1, 2, 1, 2) / inputSizeInPixels.xyxy;
    con3 = float4(0, 4, 0, 0) / inputSizeInPixels.xyxy;
}

void FsrEasuTapF(inout float3 aC, inout float aW, float2 off, float2 dir, float2 len, float lob, float clp, float3 c)
{
    // Tap color.
    // Rotate offset by direction.
    float2 v = float2(dot(off, dir), dot(off, float2(-dir.y, dir.x)));
    // Anisotropy.
    v *= len;
    // Compute distance^2.0
    float d2 = min(dot(v, v), clp);
    // Limit to the window as at corner, 2 taps can easily be outside.
    // Approximation of lancos2 without sin() or rcp(), or sqrt() to get x.
    //  (25 / 16 * (2 / 5 * x^2 - 1)^2 - (25 / 16 - 1)) * (1 / 4 * x^2 - 1)^2
    //  |_______________________________________|   |_______________|
    //                   base                             window
    // The general form of the 'base' is,
    //  (a* (b * x^2 - 1)^2- (a - 1))
    // Where 'a = 1/ (2 * b - b^2)' and 'b' moves around the negative lobe.
    float wB = 0.4 * d2 - 1.0;
    float wA = lob * d2 -1.0;
    wB *= wB;
    wA *= wA;
    wB = 1.5625 * wB-0.5625;
    float w=  wB * wA;
    // Do weighted average.
    aC += c * w;
    aW += w;
}

//------------------------------------------------------------------------------------------------------------------------------
// Accumulate direction and length.
void FsrEasuSetF(inout float2 dir, inout float len, float w, float lA, float lB, float lC, float lD, float lE)
{
    // Direction is the '+' diff.
    //    a
    //  b c d
    //    e
    // Then takes magnitude from abs average of both sides of 'c'.
    // Length converts gradient reversal to 0, smoothly to non - reversal at 1, shaped, then adding horz and vert terms.
    float lenX = max(abs(lD - lC), abs(lC - lB));
    float dirX = lD - lB;
    dir.x += dirX * w;
    lenX = clamp(abs(dirX) / lenX, 0.0, 1.0);
    lenX *= lenX;
    len += lenX * w;
    // Repeat for the y axis.
    float lenY = max(abs(lE - lC), abs(lC - lA));
    float dirY = lE - lA;
    dir.y += dirY * w;
    lenY = clamp(abs(dirY) / lenY, 0.0, 1.0);
    lenY *= lenY;
    len += lenY * w;
}

//------------------------------------------------------------------------------------------------------------------------------
void FsrEasuF(out float3 pix, float2 ip, float4 con0, float4 con1, float4 con2, float4 con3)
{
    //------------------------------------------------------------------------------------------------------------------------------
    // Get position of 'f'.
    float2 pp = ip * con0.xy + con0.zw; // Corresponding input pixel / subpixel
    float2 fp = floor(pp);// fp = source nearest pixel
    pp -= fp; // pp = source subpixel

    //------------------------------------------------------------------------------------------------------------------------------
    // 12 - tap kernel.
    //    b c
    //  e f g h
    //  i j k l
    //    n o
    // Gather 4 ordering.
    //  a b
    //  r g
    float2 p0 = fp * con1.xy + con1.zw;
    
    // These are from p0 to avoid pulling two constants on pre - Navi hardware.
    float2 p1 = p0 + con2.xy;
    float2 p2 = p0 + con2.zw;
    float2 p3 = p0 + con3.xy;

    // TextureGather is not available on WebGL2
    float4 off = float4(-0.5, 0.5, -0.5, 0.5) * con1.xxyy;
    // tex2DGather to tex2D offsets
    // x = west y = east z = north w = south
    float3 bC = FsrEasuCF(p0 + off.xw); float bL = bC.g + 0.5 * (bC.r + bC.b);
    float3 cC = FsrEasuCF(p0 + off.yw); float cL = cC.g + 0.5 * (cC.r + cC.b);
    float3 iC = FsrEasuCF(p1 + off.xw); float iL = iC.g + 0.5 * (iC.r + iC.b);
    float3 jC = FsrEasuCF(p1 + off.yw); float jL = jC.g + 0.5 * (jC.r + jC.b);
    float3 fC = FsrEasuCF(p1 + off.yz); float fL = fC.g + 0.5 * (fC.r + fC.b);
    float3 eC = FsrEasuCF(p1 + off.xz); float eL = eC.g + 0.5 * (eC.r + eC.b);
    float3 kC = FsrEasuCF(p2 + off.xw); float kL = kC.g + 0.5 * (kC.r + kC.b);
    float3 lC = FsrEasuCF(p2 + off.yw); float lL = lC.g + 0.5 * (lC.r + lC.b);
    float3 hC = FsrEasuCF(p2 + off.yz); float hL = hC.g + 0.5 * (hC.r + hC.b);
    float3 gC = FsrEasuCF(p2 + off.xz); float gL = gC.g + 0.5 * (gC.r + gC.b);
    float3 oC = FsrEasuCF(p3 + off.yz); float oL = oC.g + 0.5 * (oC.r + oC.b);
    float3 nC = FsrEasuCF(p3 + off.xz); float nL = nC.g + 0.5 * (nC.r + nC.b);
   
    //------------------------------------------------------------------------------------------------------------------------------
    // Simplest multi - channel approximate luma possible (luma times 2, in 2 FMA / MAD).
    // Accumulate for bilinear interpolation.
    float2 dir = float2(0, 0);
    float len = 0.0;

    FsrEasuSetF(dir, len, (1.0-pp.x)* (1.0-pp.y), bL, eL, fL, gL, jL);
    FsrEasuSetF(dir, len,    pp.x  * (1.0-pp.y), cL, fL, gL, hL, kL);
    FsrEasuSetF(dir, len, (1.0-pp.x)*  pp.y , fL, iL, jL, kL, nL);
    FsrEasuSetF(dir, len,    pp.x  *  pp.y , gL, jL, kL, lL, oL);

    //------------------------------------------------------------------------------------------------------------------------------
    // Normalize with approximation, and cleanup close to zero.
    float2 dir2 = dir * dir;
    float dirR = dir2.x + dir2.y;
    bool zro = dirR < (1.0 / 32768.0);
    dirR = rsqrt(dirR);
    dirR = zro ? 1.0 : dirR;
    dir.x = zro ? 1.0 : dir.x;
    dir *= float2(dirR, dirR);
    // Transform from {0 to 2} to {0 to 1} range, and shape with square.
    len = len * 0.5;
    len *= len;
    // Stretch kernel {1.0 vert|horz, to sqrt(2.0) on diagonal}.
    float stretch = dot(dir, dir) / (max(abs(dir.x), abs(dir.y)));
    // Anisotropic length after rotation,
    //  x := 1.0 lerp to 'stretch' on edges
    //  y := 1.0 lerp to 2x on edges
    float2 len2 = float2(1.0 + (stretch - 1.0) * len, 1.0 -0.5 * len);
    // Based on the amount of 'edge',
    // the window shifts from +/-{sqrt(2.0) to slightly beyond 2.0}.
    float lob = 0.5 - 0.29 * len;
    // Set distance^2 clipping point to the end of the adjustable window.
    float clp = 1.0/lob;

    //------------------------------------------------------------------------------------------------------------------------------
    // Accumulation lerped with min / max of 4 nearest.
    //    b c
    //  e f g h
    //  i j k l
    //    n o
    float3 min4 = min(min(fC, gC), min(jC, kC));
    float3 max4 = max(max(fC, gC), max(jC, kC));
    // Accumulation.
    float3 aC = float3(0, 0, 0);
    float aW = 0.0;
    FsrEasuTapF(aC, aW, float2( 0, -1) - pp, dir, len2, lob, clp, bC);
    FsrEasuTapF(aC, aW, float2( 1, -1) - pp, dir, len2, lob, clp, cC);
    FsrEasuTapF(aC, aW, float2(-1, 1) - pp, dir, len2, lob, clp, iC);
    FsrEasuTapF(aC, aW, float2( 0, 1) - pp, dir, len2, lob, clp, jC);
    FsrEasuTapF(aC, aW, float2( 0, 0) - pp, dir, len2, lob, clp, fC);
    FsrEasuTapF(aC, aW, float2(-1, 0) - pp, dir, len2, lob, clp, eC);
    FsrEasuTapF(aC, aW, float2( 1, 1) - pp, dir, len2, lob, clp, kC);
    FsrEasuTapF(aC, aW, float2( 2, 1) - pp, dir, len2, lob, clp, lC);
    FsrEasuTapF(aC, aW, float2( 2, 0) - pp, dir, len2, lob, clp, hC);
    FsrEasuTapF(aC, aW, float2( 1, 0) - pp, dir, len2, lob, clp, gC);
    FsrEasuTapF(aC, aW, float2( 1, 2) - pp, dir, len2, lob, clp, oC);
    FsrEasuTapF(aC, aW, float2( 0, 2) - pp, dir, len2, lob, clp, nC);
    //------------------------------------------------------------------------------------------------------------------------------
    // Normalize and dering.
    pix = min(max4, max(min4, aC / aW));
}


PixelOutput ps_main( const PixelInput pixel )
{
    PixelOutput fragment;

    float3 c;
    float4 con0, con1, con2, con3;
    
    float2 rendersize = renderTargetSize.xy;
    FsrEasuCon(con0, con1, con2, con3, rendersize, rendersize, vec2(1.0f, 1.0f));
    FsrEasuF(c, pixel.texcoord-0.5f, con0, con1, con2, con3);
    fragment.color = float4(c.xyz, 1.0f);

	return fragment;
}
