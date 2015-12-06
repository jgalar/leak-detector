#!/usr/bin/env python3
import babeltrace
import sys
import os
import time

malloc_event_name = "lttng_ust_libc:malloc"
free_event_name = "lttng_ust_libc:free"

target_events = [malloc_event_name, free_event_name]

NSEC_PER_SEC = 1000000000


def ns_to_hour_nsec(ns, multi_day=False, gmt=False):
    if gmt:
        date = time.gmtime(ns / NSEC_PER_SEC)
    else:
        date = time.localtime(ns / NSEC_PER_SEC)
    if multi_day:
        return ('%04d-%02d-%02d %02d:%02d:%02d.%09d' %
                (date.tm_year, date.tm_mon, date.tm_mday, date.tm_hour,
                 date.tm_min, date.tm_sec, ns % NSEC_PER_SEC))
    else:
        return ('%02d:%02d:%02d.%09d' %
                (date.tm_hour, date.tm_min, date.tm_sec, ns % NSEC_PER_SEC))


if len(sys.argv) != 2:
    print("Usage: leak-detector /path/to/trace")
    sys.exit(1)

# get the trace path from the first command line argument
trace_path = sys.argv[1]

if not os.path.isdir(trace_path):
    print("Path {} does not exist, aborting...".format(trace_path))
    sys.exit(1)

trace_collection = babeltrace.TraceCollection()
trace_collection.add_traces_recursive(trace_path, 'ctf')

alloc_state = {}
for event in trace_collection.events:
    if event.name not in target_events:
        continue

    if event.name == malloc_event_name:
        alloc_state[event["ptr"]] = dict(ts=event.timestamp,
                                         size=event["size"])
    else:
        alloc_state.pop(event["ptr"], None)

if len(alloc_state) != 0:
    print("<Leaks detected>")
    for ptr, entry in alloc_state.items():
        print("[{}] {} bytes leaked at {}".format(
            ns_to_hour_nsec(entry["ts"]), entry["size"], hex(ptr)))
