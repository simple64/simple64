#ifdef N64VIDEO_C

static int32_t blenderone = 0xff;

static uint8_t bldiv_hwaccurate_table[0x8000];

static INLINE void set_blender_input(struct rdp_state* wstate, int cycle, int which, int32_t **input_r, int32_t **input_g, int32_t **input_b, int32_t **input_a, int a, int b)
{

    switch (a & 0x3)
    {
        case 0:
        {
            if (cycle == 0)
            {
                *input_r = &wstate->pixel_color.r;
                *input_g = &wstate->pixel_color.g;
                *input_b = &wstate->pixel_color.b;
            }
            else
            {
                *input_r = &wstate->blended_pixel_color.r;
                *input_g = &wstate->blended_pixel_color.g;
                *input_b = &wstate->blended_pixel_color.b;
            }
            break;
        }

        case 1:
        {
            *input_r = &wstate->memory_color.r;
            *input_g = &wstate->memory_color.g;
            *input_b = &wstate->memory_color.b;
            break;
        }

        case 2:
        {
            *input_r = &wstate->blend_color.r;      *input_g = &wstate->blend_color.g;      *input_b = &wstate->blend_color.b;
            break;
        }

        case 3:
        {
            *input_r = &wstate->fog_color.r;        *input_g = &wstate->fog_color.g;        *input_b = &wstate->fog_color.b;
            break;
        }
    }

    if (which == 0)
    {
        switch (b & 0x3)
        {
            case 0:     *input_a = &wstate->pixel_color.a; break;
            case 1:     *input_a = &wstate->fog_color.a; break;
            case 2:     *input_a = &wstate->blender_shade_alpha; break;
            case 3:     *input_a = &zero_color; break;
        }
    }
    else
    {
        switch (b & 0x3)
        {
            case 0:     *input_a = &wstate->inv_pixel_color.a; break;
            case 1:     *input_a = &wstate->memory_color.a; break;
            case 2:     *input_a = &blenderone; break;
            case 3:     *input_a = &zero_color; break;
        }
    }
}

static STRICTINLINE int alpha_compare(struct rdp_state* wstate, int32_t comb_alpha)
{
    int32_t threshold;
    if (!wstate->other_modes.alpha_compare_en)
        return 1;
    else
    {
        if (!wstate->other_modes.dither_alpha_en)
            threshold = wstate->blend_color.a;
        else
            threshold = irand(&wstate->rseed) & 0xff;


        if (comb_alpha >= threshold)
            return 1;
        else
            return 0;
    }
}

static STRICTINLINE void blender_equation_cycle0(struct rdp_state* wstate, int* r, int* g, int* b)
{
    int blend1a, blend2a;
    int blr, blg, blb, sum;
    blend1a = *wstate->blender1b_a[0] >> 3;
    blend2a = *wstate->blender2b_a[0] >> 3;

    int mulb;



    if (wstate->blender2b_a[0] == &wstate->memory_color.a)
    {
        blend1a = (blend1a >> wstate->blshifta) & 0x3C;
        blend2a = (blend2a >> wstate->blshiftb) | 3;
    }

    mulb = blend2a + 1;


    blr = (*wstate->blender1a_r[0]) * blend1a + (*wstate->blender2a_r[0]) * mulb;
    blg = (*wstate->blender1a_g[0]) * blend1a + (*wstate->blender2a_g[0]) * mulb;
    blb = (*wstate->blender1a_b[0]) * blend1a + (*wstate->blender2a_b[0]) * mulb;



    if (!wstate->other_modes.force_blend)
    {





        sum = ((blend1a & ~3) + (blend2a & ~3) + 4) << 9;
        *r = bldiv_hwaccurate_table[sum | ((blr >> 2) & 0x7ff)];
        *g = bldiv_hwaccurate_table[sum | ((blg >> 2) & 0x7ff)];
        *b = bldiv_hwaccurate_table[sum | ((blb >> 2) & 0x7ff)];
    }
    else
    {
        *r = (blr >> 5) & 0xff;
        *g = (blg >> 5) & 0xff;
        *b = (blb >> 5) & 0xff;
    }
}

static STRICTINLINE void blender_equation_cycle0_2(struct rdp_state* wstate, int* r, int* g, int* b)
{
    int blend1a, blend2a;
    blend1a = *wstate->blender1b_a[0] >> 3;
    blend2a = *wstate->blender2b_a[0] >> 3;

    if (wstate->blender2b_a[0] == &wstate->memory_color.a)
    {
        blend1a = (blend1a >> wstate->pastblshifta) & 0x3C;
        blend2a = (blend2a >> wstate->pastblshiftb) | 3;
    }

    blend2a += 1;
    *r = (((*wstate->blender1a_r[0]) * blend1a + (*wstate->blender2a_r[0]) * blend2a) >> 5) & 0xff;
    *g = (((*wstate->blender1a_g[0]) * blend1a + (*wstate->blender2a_g[0]) * blend2a) >> 5) & 0xff;
    *b = (((*wstate->blender1a_b[0]) * blend1a + (*wstate->blender2a_b[0]) * blend2a) >> 5) & 0xff;
}

static STRICTINLINE void blender_equation_cycle1(struct rdp_state* wstate, int* r, int* g, int* b)
{
    int blend1a, blend2a;
    int blr, blg, blb, sum;
    blend1a = *wstate->blender1b_a[1] >> 3;
    blend2a = *wstate->blender2b_a[1] >> 3;

    int mulb;
    if (wstate->blender2b_a[1] == &wstate->memory_color.a)
    {
        blend1a = (blend1a >> wstate->blshifta) & 0x3C;
        blend2a = (blend2a >> wstate->blshiftb) | 3;
    }

    mulb = blend2a + 1;
    blr = (*wstate->blender1a_r[1]) * blend1a + (*wstate->blender2a_r[1]) * mulb;
    blg = (*wstate->blender1a_g[1]) * blend1a + (*wstate->blender2a_g[1]) * mulb;
    blb = (*wstate->blender1a_b[1]) * blend1a + (*wstate->blender2a_b[1]) * mulb;

    if (!wstate->other_modes.force_blend)
    {
        sum = ((blend1a & ~3) + (blend2a & ~3) + 4) << 9;
        *r = bldiv_hwaccurate_table[sum | ((blr >> 2) & 0x7ff)];
        *g = bldiv_hwaccurate_table[sum | ((blg >> 2) & 0x7ff)];
        *b = bldiv_hwaccurate_table[sum | ((blb >> 2) & 0x7ff)];
    }
    else
    {
        *r = (blr >> 5) & 0xff;
        *g = (blg >> 5) & 0xff;
        *b = (blb >> 5) & 0xff;
    }
}

static STRICTINLINE int blender_1cycle(struct rdp_state* wstate, uint32_t* fr, uint32_t* fg, uint32_t* fb, int dith, uint32_t blend_en, uint32_t prewrap, uint32_t curpixel_cvg, uint32_t curpixel_cvbit)
{
    int r, g, b, dontblend;


    if (alpha_compare(wstate, wstate->pixel_color.a))
    {






        if (wstate->other_modes.antialias_en ? curpixel_cvg : curpixel_cvbit)
        {

            if (!wstate->other_modes.color_on_cvg || prewrap)
            {
                dontblend = (wstate->other_modes.f.partialreject_1cycle && wstate->pixel_color.a >= 0xff);
                if (!blend_en || dontblend)
                {
                    r = *wstate->blender1a_r[0];
                    g = *wstate->blender1a_g[0];
                    b = *wstate->blender1a_b[0];
                }
                else
                {
                    wstate->inv_pixel_color.a =  (~(*wstate->blender1b_a[0])) & 0xff;





                    blender_equation_cycle0(wstate, &r, &g, &b);
                }
            }
            else
            {
                r = *wstate->blender2a_r[0];
                g = *wstate->blender2a_g[0];
                b = *wstate->blender2a_b[0];
            }

            if (wstate->other_modes.rgb_dither_sel != 3)
                rgb_dither(wstate->other_modes.rgb_dither_sel, &r, &g, &b, dith);

            *fr = r;
            *fg = g;
            *fb = b;
            return 1;
        }
        else
            return 0;
        }
    else
        return 0;
}

static STRICTINLINE int blender_2cycle_cycle0(struct rdp_state* wstate, uint32_t curpixel_cvg, uint32_t curpixel_cvbit)
{
    int r, g, b;
    int wen = (wstate->other_modes.antialias_en ? curpixel_cvg : curpixel_cvbit) > 0 ? 1 : 0;

    if (wen)
    {
        wstate->inv_pixel_color.a =  (~(*wstate->blender1b_a[0])) & 0xff;

        blender_equation_cycle0_2(wstate, &r, &g, &b);

        wstate->blended_pixel_color.r = r;
        wstate->blended_pixel_color.g = g;
        wstate->blended_pixel_color.b = b;
    }

    wstate->memory_color = wstate->pre_memory_color;

    return wen;
}


static STRICTINLINE void blender_2cycle_cycle1(struct rdp_state* wstate, uint32_t* fr, uint32_t* fg, uint32_t* fb, int dith, uint32_t blend_en, uint32_t prewrap)
{
    int r, g, b, dontblend;

    if (!wstate->other_modes.color_on_cvg || prewrap)
    {
        dontblend = (wstate->other_modes.f.partialreject_2cycle && wstate->pixel_color.a >= 0xff);
        if (!blend_en || dontblend)
        {
            r = *wstate->blender1a_r[1];
            g = *wstate->blender1a_g[1];
            b = *wstate->blender1a_b[1];
        }
        else
        {
            wstate->inv_pixel_color.a =  (~(*wstate->blender1b_a[1])) & 0xff;
            blender_equation_cycle1(wstate, &r, &g, &b);
        }
    }
    else
    {
        r = *wstate->blender2a_r[1];
        g = *wstate->blender2a_g[1];
        b = *wstate->blender2a_b[1];
    }

    if (wstate->other_modes.rgb_dither_sel != 3)
        rgb_dither(wstate->other_modes.rgb_dither_sel, &r, &g, &b, dith);

    *fr = r;
    *fg = g;
    *fb = b;
}

static void blender_init_lut(void)
{
    int i, k;
    int d = 0, n = 0, temp = 0, res = 0, invd = 0, nbit = 0;
    int ps[9];
    for (i = 0; i < 0x8000; i++)
    {
        res = 0;
        d = (i >> 11) & 0xf;
        n = i & 0x7ff;
        invd = (~d) & 0xf;


        temp = invd + (n >> 8) + 1;
        ps[0] = temp & 7;
        for (k = 0; k < 8; k++)
        {
            nbit = (n >> (7 - k)) & 1;
            if (res & (0x100 >> k))
                temp = invd + (ps[k] << 1) + nbit + 1;
            else
                temp = d + (ps[k] << 1) + nbit;
            ps[k + 1] = temp & 7;
            if (temp & 0x10)
                res |= (1 << (7 - k));
        }
        bldiv_hwaccurate_table[i] = (uint8_t)res;
    }
}

void rdp_set_fog_color(struct rdp_state* wstate, const uint32_t* args)
{
    wstate->fog_color.r = RGBA32_R(args[1]);
    wstate->fog_color.g = RGBA32_G(args[1]);
    wstate->fog_color.b = RGBA32_B(args[1]);
    wstate->fog_color.a = RGBA32_A(args[1]);
}

void rdp_set_blend_color(struct rdp_state* wstate, const uint32_t* args)
{
    wstate->blend_color.r = RGBA32_R(args[1]);
    wstate->blend_color.g = RGBA32_G(args[1]);
    wstate->blend_color.b = RGBA32_B(args[1]);
    wstate->blend_color.a = RGBA32_A(args[1]);
}

#endif // N64VIDEO_C
