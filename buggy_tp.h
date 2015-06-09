#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER buggy

#if !defined(_BUGGY_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _BUGGY_TP_H

/*
 * Copyright (C) 2015 EfficiOS Inc.
 * Copyright (C) 2015 Jérémie Galarneau <jeremie.galarneau@efficios.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <lttng/tracepoint.h>
#include <stdint.h>

/* Triggered when a request is ready for processing */
TRACEPOINT_EVENT(buggy, request_ready,
	TP_ARGS(char *, request_ptr),
	TP_FIELDS(
		ctf_integer_hex(char *, request_ptr, request_ptr)
	)
)

/* Triggered when request processing fails */
TRACEPOINT_EVENT(buggy, request_fail,
	TP_ARGS(char *, request_ptr),
	TP_FIELDS(
		ctf_integer_hex(char *, request_ptr, request_ptr)
	)
)

/* Triggered when a request is about to be read */
TRACEPOINT_EVENT(buggy, request_read,
        TP_ARGS(int, fd, uint16_t, size),
	TP_FIELDS(
		ctf_integer(int, fd, fd)
	        ctf_integer(uint16_t, size, size)
	)
)

#endif /* _BUGGY_TP_H */

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./buggy_tp.h"

/* This part must be outside ifdef protection */
#include <lttng/tracepoint-event.h>
