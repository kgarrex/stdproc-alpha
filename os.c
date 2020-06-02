enum {
WINDOWS_XP_STARTER,
WINDOWS_XP_HOME,
WINDOWS_XP_PROFESSIONAL,
WINDOWS_XP_64,
WINDOWS_VISTA_STARTER,
WINDOWS_VISTA_HOME_BASIC,
WINDOWS_VISTA_HOME_PREMIUM,
WINDOWS_VISTA_BUSINESS,
WINDOWS_VISTA_ENTERPRISE,
WINDOWS_VISTA_ULTIMATE,
WINDOWS_7_STARTER,
WINDOWS_7_HOME_BASIC,
WINDOWS_7_HOME_PREMIUM,
WINDOWS_7_PRO,
WINDOWS_7_ENTERPRISE_BUSINESS,
WINDOWS_7_ULTIMATE,
WINDOWS_7_THIN,
WINDOWS_8,
WINDOWS_8_PRO,
WINDOWS_8_ENTERPRISE,
WINDOWS_8_OEM,
WINDOWS_8_1,
WINDOWS_8_1_PRO,
WINDOWS_8_1_ENTERPRISE,
WINDOWS_8_1_OEM,
WINDOWS_8_1_BING,
WINDOWS_10_HOME,
WINDOWS_10_PRO,
WINDOWS_10_PRO_EDUCTATION,
WINDOWS_10_ENTERPRISE,
WINDOWS_10_ENTERPRISE_LTSB,
WINDOWS_10_EDUCATION,
WINDOWS_10_IoT_CORE,
WINDOWS_10_IoT_ENTERPRISE,
WINDOWS_10_S
};

void get_os_version_info()
{
OSVERSIONINFOEX osinfo = {0};

osinfo.dwOSVersionInfoSize = sizeof(osinfo);
GetVersionEx(&osinfo);
/* GetProductInfo(); */
switch(osinfo.dwMajorVersion)
    {
    case 10:
    if(osinfo.dwMinorVersion == 0)
        process.os_version = PACK2_UINT8(10, 0); 
    break;
    case 6:
    if(osinfo.dwMinorVersion == 3)
	process.os_version = PACK2_UINT8(8, 1);
    else if(osinfo.dwMinorVersion == 2)
	process.os_version = PACK2_UINT8(8, 0);
    else if(osinfo.dwMinorVersion == 1)
	process.os_version = PACK2_UINT8(7, 0);
    }
}
