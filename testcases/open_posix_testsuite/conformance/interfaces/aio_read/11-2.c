/*
 * Copyright (c) 2004, Bull SA. All rights reserved.
 * Created by:  Laurent.Vivier@bull.net
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this
 * source tree.
 */

/*
 * assertion:
 *
 * aio_read() shall fail with [EINVAL] or the error status of the operation
 * shall be [EINVAL] if aio_offset would be invalid, or aio_reqprio is not
 * a valid value, or aio_nbytes is an invalid value.
 *
 * Testing invalid reqprio
 *
 * method:
 *
 *	- Create an aiocb with an invalid aio_reqprio
 *	- call aio_read with this aiocb
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <aio.h>

#include "posixtest.h"
#include "tempfile.h"

#define TNAME "aio_read/11-2.c"

int main(void)
{
	char tmpfname[PATH_MAX];
#define BUF_SIZE 111
	char buf[BUF_SIZE];
	int fd;
	struct aiocb aiocb;

	if (sysconf(_SC_ASYNCHRONOUS_IO) < 200112L)
		return PTS_UNSUPPORTED;

	PTS_GET_TMP_FILENAME(tmpfname, "pts_aio_read_11_2");
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		printf(TNAME " Error at open(): %s\n", strerror(errno));
		exit(PTS_UNRESOLVED);
	}

	unlink(tmpfname);

	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = buf;
	aiocb.aio_reqprio = -1;
	aiocb.aio_nbytes = BUF_SIZE;

	if (aio_read(&aiocb) != -1) {
		printf(TNAME " bad aio_read return value()\n");
		exit(PTS_FAIL);
	}

	if (errno != EINVAL) {
		printf(TNAME " errno is not EINVAL %s\n", strerror(errno));
		exit(PTS_FAIL);
	}

	close(fd);
	printf("Test PASSED\n");
	return PTS_PASS;
}
