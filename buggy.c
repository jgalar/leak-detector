/*
 * Buggy application *suspected* of leaking memory...
 *
 * Part of Python Analysis Training
 *
 * Copyright (c) 2015 EfficiOS Inc.
 * Copyright (c) 2015 Jérémie Galarneau <jeremie.galarneau@efficios.com>
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

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "buggy_tp.h"

#define REQUEST_PATH "/dev/urandom"
#define BILLING_PATH "/dev/null"
#define ITERATION_COUNT 1000

int open_resource(void)
{
	int fd = open(REQUEST_PATH, O_RDONLY);

	if (fd < 0) {
		perror("Opening request fd");
	}

	return fd;
}

int bill_customer(void)
{
	int ret = 0;
	const uint8_t amount = 1;
	int billing_fd = open(BILLING_PATH, O_WRONLY);

	if (billing_fd < 0) {
		perror("Opening billing resource");
		ret = -1;
		goto end;
	}

	ret = write(billing_fd, &amount, sizeof(amount));
	if (ret < 0) {
		perror("Billing customer");
		goto end;
	}
end:
	if (billing_fd <= 0) {
		ret = close(billing_fd);
		perror("Closing billing resource");
	}
	return ret;
}

char *get_request_data(int fd)
{
	uint16_t bytes_to_read = 0;
	char *data = NULL;
	ssize_t ret;

	if (fd < 0) {
		goto end;
	}

	ret = read(fd, &bytes_to_read, sizeof(bytes_to_read));
	if (ret < 0) {
		/* Perhaps this should handle EINTR? :) */
		goto end;
	}

	data = malloc(bytes_to_read);
	if (!data) {
		/* Someone should definitely look into this memory leak... */
		perror("Could not allocate memory for request");
		goto end;
	}

	tracepoint(buggy, request_read, fd, bytes_to_read);
	ret = read(fd, data, (size_t) bytes_to_read);
	if (ret < 0) {
		goto end;
	}
end:
	return data;
}

int process_data(char *data)
{
	int ret = 0;

	if (!data) {
		ret = -1;
		goto end;
	}

	if (!data[0]) {
		ret = -1;
	}
end:
	return ret;
}
	
int main(int argc, char **argv)
{
	int fd = -1, ret = EXIT_SUCCESS, i;

	fd = open_resource();
	if (fd < 0) {
		ret = EXIT_FAILURE;
		goto end;
	}

	/* Process requests */
	for (i = 0; i < ITERATION_COUNT; i++) {
		char *request_data = get_request_data(fd);

		if (!request_data) {
			ret = EXIT_FAILURE;
			goto end;
		}

		
		tracepoint(buggy, request_ready, request_data);
		ret = process_data(request_data);
		if (ret < 0) {
			tracepoint(buggy, request_fail, request_data);
			/* Bad connection? Let's retry... */
		        ret = close(fd);
			if (ret < 0) {
				perror("Closing fd after bad request");
				ret = EXIT_FAILURE;
				goto end;
			}

			fd = open_resource();
			if (fd < 0) {
				ret = EXIT_FAILURE;
				goto end;
			}

			/* Don't bill customer */
			continue;
		}
	        ret = bill_customer();
		if (ret < 0) {
			goto end;
		}
		free(request_data);
	}

end:
	if (fd >= 0) {
		ret = close(fd);
		if (ret < 0) {
			perror("Closing request fd");
		}
	}
	return ret;
}
