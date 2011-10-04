#ifndef ACPI_CALL_IO_H
#define ACPI_CALL_IO_H

#include <sys/ioccom.h>
#include <contrib/dev/acpica/include/actypes.h>

struct acpi_call_descr
{
	char		*path;
	ACPI_OBJECT_LIST	args;
};

#define ACPIIO_CALL	_IOWR('X', 0x01, caddr_t)

#endif // ACPI_CALL_IO_H
