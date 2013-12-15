/** \file
 *
 *  Header file for wcfw.c.
 */

#ifndef _WCFW_H_
#define _WCFW_H_

void SetupHardware(void);
void CDC_Task(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

#endif
