#!/usr/bin/env python3
import babeltrace
import sys

# get the trace path from the first command line argument
trace_path = sys.argv[1]

trace_collection = babeltrace.TraceCollection()
trace_collection.add_traces_recursive(trace_path, 'ctf')

for event in trace_collection.events:
	# Good luck! :-)
	print("[{}] - {}".format(event.timestamp, event.name))
