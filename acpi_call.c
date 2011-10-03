#include <sys/types.h>
#include <sys/module.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/kernel.h>

static int
acpi_call_loader(struct module *m, int what, void *arg)
{
	int err = 0;
        
     	switch (what) {
	case MOD_LOAD:
		uprintf("acpi_call module loaded.\n");
		break;
	case MOD_UNLOAD:
		uprintf("acpi_call module unloaded.\n");
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

DECLARE_MODULE(acpi_call, acpi_call_mod, SI_SUB_KLD, SI_ORDER_ANY);
