// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteLeaderboardApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Leaderboard::Leaderboard(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Leaderboard::~Leaderboard()
{}

void Leaderboard::GetRankings(FString const& LeaderboardCode
	, EAccelByteLeaderboardTimeFrame const& TimeFrame
	, uint32 Offset
	, uint32 Limit
	, THandler<FAccelByteModelsLeaderboardRankingResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString TimeFrameString = "";

	switch (TimeFrame)
	{
	case EAccelByteLeaderboardTimeFrame::ALL_TIME:
		TimeFrameString = "alltime";
		break;
	case EAccelByteLeaderboardTimeFrame::CURRENT_SEASON:
		TimeFrameString = "season";
		break;
	case EAccelByteLeaderboardTimeFrame::CURRENT_MONTH:
		TimeFrameString = "month";
		break;
	case EAccelByteLeaderboardTimeFrame::CURRENT_WEEK:
		TimeFrameString = "week";
		break;
	case EAccelByteLeaderboardTimeFrame::TODAY:
		TimeFrameString = "today";
		break;
	default:
		break;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/leaderboards/%s/%s")
		, *SettingsRef.LeaderboardServerUrl
		, *SettingsRef.Namespace
		, *LeaderboardCode
		, *TimeFrameString);

	TMap<FString, FString> QueryParams;

	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}
	
	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

void Leaderboard::GetUserRanking(FString const& UserId
	, FString const& LeaderboardCode
	, THandler<FAccelByteModelsUserRankingData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/leaderboards/%s/users/%s")
		, *SettingsRef.LeaderboardServerUrl
		, *CredentialsRef.GetNamespace()
		, *LeaderboardCode
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
