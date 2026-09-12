# Makefile for Post Office Server (PO)

PROG=	po
SRCS=	main.c post.c retrieve.c check.c misc.c retr_wait.c send_back.c forward.c  send_bomb.c

DPADD+=	${LIBSYS} ${LIBTIMERS}
LDADD+=	-lsys -lminlib -ltimers -lcompat_minix

MAN=

BINDIR?= /usr/sbin

BINDIR?= /usr/sbin
FILES=po.conf
FILESNAME=po
FILESDIR= /etc/system.conf.d

.include <minix.service.mk>