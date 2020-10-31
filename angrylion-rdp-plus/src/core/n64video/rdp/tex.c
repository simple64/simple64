#ifdef N64VIDEO_C

static STRICTINLINE void tcmask(struct tile* tile, int32_t* S, int32_t* T)
{
    int32_t wrap;



    if (tile->mask_s)
    {
        if (tile->ms)
        {
            wrap = *S >> tile->f.masksclamped;
            wrap &= 1;
            *S ^= (-wrap);
        }
        *S &= maskbits_table[tile->mask_s];
    }

    if (tile->mask_t)
    {
        if (tile->mt)
        {
            wrap = *T >> tile->f.masktclamped;
            wrap &= 1;
            *T ^= (-wrap);
        }

        *T &= maskbits_table[tile->mask_t];
    }
}


static STRICTINLINE void tcmask_coupled(struct tile* tile, int32_t* S, int32_t* sdiff, int32_t* T, int32_t* tdiff)
{
    int32_t wrap;
    int32_t maskbits;
    int32_t wrapthreshold;


    if (tile->mask_s)
    {
        maskbits = maskbits_table[tile->mask_s];

        if (tile->ms)
        {
            wrapthreshold = tile->f.masksclamped;

            wrap = (*S >> wrapthreshold) & 1;
            *S ^= (-wrap);
            *S &= maskbits;


            if (((*S - wrap) & maskbits) == maskbits)
                *sdiff = 0;
            else
                *sdiff = 1 - (wrap << 1);
        }
        else
        {
            *S &= maskbits;
            if (*S == maskbits)
                *sdiff = -(*S);
            else
                *sdiff = 1;
        }
    }
    else
        *sdiff = 1;

    if (tile->mask_t)
    {
        maskbits = maskbits_table[tile->mask_t];

        if (tile->mt)
        {
            wrapthreshold = tile->f.masktclamped;

            wrap = (*T >> wrapthreshold) & 1;
            *T ^= (-wrap);
            *T &= maskbits;

            if (((*T - wrap) & maskbits) == maskbits)
                *tdiff = 0;
            else
                *tdiff = 1 - (wrap << 1);
        }
        else
        {
            *T &= maskbits;
            if (*T == maskbits)
                *tdiff = -(*T & 0xff);
            else
                *tdiff = 1;
        }
    }
    else
        *tdiff = 1;
}


static INLINE void calculate_clamp_diffs(struct tile* t)
{
    t->f.clampdiffs = ((t->sh >> 2) - (t->sl >> 2)) & 0x3ff;
    t->f.clampdifft = ((t->th >> 2) - (t->tl >> 2)) & 0x3ff;
}


static INLINE void calculate_tile_derivs(struct tile* t)
{
    t->f.clampens = t->cs || !t->mask_s;
    t->f.clampent = t->ct || !t->mask_t;
    t->f.masksclamped = t->mask_s <= 10 ? t->mask_s : 10;
    t->f.masktclamped = t->mask_t <= 10 ? t->mask_t : 10;
    t->f.notlutswitch = (t->format << 2) | t->size;
    t->f.tlutswitch = (t->size << 2) | ((t->format + 2) & 3);

    if (t->format < 5)
    {
        t->f.notlutswitch = (t->format << 2) | t->size;
        t->f.tlutswitch = (t->size << 2) | ((t->format + 2) & 3);
    }
    else
    {
        t->f.notlutswitch = 0x10 | t->size;
        t->f.tlutswitch = (t->size << 2) | 2;
    }
}

static STRICTINLINE void get_texel1_1cycle(struct rdp_state* wstate, int32_t* s1, int32_t* t1, int32_t s, int32_t t, int32_t w, int32_t dsinc, int32_t dtinc, int32_t dwinc, int32_t scanline, struct spansigs* sigs)
{
    int32_t nexts, nextt, nextsw;

    if (!sigs->endspan || !sigs->longspan || !wstate->span[scanline + 1].validline)
    {


        nextsw = (w + dwinc) >> 16;
        nexts = (s + dsinc) >> 16;
        nextt = (t + dtinc) >> 16;
    }
    else
    {







        int32_t nextscan = scanline + 1;
        nextt = wstate->span[nextscan].t >> 16;
        nexts = wstate->span[nextscan].s >> 16;
        nextsw = wstate->span[nextscan].w >> 16;
    }

    wstate->tcdiv_ptr(nexts, nextt, nextsw, s1, t1);
}

static STRICTINLINE void texture_pipeline_cycle(struct rdp_state* wstate, struct color* TEX, struct color* prev, int32_t SSS, int32_t SST, uint32_t tilenum, uint32_t cycle)
{
    int32_t maxs, maxt, invt3r, invt3g, invt3b, invt3a;
    int32_t sfrac, tfrac, invsf, invtf, sfracrg, invsfrg;
    int upper, upperrg, center, centerrg;


    int bilerp = cycle ? wstate->other_modes.bi_lerp1 : wstate->other_modes.bi_lerp0;
    int convert = wstate->other_modes.convert_one && cycle;
    struct color t0, t1, t2, t3;
    int sss1, sst1, sdiff, tdiff;

    sss1 = SSS;
    sst1 = SST;

    tcshift_cycle(&wstate->tile[tilenum], &sss1, &sst1, &maxs, &maxt);

    sss1 = TRELATIVE(sss1, wstate->tile[tilenum].sl);
    sst1 = TRELATIVE(sst1, wstate->tile[tilenum].tl);

    if (wstate->other_modes.sample_type || wstate->other_modes.en_tlut)
    {
        sfrac = sss1 & 0x1f;
        tfrac = sst1 & 0x1f;




        tcclamp_cycle(&wstate->tile[tilenum], &sss1, &sst1, &sfrac, &tfrac, maxs, maxt);






        tcmask_coupled(&wstate->tile[tilenum], &sss1, &sdiff, &sst1, &tdiff);







        upper = (sfrac + tfrac) & 0x20;




        if (wstate->tile[tilenum].format == FORMAT_YUV)
        {
            sfracrg = (sfrac >> 1) | ((sss1 & 1) << 4);



            upperrg = (sfracrg + tfrac) & 0x20;
        }
        else
        {
            upperrg = upper;
            sfracrg = sfrac;
        }

















        if (bilerp)
        {

            if (!wstate->other_modes.sample_type)
                fetch_texel_entlut_quadro_nearest(wstate, &t0, &t1, &t2, &t3, sss1, sst1, tilenum, upper, upperrg);
            else if (wstate->other_modes.en_tlut)
                fetch_texel_entlut_quadro(wstate, &t0, &t1, &t2, &t3, sss1, sdiff, sst1, tdiff, tilenum, upper, upperrg);
            else
                fetch_texel_quadro(wstate, &t0, &t1, &t2, &t3, sss1, sdiff, sst1, tdiff, tilenum, upper - upperrg);

            if (!wstate->other_modes.mid_texel)
                center = centerrg = 0;
            else
            {

                center = (sfrac == 0x10 && tfrac == 0x10);
                centerrg = (sfracrg == 0x10 && tfrac == 0x10);
            }

            if (!convert)
            {
                invtf = 0x20 - tfrac;

                if (!centerrg)
                {


                    if (upperrg)
                    {

                        invsfrg = 0x20 - sfracrg;

                        TEX->r = t3.r + ((invsfrg * (t2.r - t3.r) + invtf * (t1.r - t3.r) + 0x10) >> 5);
                        TEX->g = t3.g + ((invsfrg * (t2.g - t3.g) + invtf * (t1.g - t3.g) + 0x10) >> 5);
                    }
                    else
                    {
                        TEX->r = t0.r + ((sfracrg * (t1.r - t0.r) + tfrac * (t2.r - t0.r) + 0x10) >> 5);
                        TEX->g = t0.g + ((sfracrg * (t1.g - t0.g) + tfrac * (t2.g - t0.g) + 0x10) >> 5);
                    }
                }
                else
                {

                    invt3r = ~t3.r;
                    invt3g = ~t3.g;


                    TEX->r = t3.r + ((((t1.r + t2.r) << 6) - (t3.r << 7) + ((invt3r + t0.r) << 6) + 0xc0) >> 8);
                    TEX->g = t3.g + ((((t1.g + t2.g) << 6) - (t3.g << 7) + ((invt3g + t0.g) << 6) + 0xc0) >> 8);
                }

                if (!center)
                {
                    if (upper)
                    {
                        invsf = 0x20 - sfrac;

                        TEX->b = t3.b + ((invsf * (t2.b - t3.b) + invtf * (t1.b - t3.b) + 0x10) >> 5);
                        TEX->a = t3.a + ((invsf * (t2.a - t3.a) + invtf * (t1.a - t3.a) + 0x10) >> 5);
                    }
                    else
                    {
                        TEX->b = t0.b + ((sfrac * (t1.b - t0.b) + tfrac * (t2.b - t0.b) + 0x10) >> 5);
                        TEX->a = t0.a + ((sfrac * (t1.a - t0.a) + tfrac * (t2.a - t0.a) + 0x10) >> 5);
                    }
                }
                else
                {
                    invt3b = ~t3.b;
                    invt3a = ~t3.a;

                    TEX->b = t3.b + ((((t1.b + t2.b) << 6) - (t3.b << 7) + ((invt3b + t0.b) << 6) + 0xc0) >> 8);
                    TEX->a = t3.a + ((((t1.a + t2.a) << 6) - (t3.a << 7) + ((invt3a + t0.a) << 6) + 0xc0) >> 8);
                }
            }
            else
            {
                int32_t prevr, prevg, prevb;
                prevr = SIGN(prev->r, 9);
                prevg = SIGN(prev->g, 9);
                prevb = SIGN(prev->b, 9);

                if (!centerrg)
                {
                    if (upperrg)
                    {
                        TEX->r = prevb + ((prevr * (t2.r - t3.r) + prevg * (t1.r - t3.r) + 0x80) >> 8);
                        TEX->g = prevb + ((prevr * (t2.g - t3.g) + prevg * (t1.g - t3.g) + 0x80) >> 8);
                    }
                    else
                    {
                        TEX->r = prevb + ((prevr * (t1.r - t0.r) + prevg * (t2.r - t0.r) + 0x80) >> 8);
                        TEX->g = prevb + ((prevr * (t1.g - t0.g) + prevg * (t2.g - t0.g) + 0x80) >> 8);
                    }
                }
                else
                {
                    invt3r = ~t3.r;
                    invt3g = ~t3.g;

                    TEX->r = prevb + ((prevr * (t2.r - t3.r) + prevg * (t1.r - t3.r) + ((invt3r + t0.r) << 6) + 0xc0) >> 8);
                    TEX->g = prevb + ((prevr * (t2.g - t3.g) + prevg * (t1.g - t3.g) + ((invt3g + t0.g) << 6) + 0xc0) >> 8);
                }

                if (!center)
                {
                    if (upper)
                    {
                        TEX->b = prevb + ((prevr * (t2.b - t3.b) + prevg * (t1.b - t3.b) + 0x80) >> 8);
                        TEX->a = prevb + ((prevr * (t2.a - t3.a) + prevg * (t1.a - t3.a) + 0x80) >> 8);
                    }
                    else
                    {
                        TEX->b = prevb + ((prevr * (t1.b - t0.b) + prevg * (t2.b - t0.b) + 0x80) >> 8);
                        TEX->a = prevb + ((prevr * (t1.a - t0.a) + prevg * (t2.a - t0.a) + 0x80) >> 8);
                    }
                }
                else
                {
                    invt3b = ~t3.b;
                    invt3a = ~t3.a;

                    TEX->b = prevb + ((prevr * (t2.b - t3.b) + prevg * (t1.b - t3.b) + ((invt3b + t0.b) << 6) + 0xc0) >> 8);
                    TEX->a = prevb + ((prevr * (t2.a - t3.a) + prevg * (t1.a - t3.a) + ((invt3a + t0.a) << 6) + 0xc0) >> 8);
                }
            }
        }
        else
        {



            if (convert)
            {
                t0 = t3 = *prev;
                t0.r = SIGN(t0.r, 9);
                t0.g = SIGN(t0.g, 9);
                t0.b = SIGN(t0.b, 9);
                t3.r = SIGN(t3.r, 9);
                t3.g = SIGN(t3.g, 9);
                t3.b = SIGN(t3.b, 9);
            }
            else
            {
                if (!wstate->other_modes.sample_type)
                    fetch_texel_entlut_quadro_nearest(wstate, &t0, &t1, &t2, &t3, sss1, sst1, tilenum, upper, upperrg);
                else if (wstate->other_modes.en_tlut)
                    fetch_texel_entlut_quadro(wstate, &t0, &t1, &t2, &t3, sss1, sdiff, sst1, tdiff, tilenum, upper, upperrg);
                else
                    fetch_texel_quadro(wstate, &t0, &t1, &t2, &t3, sss1, sdiff, sst1, tdiff, tilenum, upper - upperrg);
            }


            if (upperrg)
            {
                if (upper)
                {
                    TEX->r = t3.b + ((wstate->k0_tf * t3.g + 0x80) >> 8);
                    TEX->g = t3.b + ((wstate->k1_tf * t3.r + wstate->k2_tf * t3.g + 0x80) >> 8);
                    TEX->b = t3.b + ((wstate->k3_tf * t3.r + 0x80) >> 8);
                    TEX->a = t3.b;
                }
                else
                {
                    TEX->r = t0.b + ((wstate->k0_tf * t3.g + 0x80) >> 8);
                    TEX->g = t0.b + ((wstate->k1_tf * t3.r + wstate->k2_tf * t3.g + 0x80) >> 8);
                    TEX->b = t0.b + ((wstate->k3_tf * t3.r + 0x80) >> 8);
                    TEX->a = t0.b;
                }
            }
            else
            {
                if (upper)
                {
                    TEX->r = t3.b + ((wstate->k0_tf * t0.g + 0x80) >> 8);
                    TEX->g = t3.b + ((wstate->k1_tf * t0.r + wstate->k2_tf * t0.g + 0x80) >> 8);
                    TEX->b = t3.b + ((wstate->k3_tf * t0.r + 0x80) >> 8);
                    TEX->a = t3.b;
                }
                else
                {
                    TEX->r = t0.b + ((wstate->k0_tf * t0.g + 0x80) >> 8);
                    TEX->g = t0.b + ((wstate->k1_tf * t0.r + wstate->k2_tf * t0.g + 0x80) >> 8);
                    TEX->b = t0.b + ((wstate->k3_tf * t0.r + 0x80) >> 8);
                    TEX->a = t0.b;
                }
            }
        }

        TEX->r &= 0x1ff;
        TEX->g &= 0x1ff;
        TEX->b &= 0x1ff;
        TEX->a &= 0x1ff;


    }
    else
    {




        tcclamp_cycle_light(&wstate->tile[tilenum], &sss1, &sst1, maxs, maxt);

        tcmask(&wstate->tile[tilenum], &sss1, &sst1);




        if (bilerp)
        {
            if (!convert)
            {

                fetch_texel(wstate, &t0, sss1, sst1, tilenum);

                TEX->r = t0.r & 0x1ff;
                TEX->g = t0.g & 0x1ff;
                TEX->b = t0.b;
                TEX->a = t0.a;
            }
            else
                TEX->r = TEX->g = TEX->b = TEX->a = prev->b;
        }
        else
        {
            if (convert)
            {
                t0 = *prev;
                t0.r = SIGN(t0.r, 9);
                t0.g = SIGN(t0.g, 9);
                t0.b = SIGN(t0.b, 9);
            }
            else
                fetch_texel(wstate, &t0, sss1, sst1, tilenum);

            TEX->r = t0.b + ((wstate->k0_tf * t0.g + 0x80) >> 8);
            TEX->g = t0.b + ((wstate->k1_tf * t0.r + wstate->k2_tf * t0.g + 0x80) >> 8);
            TEX->b = t0.b + ((wstate->k3_tf * t0.r + 0x80) >> 8);
            TEX->a = t0.b & 0x1ff;
            TEX->r &= 0x1ff;
            TEX->g &= 0x1ff;
            TEX->b &= 0x1ff;
        }
    }

}

static void loading_pipeline(struct rdp_state* wstate, int start, int end, int tilenum, int coord_quad, int ltlut)
{


    //int localdebugmode = 0, cnt = 0;
    int i, j;

    int dsinc, dtinc;
    dsinc = wstate->spans_ds;
    dtinc = wstate->spans_dt;

    int s, t;
    int ss, st;
    int xstart, xend, xendsc;
    int sss = 0, sst = 0;
    int ti_index, length;

    uint32_t tmemidx0 = 0, tmemidx1 = 0, tmemidx2 = 0, tmemidx3 = 0;
    int dswap = 0;
    uint32_t readval0, readval1, readval2, readval3;
    uint32_t readidx32;
    uint64_t loadqword = 0;
    uint16_t tempshort = 0;
    int tmem_formatting = 0;
    uint32_t bit3fl = 0, hibit = 0;

    if (end > start && ltlut)
    {
        rdp_pipeline_crashed = 1;
        return;
    }

    if (wstate->tile[tilenum].format == FORMAT_YUV)
        tmem_formatting = 0;
    else if (wstate->tile[tilenum].format == FORMAT_RGBA && wstate->tile[tilenum].size == PIXEL_SIZE_32BIT)
        tmem_formatting = 1;
    else
        tmem_formatting = 2;

    int tiadvance = 0, spanadvance = 0;
    int tiptr = 0;
    switch (wstate->ti_size)
    {
    case PIXEL_SIZE_4BIT:
        rdp_pipeline_crashed = 1;
        return;
        break;
    case PIXEL_SIZE_8BIT:
        tiadvance = 8;
        spanadvance = 8;
        break;
    case PIXEL_SIZE_16BIT:
        if (!ltlut)
        {
            tiadvance = 8;
            spanadvance = 4;
        }
        else
        {
            tiadvance = 2;
            spanadvance = 1;
        }
        break;
    case PIXEL_SIZE_32BIT:
        tiadvance = 8;
        spanadvance = 2;
        break;
    }

    for (i = start; i <= end; i++)
    {
        xstart = wstate->span[i].lx;
        xend = wstate->span[i].unscrx;
        xendsc = wstate->span[i].rx;
        s = wstate->span[i].s;
        t = wstate->span[i].t;

        ti_index = wstate->ti_width * i + xend;
        tiptr = wstate->ti_address + PIXELS_TO_BYTES(ti_index, wstate->ti_size);

        length = (xstart - xend + 1) & 0xfff;

        for (j = 0; j < length; j+= spanadvance)
        {
            ss = s >> 16;
            st = t >> 16;







            sss = ss & 0xffff;
            sst = st & 0xffff;

            tc_pipeline_load(&wstate->tile[tilenum], &sss, &sst, coord_quad);

            dswap = sst & 1;


            get_tmem_idx(wstate, sss, sst, tilenum, &tmemidx0, &tmemidx1, &tmemidx2, &tmemidx3, &bit3fl, &hibit);

            readidx32 = (tiptr >> 2) & ~1;
            RREADIDX32(readval0, readidx32);
            readidx32++;
            RREADIDX32(readval1, readidx32);
            readidx32++;
            RREADIDX32(readval2, readidx32);
            readidx32++;
            RREADIDX32(readval3, readidx32);


            switch(tiptr & 7)
            {
            case 0:
                if (!ltlut)
                    loadqword = ((uint64_t)readval0 << 32) | readval1;
                else
                {
                    tempshort = readval0 >> 16;
                    loadqword = ((uint64_t)tempshort << 48) | ((uint64_t) tempshort << 32) | ((uint64_t) tempshort << 16) | tempshort;
                }
                break;
            case 1:
                loadqword = ((uint64_t)readval0 << 40) | ((uint64_t)readval1 << 8) | (readval2 >> 24);
                break;
            case 2:
                if (!ltlut)
                    loadqword = ((uint64_t)readval0 << 48) | ((uint64_t)readval1 << 16) | (readval2 >> 16);
                else
                {
                    tempshort = readval0 & 0xffff;
                    loadqword = ((uint64_t)tempshort << 48) | ((uint64_t) tempshort << 32) | ((uint64_t) tempshort << 16) | tempshort;
                }
                break;
            case 3:
                loadqword = ((uint64_t)readval0 << 56) | ((uint64_t)readval1 << 24) | (readval2 >> 8);
                break;
            case 4:
                if (!ltlut)
                    loadqword = ((uint64_t)readval1 << 32) | readval2;
                else
                {
                    tempshort = readval1 >> 16;
                    loadqword = ((uint64_t)tempshort << 48) | ((uint64_t) tempshort << 32) | ((uint64_t) tempshort << 16) | tempshort;
                }
                break;
            case 5:
                loadqword = ((uint64_t)readval1 << 40) | ((uint64_t)readval2 << 8) | (readval3 >> 24);
                break;
            case 6:
                if (!ltlut)
                    loadqword = ((uint64_t)readval1 << 48) | ((uint64_t)readval2 << 16) | (readval3 >> 16);
                else
                {
                    tempshort = readval1 & 0xffff;
                    loadqword = ((uint64_t)tempshort << 48) | ((uint64_t) tempshort << 32) | ((uint64_t) tempshort << 16) | tempshort;
                }
                break;
            case 7:
                loadqword = ((uint64_t)readval1 << 56) | ((uint64_t)readval2 << 24) | (readval3 >> 8);
                break;
            }


            switch(tmem_formatting)
            {
            case 0:
                readval0 = (uint32_t)((((loadqword >> 56) & 0xff) << 24) | (((loadqword >> 40) & 0xff) << 16) | (((loadqword >> 24) & 0xff) << 8) | (((loadqword >> 8) & 0xff) << 0));
                readval1 = (uint32_t)((((loadqword >> 48) & 0xff) << 24) | (((loadqword >> 32) & 0xff) << 16) | (((loadqword >> 16) & 0xff) << 8) | (((loadqword >> 0) & 0xff) << 0));
                if (bit3fl)
                {
                    tmem16[tmemidx2 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 >> 16);
                    tmem16[tmemidx3 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 & 0xffff);
                    tmem16[(tmemidx2 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 >> 16);
                    tmem16[(tmemidx3 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 & 0xffff);
                }
                else
                {
                    tmem16[tmemidx0 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 >> 16);
                    tmem16[tmemidx1 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 & 0xffff);
                    tmem16[(tmemidx0 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 >> 16);
                    tmem16[(tmemidx1 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 & 0xffff);
                }
                break;
            case 1:
                readval0 = (uint32_t)(((loadqword >> 48) << 16) | ((loadqword >> 16) & 0xffff));
                readval1 = (uint32_t)((((loadqword >> 32) & 0xffff) << 16) | (loadqword & 0xffff));

                if (bit3fl)
                {
                    tmem16[tmemidx2 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 >> 16);
                    tmem16[tmemidx3 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 & 0xffff);
                    tmem16[(tmemidx2 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 >> 16);
                    tmem16[(tmemidx3 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 & 0xffff);
                }
                else
                {
                    tmem16[tmemidx0 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 >> 16);
                    tmem16[tmemidx1 ^ WORD_ADDR_XOR] = (uint16_t)(readval0 & 0xffff);
                    tmem16[(tmemidx0 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 >> 16);
                    tmem16[(tmemidx1 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(readval1 & 0xffff);
                }
                break;
            case 2:
                if (!dswap)
                {
                    if (!hibit)
                    {
                        tmem16[tmemidx0 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 48);
                        tmem16[tmemidx1 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 32);
                        tmem16[tmemidx2 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 16);
                        tmem16[tmemidx3 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword & 0xffff);
                    }
                    else
                    {
                        tmem16[(tmemidx0 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 48);
                        tmem16[(tmemidx1 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 32);
                        tmem16[(tmemidx2 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 16);
                        tmem16[(tmemidx3 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword & 0xffff);
                    }
                }
                else
                {
                    if (!hibit)
                    {
                        tmem16[tmemidx0 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 16);
                        tmem16[tmemidx1 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword & 0xffff);
                        tmem16[tmemidx2 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 48);
                        tmem16[tmemidx3 ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 32);
                    }
                    else
                    {
                        tmem16[(tmemidx0 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 16);
                        tmem16[(tmemidx1 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword & 0xffff);
                        tmem16[(tmemidx2 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 48);
                        tmem16[(tmemidx3 | 0x400) ^ WORD_ADDR_XOR] = (uint16_t)(loadqword >> 32);
                    }
                }
            break;
            }


            s = (s + dsinc) & ~0x1f;
            t = (t + dtinc) & ~0x1f;
            tiptr += tiadvance;
        }
    }
}

static void edgewalker_for_loads(struct rdp_state* wstate, int32_t* lewdata)
{
    int j = 0;
    int xleft = 0, xright = 0;
    int /*xstart = 0,*/ xend = 0;
    int s = 0, t = 0, w = 0;
    int dsdx = 0, dtdx = 0;
    int dsdy = 0, dtdy = 0;
    int dsde = 0, dtde = 0;
    int tilenum = 0, flip = 0;
    int32_t yl = 0, ym = 0, yh = 0;
    int32_t xl = 0, xm = 0, xh = 0;
    int32_t dxldy = 0, dxhdy = 0, dxmdy = 0;

    int cmd_id = CMD_ID(lewdata);
    int ltlut = (cmd_id == CMD_ID_LOAD_TLUT);
    int coord_quad = ltlut || (cmd_id == CMD_ID_LOAD_BLOCK);
    flip = 1;
    wstate->max_level = 0;
    tilenum = (lewdata[0] >> 16) & 7;


    yl = SIGN(lewdata[0], 14);
    ym = lewdata[1] >> 16;
    ym = SIGN(ym, 14);
    yh = SIGN(lewdata[1], 14);

    xl = SIGN(lewdata[2], 28);
    xh = SIGN(lewdata[3], 28);
    xm = SIGN(lewdata[4], 28);

    dxldy = 0;
    dxhdy = 0;
    dxmdy = 0;


    s    = lewdata[5] & 0xffff0000;
    t    = (lewdata[5] & 0xffff) << 16;
    w    = 0;
    dsdx = (lewdata[7] & 0xffff0000) | ((lewdata[6] >> 16) & 0xffff);
    dtdx = ((lewdata[7] << 16) & 0xffff0000)    | (lewdata[6] & 0xffff);
    dsde = 0;
    dtde = (lewdata[9] & 0xffff) << 16;
    dsdy = 0;
    dtdy = (lewdata[8] & 0xffff) << 16;

    wstate->spans_ds = dsdx & ~0x1f;
    wstate->spans_dt = dtdx & ~0x1f;
    wstate->spans_dw = 0;






    xright = xh & ~0x1;
    xleft = xm & ~0x1;

    int k = 0;

    //int sign_dxhdy = 0;

    //int do_offset = 0;

    int xfrac = 0;

















    int32_t maxxmx = 0, minxhx = 0;

    int spix = 0;
    int ycur =  yh & ~3;
    int ylfar = yl | 3;

    int valid_y = 1;
    //int length = 0;
    int32_t xrsc = 0, xlsc = 0/*, stickybit = 0*/;
    int32_t yllimit = yl;
    int32_t yhlimit = yh;

    xfrac = 0;
    xend = xright >> 16;


    for (k = ycur; k <= ylfar; k++)
    {
        if (k == ym)
            xleft = xl & ~1;

        spix = k & 3;

        if (!(k & ~0xfff))
        {
            j = k >> 2;
            valid_y = !(k < yhlimit || k >= yllimit);

            if (spix == 0)
            {
                maxxmx = 0;
                minxhx = 0xfff;
            }

            xrsc = (xright >> 13) & 0x7ffe;



            xlsc = (xleft >> 13) & 0x7ffe;

            if (valid_y)
            {
                maxxmx = (((xlsc >> 3) & 0xfff) > maxxmx) ? (xlsc >> 3) & 0xfff : maxxmx;
                minxhx = (((xrsc >> 3) & 0xfff) < minxhx) ? (xrsc >> 3) & 0xfff : minxhx;
            }

            if (spix == 0)
            {
                wstate->span[j].unscrx = xend;
                wstate->span[j].s = s & ~0x3ff;
                wstate->span[j].t = t & ~0x3ff;
            }

            if (spix == 3)
            {
                wstate->span[j].lx = maxxmx;
                wstate->span[j].rx = minxhx;


            }


        }

        if (spix == 3)
        {
            t += dtde;
        }



    }

    loading_pipeline(wstate, yhlimit >> 2, yllimit >> 2, tilenum, coord_quad, ltlut);
}

void rdp_set_tile_size(struct rdp_state* wstate, const uint32_t* args)
{
    int tilenum = (args[1] >> 24) & 0x7;
    wstate->tile[tilenum].sl = (args[0] >> 12) & 0xfff;
    wstate->tile[tilenum].tl = (args[0] >>  0) & 0xfff;
    wstate->tile[tilenum].sh = (args[1] >> 12) & 0xfff;
    wstate->tile[tilenum].th = (args[1] >>  0) & 0xfff;

    calculate_clamp_diffs(&wstate->tile[tilenum]);
}

void rdp_load_block(struct rdp_state* wstate, const uint32_t* args)
{
    int tilenum = (args[1] >> 24) & 0x7;
    uint16_t sl, sh, tl, dxt;


    wstate->tile[tilenum].sl = sl = ((args[0] >> 12) & 0xfff);
    wstate->tile[tilenum].tl = tl = ((args[0] >>  0) & 0xfff);
    wstate->tile[tilenum].sh = sh = ((args[1] >> 12) & 0xfff);
    wstate->tile[tilenum].th = dxt  = ((args[1] >>  0) & 0xfff);

    calculate_clamp_diffs(&wstate->tile[tilenum]);

    int tlclamped = tl & 0x3ff;

    int32_t lewdata[10];

    lewdata[0] = (args[0] & 0xff000000) | (0x10 << 19) | (tilenum << 16) | ((tlclamped << 2) | 3);
    lewdata[1] = (((tlclamped << 2) | 3) << 16) | (tlclamped << 2);
    lewdata[2] = sh << 16;
    lewdata[3] = sl << 16;
    lewdata[4] = sh << 16;
    lewdata[5] = ((sl << 3) << 16) | (tl << 3);
    lewdata[6] = (dxt & 0xff) << 8;
    lewdata[7] = ((0x80 >> wstate->ti_size) << 16) | (dxt >> 8);
    lewdata[8] = 0x20;
    lewdata[9] = 0x20;

    edgewalker_for_loads(wstate, lewdata);

}

static void tile_tlut_common_cs_decoder(struct rdp_state* wstate, const uint32_t* args)
{
    int tilenum = (args[1] >> 24) & 0x7;
    uint16_t sl, tl, sh, th;


    wstate->tile[tilenum].sl = sl = ((args[0] >> 12) & 0xfff);
    wstate->tile[tilenum].tl = tl = ((args[0] >>  0) & 0xfff);
    wstate->tile[tilenum].sh = sh = ((args[1] >> 12) & 0xfff);
    wstate->tile[tilenum].th = th = ((args[1] >>  0) & 0xfff);

    calculate_clamp_diffs(&wstate->tile[tilenum]);


    int32_t lewdata[10];

    lewdata[0] = (args[0] & 0xff000000) | (0x10 << 19) | (tilenum << 16) | (th | 3);
    lewdata[1] = ((th | 3) << 16) | (tl);
    lewdata[2] = ((sh >> 2) << 16) | ((sh & 3) << 14);
    lewdata[3] = ((sl >> 2) << 16) | ((sl & 3) << 14);
    lewdata[4] = ((sh >> 2) << 16) | ((sh & 3) << 14);
    lewdata[5] = ((sl << 3) << 16) | (tl << 3);
    lewdata[6] = 0;
    lewdata[7] = (0x200 >> wstate->ti_size) << 16;
    lewdata[8] = 0x20;
    lewdata[9] = 0x20;

    edgewalker_for_loads(wstate, lewdata);
}

void rdp_load_tlut(struct rdp_state* wstate, const uint32_t* args)
{
    tile_tlut_common_cs_decoder(wstate, args);
}

void rdp_load_tile(struct rdp_state* wstate, const uint32_t* args)
{
    tile_tlut_common_cs_decoder(wstate, args);
}

void rdp_set_tile(struct rdp_state* wstate, const uint32_t* args)
{
    int tilenum = (args[1] >> 24) & 0x7;

    wstate->tile[tilenum].format    = (args[0] >> 21) & 0x7;
    wstate->tile[tilenum].size      = (args[0] >> 19) & 0x3;
    wstate->tile[tilenum].line      = (args[0] >>  9) & 0x1ff;
    wstate->tile[tilenum].tmem      = (args[0] >>  0) & 0x1ff;
    wstate->tile[tilenum].palette   = (args[1] >> 20) & 0xf;
    wstate->tile[tilenum].ct        = (args[1] >> 19) & 0x1;
    wstate->tile[tilenum].mt        = (args[1] >> 18) & 0x1;
    wstate->tile[tilenum].mask_t    = (args[1] >> 14) & 0xf;
    wstate->tile[tilenum].shift_t   = (args[1] >> 10) & 0xf;
    wstate->tile[tilenum].cs        = (args[1] >>  9) & 0x1;
    wstate->tile[tilenum].ms        = (args[1] >>  8) & 0x1;
    wstate->tile[tilenum].mask_s    = (args[1] >>  4) & 0xf;
    wstate->tile[tilenum].shift_s   = (args[1] >>  0) & 0xf;

    calculate_tile_derivs(&wstate->tile[tilenum]);
}

void rdp_set_texture_image(struct rdp_state* wstate, const uint32_t* args)
{
    wstate->ti_format   = (args[0] >> 21) & 0x7;
    wstate->ti_size     = (args[0] >> 19) & 0x3;
    wstate->ti_width    = (args[0] & 0x3ff) + 1;
    wstate->ti_address  = args[1] & 0x0ffffff;



}

void rdp_set_convert(struct rdp_state* wstate, const uint32_t* args)
{
    int32_t k0 = (args[0] >> 13) & 0x1ff;
    int32_t k1 = (args[0] >> 4) & 0x1ff;
    int32_t k2 = ((args[0] & 0xf) << 5) | ((args[1] >> 27) & 0x1f);
    int32_t k3 = (args[1] >> 18) & 0x1ff;
    wstate->k0_tf = (SIGN(k0, 9) << 1) + 1;
    wstate->k1_tf = (SIGN(k1, 9) << 1) + 1;
    wstate->k2_tf = (SIGN(k2, 9) << 1) + 1;
    wstate->k3_tf = (SIGN(k3, 9) << 1) + 1;
    wstate->k4 = (args[1] >> 9) & 0x1ff;
    wstate->k5 = args[1] & 0x1ff;
}

static void tex_init_lut(void)
{
    tmem_init_lut();
    tcoord_init_lut();
}

static void tex_init(struct rdp_state* wstate)
{
    int i;
    tcoord_init(wstate);

    for (i = 0; i < 8; i++)
    {
        calculate_tile_derivs(&wstate->tile[i]);
        calculate_clamp_diffs(&wstate->tile[i]);
    }
}

#endif // N64VIDEO_C
