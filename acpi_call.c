/*-
 *   Copyright (C) 2011 by Maxim Ignatenko
 *   gelraen.ua@gmail.com
 *
 *   All rights reserved.                                                  *
 *                                                                         *
 *   Redistribution and use in source and binary forms, with or without    *
 *    modification, are permitted provided that the following conditions   *
 *    are met:                                                             *
 *     * Redistributions of source code must retain the above copyright    *
 *       notice, this list of conditions and the following disclaimer.     *
 *     * Redistributions in binary form must reproduce the above copyright *
 *       notice, this list of conditions and the following disclaimer in   *
 *       the documentation and/or other materials provided with the        *
 *       distribution.                                                     *
 *                                                                         *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR *
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, *
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT      *
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, *
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY *
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   *
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE *
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *
 *
 */

#include <sys/types.h>
#include <sys/module.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <contrib/dev/acpica/include/acpi.h>
#include <dev/acpica/acpiio.h>
#include "acpi_call_io.h"


void acpi_call_fixup_pointers(ACPI_OBJECT *p, UINT8 *orig);

static int
acpi_call_ioctl(u_long cmd, caddr_t addr, void *arg)
{
	struct acpi_call_descr *params;
	ACPI_BUFFER	result;

	result.Length = ACPI_ALLOCATE_BUFFER;
	result.Pointer = NULL;

	if (cmd == ACPIIO_CALL) {
		params = (struct acpi_call_descr*)addr;
		params->retval = AcpiEvaluateObject(NULL, params->path, &params->args, &result);
		if (ACPI_SUCCESS(params->retval))
		{
			if (result.Pointer != NULL)
			{
				if (params->result.Pointer != NULL)
				{	
					params->result.Length = min(params->result.Length, result.Length);
					copyout(result.Pointer, params->result.Pointer,
							params->result.Length);
					params->reslen = result.Length;
					if (result.Length >= sizeof(ACPI_OBJECT))
						acpi_call_fixup_pointers((ACPI_OBJECT*)(params->result.Pointer), result.Pointer);
				}
				AcpiOsFree(result.Pointer);
			}
		}
	}

	return (0);
}

void
acpi_call_fixup_pointers(ACPI_OBJECT *p, UINT8 *orig)
{
	switch (p->Type)
	{
	case ACPI_TYPE_STRING:
		p->String.Pointer = (char*)((UINT8*)(p->String.Pointer) - orig + (UINT8*)p);
		break;
	case ACPI_TYPE_BUFFER:
		p->Buffer.Pointer -= orig - (UINT8*)p;
		break;
	}
}

static int
acpi_call_loader(struct module *m, int what, void *arg)
{
	int err = 0;
        
   	switch (what) {
	case MOD_LOAD:
		err = acpi_register_ioctl(ACPIIO_CALL, acpi_call_ioctl, NULL);
		break;
	case MOD_UNLOAD:
		acpi_deregister_ioctl(ACPIIO_CALL, acpi_call_ioctl);
		break;
	default:
		err = EOPNOTSUPP;
		break;
	}
	return(err);
}

static moduledata_t acpi_call_mod = {
	"acpi_call",
	acpi_call_loader,
	NULL
};  

DECLARE_MODULE(acpi_call, acpi_call_mod, SI_SUB_EXEC, SI_ORDER_ANY);
MODULE_DEPEND(acpi_call, acpi, 1, 1, 1);
