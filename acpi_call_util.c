#include <contrib/dev/acpica/include/acpi.h>
#include "acpi_call_io.h"
#include <fcntl.h>
#include <sys/ioctl.h>

int main()
{
    	int fd;
	struct acpi_call_descr params;

	fd = open("/dev/acpi", O_RDWR);
	params.path = "full path to method here";
	params.args.Count = 0;
	params.args.Pointer = NULL;
	ioctl(fd, ACPIIO_CALL, &params);

	return 0;
}
