//
//  main.m
//  iOS Screenshot
//
//  Created by Ryan Pendleton on 12/29/14.
//  Copyright (c) 2014 Inline-Studios. All rights reserved.
//

#include <stdio.h>
#include <getopt.h>
#include <iostream>

#include "ISDevice.h"

bool handle_options(int argc, const char * argv[], std::string &uuid, std::string &path);
int show_usage(int argc, const char * argv[]);

int main(int argc, const char * argv[])
{
	std::string uuid;
	std::string path;
	ISDevice *device = nullptr;
	Magick::Image screenshot;
	
	if(handle_options(argc, argv, uuid, path))
	{
		return 0;
	}
	
	try
	{
		device = new ISDevice(uuid);
	}
	catch(idevice_error_t e)
	{
		if(device)
		{
			delete device;
			device = NULL;
		}
		
		std::cerr << "No device found. Is it plugged in?" << std::endl;
		return -1;
	};
	
	try
	{
		device->connect();
	}
	catch(idevice_error_t e)
	{
		std::cerr << "Failed to connect to device. Remember that the Developer disk image must be mounted on your device to use screenshotr." << std::endl;
		return -2;
	}
	
	try
	{
		screenshot = device->take_screenshot();
	}
	catch(idevice_error_t e)
	{
		std::cerr << "Failed to take screenshot. Error code: " << e << std::endl;
		return -3;
	}
	
	try
	{
		screenshot.write(path);
	}
	catch(std::exception e)
	{
		std::cerr << "Failed to save screenshot. Please make sure you have sufficient privileges to save the screenshot at " << path << std::endl;
	}
	
	return 0;
}

bool handle_options(int argc, const char * argv[], std::string &uuid, std::string &path)
{
	while(true)
	{
		static struct option long_options[] = {
			{"debug",	no_argument,		0, 'd'},
			{"help",	no_argument,		0, 'h'},
			{"uuid",	required_argument,	0, 'u'},
			{0, 0, 0, 0}
		};
		
		int option_index = 0;
		int c = getopt_long(argc, (char * const*)argv, "dhu:", long_options, &option_index);
		
		if (c == -1)
		{
			break;
		}
		
		switch (c)
		{
			case 'h':
				show_usage(argc, argv);
				return true;
				
			case 'd':
				idevice_set_debug_level(1);
				break;
				
			case 'u':
				uuid = optarg;
				break;
				
			case '?':
				break;
				
			default:
				abort ();
		}
	}
	
	if (optind < argc)
	{
		path = argv[optind++];
	}
	else
	{
		time_t now = time(NULL);
		
		char date[36];
		strftime(date, 36, "%Y-%m-%d at %H.%M.%S %p GMT", gmtime(&now));
		
		path = getenv("HOME");
		path += "/Desktop/Screen Shot ";
		path += date;
		path += ".png";
	}
	
	return false;
}

int show_usage(int argc, const char * argv[])
{
	const char *name = strrchr(argv[0], '/');
	name = name ? (name + 1) : argv[0];
	
	std::cout << "usage: " << name << " [OPTIONS] [PATH]" << std::endl;
	std::cout << "  -u, --uuid UUID\tconnect to a specific device using it's 40-digit device UUID" << std::endl;
	std::cout << "  -d, --debug\t\tenable libimobiledevice debugging" << std::endl;
	std::cout << "  -h, --help\t\tprints this usage information" << std::endl;
	
	return 0;
}
