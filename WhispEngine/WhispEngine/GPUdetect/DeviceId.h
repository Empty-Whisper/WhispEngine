// Copyright 2010 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED ""AS IS.""
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

//
// DeviceId.h
//
#pragma once


// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <string>

#define INTEL_VENDOR_ID 0x8086

typedef enum
{ 
    IGFX_UNKNOWN     = 0x0, 
    IGFX_SANDYBRIDGE = 0xC, 
    IGFX_IVYBRIDGE,    
    IGFX_HASWELL,
	IGFX_VALLEYVIEW,
	IGFX_BROADWELL,
	IGFX_SKYLAKE
} PRODUCT_FAMILY;

//
// Device dependent counter and associated data structures
//
#define INTEL_DEVICE_INFO_COUNTERS         "Intel Device Information"

struct IntelDeviceInfoV1
{
    DWORD GPUMaxFreq;
    DWORD GPUMinFreq;
};

struct IntelDeviceInfoV2
{
	DWORD GPUMaxFreq;
	DWORD GPUMinFreq;
	DWORD GTGeneration;
	DWORD EUCount;
	DWORD PackageTDP;
	DWORD MaxFillRate;
};

struct IntelDeviceInfoHeader
{
	DWORD Size;
	DWORD Version;
};

/*****************************************************************************************
 * getGraphicsDeviceInfo
 *
 *     Function to get the primary graphics device's Vendor ID and Device ID, either 
 *     through the new DXGI interface or through the older D3D9 interfaces.
 *     The function also returns the amount of memory availble for graphics using 
 *     the value shared + dedicated video memory returned from DXGI, or, if the DXGI
 *	   interface is not available, the amount of memory returned from WMI.
 *
 *****************************************************************************************/

bool getGraphicsDeviceInfo( unsigned int* VendorId,
							unsigned int* DeviceId,
							std::wstring* GFXBrand,
							unsigned __int64* VideoMemoryBudget,
							unsigned __int64* VideoMemoryCurrentUsage,
							unsigned __int64* VideoMemoryAvailable,
							unsigned __int64* VideoMemoryReserved);

/*****************************************************************************************
 * getIntelDeviceInfo
 *
 *     Returns the device info:
 *       GPU Max Frequency (Mhz)
 *       GPU Min Frequency (Mhz)
 *       GT Generation (enum)
 *       EU Count (unsigned int)
 *       Package TDP (Watts)
 *       Max Fill Rate (Pixel/Clk)
 * 
 * A return value of GGF_SUCCESS indicates 
 *	   the frequency was returned correctly. 
 *     This function is only valid on Intel graphics devices SNB and later.
 *****************************************************************************************/



/*****************************************************************************************
 * checkDxExtensionVersion
 *
 *      Returns the EXTENSION_INTERFACE_VERSION supported by the driver
 *      EXTENSION_INTERFACE_VERSION_1_0 supports extensions for pixel synchronization and
 *      instant access of graphics memory
 *
 *****************************************************************************************/


/*****************************************************************************************
* getCPUInfo
*
*      Parses CPUID output to find the brand and vendor strings.
*
*****************************************************************************************/
void getCPUInfo(std::string* cpubrand, std::string* cpuvendor);


/*****************************************************************************************
* getGTGeneration
*
*      Returns the generation by parsing the device id. The first two digits of the hex
*      number generally denote the generation. Sandybridge and Ivybridge share the same
*      numbers so they must be further parsed.
*
*      Comparison of the deviceIds (for example to see if a device is more recent than
*      another) does not always work.
*
*****************************************************************************************/
PRODUCT_FAMILY getGTGeneration(unsigned int deviceId);




#define GGF_SUCCESS 0
#define GGF_ERROR					-1
#define GGF_E_UNSUPPORTED_HARDWARE	-2
#define GGF_E_UNSUPPORTED_DRIVER	-3
#define GGF_E_D3D_ERROR				-4