#ifdef N64VIDEO_C

static STRICTINLINE int32_t normalize_dzpix(int32_t sum)
{
    int count;
    if (sum & 0xc000)
        return 0x8000;
    if (!(sum & 0xffff))
        return 1;

    if (sum == 1)
        return 3;

    for(count = 0x2000; count > 0; count >>= 1)
    {
        if (sum & count)
            return (count << 1);
    }

    return 0;
}

static void replicate_for_copy(struct rdp_state* wstate, uint32_t* outbyte, uint32_t inshort, uint32_t nybbleoffset, uint32_t tilenum, uint32_t tformat, uint32_t tsize)
{
    uint32_t lownib, hinib;
    switch(tsize)
    {
    case PIXEL_SIZE_4BIT:
        lownib = (nybbleoffset ^ 3) << 2;
        lownib = hinib = (inshort >> lownib) & 0xf;
        if (tformat == FORMAT_CI)
        {
            *outbyte = (wstate->tile[tilenum].palette << 4) | lownib;
        }
        else if (tformat == FORMAT_IA)
        {
            lownib = (lownib << 4) | lownib;
            *outbyte = (lownib & 0xe0) | ((lownib & 0xe0) >> 3) | ((lownib & 0xc0) >> 6);
        }
        else
            *outbyte = (lownib << 4) | lownib;
        break;
    case PIXEL_SIZE_8BIT:
        hinib = ((nybbleoffset ^ 3) | 1) << 2;
        if (tformat == FORMAT_IA)
        {
            lownib = (inshort >> hinib) & 0xf;
            *outbyte = (lownib << 4) | lownib;
        }
        else
        {
            lownib = (inshort >> (hinib & ~4)) & 0xf;
            hinib = (inshort >> hinib) & 0xf;
            *outbyte = (hinib << 4) | lownib;
        }
        break;
    default:
        *outbyte = (inshort >> 8) & 0xff;
        break;
    }
}

static void fetch_qword_copy(struct rdp_state* wstate, uint32_t* hidword, uint32_t* lowdword, int32_t ssss, int32_t ssst, uint32_t tilenum)
{
    uint32_t shorta, shortb, shortc, shortd;
    uint32_t sortshort[8];
    int hibits[6];
    int lowbits[6];
    int32_t sss = ssss, sst = ssst, sss1 = 0, sss2 = 0, sss3 = 0;
    int largetex = 0;

    uint32_t tformat, tsize;
    if (wstate->other_modes.en_tlut)
    {
        tsize = PIXEL_SIZE_16BIT;
        tformat = wstate->other_modes.tlut_type ? FORMAT_IA : FORMAT_RGBA;
    }
    else
    {
        tsize = wstate->tile[tilenum].size;
        tformat = wstate->tile[tilenum].format;
    }

    tc_pipeline_copy(&wstate->tile[tilenum], &sss, &sss1, &sss2, &sss3, &sst);
    read_tmem_copy(wstate, sss, sss1, sss2, sss3, sst, tilenum, sortshort, hibits, lowbits);
    largetex = (tformat == FORMAT_YUV || (tformat == FORMAT_RGBA && tsize == PIXEL_SIZE_32BIT));


    if (wstate->other_modes.en_tlut)
    {
        shorta = sortshort[4];
        shortb = sortshort[5];
        shortc = sortshort[6];
        shortd = sortshort[7];
    }
    else if (largetex)
    {
        shorta = sortshort[0];
        shortb = sortshort[1];
        shortc = sortshort[2];
        shortd = sortshort[3];
    }
    else
    {
        shorta = hibits[0] ? sortshort[4] : sortshort[0];
        shortb = hibits[1] ? sortshort[5] : sortshort[1];
        shortc = hibits[3] ? sortshort[6] : sortshort[2];
        shortd = hibits[4] ? sortshort[7] : sortshort[3];
    }

    *lowdword = (shortc << 16) | shortd;

    if (tsize == PIXEL_SIZE_16BIT)
        *hidword = (shorta << 16) | shortb;
    else
    {
        replicate_for_copy(wstate, &shorta, shorta, lowbits[0] & 3, tilenum, tformat, tsize);
        replicate_for_copy(wstate, &shortb, shortb, lowbits[1] & 3, tilenum, tformat, tsize);
        replicate_for_copy(wstate, &shortc, shortc, lowbits[3] & 3, tilenum, tformat, tsize);
        replicate_for_copy(wstate, &shortd, shortd, lowbits[4] & 3, tilenum, tformat, tsize);
        *hidword = (shorta << 24) | (shortb << 16) | (shortc << 8) | shortd;
    }
}

static STRICTINLINE void rgba_correct(struct rdp_state* wstate, int offx, int offy, int r, int g, int b, int a, uint32_t cvg)
{
    int summand_r, summand_b, summand_g, summand_a;



    if (cvg == 8)
    {
        r >>= 2;
        g >>= 2;
        b >>= 2;
        a >>= 2;
    }
    else
    {
        summand_r = offx * wstate->spans_cdr + offy * wstate->spans_drdy;
        summand_g = offx * wstate->spans_cdg + offy * wstate->spans_dgdy;
        summand_b = offx * wstate->spans_cdb + offy * wstate->spans_dbdy;
        summand_a = offx * wstate->spans_cda + offy * wstate->spans_dady;

        r = ((r << 2) + summand_r) >> 4;
        g = ((g << 2) + summand_g) >> 4;
        b = ((b << 2) + summand_b) >> 4;
        a = ((a << 2) + summand_a) >> 4;
    }


    wstate->shade_color.r = special_9bit_clamptable[r & 0x1ff];
    wstate->shade_color.g = special_9bit_clamptable[g & 0x1ff];
    wstate->shade_color.b = special_9bit_clamptable[b & 0x1ff];
    wstate->shade_color.a = special_9bit_clamptable[a & 0x1ff];
}

static STRICTINLINE void z_correct(struct rdp_state* wstate, int offx, int offy, int* z, uint32_t cvg)
{
    int summand_z;
    int sz = *z;
    int zanded;



    if (cvg == 8)
        sz = sz >> 3;
    else
    {
        summand_z = offx * wstate->spans_cdz + offy * wstate->spans_dzdy;

        sz = ((sz << 2) + summand_z) >> 5;
    }



    zanded = (sz & 0x60000) >> 17;


    switch (zanded)
    {
        case 0: *z = sz & 0x3ffff;                      break;
        case 1: *z = sz & 0x3ffff;                      break;
        case 2: *z = 0x3ffff;                           break;
        case 3: *z = 0;                                 break;
    }
}

static void render_spans_1cycle_complete(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    int zb = wstate->zb_address >> 1;
    int zbcur;
    uint8_t offx = 0;
    uint8_t offy = 0;
    struct spansigs sigs;
    uint32_t blend_en;
    uint32_t prewrap;
    uint32_t curpixel_cvg, curpixel_cvbit, curpixel_memcvg;

    int prim_tile = tilenum;
    int tile1 = tilenum;
    int newtile = tilenum;
    int news, newt;

    int i, j;

    int drinc, dginc, dbinc, dainc, dzinc, dsinc, dtinc, dwinc;
    int xinc;

    if (flip)
    {
        drinc = wstate->spans_dr;
        dginc = wstate->spans_dg;
        dbinc = wstate->spans_db;
        dainc = wstate->spans_da;
        dzinc = wstate->spans_dz;
        dsinc = wstate->spans_ds;
        dtinc = wstate->spans_dt;
        dwinc = wstate->spans_dw;
        xinc = 1;
    }
    else
    {
        drinc = -wstate->spans_dr;
        dginc = -wstate->spans_dg;
        dbinc = -wstate->spans_db;
        dainc = -wstate->spans_da;
        dzinc = -wstate->spans_dz;
        dsinc = -wstate->spans_ds;
        dtinc = -wstate->spans_dt;
        dwinc = -wstate->spans_dw;
        xinc = -1;
    }

    uint16_t dzpix;
    if (!wstate->other_modes.z_source_sel)
        dzpix = (uint16_t)wstate->spans_dzpix;
    else
    {
        dzpix = (uint16_t)wstate->primitive_delta_z;
        dzinc = wstate->spans_cdz = wstate->spans_dzdy = 0;
    }
    int dzpixenc = dz_compress(dzpix);

    int cdith = 7, adith = 0;
    int r, g, b, a, z, s, t, w;
    int sr, sg, sb, sa, sz, ss, st, sw;
    int xstart, xend, xendsc;
    int sss = 0, sst = 0;
    int32_t prelodfrac = 0;
    int curpixel = 0;
    int x, length, scdiff, lodlength;
    uint32_t fir, fig, fib;

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        r = wstate->span[i].r;
        g = wstate->span[i].g;
        b = wstate->span[i].b;
        a = wstate->span[i].a;
        z = wstate->other_modes.z_source_sel ? wstate->primitive_z : wstate->span[i].z;
        s = wstate->span[i].s;
        t = wstate->span[i].t;
        w = wstate->span[i].w;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        zbcur = zb + curpixel;

        if (!flip)
        {
            length = xendsc - xstart;
            scdiff = xend - xendsc;
            compute_cvg_noflip(wstate, i);
        }
        else
        {
            length = xstart - xendsc;
            scdiff = xendsc - xend;
            compute_cvg_flip(wstate, i);
        }



        if (scdiff)
        {


            scdiff &= 0xfff;
            r += (drinc * scdiff);
            g += (dginc * scdiff);
            b += (dbinc * scdiff);
            a += (dainc * scdiff);
            z += (dzinc * scdiff);
            s += (dsinc * scdiff);
            t += (dtinc * scdiff);
            w += (dwinc * scdiff);
        }

        lodlength = length + scdiff;

        sigs.longspan = (lodlength > 7);
        sigs.midspan = (lodlength == 7);
        sigs.onelessthanmid = (lodlength == 6);

        for (j = 0; j <= length; j++)
        {
            sr = r >> 14;
            sg = g >> 14;
            sb = b >> 14;
            sa = a >> 14;
            ss = s >> 16;
            st = t >> 16;
            sw = w >> 16;
            sz = (z >> 10) & 0x3fffff;


            sigs.endspan = (j == length);
            sigs.preendspan = (j == (length - 1));

            lookup_cvmask_derivatives(wstate->cvgbuf[x], &offx, &offy, &curpixel_cvg, &curpixel_cvbit);


            get_texel1_1cycle(wstate, &news, &newt, s, t, w, dsinc, dtinc, dwinc, i, &sigs);



            if (j)
            {
                wstate->texel0_color = wstate->texel1_color;
                wstate->lod_frac = prelodfrac;
            }
            else
            {
                wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);


                tclod_1cycle_current(wstate, &sss, &sst, news, newt, s, t, w, dsinc, dtinc, dwinc, i, prim_tile, &tile1, &sigs);




                texture_pipeline_cycle(wstate, &wstate->texel0_color, &wstate->texel0_color, sss, sst, tile1, 0);
            }

            sigs.nextspan = sigs.endspan;
            sigs.endspan = sigs.preendspan;
            sigs.preendspan = (j == (length - 2));

            s += dsinc;
            t += dtinc;
            w += dwinc;

            tclod_1cycle_next(wstate, &news, &newt, s, t, w, dsinc, dtinc, dwinc, i, prim_tile, &newtile, &sigs, &prelodfrac);

            texture_pipeline_cycle(wstate, &wstate->texel1_color, &wstate->texel1_color, news, newt, newtile, 0);

            rgba_correct(wstate, offx, offy, sr, sg, sb, sa, curpixel_cvg);
            z_correct(wstate, offx, offy, &sz, curpixel_cvg);

            if (wstate->other_modes.f.getditherlevel < 2)
                get_dither_noise(wstate, x, i, &cdith, &adith);

            combiner_1cycle(wstate, adith, &curpixel_cvg);

            wstate->fbread1_ptr(wstate, curpixel, &curpixel_memcvg);
            if (z_compare(wstate, zbcur, sz, dzpix, dzpixenc, &blend_en, &prewrap, &curpixel_cvg, curpixel_memcvg))
            {
                if (blender_1cycle(wstate, &fir, &fig, &fib, cdith, blend_en, prewrap, curpixel_cvg, curpixel_cvbit))
                {
                    wstate->fbwrite_ptr(wstate, curpixel, fir, fig, fib, blend_en, curpixel_cvg, curpixel_memcvg);
                    if (wstate->other_modes.z_update_en)
                        z_store(zbcur, sz, dzpixenc);
                }
            }




            r += drinc;
            g += dginc;
            b += dbinc;
            a += dainc;
            z += dzinc;

            x += xinc;
            curpixel += xinc;
            zbcur += xinc;
        }
        }
    }
}


static void render_spans_1cycle_notexel1(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    int zb = wstate->zb_address >> 1;
    int zbcur;
    uint8_t offx = 0;
    uint8_t offy = 0;
    struct spansigs sigs;
    uint32_t blend_en;
    uint32_t prewrap;
    uint32_t curpixel_cvg, curpixel_cvbit, curpixel_memcvg;

    int prim_tile = tilenum;
    int tile1 = tilenum;

    int i, j;

    int drinc, dginc, dbinc, dainc, dzinc, dsinc, dtinc, dwinc;
    int xinc;
    if (flip)
    {
        drinc = wstate->spans_dr;
        dginc = wstate->spans_dg;
        dbinc = wstate->spans_db;
        dainc = wstate->spans_da;
        dzinc = wstate->spans_dz;
        dsinc = wstate->spans_ds;
        dtinc = wstate->spans_dt;
        dwinc = wstate->spans_dw;
        xinc = 1;
    }
    else
    {
        drinc = -wstate->spans_dr;
        dginc = -wstate->spans_dg;
        dbinc = -wstate->spans_db;
        dainc = -wstate->spans_da;
        dzinc = -wstate->spans_dz;
        dsinc = -wstate->spans_ds;
        dtinc = -wstate->spans_dt;
        dwinc = -wstate->spans_dw;
        xinc = -1;
    }

    uint16_t dzpix;
    if (!wstate->other_modes.z_source_sel)
        dzpix = (uint16_t)wstate->spans_dzpix;
    else
    {
        dzpix = (uint16_t)wstate->primitive_delta_z;
        dzinc = wstate->spans_cdz = wstate->spans_dzdy = 0;
    }
    int dzpixenc = dz_compress(dzpix);

    int cdith = 7, adith = 0;
    int r, g, b, a, z, s, t, w;
    int sr, sg, sb, sa, sz, ss, st, sw;
    int xstart, xend, xendsc;
    int sss = 0, sst = 0;
    int curpixel = 0;
    int x, length, scdiff, lodlength;
    uint32_t fir, fig, fib;

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        r = wstate->span[i].r;
        g = wstate->span[i].g;
        b = wstate->span[i].b;
        a = wstate->span[i].a;
        z = wstate->other_modes.z_source_sel ? wstate->primitive_z : wstate->span[i].z;
        s = wstate->span[i].s;
        t = wstate->span[i].t;
        w = wstate->span[i].w;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        zbcur = zb + curpixel;

        if (!flip)
        {
            length = xendsc - xstart;
            scdiff = xend - xendsc;
            compute_cvg_noflip(wstate, i);
        }
        else
        {
            length = xstart - xendsc;
            scdiff = xendsc - xend;
            compute_cvg_flip(wstate, i);
        }

        if (scdiff)
        {
            scdiff &= 0xfff;
            r += (drinc * scdiff);
            g += (dginc * scdiff);
            b += (dbinc * scdiff);
            a += (dainc * scdiff);
            z += (dzinc * scdiff);
            s += (dsinc * scdiff);
            t += (dtinc * scdiff);
            w += (dwinc * scdiff);
        }

        lodlength = length + scdiff;

        sigs.longspan = (lodlength > 7);
        sigs.midspan = (lodlength == 7);

        for (j = 0; j <= length; j++)
        {
            sr = r >> 14;
            sg = g >> 14;
            sb = b >> 14;
            sa = a >> 14;
            ss = s >> 16;
            st = t >> 16;
            sw = w >> 16;
            sz = (z >> 10) & 0x3fffff;



            sigs.endspan = (j == length);
            sigs.preendspan = (j == (length - 1));

            lookup_cvmask_derivatives(wstate->cvgbuf[x], &offx, &offy, &curpixel_cvg, &curpixel_cvbit);

            wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

            tclod_1cycle_current_simple(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, i, prim_tile, &tile1, &sigs);

            texture_pipeline_cycle(wstate, &wstate->texel0_color, &wstate->texel0_color, sss, sst, tile1, 0);

            rgba_correct(wstate, offx, offy, sr, sg, sb, sa, curpixel_cvg);
            z_correct(wstate, offx, offy, &sz, curpixel_cvg);

            if (wstate->other_modes.f.getditherlevel < 2)
                get_dither_noise(wstate, x, i, &cdith, &adith);

            combiner_1cycle(wstate, adith, &curpixel_cvg);

            wstate->fbread1_ptr(wstate, curpixel, &curpixel_memcvg);
            if (z_compare(wstate, zbcur, sz, dzpix, dzpixenc, &blend_en, &prewrap, &curpixel_cvg, curpixel_memcvg))
            {
                if (blender_1cycle(wstate, &fir, &fig, &fib, cdith, blend_en, prewrap, curpixel_cvg, curpixel_cvbit))
                {
                    wstate->fbwrite_ptr(wstate, curpixel, fir, fig, fib, blend_en, curpixel_cvg, curpixel_memcvg);
                    if (wstate->other_modes.z_update_en)
                        z_store(zbcur, sz, dzpixenc);
                }
            }

            s += dsinc;
            t += dtinc;
            w += dwinc;
            r += drinc;
            g += dginc;
            b += dbinc;
            a += dainc;
            z += dzinc;

            x += xinc;
            curpixel += xinc;
            zbcur += xinc;
        }
        }
    }
}


static void render_spans_1cycle_notex(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    UNUSED(tilenum);

    int zb = wstate->zb_address >> 1;
    int zbcur;
    uint8_t offx = 0;
    uint8_t offy = 0;
    uint32_t blend_en;
    uint32_t prewrap;
    uint32_t curpixel_cvg, curpixel_cvbit, curpixel_memcvg;

    int i, j;

    int drinc, dginc, dbinc, dainc, dzinc;
    int xinc;

    if (flip)
    {
        drinc = wstate->spans_dr;
        dginc = wstate->spans_dg;
        dbinc = wstate->spans_db;
        dainc = wstate->spans_da;
        dzinc = wstate->spans_dz;
        xinc = 1;
    }
    else
    {
        drinc = -wstate->spans_dr;
        dginc = -wstate->spans_dg;
        dbinc = -wstate->spans_db;
        dainc = -wstate->spans_da;
        dzinc = -wstate->spans_dz;
        xinc = -1;
    }

    uint16_t dzpix;
    if (!wstate->other_modes.z_source_sel)
        dzpix = (uint16_t)wstate->spans_dzpix;
    else
    {
        dzpix = (uint16_t)wstate->primitive_delta_z;
        dzinc = wstate->spans_cdz = wstate->spans_dzdy = 0;
    }
    int dzpixenc = dz_compress(dzpix);

    int cdith = 7, adith = 0;
    int r, g, b, a, z;
    int sr, sg, sb, sa, sz;
    int xstart, xend, xendsc;
    int curpixel = 0;
    int x, length, scdiff;
    uint32_t fir, fig, fib;

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        r = wstate->span[i].r;
        g = wstate->span[i].g;
        b = wstate->span[i].b;
        a = wstate->span[i].a;
        z = wstate->other_modes.z_source_sel ? wstate->primitive_z : wstate->span[i].z;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        zbcur = zb + curpixel;

        if (!flip)
        {
            length = xendsc - xstart;
            scdiff = xend - xendsc;
            compute_cvg_noflip(wstate, i);
        }
        else
        {
            length = xstart - xendsc;
            scdiff = xendsc - xend;
            compute_cvg_flip(wstate, i);
        }

        if (scdiff)
        {
            scdiff &= 0xfff;
            r += (drinc * scdiff);
            g += (dginc * scdiff);
            b += (dbinc * scdiff);
            a += (dainc * scdiff);
            z += (dzinc * scdiff);
        }

        for (j = 0; j <= length; j++)
        {
            sr = r >> 14;
            sg = g >> 14;
            sb = b >> 14;
            sa = a >> 14;
            sz = (z >> 10) & 0x3fffff;

            lookup_cvmask_derivatives(wstate->cvgbuf[x], &offx, &offy, &curpixel_cvg, &curpixel_cvbit);

            rgba_correct(wstate, offx, offy, sr, sg, sb, sa, curpixel_cvg);
            z_correct(wstate, offx, offy, &sz, curpixel_cvg);

            if (wstate->other_modes.f.getditherlevel < 2)
                get_dither_noise(wstate, x, i, &cdith, &adith);

            combiner_1cycle(wstate, adith, &curpixel_cvg);

            wstate->fbread1_ptr(wstate, curpixel, &curpixel_memcvg);
            if (z_compare(wstate, zbcur, sz, dzpix, dzpixenc, &blend_en, &prewrap, &curpixel_cvg, curpixel_memcvg))
            {
                if (blender_1cycle(wstate, &fir, &fig, &fib, cdith, blend_en, prewrap, curpixel_cvg, curpixel_cvbit))
                {
                    wstate->fbwrite_ptr(wstate, curpixel, fir, fig, fib, blend_en, curpixel_cvg, curpixel_memcvg);
                    if (wstate->other_modes.z_update_en)
                        z_store(zbcur, sz, dzpixenc);
                }
            }
            r += drinc;
            g += dginc;
            b += dbinc;
            a += dainc;
            z += dzinc;

            x += xinc;
            curpixel += xinc;
            zbcur += xinc;
        }
        }
    }
}

static void render_spans_2cycle_complete(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    int zb = wstate->zb_address >> 1;
    int zbcur;
    uint8_t offx = 0;
    uint8_t offy = 0;
    int32_t prelodfrac;
    struct color nexttexel1_color;
    uint32_t blend_en;
    uint32_t prewrap;
    uint32_t curpixel_cvg = 0, curpixel_cvbit = 0, curpixel_memcvg = 0;
    uint32_t nextpixel_cvg;
    uint32_t acalpha;



    int tile2 = (tilenum + 1) & 7;
    int tile1 = tilenum;
    int prim_tile = tilenum;
    int tile3 = tilenum;

    int i, j;

    int drinc, dginc, dbinc, dainc, dzinc, dsinc, dtinc, dwinc;
    int xinc;
    if (flip)
    {
        drinc = wstate->spans_dr;
        dginc = wstate->spans_dg;
        dbinc = wstate->spans_db;
        dainc = wstate->spans_da;
        dzinc = wstate->spans_dz;
        dsinc = wstate->spans_ds;
        dtinc = wstate->spans_dt;
        dwinc = wstate->spans_dw;
        xinc = 1;
    }
    else
    {
        drinc = -wstate->spans_dr;
        dginc = -wstate->spans_dg;
        dbinc = -wstate->spans_db;
        dainc = -wstate->spans_da;
        dzinc = -wstate->spans_dz;
        dsinc = -wstate->spans_ds;
        dtinc = -wstate->spans_dt;
        dwinc = -wstate->spans_dw;
        xinc = -1;
    }

    uint16_t dzpix;
    if (!wstate->other_modes.z_source_sel)
        dzpix = (uint16_t)wstate->spans_dzpix;
    else
    {
        dzpix = (uint16_t)wstate->primitive_delta_z;
        dzinc = wstate->spans_cdz = wstate->spans_dzdy = 0;
    }
    int dzpixenc = dz_compress(dzpix);

    int cdith = 7, adith = 0;

    int r, g, b, a, z, s, t, w;
    int sr, sg, sb, sa, sz, ss, st, sw;
    int xstart, xend, xendsc;
    int sss = 0, sst = 0;
    uint32_t curpixel = 0;
    int wen;

    int x, length, scdiff, lodlength;
    uint32_t fir, fig, fib;

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        r = wstate->span[i].r;
        g = wstate->span[i].g;
        b = wstate->span[i].b;
        a = wstate->span[i].a;
        z = wstate->other_modes.z_source_sel ? wstate->primitive_z : wstate->span[i].z;
        s = wstate->span[i].s;
        t = wstate->span[i].t;
        w = wstate->span[i].w;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        zbcur = zb + curpixel;

        if (!flip)
        {
            length = xendsc - xstart;
            scdiff = xend - xendsc;
            compute_cvg_noflip(wstate, i);
        }
        else
        {
            length = xstart - xendsc;
            scdiff = xendsc - xend;
            compute_cvg_flip(wstate, i);
        }








        if (scdiff)
        {
            scdiff &= 0xfff;
            r += (drinc * scdiff);
            g += (dginc * scdiff);
            b += (dbinc * scdiff);
            a += (dainc * scdiff);
            z += (dzinc * scdiff);
            s += (dsinc * scdiff);
            t += (dtinc * scdiff);
            w += (dwinc * scdiff);
        }

        lodlength = length + scdiff;

        for (j = 0; j <= length; j++)
        {
            sz = (z >> 10) & 0x3fffff;

            if (!j)
            {
                sr = r >> 14;
                sg = g >> 14;
                sb = b >> 14;
                sa = a >> 14;
                ss = s >> 16;
                st = t >> 16;
                sw = w >> 16;

                wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

                tclod_2cycle(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1, &tile2, &wstate->lod_frac);

                texture_pipeline_cycle(wstate, &wstate->texel0_color, &wstate->texel0_color, sss, sst, tile1, 0);
                texture_pipeline_cycle(wstate, &wstate->texel1_color, &wstate->texel0_color, sss, sst, tile2, 1);

                lookup_cvmask_derivatives(wstate->cvgbuf[x], &offx, &offy, &curpixel_cvg, &curpixel_cvbit);

                rgba_correct(wstate, offx, offy, sr, sg, sb, sa, curpixel_cvg);

                if (wstate->other_modes.f.getditherlevel < 2)
                    get_dither_noise(wstate, x, i, &cdith, &adith);

                combiner_2cycle_cycle0(wstate, adith, curpixel_cvg, &acalpha);
            }



            s += dsinc;
            t += dtinc;
            w += dwinc;

            ss = s >> 16;
            st = t >> 16;
            sw = w >> 16;

            wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

            if (j < length || !wstate->span[i + 1].validline || lodlength < 3)
            {
                tclod_2cycle(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1, &tile2, &prelodfrac);

                texture_pipeline_cycle(wstate, &wstate->nexttexel_color, &wstate->nexttexel_color, sss, sst, tile1, 0);
                texture_pipeline_cycle(wstate, &nexttexel1_color, &wstate->nexttexel_color, sss, sst, tile2, 1);
            }
            else
            {
                int sss2, sst2;

                ss = wstate->span[i + 1].s >> 16;
                st = wstate->span[i + 1].t >> 16;
                sw = wstate->span[i + 1].w >> 16;
                wstate->tcdiv_ptr(ss, st, sw, &sss2, &sst2);

                tclod_2cycle_next(wstate, &sss, &sst, &sss2, &sst2, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1, &tile3, &prelodfrac, i);

                texture_pipeline_cycle(wstate, &wstate->nexttexel_color, &wstate->nexttexel_color, sss, sst, tile1, 0);
                texture_pipeline_cycle(wstate, &nexttexel1_color, &wstate->nexttexel_color, sss2, sst2, tile3, 0);
            }

            z_correct(wstate, offx, offy, &sz, curpixel_cvg);

            combiner_2cycle_cycle1(wstate, adith, &curpixel_cvg);

            wstate->fbread2_ptr(wstate, curpixel, &curpixel_memcvg);


            wen = z_compare(wstate, zbcur, sz, dzpix, dzpixenc, &blend_en, &prewrap, &curpixel_cvg, curpixel_memcvg);

            if (wen)
                wen &= blender_2cycle_cycle0(wstate, curpixel_cvg, curpixel_cvbit);
            else
                wstate->memory_color = wstate->pre_memory_color;






            x += xinc;

            r += drinc;
            g += dginc;
            b += dbinc;
            a += dainc;

            sr = r >> 14;
            sg = g >> 14;
            sb = b >> 14;
            sa = a >> 14;




            lookup_cvmask_derivatives(j < length ? wstate->cvgbuf[x] : 0, &offx, &offy, &nextpixel_cvg, &curpixel_cvbit);

            rgba_correct(wstate, offx, offy, sr, sg, sb, sa, nextpixel_cvg);

            wstate->lod_frac = prelodfrac;
            wstate->texel0_color = wstate->nexttexel_color;
            wstate->texel1_color = nexttexel1_color;


            combiner_2cycle_cycle0(wstate, adith, nextpixel_cvg, &acalpha);

            if (wen)
            {
                wen &= alpha_compare(wstate, acalpha);




                if (wen)
                {
                    blender_2cycle_cycle1(wstate, &fir, &fig, &fib, cdith, blend_en, prewrap);
                    wstate->fbwrite_ptr(wstate, curpixel, fir, fig, fib, blend_en, curpixel_cvg, curpixel_memcvg);
                    if (wstate->other_modes.z_update_en)
                        z_store(zbcur, sz, dzpixenc);
                }
            }

            if (wstate->other_modes.f.getditherlevel < 2)
                get_dither_noise(wstate, x, i, &cdith, &adith);

            curpixel_cvg = nextpixel_cvg;






            z += dzinc;

            curpixel += xinc;
            zbcur += xinc;
        }
        }
    }
}



static void render_spans_2cycle_notexelnext(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    int zb = wstate->zb_address >> 1;
    int zbcur;
    uint8_t offx = 0;
    uint8_t offy = 0;
    uint32_t blend_en;
    uint32_t prewrap;
    uint32_t curpixel_cvg = 0, curpixel_cvbit = 0, curpixel_memcvg = 0;
    uint32_t nextpixel_cvg;
    uint32_t acalpha;

    int tile2 = (tilenum + 1) & 7;
    int tile1 = tilenum;
    int prim_tile = tilenum;

    int i, j;

    int drinc, dginc, dbinc, dainc, dzinc, dsinc, dtinc, dwinc;
    int xinc;
    if (flip)
    {
        drinc = wstate->spans_dr;
        dginc = wstate->spans_dg;
        dbinc = wstate->spans_db;
        dainc = wstate->spans_da;
        dzinc = wstate->spans_dz;
        dsinc = wstate->spans_ds;
        dtinc = wstate->spans_dt;
        dwinc = wstate->spans_dw;
        xinc = 1;
    }
    else
    {
        drinc = -wstate->spans_dr;
        dginc = -wstate->spans_dg;
        dbinc = -wstate->spans_db;
        dainc = -wstate->spans_da;
        dzinc = -wstate->spans_dz;
        dsinc = -wstate->spans_ds;
        dtinc = -wstate->spans_dt;
        dwinc = -wstate->spans_dw;
        xinc = -1;
    }

    uint16_t dzpix;
    if (!wstate->other_modes.z_source_sel)
        dzpix = (uint16_t)wstate->spans_dzpix;
    else
    {
        dzpix = (uint16_t)wstate->primitive_delta_z;
        dzinc = wstate->spans_cdz = wstate->spans_dzdy = 0;
    }
    int dzpixenc = dz_compress(dzpix);

    int cdith = 7, adith = 0;

    int r, g, b, a, z, s, t, w;
    int sr, sg, sb, sa, sz, ss, st, sw;
    int xstart, xend, xendsc;
    int sss = 0, sst = 0;
    int curpixel = 0;
    int wen;

    int x, length, scdiff;
    uint32_t fir, fig, fib;

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        r = wstate->span[i].r;
        g = wstate->span[i].g;
        b = wstate->span[i].b;
        a = wstate->span[i].a;
        z = wstate->other_modes.z_source_sel ? wstate->primitive_z : wstate->span[i].z;
        s = wstate->span[i].s;
        t = wstate->span[i].t;
        w = wstate->span[i].w;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        zbcur = zb + curpixel;

        if (!flip)
        {
            length = xendsc - xstart;
            scdiff = xend - xendsc;
            compute_cvg_noflip(wstate, i);
        }
        else
        {
            length = xstart - xendsc;
            scdiff = xendsc - xend;
            compute_cvg_flip(wstate, i);
        }

        if (scdiff)
        {
            scdiff &= 0xfff;
            r += (drinc * scdiff);
            g += (dginc * scdiff);
            b += (dbinc * scdiff);
            a += (dainc * scdiff);
            z += (dzinc * scdiff);
            s += (dsinc * scdiff);
            t += (dtinc * scdiff);
            w += (dwinc * scdiff);
        }

        for (j = 0; j <= length; j++)
        {
            sz = (z >> 10) & 0x3fffff;

            if (!j)
            {
                sr = r >> 14;
                sg = g >> 14;
                sb = b >> 14;
                sa = a >> 14;
                ss = s >> 16;
                st = t >> 16;
                sw = w >> 16;

                wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

                tclod_2cycle(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1, &tile2, &wstate->lod_frac);

                texture_pipeline_cycle(wstate, &wstate->texel0_color, &wstate->texel0_color, sss, sst, tile1, 0);
                texture_pipeline_cycle(wstate, &wstate->texel1_color, &wstate->texel0_color, sss, sst, tile2, 1);

                lookup_cvmask_derivatives(wstate->cvgbuf[x], &offx, &offy, &curpixel_cvg, &curpixel_cvbit);

                rgba_correct(wstate, offx, offy, sr, sg, sb, sa, curpixel_cvg);

                if (wstate->other_modes.f.getditherlevel < 2)
                    get_dither_noise(wstate, x, i, &cdith, &adith);

                combiner_2cycle_cycle0(wstate, adith, curpixel_cvg, &acalpha);
            }

            z_correct(wstate, offx, offy, &sz, curpixel_cvg);

            combiner_2cycle_cycle1(wstate, adith, &curpixel_cvg);

            wstate->fbread2_ptr(wstate, curpixel, &curpixel_memcvg);

            wen = z_compare(wstate, zbcur, sz, dzpix, dzpixenc, &blend_en, &prewrap, &curpixel_cvg, curpixel_memcvg);

            if (wen)
                wen &= blender_2cycle_cycle0(wstate, curpixel_cvg, curpixel_cvbit);
            else
                wstate->memory_color = wstate->pre_memory_color;

            x += xinc;

            r += drinc;
            g += dginc;
            b += dbinc;
            a += dainc;
            s += dsinc;
            t += dtinc;
            w += dwinc;

            sr = r >> 14;
            sg = g >> 14;
            sb = b >> 14;
            sa = a >> 14;
            ss = s >> 16;
            st = t >> 16;
            sw = w >> 16;

            lookup_cvmask_derivatives(j < length ? wstate->cvgbuf[x] : 0, &offx, &offy, &nextpixel_cvg, &curpixel_cvbit);

            rgba_correct(wstate, offx, offy, sr, sg, sb, sa, nextpixel_cvg);

            wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

            tclod_2cycle(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1, &tile2, &wstate->lod_frac);

            texture_pipeline_cycle(wstate, &wstate->texel0_color, &wstate->texel0_color, sss, sst, tile1, 0);
            texture_pipeline_cycle(wstate, &wstate->texel1_color, &wstate->texel0_color, sss, sst, tile2, 1);

            combiner_2cycle_cycle0(wstate, adith, nextpixel_cvg, &acalpha);

            if (wen)
            {
                wen &= alpha_compare(wstate, acalpha);

                if (wen)
                {
                    blender_2cycle_cycle1(wstate, &fir, &fig, &fib, cdith, blend_en, prewrap);
                    wstate->fbwrite_ptr(wstate, curpixel, fir, fig, fib, blend_en, curpixel_cvg, curpixel_memcvg);
                    if (wstate->other_modes.z_update_en)
                        z_store(zbcur, sz, dzpixenc);
                }
            }

            if (wstate->other_modes.f.getditherlevel < 2)
                get_dither_noise(wstate, x, i, &cdith, &adith);

            curpixel_cvg = nextpixel_cvg;

            z += dzinc;

            curpixel += xinc;
            zbcur += xinc;
        }
        }
    }
}


static void render_spans_2cycle_notexel1(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    int zb = wstate->zb_address >> 1;
    int zbcur;
    uint8_t offx = 0;
    uint8_t offy = 0;
    uint32_t blend_en;
    uint32_t prewrap;
    uint32_t curpixel_cvg = 0, curpixel_cvbit = 0, curpixel_memcvg = 0;
    uint32_t nextpixel_cvg;
    uint32_t acalpha;

    int tile1 = tilenum;
    int prim_tile = tilenum;

    int i, j;

    int drinc, dginc, dbinc, dainc, dzinc, dsinc, dtinc, dwinc;
    int xinc;
    if (flip)
    {
        drinc = wstate->spans_dr;
        dginc = wstate->spans_dg;
        dbinc = wstate->spans_db;
        dainc = wstate->spans_da;
        dzinc = wstate->spans_dz;
        dsinc = wstate->spans_ds;
        dtinc = wstate->spans_dt;
        dwinc = wstate->spans_dw;
        xinc = 1;
    }
    else
    {
        drinc = -wstate->spans_dr;
        dginc = -wstate->spans_dg;
        dbinc = -wstate->spans_db;
        dainc = -wstate->spans_da;
        dzinc = -wstate->spans_dz;
        dsinc = -wstate->spans_ds;
        dtinc = -wstate->spans_dt;
        dwinc = -wstate->spans_dw;
        xinc = -1;
    }

    uint16_t dzpix;
    if (!wstate->other_modes.z_source_sel)
        dzpix = (uint16_t)wstate->spans_dzpix;
    else
    {
        dzpix = (uint16_t)wstate->primitive_delta_z;
        dzinc = wstate->spans_cdz = wstate->spans_dzdy = 0;
    }
    int dzpixenc = dz_compress(dzpix);

    int cdith = 7, adith = 0;

    int r, g, b, a, z, s, t, w;
    int sr, sg, sb, sa, sz, ss, st, sw;
    int xstart, xend, xendsc;
    int sss = 0, sst = 0;
    int curpixel = 0;
    int wen;

    int x, length, scdiff;
    uint32_t fir, fig, fib;

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        r = wstate->span[i].r;
        g = wstate->span[i].g;
        b = wstate->span[i].b;
        a = wstate->span[i].a;
        z = wstate->other_modes.z_source_sel ? wstate->primitive_z : wstate->span[i].z;
        s = wstate->span[i].s;
        t = wstate->span[i].t;
        w = wstate->span[i].w;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        zbcur = zb + curpixel;

        if (!flip)
        {
            length = xendsc - xstart;
            scdiff = xend - xendsc;
            compute_cvg_noflip(wstate, i);
        }
        else
        {
            length = xstart - xendsc;
            scdiff = xendsc - xend;
            compute_cvg_flip(wstate, i);
        }

        if (scdiff)
        {
            scdiff &= 0xfff;
            r += (drinc * scdiff);
            g += (dginc * scdiff);
            b += (dbinc * scdiff);
            a += (dainc * scdiff);
            z += (dzinc * scdiff);
            s += (dsinc * scdiff);
            t += (dtinc * scdiff);
            w += (dwinc * scdiff);
        }

        for (j = 0; j <= length; j++)
        {
            sz = (z >> 10) & 0x3fffff;

            if (!j)
            {
                sr = r >> 14;
                sg = g >> 14;
                sb = b >> 14;
                sa = a >> 14;
                ss = s >> 16;
                st = t >> 16;
                sw = w >> 16;

                wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

                tclod_2cycle_notexel1(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1);

                texture_pipeline_cycle(wstate, &wstate->texel0_color, &wstate->texel0_color, sss, sst, tile1, 0);

                lookup_cvmask_derivatives(wstate->cvgbuf[x], &offx, &offy, &curpixel_cvg, &curpixel_cvbit);

                rgba_correct(wstate, offx, offy, sr, sg, sb, sa, curpixel_cvg);

                if (wstate->other_modes.f.getditherlevel < 2)
                    get_dither_noise(wstate, x, i, &cdith, &adith);

                combiner_2cycle_cycle0(wstate, adith, curpixel_cvg, &acalpha);
            }

            z_correct(wstate, offx, offy, &sz, curpixel_cvg);

            combiner_2cycle_cycle1(wstate, adith, &curpixel_cvg);

            wstate->fbread2_ptr(wstate, curpixel, &curpixel_memcvg);

            wen = z_compare(wstate, zbcur, sz, dzpix, dzpixenc, &blend_en, &prewrap, &curpixel_cvg, curpixel_memcvg);

            if (wen)
                wen &= blender_2cycle_cycle0(wstate, curpixel_cvg, curpixel_cvbit);
            else
                wstate->memory_color = wstate->pre_memory_color;

            x += xinc;

            r += drinc;
            g += dginc;
            b += dbinc;
            a += dainc;
            s += dsinc;
            t += dtinc;
            w += dwinc;

            sr = r >> 14;
            sg = g >> 14;
            sb = b >> 14;
            sa = a >> 14;
            ss = s >> 16;
            st = t >> 16;
            sw = w >> 16;

            lookup_cvmask_derivatives(j < length ? wstate->cvgbuf[x] : 0, &offx, &offy, &nextpixel_cvg, &curpixel_cvbit);

            rgba_correct(wstate, offx, offy, sr, sg, sb, sa, nextpixel_cvg);

            wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

            tclod_2cycle_notexel1(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1);

            texture_pipeline_cycle(wstate, &wstate->texel0_color, &wstate->texel0_color, sss, sst, tile1, 0);

            combiner_2cycle_cycle0(wstate, adith, nextpixel_cvg, &acalpha);

            if (wen)
            {
                wen &= alpha_compare(wstate, acalpha);

                if (wen)
                {
                    blender_2cycle_cycle1(wstate, &fir, &fig, &fib, cdith, blend_en, prewrap);
                    wstate->fbwrite_ptr(wstate, curpixel, fir, fig, fib, blend_en, curpixel_cvg, curpixel_memcvg);
                    if (wstate->other_modes.z_update_en)
                        z_store(zbcur, sz, dzpixenc);
                }
            }

            if (wstate->other_modes.f.getditherlevel < 2)
                get_dither_noise(wstate, x, i, &cdith, &adith);

            curpixel_cvg = nextpixel_cvg;

            z += dzinc;

            curpixel += xinc;
            zbcur += xinc;
        }
        }
    }
}


static void render_spans_2cycle_notex(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    UNUSED(tilenum);

    int zb = wstate->zb_address >> 1;
    int zbcur;
    uint8_t offx = 0;
    uint8_t offy = 0;
    uint32_t blend_en;
    uint32_t prewrap;
    uint32_t curpixel_cvg = 0, curpixel_cvbit = 0, curpixel_memcvg = 0;
    uint32_t nextpixel_cvg;
    uint32_t acalpha;

    int i, j;

    int drinc, dginc, dbinc, dainc, dzinc;
    int xinc;
    if (flip)
    {
        drinc = wstate->spans_dr;
        dginc = wstate->spans_dg;
        dbinc = wstate->spans_db;
        dainc = wstate->spans_da;
        dzinc = wstate->spans_dz;
        xinc = 1;
    }
    else
    {
        drinc = -wstate->spans_dr;
        dginc = -wstate->spans_dg;
        dbinc = -wstate->spans_db;
        dainc = -wstate->spans_da;
        dzinc = -wstate->spans_dz;
        xinc = -1;
    }

    uint16_t dzpix;
    if (!wstate->other_modes.z_source_sel)
        dzpix = (uint16_t)wstate->spans_dzpix;
    else
    {
        dzpix = (uint16_t)wstate->primitive_delta_z;
        dzinc = wstate->spans_cdz = wstate->spans_dzdy = 0;
    }
    int dzpixenc = dz_compress(dzpix);

    int cdith = 7, adith = 0;

    int r, g, b, a, z;
    int sr, sg, sb, sa, sz;
    int xstart, xend, xendsc;
    int curpixel = 0;
    int wen;

    int x, length, scdiff;
    uint32_t fir, fig, fib;

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        r = wstate->span[i].r;
        g = wstate->span[i].g;
        b = wstate->span[i].b;
        a = wstate->span[i].a;
        z = wstate->other_modes.z_source_sel ? wstate->primitive_z : wstate->span[i].z;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        zbcur = zb + curpixel;

        if (!flip)
        {
            length = xendsc - xstart;
            scdiff = xend - xendsc;
            compute_cvg_noflip(wstate, i);
        }
        else
        {
            length = xstart - xendsc;
            scdiff = xendsc - xend;
            compute_cvg_flip(wstate, i);
        }

        if (scdiff)
        {
            scdiff &= 0xfff;
            r += (drinc * scdiff);
            g += (dginc * scdiff);
            b += (dbinc * scdiff);
            a += (dainc * scdiff);
            z += (dzinc * scdiff);
        }

        for (j = 0; j <= length; j++)
        {
            sz = (z >> 10) & 0x3fffff;

            if (!j)
            {
                sr = r >> 14;
                sg = g >> 14;
                sb = b >> 14;
                sa = a >> 14;

                lookup_cvmask_derivatives(wstate->cvgbuf[x], &offx, &offy, &curpixel_cvg, &curpixel_cvbit);

                rgba_correct(wstate, offx, offy, sr, sg, sb, sa, curpixel_cvg);

                if (wstate->other_modes.f.getditherlevel < 2)
                    get_dither_noise(wstate, x, i, &cdith, &adith);

                combiner_2cycle_cycle0(wstate, adith, curpixel_cvg, &acalpha);
            }

            z_correct(wstate, offx, offy, &sz, curpixel_cvg);

            combiner_2cycle_cycle1(wstate, adith, &curpixel_cvg);

            wstate->fbread2_ptr(wstate, curpixel, &curpixel_memcvg);

            wen = z_compare(wstate, zbcur, sz, dzpix, dzpixenc, &blend_en, &prewrap, &curpixel_cvg, curpixel_memcvg);

            if (wen)
                wen &= blender_2cycle_cycle0(wstate, curpixel_cvg, curpixel_cvbit);
            else
                wstate->memory_color = wstate->pre_memory_color;

            x += xinc;

            r += drinc;
            g += dginc;
            b += dbinc;
            a += dainc;

            sr = r >> 14;
            sg = g >> 14;
            sb = b >> 14;
            sa = a >> 14;

            lookup_cvmask_derivatives(j < length ? wstate->cvgbuf[x] : 0, &offx, &offy, &nextpixel_cvg, &curpixel_cvbit);

            rgba_correct(wstate, offx, offy, sr, sg, sb, sa, nextpixel_cvg);

            combiner_2cycle_cycle0(wstate, adith, nextpixel_cvg, &acalpha);

            if (wen)
            {
                wen &= alpha_compare(wstate, acalpha);

                if (wen)
                {
                    blender_2cycle_cycle1(wstate, &fir, &fig, &fib, cdith, blend_en, prewrap);
                    wstate->fbwrite_ptr(wstate, curpixel, fir, fig, fib, blend_en, curpixel_cvg, curpixel_memcvg);
                    if (wstate->other_modes.z_update_en)
                        z_store(zbcur, sz, dzpixenc);
                }
            }

            if (wstate->other_modes.f.getditherlevel < 2)
                get_dither_noise(wstate, x, i, &cdith, &adith);

            curpixel_cvg = nextpixel_cvg;

            z += dzinc;

            curpixel += xinc;
            zbcur += xinc;
        }
        }
    }
}


static void render_spans_fill(struct rdp_state* wstate, int start, int end, int flip)
{
    if (wstate->fb_size == PIXEL_SIZE_4BIT)
    {
        rdp_pipeline_crashed = 1;
        return;
    }

    int i, j;

    int fastkillbits = wstate->other_modes.image_read_en || wstate->other_modes.z_compare_en;
    int slowkillbits = wstate->other_modes.z_update_en && !wstate->other_modes.z_source_sel && !fastkillbits;

    int xinc = flip ? 1 : -1;

    int xstart = 0, xendsc;
    int prevxstart;
    int curpixel = 0;
    int x, length;

    for (i = start; i <= end; i++)
    {
        prevxstart = xstart;
        xstart = wstate->span[i].lx;
        xendsc = wstate->span[i].rx;

        x = xendsc;
        curpixel = wstate->fb_width * i + x;
        length = flip ? (xstart - xendsc) : (xendsc - xstart);

        if (wstate->span[i].validline)
        {
            if (fastkillbits && length >= 0)
            {
                if (!onetimewarnings.fillmbitcrashes)
                    msg_warning("render_spans_fill: image_read_en %x z_update_en %x z_compare_en %x. RDP crashed",
                    wstate->other_modes.image_read_en, wstate->other_modes.z_update_en, wstate->other_modes.z_compare_en);
                onetimewarnings.fillmbitcrashes = true;
                rdp_pipeline_crashed = 1;
                return;
            }







            for (j = 0; j <= length; j++)
            {

                switch(wstate->fb_size)
                {
                case 0:
                    fbfill_4(wstate, curpixel);
                    break;
                case 1:
                    fbfill_8(wstate, curpixel);
                    break;
                case 2:
                    fbfill_16(wstate, curpixel);
                    break;
                case 3:
                default:
                    fbfill_32(wstate, curpixel);
                    break;
                }

                x += xinc;
                curpixel += xinc;
            }

            if (slowkillbits && length >= 0)
            {
                if (!onetimewarnings.fillmbitcrashes)
                    msg_warning("render_spans_fill: image_read_en %x z_update_en %x z_compare_en %x z_source_sel %x. RDP crashed",
                    wstate->other_modes.image_read_en, wstate->other_modes.z_update_en, wstate->other_modes.z_compare_en, wstate->other_modes.z_source_sel);
                onetimewarnings.fillmbitcrashes = 1;
                rdp_pipeline_crashed = 1;
                return;
            }
        }
    }
}

static void render_spans_copy(struct rdp_state* wstate, int start, int end, int tilenum, int flip)
{
    int i, j, k;

    if (wstate->fb_size == PIXEL_SIZE_32BIT)
    {
        rdp_pipeline_crashed = 1;
        return;
    }

    int tile1 = tilenum;
    int prim_tile = tilenum;

    int dsinc, dtinc, dwinc;
    int xinc;
    if (flip)
    {
        dsinc = wstate->spans_ds;
        dtinc = wstate->spans_dt;
        dwinc = wstate->spans_dw;
        xinc = 1;
    }
    else
    {
        dsinc = -wstate->spans_ds;
        dtinc = -wstate->spans_dt;
        dwinc = -wstate->spans_dw;
        xinc = -1;
    }

    int xstart = 0, xendsc;
    int s = 0, t = 0, w = 0, ss = 0, st = 0, sw = 0, sss = 0, sst = 0/*, ssw = 0*/;
    int fb_index, length;
    //int diff = 0;

    uint32_t hidword = 0, lowdword = 0;
    //uint32_t hidword1 = 0, lowdword1 = 0;
    int fbadvance = (wstate->fb_size == PIXEL_SIZE_4BIT) ? 8 : 16 >> wstate->fb_size;
    uint32_t fbptr = 0;
    int fbptr_advance = flip ? 8 : -8;
    uint64_t copyqword = 0;
    uint32_t tempdword = 0;
    uint8_t tempbyte = 0;
    int copywmask = 0, alphamask = 0;
    int bytesperpixel = (wstate->fb_size == PIXEL_SIZE_4BIT) ? 1 : (1 << (wstate->fb_size - 1));
    uint32_t fbendptr = 0;
    int32_t threshold, currthreshold;

#define PIXELS_TO_BYTES_SPECIAL4(pix, siz) ((siz) ? PIXELS_TO_BYTES(pix, siz) : (pix))

    for (i = start; i <= end; i++)
    {
        if (wstate->span[i].validline)
        {

        s = wstate->span[i].s;
        t = wstate->span[i].t;
        w = wstate->span[i].w;

        xstart = wstate->span[i].lx;
        xendsc = wstate->span[i].rx;

        fb_index = wstate->fb_width * i + xendsc;
        fbptr = wstate->fb_address + PIXELS_TO_BYTES_SPECIAL4(fb_index, wstate->fb_size);
        fbendptr = wstate->fb_address + PIXELS_TO_BYTES_SPECIAL4((wstate->fb_width * i + xstart), wstate->fb_size);
        length = flip ? (xstart - xendsc) : (xendsc - xstart);




        for (j = 0; j <= length; j += fbadvance)
        {
            ss = s >> 16;
            st = t >> 16;
            sw = w >> 16;

            wstate->tcdiv_ptr(ss, st, sw, &sss, &sst);

            tclod_copy(wstate, &sss, &sst, s, t, w, dsinc, dtinc, dwinc, prim_tile, &tile1);



            fetch_qword_copy(wstate, &hidword, &lowdword, sss, sst, tile1);



            if (wstate->fb_size == PIXEL_SIZE_16BIT || wstate->fb_size == PIXEL_SIZE_8BIT)
                copyqword = ((uint64_t)hidword << 32) | ((uint64_t)lowdword);
            else
                copyqword = 0;


            if (!wstate->other_modes.alpha_compare_en)
                alphamask = 0xff;
            else if (wstate->fb_size == PIXEL_SIZE_16BIT)
            {
                alphamask = 0;
                alphamask |= (((copyqword >> 48) & 1) ? 0xC0 : 0);
                alphamask |= (((copyqword >> 32) & 1) ? 0x30 : 0);
                alphamask |= (((copyqword >> 16) & 1) ? 0xC : 0);
                alphamask |= ((copyqword & 1) ? 0x3 : 0);
            }
            else if (wstate->fb_size == PIXEL_SIZE_8BIT)
            {
                alphamask = 0;
                threshold = (wstate->other_modes.dither_alpha_en) ? (irand(&wstate->rseed) & 0xff) : wstate->blend_color.a;
                if (wstate->other_modes.dither_alpha_en)
                {
                    currthreshold = threshold;
                    alphamask |= (((copyqword >> 24) & 0xff) >= currthreshold ? 0xC0 : 0);
                    currthreshold = ((threshold & 3) << 6) | (threshold >> 2);
                    alphamask |= (((copyqword >> 16) & 0xff) >= currthreshold ? 0x30 : 0);
                    currthreshold = ((threshold & 0xf) << 4) | (threshold >> 4);
                    alphamask |= (((copyqword >> 8) & 0xff) >= currthreshold ? 0xC : 0);
                    currthreshold = ((threshold & 0x3f) << 2) | (threshold >> 6);
                    alphamask |= ((copyqword & 0xff) >= currthreshold ? 0x3 : 0);
                }
                else
                {
                    alphamask |= (((copyqword >> 24) & 0xff) >= threshold ? 0xC0 : 0);
                    alphamask |= (((copyqword >> 16) & 0xff) >= threshold ? 0x30 : 0);
                    alphamask |= (((copyqword >> 8) & 0xff) >= threshold ? 0xC : 0);
                    alphamask |= ((copyqword & 0xff) >= threshold ? 0x3 : 0);
                }
            }
            else
                alphamask = 0;

            copywmask = (flip) ? (fbendptr - fbptr + bytesperpixel) : (fbptr - fbendptr + bytesperpixel);

            if (copywmask > 8)
                copywmask = 8;
            tempdword = fbptr;
            k = 7;
            while(copywmask > 0)
            {
                tempbyte = (uint8_t)((copyqword >> (k << 3)) & 0xff);
                if (alphamask & (1 << k))
                {
                    PAIRWRITE8(tempdword, tempbyte);
                }
                k--;
                tempdword += xinc;
                copywmask--;
            }

            s += dsinc;
            t += dtinc;
            w += dwinc;
            fbptr += fbptr_advance;
        }
        }
    }
}

static void edgewalker_for_prims(struct rdp_state* wstate, int32_t* ewdata)
{
    int j = 0;
    int xleft = 0, xright = 0, xleft_inc = 0, xright_inc = 0;
    int r = 0, g = 0, b = 0, a = 0, z = 0, s = 0, t = 0, w = 0;
    //int dr = 0, dg = 0, db = 0, da = 0;
    int drdx = 0, dgdx = 0, dbdx = 0, dadx = 0, dzdx = 0, dsdx = 0, dtdx = 0, dwdx = 0;
    int drdy = 0, dgdy = 0, dbdy = 0, dady = 0, dzdy = 0, dsdy = 0, dtdy = 0, dwdy = 0;
    int drde = 0, dgde = 0, dbde = 0, dade = 0, dzde = 0, dsde = 0, dtde = 0, dwde = 0;
    int tilenum = 0, flip = 0;
    int32_t yl = 0, ym = 0, yh = 0;
    int32_t xl = 0, xm = 0, xh = 0;
    int32_t dxldy = 0, dxhdy = 0, dxmdy = 0;

    if (wstate->other_modes.f.stalederivs)
    {
        deduce_derivatives(wstate);
        wstate->other_modes.f.stalederivs = 0;
    }


    flip = (ewdata[0] & 0x800000) != 0;
    wstate->max_level = (ewdata[0] >> 19) & 7;
    tilenum = (ewdata[0] >> 16) & 7;


    yl = SIGN(ewdata[0], 14);
    ym = ewdata[1] >> 16;
    ym = SIGN(ym, 14);
    yh = SIGN(ewdata[1], 14);

    xl = SIGN(ewdata[2], 28);
    xh = SIGN(ewdata[4], 28);
    xm = SIGN(ewdata[6], 28);

    dxldy = SIGN(ewdata[3], 30);



    dxhdy = SIGN(ewdata[5], 30);
    dxmdy = SIGN(ewdata[7], 30);


    r    = (ewdata[8] & 0xffff0000) | ((ewdata[12] >> 16) & 0x0000ffff);
    g    = ((ewdata[8] << 16) & 0xffff0000) | (ewdata[12] & 0x0000ffff);
    b    = (ewdata[9] & 0xffff0000) | ((ewdata[13] >> 16) & 0x0000ffff);
    a    = ((ewdata[9] << 16) & 0xffff0000) | (ewdata[13] & 0x0000ffff);
    drdx = (ewdata[10] & 0xffff0000) | ((ewdata[14] >> 16) & 0x0000ffff);
    dgdx = ((ewdata[10] << 16) & 0xffff0000) | (ewdata[14] & 0x0000ffff);
    dbdx = (ewdata[11] & 0xffff0000) | ((ewdata[15] >> 16) & 0x0000ffff);
    dadx = ((ewdata[11] << 16) & 0xffff0000) | (ewdata[15] & 0x0000ffff);
    drde = (ewdata[16] & 0xffff0000) | ((ewdata[20] >> 16) & 0x0000ffff);
    dgde = ((ewdata[16] << 16) & 0xffff0000) | (ewdata[20] & 0x0000ffff);
    dbde = (ewdata[17] & 0xffff0000) | ((ewdata[21] >> 16) & 0x0000ffff);
    dade = ((ewdata[17] << 16) & 0xffff0000) | (ewdata[21] & 0x0000ffff);
    drdy = (ewdata[18] & 0xffff0000) | ((ewdata[22] >> 16) & 0x0000ffff);
    dgdy = ((ewdata[18] << 16) & 0xffff0000) | (ewdata[22] & 0x0000ffff);
    dbdy = (ewdata[19] & 0xffff0000) | ((ewdata[23] >> 16) & 0x0000ffff);
    dady = ((ewdata[19] << 16) & 0xffff0000) | (ewdata[23] & 0x0000ffff);


    s    = (ewdata[24] & 0xffff0000) | ((ewdata[28] >> 16) & 0x0000ffff);
    t    = ((ewdata[24] << 16) & 0xffff0000)    | (ewdata[28] & 0x0000ffff);
    w    = (ewdata[25] & 0xffff0000) | ((ewdata[29] >> 16) & 0x0000ffff);
    dsdx = (ewdata[26] & 0xffff0000) | ((ewdata[30] >> 16) & 0x0000ffff);
    dtdx = ((ewdata[26] << 16) & 0xffff0000)    | (ewdata[30] & 0x0000ffff);
    dwdx = (ewdata[27] & 0xffff0000) | ((ewdata[31] >> 16) & 0x0000ffff);
    dsde = (ewdata[32] & 0xffff0000) | ((ewdata[36] >> 16) & 0x0000ffff);
    dtde = ((ewdata[32] << 16) & 0xffff0000)    | (ewdata[36] & 0x0000ffff);
    dwde = (ewdata[33] & 0xffff0000) | ((ewdata[37] >> 16) & 0x0000ffff);
    dsdy = (ewdata[34] & 0xffff0000) | ((ewdata[38] >> 16) & 0x0000ffff);
    dtdy = ((ewdata[34] << 16) & 0xffff0000)    | (ewdata[38] & 0x0000ffff);
    dwdy = (ewdata[35] & 0xffff0000) | ((ewdata[39] >> 16) & 0x0000ffff);


    z    = ewdata[40];
    dzdx = ewdata[41];
    dzde = ewdata[42];
    dzdy = ewdata[43];







    wstate->spans_ds = dsdx & ~0x1f;
    wstate->spans_dt = dtdx & ~0x1f;
    wstate->spans_dw = dwdx & ~0x1f;
    wstate->spans_dr = drdx & ~0x1f;
    wstate->spans_dg = dgdx & ~0x1f;
    wstate->spans_db = dbdx & ~0x1f;
    wstate->spans_da = dadx & ~0x1f;
    wstate->spans_dz = dzdx;


    wstate->spans_drdy = drdy >> 14;
    wstate->spans_dgdy = dgdy >> 14;
    wstate->spans_dbdy = dbdy >> 14;
    wstate->spans_dady = dady >> 14;
    wstate->spans_dzdy = dzdy >> 10;
    wstate->spans_drdy = SIGN(wstate->spans_drdy, 13);
    wstate->spans_dgdy = SIGN(wstate->spans_dgdy, 13);
    wstate->spans_dbdy = SIGN(wstate->spans_dbdy, 13);
    wstate->spans_dady = SIGN(wstate->spans_dady, 13);
    wstate->spans_dzdy = SIGN(wstate->spans_dzdy, 22);
    wstate->spans_cdr = wstate->spans_dr >> 14;
    wstate->spans_cdr = SIGN(wstate->spans_cdr, 13);
    wstate->spans_cdg = wstate->spans_dg >> 14;
    wstate->spans_cdg = SIGN(wstate->spans_cdg, 13);
    wstate->spans_cdb = wstate->spans_db >> 14;
    wstate->spans_cdb = SIGN(wstate->spans_cdb, 13);
    wstate->spans_cda = wstate->spans_da >> 14;
    wstate->spans_cda = SIGN(wstate->spans_cda, 13);
    wstate->spans_cdz = wstate->spans_dz >> 10;
    wstate->spans_cdz = SIGN(wstate->spans_cdz, 22);

    wstate->spans_dsdy = dsdy & ~0x7fff;
    wstate->spans_dtdy = dtdy & ~0x7fff;
    wstate->spans_dwdy = dwdy & ~0x7fff;


    int dzdy_dz = (dzdy >> 16) & 0xffff;
    int dzdx_dz = (dzdx >> 16) & 0xffff;

    wstate->spans_dzpix = ((dzdy_dz & 0x8000) ? ((~dzdy_dz) & 0x7fff) : dzdy_dz) + ((dzdx_dz & 0x8000) ? ((~dzdx_dz) & 0x7fff) : dzdx_dz);
    wstate->spans_dzpix = normalize_dzpix(wstate->spans_dzpix & 0xffff) & 0xffff;



    xleft_inc = (dxmdy >> 2) & ~0x1;
    xright_inc = (dxhdy >> 2) & ~0x1;



    xright = xh & ~0x1;
    xleft = xm & ~0x1;

    int k = 0;

    int dsdiff, dtdiff, dwdiff, drdiff, dgdiff, dbdiff, dadiff, dzdiff;
    int sign_dxhdy = (ewdata[5] & 0x80000000) != 0;

    int dsdeh, dtdeh, dwdeh, drdeh, dgdeh, dbdeh, dadeh, dzdeh, dsdyh, dtdyh, dwdyh, drdyh, dgdyh, dbdyh, dadyh, dzdyh;
    int do_offset = !(sign_dxhdy ^ flip);

    if (do_offset)
    {
        dsdeh = dsde & ~0x1ff;
        dtdeh = dtde & ~0x1ff;
        dwdeh = dwde & ~0x1ff;
        drdeh = drde & ~0x1ff;
        dgdeh = dgde & ~0x1ff;
        dbdeh = dbde & ~0x1ff;
        dadeh = dade & ~0x1ff;
        dzdeh = dzde & ~0x1ff;

        dsdyh = dsdy & ~0x1ff;
        dtdyh = dtdy & ~0x1ff;
        dwdyh = dwdy & ~0x1ff;
        drdyh = drdy & ~0x1ff;
        dgdyh = dgdy & ~0x1ff;
        dbdyh = dbdy & ~0x1ff;
        dadyh = dady & ~0x1ff;
        dzdyh = dzdy & ~0x1ff;







        dsdiff = dsdeh - (dsdeh >> 2) - dsdyh + (dsdyh >> 2);
        dtdiff = dtdeh - (dtdeh >> 2) - dtdyh + (dtdyh >> 2);
        dwdiff = dwdeh - (dwdeh >> 2) - dwdyh + (dwdyh >> 2);
        drdiff = drdeh - (drdeh >> 2) - drdyh + (drdyh >> 2);
        dgdiff = dgdeh - (dgdeh >> 2) - dgdyh + (dgdyh >> 2);
        dbdiff = dbdeh - (dbdeh >> 2) - dbdyh + (dbdyh >> 2);
        dadiff = dadeh - (dadeh >> 2) - dadyh + (dadyh >> 2);
        dzdiff = dzdeh - (dzdeh >> 2) - dzdyh + (dzdyh >> 2);

    }
    else
        dsdiff = dtdiff = dwdiff = drdiff = dgdiff = dbdiff = dadiff = dzdiff = 0;

    int xfrac = 0;

    int dsdxh, dtdxh, dwdxh, drdxh, dgdxh, dbdxh, dadxh, dzdxh;
    if (wstate->other_modes.cycle_type != CYCLE_TYPE_COPY)
    {
        dsdxh = (dsdx >> 8) & ~1;
        dtdxh = (dtdx >> 8) & ~1;
        dwdxh = (dwdx >> 8) & ~1;
        drdxh = (drdx >> 8) & ~1;
        dgdxh = (dgdx >> 8) & ~1;
        dbdxh = (dbdx >> 8) & ~1;
        dadxh = (dadx >> 8) & ~1;
        dzdxh = (dzdx >> 8) & ~1;
    }
    else
        dsdxh = dtdxh = dwdxh = drdxh = dgdxh = dbdxh = dadxh = dzdxh = 0;





#define ADJUST_ATTR_PRIM()      \
{                           \
    wstate->span[j].s = ((s & ~0x1ff) + dsdiff - (xfrac * dsdxh)) & ~0x3ff;             \
    wstate->span[j].t = ((t & ~0x1ff) + dtdiff - (xfrac * dtdxh)) & ~0x3ff;             \
    wstate->span[j].w = ((w & ~0x1ff) + dwdiff - (xfrac * dwdxh)) & ~0x3ff;             \
    wstate->span[j].r = ((r & ~0x1ff) + drdiff - (xfrac * drdxh)) & ~0x3ff;             \
    wstate->span[j].g = ((g & ~0x1ff) + dgdiff - (xfrac * dgdxh)) & ~0x3ff;             \
    wstate->span[j].b = ((b & ~0x1ff) + dbdiff - (xfrac * dbdxh)) & ~0x3ff;             \
    wstate->span[j].a = ((a & ~0x1ff) + dadiff - (xfrac * dadxh)) & ~0x3ff;             \
    wstate->span[j].z = ((z & ~0x1ff) + dzdiff - (xfrac * dzdxh)) & ~0x3ff;             \
}


#define ADDVALUES_PRIM() {  \
            s += dsde;  \
            t += dtde;  \
            w += dwde; \
            r += drde; \
            g += dgde; \
            b += dbde; \
            a += dade; \
            z += dzde; \
}

    int32_t maxxmx = 0, minxmx = 0, maxxhx = 0, minxhx = 0;

    int spix = 0;
    int ycur =  yh & ~3;
    int ldflag = (sign_dxhdy ^ flip) ? 0 : 3;
    int invaly = 1;
    //int length = 0;
    int32_t xrsc = 0, xlsc = 0, stickybit = 0;
    int32_t yllimit = 0, yhlimit = 0;
    if (yl & 0x2000)
        yllimit = 1;
    else if (yl & 0x1000)
        yllimit = 0;
    else
        yllimit = (yl & 0xfff) < wstate->clip.yl;
    yllimit = yllimit ? yl : wstate->clip.yl;

    int ylfar = yllimit | 3;
    if ((yl >> 2) > (ylfar >> 2))
        ylfar += 4;
    else if ((yllimit >> 2) >= 0 && (yllimit >> 2) < 1023)
        wstate->span[(yllimit >> 2) + 1].validline = 0;


    if (yh & 0x2000)
        yhlimit = 0;
    else if (yh & 0x1000)
        yhlimit = 1;
    else
        yhlimit = (yh >= wstate->clip.yh);
    yhlimit = yhlimit ? yh : wstate->clip.yh;

    int yhclose = yhlimit & ~3;

    int32_t clipxlshift = wstate->clip.xl << 1;
    int32_t clipxhshift = wstate->clip.xh << 1;
    int allover = 1, allunder = 1, curover = 0, curunder = 0;
    int allinval = 1;
    int32_t curcross = 0;

    xfrac = ((xright >> 8) & 0xff);


    if (flip)
    {
    for (k = ycur; k <= ylfar; k++)
    {
        if (k == ym)
        {

            xleft = xl & ~1;
            xleft_inc = (dxldy >> 2) & ~1;
        }

        spix = k & 3;

        if (k >= yhclose)
        {
            invaly = k < yhlimit || k >= yllimit;

            j = k >> 2;

            if (spix == 0)
            {
                maxxmx = 0;
                minxhx = 0xfff;
                allover = allunder = 1;
                allinval = 1;
            }

            stickybit = ((xright >> 1) & 0x1fff) > 0;
            xrsc = ((xright >> 13) & 0x1ffe) | stickybit;


            curunder = ((xright & 0x8000000) || (xrsc < clipxhshift && !(xright & 0x4000000)));

            xrsc = curunder ? clipxhshift : (((xright >> 13) & 0x3ffe) | stickybit);
            curover = ((xrsc & 0x2000) || (xrsc & 0x1fff) >= clipxlshift);
            xrsc = curover ? clipxlshift : xrsc;
            wstate->span[j].majorx[spix] = xrsc & 0x1fff;
            allover &= curover;
            allunder &= curunder;

            stickybit = ((xleft >> 1) & 0x1fff) > 0;
            xlsc = ((xleft >> 13) & 0x1ffe) | stickybit;
            curunder = ((xleft & 0x8000000) || (xlsc < clipxhshift && !(xleft & 0x4000000)));
            xlsc = curunder ? clipxhshift : (((xleft >> 13) & 0x3ffe) | stickybit);
            curover = ((xlsc & 0x2000) || (xlsc & 0x1fff) >= clipxlshift);
            xlsc = curover ? clipxlshift : xlsc;
            wstate->span[j].minorx[spix] = xlsc & 0x1fff;
            allover &= curover;
            allunder &= curunder;



            curcross = ((xleft ^ (1 << 27)) & (0x3fff << 14)) < ((xright ^ (1 << 27)) & (0x3fff << 14));


            invaly |= curcross;
            wstate->span[j].invalyscan[spix] = invaly;
            allinval &= invaly;

            if (!invaly)
            {
                maxxmx = (((xlsc >> 3) & 0xfff) > maxxmx) ? (xlsc >> 3) & 0xfff : maxxmx;
                minxhx = (((xrsc >> 3) & 0xfff) < minxhx) ? (xrsc >> 3) & 0xfff : minxhx;
            }

            if (spix == ldflag)
            {




                wstate->span[j].unscrx = SIGN(xright >> 16, 12);
                xfrac = (xright >> 8) & 0xff;
                ADJUST_ATTR_PRIM();
            }

            if (spix == 3)
            {
                wstate->span[j].lx = maxxmx;
                wstate->span[j].rx = minxhx;
                wstate->span[j].validline  = !allinval && !allover && !allunder && (!wstate->scfield || (wstate->scfield && !(wstate->sckeepodd ^ (j & 1)))) && (!wstate->stride || j % wstate->stride == wstate->offset);

            }


        }

        if (spix == 3)
        {
            ADDVALUES_PRIM();
        }



        xleft += xleft_inc;
        xright += xright_inc;

    }
    }
    else
    {
    for (k = ycur; k <= ylfar; k++)
    {
        if (k == ym)
        {
            xleft = xl & ~1;
            xleft_inc = (dxldy >> 2) & ~1;
        }

        spix = k & 3;

        if (k >= yhclose)
        {
            invaly = k < yhlimit || k >= yllimit;
            j = k >> 2;

            if (spix == 0)
            {
                maxxhx = 0;
                minxmx = 0xfff;
                allover = allunder = 1;
                allinval = 1;
            }

            stickybit = ((xright >> 1) & 0x1fff) > 0;
            xrsc = ((xright >> 13) & 0x1ffe) | stickybit;
            curunder = ((xright & 0x8000000) || (xrsc < clipxhshift && !(xright & 0x4000000)));
            xrsc = curunder ? clipxhshift : (((xright >> 13) & 0x3ffe) | stickybit);
            curover = ((xrsc & 0x2000) || (xrsc & 0x1fff) >= clipxlshift);
            xrsc = curover ? clipxlshift : xrsc;
            wstate->span[j].majorx[spix] = xrsc & 0x1fff;
            allover &= curover;
            allunder &= curunder;

            stickybit = ((xleft >> 1) & 0x1fff) > 0;
            xlsc = ((xleft >> 13) & 0x1ffe) | stickybit;
            curunder = ((xleft & 0x8000000) || (xlsc < clipxhshift && !(xleft & 0x4000000)));
            xlsc = curunder ? clipxhshift : (((xleft >> 13) & 0x3ffe) | stickybit);
            curover = ((xlsc & 0x2000) || (xlsc & 0x1fff) >= clipxlshift);
            xlsc = curover ? clipxlshift : xlsc;
            wstate->span[j].minorx[spix] = xlsc & 0x1fff;
            allover &= curover;
            allunder &= curunder;

            curcross = ((xright ^ (1 << 27)) & (0x3fff << 14)) < ((xleft ^ (1 << 27)) & (0x3fff << 14));

            invaly |= curcross;
            wstate->span[j].invalyscan[spix] = invaly;
            allinval &= invaly;

            if (!invaly)
            {
                minxmx = (((xlsc >> 3) & 0xfff) < minxmx) ? (xlsc >> 3) & 0xfff : minxmx;
                maxxhx = (((xrsc >> 3) & 0xfff) > maxxhx) ? (xrsc >> 3) & 0xfff : maxxhx;
            }

            if (spix == ldflag)
            {
                wstate->span[j].unscrx  = SIGN(xright >> 16, 12);
                xfrac = (xright >> 8) & 0xff;
                ADJUST_ATTR_PRIM();
            }

            if (spix == 3)
            {
                wstate->span[j].lx = minxmx;
                wstate->span[j].rx = maxxhx;
                wstate->span[j].validline  = !allinval && !allover && !allunder && (!wstate->scfield || (wstate->scfield && !(wstate->sckeepodd ^ (j & 1)))) && (!wstate->stride || j % wstate->stride == wstate->offset);
            }

        }

        if (spix == 3)
        {
            ADDVALUES_PRIM();
        }

        xleft += xleft_inc;
        xright += xright_inc;

    }
    }




    switch(wstate->other_modes.cycle_type)
    {
        case CYCLE_TYPE_1:
            switch (wstate->other_modes.f.textureuselevel0)
            {
                case 0: render_spans_1cycle_complete(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
                case 1: render_spans_1cycle_notexel1(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
                case 2: default: render_spans_1cycle_notex(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
            }
            break;
        case CYCLE_TYPE_2:
            switch (wstate->other_modes.f.textureuselevel1)
            {
                case 0: render_spans_2cycle_complete(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
                case 1: render_spans_2cycle_notexelnext(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
                case 2: render_spans_2cycle_notexel1(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
                case 3: default: render_spans_2cycle_notex(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
            }
            break;
        case CYCLE_TYPE_COPY: render_spans_copy(wstate, yhlimit >> 2, yllimit >> 2, tilenum, flip); break;
        case CYCLE_TYPE_FILL: render_spans_fill(wstate, yhlimit >> 2, yllimit >> 2, flip); break;
        default: msg_error("cycle_type %d", wstate->other_modes.cycle_type); break;
    }


}

static void rasterizer_init(struct rdp_state* wstate)
{
    wstate->clip.xh = 0x2000;
    wstate->clip.yh = 0x2000;
}

void rdp_tri_noshade(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, 8 * sizeof(int32_t));
    memset(&ewdata[8], 0, 36 * sizeof(int32_t));
    edgewalker_for_prims(wstate, ewdata);
}

void rdp_tri_noshade_z(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, 8 * sizeof(int32_t));
    memset(&ewdata[8], 0, 32 * sizeof(int32_t));
    memcpy(&ewdata[40], args + 8, 4 * sizeof(int32_t));
    edgewalker_for_prims(wstate, ewdata);
}

void rdp_tri_tex(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, 8 * sizeof(int32_t));
    memset(&ewdata[8], 0, 16 * sizeof(int32_t));
    memcpy(&ewdata[24], args + 8, 16 * sizeof(int32_t));
    memset(&ewdata[40], 0, 4 * sizeof(int32_t));
    edgewalker_for_prims(wstate, ewdata);
}

void rdp_tri_tex_z(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, 8 * sizeof(int32_t));
    memset(&ewdata[8], 0, 16 * sizeof(int32_t));
    memcpy(&ewdata[24], args + 8, 16 * sizeof(int32_t));
    memcpy(&ewdata[40], args + 24, 4 * sizeof(int32_t));






    edgewalker_for_prims(wstate, ewdata);


}

void rdp_tri_shade(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, 24 * sizeof(int32_t));
    memset(&ewdata[24], 0, 20 * sizeof(int32_t));
    edgewalker_for_prims(wstate, ewdata);
}

void rdp_tri_shade_z(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, 24 * sizeof(int32_t));
    memset(&ewdata[24], 0, 16 * sizeof(int32_t));
    memcpy(&ewdata[40], args + 24, 4 * sizeof(int32_t));
    edgewalker_for_prims(wstate, ewdata);
}

void rdp_tri_texshade(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, 40 * sizeof(int32_t));
    memset(&ewdata[40], 0, 4 * sizeof(int32_t));
    edgewalker_for_prims(wstate, ewdata);
}

void rdp_tri_texshade_z(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t ewdata[CMD_MAX_INTS];
    memcpy(&ewdata[0], args, CMD_MAX_SIZE);





    edgewalker_for_prims(wstate, ewdata);


}

void rdp_tex_rect(struct rdp_state* wstate, const uint32_t* args)
{
    uint32_t tilenum    = (args[1] >> 24) & 0x7;
    uint32_t xl = (args[0] >> 12) & 0xfff;
    uint32_t yl = (args[0] >>  0) & 0xfff;
    uint32_t xh = (args[1] >> 12) & 0xfff;
    uint32_t yh = (args[1] >>  0) & 0xfff;

    int32_t s = (args[2] >> 16) & 0xffff;
    int32_t t = (args[2] >>  0) & 0xffff;
    int32_t dsdx = (args[3] >> 16) & 0xffff;
    int32_t dtdy = (args[3] >>  0) & 0xffff;

    dsdx = SIGN16(dsdx);
    dtdy = SIGN16(dtdy);

    if (wstate->other_modes.cycle_type == CYCLE_TYPE_FILL || wstate->other_modes.cycle_type == CYCLE_TYPE_COPY)
        yl |= 3;

    uint32_t xlint = (xl >> 2) & 0x3ff;
    uint32_t xhint = (xh >> 2) & 0x3ff;

    int32_t ewdata[CMD_MAX_INTS];
    ewdata[0] = (0x24 << 24) | ((0x80 | tilenum) << 16) | yl;
    ewdata[1] = (yl << 16) | yh;
    ewdata[2] = (xlint << 16) | ((xl & 3) << 14);
    ewdata[3] = 0;
    ewdata[4] = (xhint << 16) | ((xh & 3) << 14);
    ewdata[5] = 0;
    ewdata[6] = (xlint << 16) | ((xl & 3) << 14);
    ewdata[7] = 0;
    memset(&ewdata[8], 0, 16 * sizeof(uint32_t));
    ewdata[24] = (s << 16) | t;
    ewdata[25] = 0;
    ewdata[26] = ((dsdx >> 5) << 16);
    ewdata[27] = 0;
    ewdata[28] = 0;
    ewdata[29] = 0;
    ewdata[30] = ((dsdx & 0x1f) << 11) << 16;
    ewdata[31] = 0;
    ewdata[32] = (dtdy >> 5) & 0xffff;
    ewdata[33] = 0;
    ewdata[34] = (dtdy >> 5) & 0xffff;
    ewdata[35] = 0;
    ewdata[36] = (dtdy & 0x1f) << 11;
    ewdata[37] = 0;
    ewdata[38] = (dtdy & 0x1f) << 11;
    ewdata[39] = 0;
    memset(&ewdata[40], 0, 4 * sizeof(int32_t));



    edgewalker_for_prims(wstate, ewdata);

}

void rdp_tex_rect_flip(struct rdp_state* wstate, const uint32_t* args)
{
    uint32_t tilenum    = (args[1] >> 24) & 0x7;
    uint32_t xl = (args[0] >> 12) & 0xfff;
    uint32_t yl = (args[0] >>  0) & 0xfff;
    uint32_t xh = (args[1] >> 12) & 0xfff;
    uint32_t yh = (args[1] >>  0) & 0xfff;

    int32_t s = (args[2] >> 16) & 0xffff;
    int32_t t = (args[2] >>  0) & 0xffff;
    int32_t dsdx = (args[3] >> 16) & 0xffff;
    int32_t dtdy = (args[3] >>  0) & 0xffff;

    dsdx = SIGN16(dsdx);
    dtdy = SIGN16(dtdy);

    if (wstate->other_modes.cycle_type == CYCLE_TYPE_FILL || wstate->other_modes.cycle_type == CYCLE_TYPE_COPY)
        yl |= 3;

    uint32_t xlint = (xl >> 2) & 0x3ff;
    uint32_t xhint = (xh >> 2) & 0x3ff;

    int32_t ewdata[CMD_MAX_INTS];
    ewdata[0] = (0x25 << 24) | ((0x80 | tilenum) << 16) | yl;
    ewdata[1] = (yl << 16) | yh;
    ewdata[2] = (xlint << 16) | ((xl & 3) << 14);
    ewdata[3] = 0;
    ewdata[4] = (xhint << 16) | ((xh & 3) << 14);
    ewdata[5] = 0;
    ewdata[6] = (xlint << 16) | ((xl & 3) << 14);
    ewdata[7] = 0;
    memset(&ewdata[8], 0, 16 * sizeof(int32_t));
    ewdata[24] = (s << 16) | t;
    ewdata[25] = 0;

    ewdata[26] = (dtdy >> 5) & 0xffff;
    ewdata[27] = 0;
    ewdata[28] = 0;
    ewdata[29] = 0;
    ewdata[30] = ((dtdy & 0x1f) << 11);
    ewdata[31] = 0;
    ewdata[32] = (dsdx >> 5) << 16;
    ewdata[33] = 0;
    ewdata[34] = (dsdx >> 5) << 16;
    ewdata[35] = 0;
    ewdata[36] = (dsdx & 0x1f) << 27;
    ewdata[37] = 0;
    ewdata[38] = (dsdx & 0x1f) << 27;
    ewdata[39] = 0;
    memset(&ewdata[40], 0, 4 * sizeof(int32_t));

    edgewalker_for_prims(wstate, ewdata);
}

void rdp_fill_rect(struct rdp_state* wstate, const uint32_t* args)
{
    uint32_t xl = (args[0] >> 12) & 0xfff;
    uint32_t yl = (args[0] >>  0) & 0xfff;
    uint32_t xh = (args[1] >> 12) & 0xfff;
    uint32_t yh = (args[1] >>  0) & 0xfff;

    if (wstate->other_modes.cycle_type == CYCLE_TYPE_FILL || wstate->other_modes.cycle_type == CYCLE_TYPE_COPY)
        yl |= 3;

    uint32_t xlint = (xl >> 2) & 0x3ff;
    uint32_t xhint = (xh >> 2) & 0x3ff;

    int32_t ewdata[CMD_MAX_INTS];
    ewdata[0] = (0x3680 << 16) | yl;
    ewdata[1] = (yl << 16) | yh;
    ewdata[2] = (xlint << 16) | ((xl & 3) << 14);
    ewdata[3] = 0;
    ewdata[4] = (xhint << 16) | ((xh & 3) << 14);
    ewdata[5] = 0;
    ewdata[6] = (xlint << 16) | ((xl & 3) << 14);
    ewdata[7] = 0;
    memset(&ewdata[8], 0, 36 * sizeof(int32_t));

    edgewalker_for_prims(wstate, ewdata);
}

void rdp_set_prim_depth(struct rdp_state* wstate, const uint32_t* args)
{
    wstate->primitive_z = args[1] & (0x7fff << 16);


    wstate->primitive_delta_z = (uint16_t)(args[1]);
}

void rdp_set_scissor(struct rdp_state* wstate, const uint32_t* args)
{
    wstate->clip.xh = (args[0] >> 12) & 0xfff;
    wstate->clip.yh = (args[0] >>  0) & 0xfff;
    wstate->clip.xl = (args[1] >> 12) & 0xfff;
    wstate->clip.yl = (args[1] >>  0) & 0xfff;

    wstate->scfield = (args[1] >> 25) & 1;
    wstate->sckeepodd = (args[1] >> 24) & 1;
}

#endif // N64VIDEO_C
