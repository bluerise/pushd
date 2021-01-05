#	$OpenBSD$

PROG=	pushd
NOMAN=	yes
CFLAGS+=-Wall -Wunused -I/usr/local/include
LDFLAGS=-L/usr/local/lib
LDADD=	-lcurl
LDSTATC=-static

.include <bsd.prog.mk>
