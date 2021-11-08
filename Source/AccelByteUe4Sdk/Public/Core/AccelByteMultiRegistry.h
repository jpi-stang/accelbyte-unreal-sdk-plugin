// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpClient.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteFulfillmentApi.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteGameProfileApi.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Api/AccelByteQos.h"
#include "Api/AccelByteLeaderboardApi.h"
#include "Api/AccelByteCloudSaveApi.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Api/AccelByteAgreementApi.h"
#include "Api/AccelByteAchievementApi.h"
#include "Api/AccelByteSessionBrowserApi.h"
#include "Api/AccelByteTurnManagerApi.h"
#include "Api/AccelByteUGCApi.h"
#include "Api/AccelByteSeasonPassApi.h"
#include "Api/AccelByteReportingApi.h"
#include "Api/AccelByteCurrencyApi.h"
#include "Api/AccelByteMiscellaneousApi.h"
#include "Api/AccelByteRewardApi.h"

using namespace AccelByte;

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FApiBase
{
public:
	FApiBase(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler & HttpRef);
	
protected:
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
	FHttpRetryScheduler& HttpRef;
	FHttpClient HttpClient;
};

class ACCELBYTEUE4SDK_API FApiClient final
{
public:
	FApiClient();
	FApiClient(AccelByte::Credentials& Credentials, AccelByte::FHttpRetryScheduler& Http);
	~FApiClient();

	bool bUseSharedCredentials;

	FCredentialsRef CredentialsRef{};
	FHttpRetrySchedulerRef HttpRef{};
	Api::User User{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::UserProfile UserProfile{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Category Category{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Entitlement Entitlement{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Order Order{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Item Item{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Wallet Wallet{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Fulfillment Fulfillment{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::CloudStorage CloudStorage{*CredentialsRef, FRegistry::Settings};
	Api::Lobby Lobby{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::GameProfile GameProfile{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Statistic Statistic{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::QosManager QosManager{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Qos Qos{};
	Api::Leaderboard Leaderboard{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::CloudSave CloudSave{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::GameTelemetry GameTelemetry{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Agreement Agreement{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Achievement Achievement{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::SessionBrowser SessionBrowser{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::UGC UGC{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::SeasonPass SeasonPass{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Reporting Reporting{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Currency Currency{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Miscellaneous Miscellaneous{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::Reward Reward{*CredentialsRef, FRegistry::Settings, *HttpRef};
	Api::TurnManager TurnManager{*CredentialsRef, FRegistry::Settings, *HttpRef};
	
	template<typename T, typename... U>
	T GetApi(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return T(*CredentialsRef, FRegistry::Settings, *HttpRef, Forward<U>(Args)...);
	}

	template<typename T, typename... U>
	TSharedPtr<T, ESPMode::ThreadSafe> GetApiPtr(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return MakeShared<T, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef, Forward<U>(Args)...);
	}
};

typedef TSharedRef<FApiClient, ESPMode::ThreadSafe> FApiClientRef;
typedef TSharedPtr<FApiClient, ESPMode::ThreadSafe> FApiClientPtr;

class ACCELBYTEUE4SDK_API FMultiRegistry
{
public:
	static FApiClientPtr GetApiClient(FString const Key = TEXT("default"));
	static bool RegisterApiClient(FString const Key, FApiClientPtr ApiClient);
private:
	static TMap<FString, FApiClientPtr> ApiClientInstances;

	FMultiRegistry() = delete;
	FMultiRegistry(FMultiRegistry const& Other) = delete;
	FMultiRegistry& operator=(FMultiRegistry const& Other) = delete;
	FMultiRegistry(FMultiRegistry&& Other) = delete;
	FMultiRegistry& operator=(FMultiRegistry&& Other) = delete;
	~FMultiRegistry() = delete;
};

}
