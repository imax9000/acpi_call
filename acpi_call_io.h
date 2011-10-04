#ifndef ACPI_CALL_IO_H
#define ACPI_CALL_IO_H

#include <sys/ioccom.h>
#include <contrib/dev/acpica/include/actypes.h>

struct acpi_call_descr
{
	char		*path;
	uint8_t		argc;
	ACPI_OBJECT	*argv;
};

#define ACPIIO_CALL	_IOWR('X', 0x01, struct acpi_call_descr*)

#endif // ACPI_CALL_IO_H
