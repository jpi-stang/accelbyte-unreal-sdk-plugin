// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteUe4SdkModule.h"

namespace AccelByte
{

FApiClientPtr AccelByte::FMultiRegistry::GetApiClient(const FString &Key)
{
	if (!ApiClientInstances.Contains(Key))
	{
		FApiClientPtr NewClient = nullptr;
		
		if (Key.Compare(TEXT("default")) == 0) 
		{
			NewClient = MakeShared<FApiClient, ESPMode::ThreadSafe>(FRegistry::Credentials, FRegistry::HttpRetryScheduler);
		}
		else 
		{
			IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
			NewClient = MakeShared<FApiClient, ESPMode::ThreadSafe>();
			NewClient->CredentialsRef->SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
		}

		ApiClientInstances.Add(Key, NewClient);
	}

	return ApiClientInstances[Key];
}

FServerApiClientPtr AccelByte::FMultiRegistry::GetServerApiClient(const FString &Key)
{
	if (!ServerApiClientInstances.Contains(Key))
	{
		FServerApiClientPtr NewClient = nullptr;
		
		if (Key.Compare(TEXT("default")) == 0) 
		{
			NewClient = MakeShared<FServerApiClient, ESPMode::ThreadSafe>(FRegistry::ServerCredentials, FRegistry::HttpRetryScheduler);
		}
		else 
		{
			IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
			NewClient = MakeShared<FServerApiClient, ESPMode::ThreadSafe>();
			NewClient->ServerCredentialsRef->SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
		}

		ServerApiClientInstances.Add(Key, NewClient);
	}
	
	return ServerApiClientInstances[Key];
}

bool FMultiRegistry::RegisterApiClient(FString const &Key, FApiClientPtr ApiClient)
{
	bool bResult = false;

	if (!Key.IsEmpty())
	{
		ApiClientInstances.Add(Key, ApiClient);
		bResult = true;
	}

	return bResult;
}

bool FMultiRegistry::RemoveApiClient(const FString &Key)
{
	bool bResult = false;

    if (!Key.IsEmpty())
    {
        const int32 RemovedNum = ApiClientInstances.Remove(Key);
        bResult = RemovedNum > 0;
    }

    return bResult;
}

bool FMultiRegistry::RemoveServerApiClient(const FString &Key)
{
	bool bResult = false;
	
	if (!Key.IsEmpty())
	{
        const int32 RemovedNum = ServerApiClientInstances.Remove(Key);
        bResult = RemovedNum > 0;
	}

	return bResult;
}

TMap<FString, FApiClientPtr> FMultiRegistry::ApiClientInstances;
TMap<FString, FServerApiClientPtr> FMultiRegistry::ServerApiClientInstances;

}
