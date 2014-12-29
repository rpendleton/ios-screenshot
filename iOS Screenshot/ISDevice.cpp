//
//  ISDevice.cpp
//  iOS Screenshot
//
//  Created by Ryan Pendleton on 12/29/14.
//  Copyright (c) 2014 Inline-Studios. All rights reserved.
//

#include "ISDevice.h"
#include <stdio.h>
#include <stdlib.h>

ISDevice::ISDevice(const std::string &uuid)
{
	idevice_error_t ret = IDEVICE_E_SUCCESS;
	
	if(uuid.length() > 0)
	{
		ret = idevice_new(&device, uuid.c_str());
	}
	else
	{
		ret = idevice_new(&device, NULL);
	}
	
	if(ret != IDEVICE_E_SUCCESS)
	{
		throw IDEVICE_E_SUCCESS;
	}
}

ISDevice::~ISDevice()
{
	if(screenshotr)
	{
		screenshotr_client_free(screenshotr);
	}
	
	if(lockdownd)
	{
		lockdownd_client_free(lockdownd);
	}
	
	if(device)
	{
		idevice_free(device);
	}
}

void ISDevice::connect()
{
	lockdownd = nullptr;
	screenshotr = nullptr;
	
	int ret = lockdownd_client_new_with_handshake(device, &lockdownd, "com.inline-studios.ios-link");
	
	if(ret == IDEVICE_E_SUCCESS)
	{
		lockdownd_service_descriptor_t screenshotr_service = NULL;
		ret = lockdownd_start_service(lockdownd, SCREENSHOTR_SERVICE_NAME, &screenshotr_service);
		
		if(ret == LOCKDOWN_E_SUCCESS)
		{
			ret = screenshotr_client_new(device, screenshotr_service, &screenshotr);
		}
		
		lockdownd_service_descriptor_free(screenshotr_service);
	}
	
	if(ret != IDEVICE_E_SUCCESS)
	{
		throw (idevice_error_t)ret;
	}
}

Magick::Image ISDevice::take_screenshot()
{
	char *data = NULL;
	uint64_t size = 0;
	
	screenshotr_error_t ret = screenshotr_take_screenshot(screenshotr, &data, &size);
	
	if(ret != IDEVICE_E_SUCCESS)
	{
		delete[] data;
		throw ret;
	}
	
	Magick::Blob blob;
	blob.updateNoCopy(data, size);
	
	Magick::Image image(blob);
	image.magick("png");
	
	return image;
}
