PUBLIC void __stdcall ServiceMain(DWORD, char **);

PUBLIC void __stdcall
service_control_handler(
DWORD control_code,
DWORD event_type,
void  *event_data,
void  *context)
{
switch(control_code)
    {
    case SERVICE_CONTROL_STOP:
	    on_service_stop();
    case SERVICE_CONTROL_PAUSE:
		on_service_pause();
    case SERVICE_CONTROL_CONTINUE:
		on_service_continue();
    case SERVICE_CONTROL_INTERROGATE:
		on_service_interrogate();
	case SERVICE_CONTROL_PRESHUTDOWN:
		on_service_preshutdown();
    case SERVICE_CONTROL_SHUTDOWN:
		on_service_shutdown();
	case SERVICE_CONTROL_PARAMCHANGE:
		on_service_param_change();

/* handlerex codes */
	case SERVICE_CONTROL_DEVICEEVENT:
		on_service_device_event();
	case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
		on_service_hardware_profile_change();
	case SERVICE_CONTROL_POWEREVENT:
		on_service_power_event();
	case SERVICE_CONTROL_SESSIONCHANGE:
		on_service_session_change();
	case SERVICE_CONTROL_TIMECHANGE:
		on_service_time_change();
	case SERVICE_CONTROL_TRIGGEREVENT:
		on_service_trigger_event();
	case SERVICE_CONTROL_USERMODEREBOOT:
		on_service_user_mode_reboot();
    break;
    }
}
 
SERVICE_TABLE_ENTRY service_table[] =
{
{service_name, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
{0, 0}
};
/* call from the main function */
/* StartServiceCtrlDispatcher(service_table); */

PUBLIC void __stdcall
ServiceMain(DWORD argc, char **argv)
{
SERVICE_STATUS_HANDLE hstatus;

SERVICE_STATUS status;
status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
//status.dwCurrentState = 
//status.dwControlsAccepted
//status.dwWin32ExitCode
//status.dwServiceSpecificExitCode
//status.dwCheckPoint
//status.dwWaitHint

hstatus = RegisterServiceCtrlHandlerEx(
    service_name, service_control_handler, 0);

/* wait for service to stop */
}
