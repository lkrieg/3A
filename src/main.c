#include "common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#define TMPFILE "_tmp"
#define ROWS    7
#define COLS    52
#define NSDAY   86400
#define NSHOUR  3600
#define NSMIN   60
#define NSSEC   1

/* 7 rows * 52 cols = 364 */
static unsigned char weeks[52];
static time_t head, tail;
static int tmpfd = -1;

static int load(const char *path);
static int emit(time_t date);
static int maketmp(void);

#include <errno.h>

int main(int argc, char **argv)
{
	time_t t;
	struct tm *local;

	if (argc < 2)
		Error(E_NOINPUT);

	if (load(argv[1]) < 0)
		Error(E_OPENFILE" '%s'", argv[1]);

	if (maketmp() < 0)
		Error(E_MAKETMP" '%s'", TMPFILE);

	t = time(NULL);
	local = localtime(&t);
	if (local == NULL)
		Error(E_GETTIME);

	tail = t
	     - local->tm_wday * NSDAY
	     - local->tm_hour * NSHOUR
	     - local->tm_min  * NSMIN
	     - local->tm_sec  * NSSEC;

	head = tail - ROWS * COLS * NSDAY;
	for (; head < tail; head += NSDAY)
		emit(head);

	close(tmpfd);
	remove(TMPFILE);

	return 0;
}

static int load(const char *path)
{
	int fd;
	char *buf = NULL;
	int nread = -1;
	int size  =  0;
	int max   =  0;

	if ((fd = open(path, O_RDONLY)) < 0)
		return -1;

	while (nread != 0) {
		if (size >= max) {
			max += CHUNK_SIZE;
			if ((buf = realloc(buf, max)) == NULL)
				Error(E_NOMEM);
		}

		if ((nread = read(fd, buf, CHUNK_SIZE)) < 0)
			Error(E_READFILE" '%s'", path);

		size += nread;
	}

	/* TODO: Parse input file */
	/* for (i = 0; i < size; i++) */
	/*	Info("%c", buf[i]); */

	close(fd);
	return 0;
}

static int emit(time_t date)
{
	char cmd[1024];

	Info("Emitting T=%lu...", date);

	if (maketmp() < 0)
		Error(E_MAKETMP" '%s'", TMPFILE);

	sprintf(cmd, "git commit -m \".\" --date %d", date);
	if ((system("git add "TMPFILE) != 0)
	|| ((system(cmd) != 0)))
		Error(E_COMMIT);

	/* TODO: Commit for given date */
	/* Info("%s\n", ctime(&date)); */
	UNUSED(weeks);

	return 0;
}


static int maketmp(void)
{
	if (tmpfd < 0) /* TODO: Ensure unique filename */
		return (tmpfd = creat(TMPFILE, 0777));

	if (write(tmpfd, ".", 1) < 1)
		return -1;

	return 0;
}
