//Original Source Code Availability: https://github.com/bob-beck/examples/blob/master/fork1.c
/*
 * Copyright (c) 2008 Bob Beck <beck@obtuse.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t p;
	int seconds;

	printf("Process %d, starting up\n", getpid());

	p = fork();
	if (p == 0) {
		printf("Process %d (child), fork returned %d\n", getpid(), p);
		srandom(getpid());
		seconds = random() % 5;
		printf("Process %d (child) will be sleeping for %d seconds\n",
		    getpid(), seconds);
		sleep(seconds);
		printf("Process %d (child) now exiting\n", getpid());
		exit(0);
	}
	if (p == -1)
		err(1, "Fork failed! Can't create child!");
	printf("Process %d (parent), fork returned %d\n", getpid(), p);
	srandom(getpid());
	seconds = random() % 5;
	printf("Process %d (parent) will be sleeping for %d seconds\n",
	    getpid(), seconds);
	sleep(seconds);
	printf("Process %d (parent) now exiting\n", getpid());
	exit(0);
}