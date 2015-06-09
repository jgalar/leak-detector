#!/bin/sh
#
# Copyright (c) 2015 EfficiOS Inc.
# Copyright (c) 2015 Jérémie Galarneau <jeremie.galarneau@efficios.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; only version 2
# of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
CURDIR=$(dirname $0)/

if [ ! -f buggy ]; then
    echo "buggy not found, trying to build..."
    make
    if [ $? -ne 0 ]; then
        exit
    fi
fi

LD_PRELOAD=liblttng-ust-libc-wrapper.so ./buggy
