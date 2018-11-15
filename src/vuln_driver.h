/**
* This header file defines the IOCTL numbers and macros for interacting 
* with the different vulnerabilities.
*/

#ifndef _VULN_DRIVER_

	#define _VULN_DRIVER_

	

	#define DEVICE_NAME "vulnerable_device"

	#define IOCTL_NUM 0xFE  // 幻数设定



	//ckx
	#define CKX_ESCAPEROOT _IO(IOCTL_NUM,14)
	#define CKX_ROOTONLY _IO(IOCTL_NUM, 15)
	#define ARBITRARY_RW_WRITE_USER_CKX _IOR(IOCTL_NUM, 16, unsigned long)
	#define CKX_NULL _IOR(IOCTL_NUM, 17, unsigned long)


#endif
