# Copyright (C) 2015 EfficiOS Inc.
# Copyright (C) 2015 Jérémie Galarneau <jeremie.galarneau@efficios.com>
#
# THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
# OR IMPLIED. ANY USE IS AT YOUR OWN RISK.
#
# Permission is hereby granted to use or copy this program for any
# purpose, provided the above notices are retained on all copies.
# Permission to modify the code and to distribute modified code is
# granted, provided the above notices are retained, and a notice that
# the code was modified is included with the above copyright notice.
#
# This Makefile is not using automake so that users may see how to build
# a program with tracepoint provider probes compiled as static libraries.
#
# This makefile is purposefully kept simple to support GNU and BSD make.

LOCAL_CPPFLAGS += -I.
LIBS = -llttng-ust

all: buggy

lttng-ust-provider-buggy.o: tp.c buggy_tp.h
	$(CC) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(CFLAGS) $(AM_CPPFLAGS) \
		$(AM_CFLAGS) -c -o $@ $<

lttng-ust-provider-buggy.a: lttng-ust-provider-buggy.o
	ar -rc $@ lttng-ust-provider-buggy.o

buggy.o: buggy.c
	$(CC) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(CFLAGS) $(AM_CPPFLAGS) \
		$(AM_CFLAGS) -c -o $@ $<

buggy: buggy.o lttng-ust-provider-buggy.a
	$(CC) -o $@ $(LDFLAGS) $(CPPFLAGS) $(AM_LDFLAGS) $(AM_CFLAGS) \
		buggy.o lttng-ust-provider-buggy.a $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *.a buggy
