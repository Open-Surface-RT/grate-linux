#include "efistub.h"

struct winrt_device_firmware{
	char firmware_vendor[40];
	enum winrt_device_names device_name;
} ;

struct winrt_device_firmware df[] = {
	{
		.device_name = surface_rt,
		.firmware_vendor = "OemkS EFI Jan 24 2014 18:00:42",
	},
	{
		.device_name = surface_2,
		.firmware_vendor = "Covington EFI Aug 26 2013 19:20:25",
	},
	{
		.device_name = surface_2,
		.firmware_vendor = "Surface 2 EFI Sep 11 2014 00:32:29",
	},
	{
		.device_name = unknown_device,
		.firmware_vendor = "ASUS UEFI Jul 24 2013 18:23:22"
	}
};

void to_ascii(char *out, efi_char16_t* in) {
	int i = 0;

	while (in[i] != '\0') {
		out[i] = in[i];
		i++;
	}
	out[i] = in[i];
}

enum winrt_device_names winrt_device_lookup(char* fw_ven) {
	int i;

	int dev_len = sizeof (df) / sizeof (df)[0];

	for (i = 0; i < dev_len; i++) {
		char *tmp = df[i].firmware_vendor;
		if (!strcmp(fw_ven, tmp)) {
			return df[i].device_name;
		}
	}

	return unknown_device;
}

enum winrt_device_names winrt_setup() {
	char fw_ascii[40];
	efi_char16_t * p_fw_ven = (void*)efi_system_table->fw_vendor;
	enum winrt_device_names ret = unknown_device;

	to_ascii(fw_ascii, p_fw_ven);
	ret = winrt_device_lookup(fw_ascii);

	if (ret < unknown_device) {
		efi_info("WinRT: Supported device detected\n");
		efi_info("WinRT: %s\n", fw_ascii);
	} else {
		efi_info("WinRT: No supported device detected\n");
		efi_info("WinRT: %s\n", fw_ascii);
	}

	return ret;
}

