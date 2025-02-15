﻿// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"

#if (ENGINE_MAJOR_VERSION==5 && ENGINE_MINOR_VERSION>=0)
#include "Containers/Ticker.h"

using FTickerAlias = FTSTicker;
using FDelegateHandleAlias = FTSTicker::FDelegateHandle;
#else
using FTickerAlias = FTicker;
using FDelegateHandleAlias = FDelegateHandle;
#endif
