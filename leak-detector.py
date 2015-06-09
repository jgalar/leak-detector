#!/usr/bin/env python3
import babeltrace
import sys
import os

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

for event in trace_collection.events:
	# Good luck! :-)
	print("[{}] - {}".format(event.timestamp, event.name))
