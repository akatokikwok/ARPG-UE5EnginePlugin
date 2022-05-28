// Copyright (C) RenZhai.2019.All Rights Reserved.

#include "SimpleThread.h"
#include "ThreadManage.h"

#define LOCTEXT_NAMESPACE "FSimpleThreadModule"

void FSimpleThreadModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSimpleThreadModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	GThread::Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleThreadModule, SimpleThread)