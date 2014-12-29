//
//  ISDevice.h
//  iOS Screenshot
//
//  Created by Ryan Pendleton on 12/29/14.
//  Copyright (c) 2014 Inline-Studios. All rights reserved.
//

#ifndef __iOS_Screenshot__ISDevice__
#define __iOS_Screenshot__ISDevice__

#define MAGICKCORE_QUANTUM_DEPTH	16
#define MAGICKCORE_HDRI_ENABLE		0

#include <stdio.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/screenshotr.h>
#include <Magick++.h>

class ISDevice {
	idevice_t device;
	lockdownd_client_t lockdownd;
	
	screenshotr_client_t screenshotr;

public:
	ISDevice(const std::string &uuid);
	~ISDevice();
	
	void connect();
	Magick::Image take_screenshot();
};

#endif /* defined(__iOS_Screenshot__ISDevice__) */
