SRCS=acpi_call.c opt_acpi.h
KMOD=acpi_call

UTIL_SRCS=acpi_call_util.c

util:
	${CC} ${UTIL_SRCS} -I@ -o acpi_call

.include <bsd.kmod.mk>
