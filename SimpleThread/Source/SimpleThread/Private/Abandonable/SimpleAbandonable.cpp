// Copyright (C) RenZhai.2019.All Rights Reserved.

#include "Abandonable/SimpleAbandonable.h"

FSimpleAbandonable::FSimpleAbandonable(const FSimpleDelegate &InThreadDelegate)
	:ThreadDelegate(InThreadDelegate)
{

}

void FSimpleAbandonable::DoWork()
{
	ThreadDelegate.ExecuteIfBound();
}