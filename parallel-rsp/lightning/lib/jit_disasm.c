/*
 * Copyright (C) 2012-2023  Free Software Foundation, Inc.
 *
 * This file is part of GNU lightning.
 *
 * GNU lightning is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU lightning is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * Authors:
 *	Paulo Cesar Pereira de Andrade
 */

#include <lightning.h>
#include <lightning/jit_private.h>
#if DISASSEMBLER
#  include <dis-asm.h>
#endif

/*
 * Prototypes
 */
#if DISASSEMBLER
static int
disasm_compare_symbols(const void *ap, const void *bp);

static void
disasm_print_address(bfd_vma addr, struct disassemble_info *info);

#define disassemble(u, v)	_disassemble(_jit, u, v)
static void
_disassemble(jit_state_t *_jit, jit_pointer_t code, jit_int32_t length);
#endif

/*
 * Initialization
 */
#if DISASSEMBLER
static bfd			 *disasm_bfd;
static disassemble_info		  disasm_info;
static disassembler_ftype	  disasm_print;
static asymbol			**disasm_symbols;
static asymbol			 *disasm_synthetic;
static long			  disasm_num_symbols;
static long			  disasm_num_synthetic;
static jit_state_t		 *disasm_jit;
static FILE			 *disasm_stream;
#endif

#if BINUTILS_2_38
static int fprintf_styled(void * stream, enum disassembler_style style, const char* fmt, ...)
{
  va_list args;
  int r;

  va_start(args, fmt);
  r = vfprintf(disasm_stream, fmt, args);
  va_end(args);

  return r;
}
#endif

/*
 * Implementation
 */
void
jit_init_debug(const char *progname)
{
    jit_init_print();
#if DISASSEMBLER
    bfd_init();

    if (progname)
	disasm_bfd = bfd_openr(progname, NULL);
    if (disasm_bfd == NULL) {
#if defined(__linux__)
	disasm_bfd = bfd_openr("/proc/self/exe", NULL);
	if (disasm_bfd == NULL)
#endif
	    return;
    }
    bfd_check_format(disasm_bfd, bfd_object);
    bfd_check_format(disasm_bfd, bfd_archive);
    if (!disasm_stream)
	disasm_stream = stderr;

#if BINUTILS_2_38
    INIT_DISASSEMBLE_INFO(disasm_info, disasm_stream, fprintf, fprintf_styled);
#else
    INIT_DISASSEMBLE_INFO(disasm_info, disasm_stream, fprintf);
#endif
    disasm_info.arch = bfd_get_arch(disasm_bfd);
    disasm_info.mach = bfd_get_mach(disasm_bfd);

#  if HAVE_DISASSEMBLE_INIT_FOR_TARGET
    disassemble_init_for_target(&disasm_info);
#  endif

#  if defined(__powerpc64__)
    disasm_info.disassembler_options = "64";
#  endif
#  if defined(__sparc__) || defined(__s390__) || defined(__s390x__)
    disasm_info.endian = disasm_info.display_endian = BFD_ENDIAN_BIG;
#  endif
#  if defined(__s390__) || defined(__s390x__)
    disasm_info.disassembler_options = "zarch";
#  endif
    disasm_info.print_address_func = disasm_print_address;

# if BINUTILS_2_29
    disasm_print = disassembler(disasm_info.arch, __BYTE_ORDER == __BIG_ENDIAN,
				disasm_info.mach, disasm_bfd);
#  else
    disasm_print = disassembler(disasm_bfd);
#  endif
    assert(disasm_print);

    if (bfd_get_file_flags(disasm_bfd) & HAS_SYMS) {
	asymbol		**in;
	asymbol		**out;
	asymbol		 *symbol;
	long		  offset;
	long		  sym_count;
	long		  dyn_count;
	long		  sym_storage;
	long		  dyn_storage;

	if ((sym_storage = bfd_get_symtab_upper_bound(disasm_bfd)) >= 0) {

	    if (bfd_get_file_flags(disasm_bfd) & DYNAMIC) {
		dyn_storage = bfd_get_dynamic_symtab_upper_bound(disasm_bfd);
#  if defined(__alpha__)
		/* XXX */
		if (dyn_storage < 0)
		    dyn_storage = 0;
#  else
		assert(dyn_storage >= 0);
#  endif
	    }
	    else
		dyn_storage = 0;

	    jit_alloc((jit_pointer_t *)&disasm_symbols,
		      (sym_storage + dyn_storage) * sizeof(asymbol *));
	    sym_count = bfd_canonicalize_symtab(disasm_bfd, disasm_symbols);
	    assert(sym_count >= 0);
	    if (dyn_storage) {
		dyn_count = bfd_canonicalize_dynamic_symtab(disasm_bfd,
							    disasm_symbols +
							    sym_count);
		assert(dyn_count >= 0);
	    }
	    else
		dyn_count = 0;
	    disasm_num_symbols = sym_count + dyn_count;

	    disasm_num_synthetic = bfd_get_synthetic_symtab(disasm_bfd,
							    sym_count,
							    disasm_symbols,
							    dyn_count,
							    disasm_symbols +
							    sym_count,
							    &disasm_synthetic);
	    if (disasm_num_synthetic > 0) {
		jit_realloc((jit_pointer_t *)&disasm_symbols,
			    (sym_storage + dyn_storage) * sizeof(asymbol *),
			    (sym_storage + dyn_storage + disasm_num_synthetic) *
			    sizeof(asymbol *));
		for (offset = 0; offset < disasm_num_synthetic; offset++)
		    disasm_symbols[disasm_num_symbols++] =
			disasm_synthetic + offset;
	    }

	    /* remove symbols not useful for disassemble */
	    in = out = disasm_symbols;
	    for (offset = 0; offset < disasm_num_symbols; offset++) {
		symbol = *in++;
		if (symbol->name &&
		    symbol->name[0] != '\0' &&
		    !(symbol->flags & (BSF_DEBUGGING | BSF_SECTION_SYM)) &&
		    !bfd_is_und_section(symbol->section) &&
		    !bfd_is_com_section(symbol->section))
		    *out++ = symbol;
	    }
	    disasm_num_symbols = out - disasm_symbols;
	    qsort(disasm_symbols, disasm_num_symbols,
		  sizeof(asymbol *), disasm_compare_symbols);
	}
    }
#endif
}

void
jit_finish_debug(void)
{
#if DISASSEMBLER
    if (disasm_synthetic)
	jit_free((jit_pointer_t *)&disasm_synthetic);
    if (disasm_symbols)
	jit_free((jit_pointer_t *)&disasm_symbols);
    if (disasm_bfd)
	bfd_close (disasm_bfd);
#endif
}

void
_jit_disassemble(jit_state_t *_jit)
{
#if DISASSEMBLER
    if (disasm_bfd) {
#  if defined(__arm__)
	/* FIXME add mapping for prolog switching to arm and possible jump
	 * before first prolog also in arm mode */
	disasm_info.disassembler_options = jit_cpu.thumb ? "force-thumb" : "";
#  endif

	disassemble(_jit->code.ptr, _jit->pc.uc - _jit->code.ptr);
    }
#endif
}

#if DISASSEMBLER
/* Based on objdump source */
static int
disasm_compare_symbols(const void *ap, const void *bp)
{
    const asymbol	*a = *(const asymbol **)ap;
    const asymbol	*b = *(const asymbol **)bp;

    if (bfd_asymbol_value(a) > bfd_asymbol_value(b))
	return (1);
    if (bfd_asymbol_value(a) < bfd_asymbol_value(b))
	return (-1);
    return (0);
}

#if __WORDSIZE == 32
#  define address_buffer_length		16
#  define address_buffer_format		"%llx"
#else
#  define address_buffer_length		32
#  define address_buffer_format		"%lx"
#endif
static void
disasm_print_address(bfd_vma addr, struct disassemble_info *info)
{
    char		*name;
    char		*file;
    int			 line;
    char		 buffer[address_buffer_length];

    sprintf(buffer, address_buffer_format, addr);
    (*info->fprintf_func)(info->stream, "0x%s", buffer);

#  define _jit				disasm_jit
#  undef jit_pointer_p
#  define jit_pointer_p(u)					\
	((u) >= _jit->code.ptr && (u) < _jit->pc.uc)
    if (jit_pointer_p((jit_uint8_t *)(jit_word_t)addr)) {
	if (jit_get_note((jit_uint8_t *)(jit_word_t)addr, &name, &file, &line))
	    (*info->fprintf_func)(info->stream, " %s:%s:%d",
				  name ? name : "",
				  file ? file : "",
				  line);
    }
#  undef jit_pointer_p
#  undef _jit
    else if (disasm_num_symbols) {
	long		 low;
	long		 high;
	long		 offset;
	asymbol		*symbol;

	low = 0;
	high = disasm_num_symbols;
	do {
	    offset = (low + high) >> 1;
	    symbol = disasm_symbols[offset];
	    if (bfd_asymbol_value(symbol) > addr)
		high = offset - 1;
	    else if (bfd_asymbol_value(symbol) < addr)
		low = offset + 1;
	    else
		break;
	} while (low < high);

	if (offset >= 0 && offset < disasm_num_symbols) {
	    if (bfd_asymbol_value(symbol) < addr) {
		while (++offset < disasm_num_symbols) {
		    symbol = disasm_symbols[offset];
		    if (bfd_asymbol_value(symbol) >= addr)
			break;
		}
	    }
	    else if (bfd_asymbol_value(symbol) > addr) {
		while (offset--) {
		    if (bfd_asymbol_value(disasm_symbols[offset]) < addr)
			break;
		    symbol = disasm_symbols[offset];
		}
	    }
	    if (bfd_asymbol_value(symbol) == addr)
		(*info->fprintf_func)(info->stream, " # %s", symbol->name);
	}
    }
}

static void
_disassemble(jit_state_t *_jit, jit_pointer_t code, jit_int32_t length)
{
    int			 bytes;
    char		*name, *old_name;
    char		*file, *old_file;
    int			 line,  old_line;
#if __riscv && __WORDSIZE == 64
    jit_word_t		*vector;
    jit_int32_t		 offset;
#elif __arm__
    jit_int32_t		 offset;
    jit_bool_t		 data_info;
    jit_int32_t		 data_offset;
#endif
    bfd_vma		 pc = (jit_uword_t)code;
    bfd_vma		 end = (jit_uword_t)code + length;
    char		 buffer[address_buffer_length];
#if DEVEL_DISASSEMBLER
    jit_node_t		*node;
    jit_uword_t		 prevw;
#endif

#if __riscv && __WORDSIZE == 64
    end -= _jitc->consts.hash.count * 8;
#endif

#if __arm__
    data_info = _jitc && _jitc->data_info.ptr;
    data_offset = 0;
#endif
    disasm_info.buffer = code;
    disasm_info.buffer_vma = (jit_uword_t)code;
    disasm_info.buffer_length = length;
    old_file = old_name = NULL;
    old_line = 0;
    disasm_jit = _jit;
#if DEVEL_DISASSEMBLER
    node = _jitc->head;
    prevw = pc;
#endif
    while (pc < end) {
#if DEVEL_DISASSEMBLER
	while (node && (jit_uword_t)(prevw + node->offset) < (jit_uword_t)pc) {
	    prevw += node->offset;
	    node = node->next;
	}
	while (node && (jit_uword_t)(prevw + node->offset) == (jit_uword_t)pc) {
	    jit_print_node(node);
	    fputc('\n', disasm_stream);
	    prevw += node->offset;
	    node = node->next;
	}
#endif
#if __arm__
    again:
	if (data_info) {
	    while (_jitc->data_info.ptr[data_offset].code < pc) {
		if (++data_offset >= _jitc->data_info.length) {
		    data_info = 0;
		    goto again;
		}
	    }
	    if (pc == _jitc->data_info.ptr[data_offset].code) {
		offset = _jitc->data_info.ptr[data_offset].length;
		for (; offset >= 4; offset -= 4, pc += 4) {
		    bytes = sprintf(buffer, address_buffer_format, pc);
		    (*disasm_info.fprintf_func)(disasm_stream,
						"%*c0x%s\t.data\t0x%08x\n",
						16 - bytes, ' ', buffer,
						*(jit_uint32_t *)
						(jit_uint32_t)pc);
		}
		/* reset disassemble information instead of attempting
		 * to hack the arm specific backend data structures to
		 * tell it to forward the required number of bytes. */
		disasm_info.buffer = (jit_pointer_t)(jit_uint32_t)pc;
		disasm_info.buffer_vma = (jit_uword_t)pc;
		if ((disasm_info.buffer_length = end - pc) <= 0)
		    break;
	    }
	}
#endif
	if (jit_get_note((jit_uint8_t *)(jit_word_t)pc, &name, &file, &line) &&
	    (name != old_name || file != old_file || line != old_line)) {
	    (*disasm_info.fprintf_func)(disasm_stream, "# %s:%s:%d\n",
					name ? name : "",
					file ? file : "",
					line);
	    old_name = name;
	    old_file = file;
	    old_line = line;
	}

	bytes = sprintf(buffer, address_buffer_format, pc);
	(*disasm_info.fprintf_func)(disasm_stream, "%*c0x%s\t",
				    16 - bytes, ' ', buffer);
	pc += (*disasm_print)(pc, &disasm_info);
	putc('\n', disasm_stream);
    }
#if __riscv && __WORDSIZE == 64
    for (vector = (jit_word_t *)end, offset = 0;
	 offset < _jitc->consts.hash.count; offset++) {
	bytes = sprintf(buffer, address_buffer_format,
			(long long)end + offset * sizeof(jit_word_t));
	(*disasm_info.fprintf_func)(disasm_stream,
				    "%*c0x%s\t.quad\t0x%016lx\t# (%ld)\n",
				    16 - bytes, ' ', buffer,
				    vector[offset], vector[offset]);
    }
#endif
}
#endif
