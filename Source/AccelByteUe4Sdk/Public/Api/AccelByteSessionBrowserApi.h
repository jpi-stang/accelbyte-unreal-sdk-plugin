// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSessionBrowserModels.h"

class FJsonObject;

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
* @brief Leaderboard API to obtain player's ranking in a namespace.
*/

class ACCELBYTEUE4SDK_API SessionBrowser 
{
public:
	SessionBrowser(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler& HttpRef);
	~SessionBrowser();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;

public:
	/**
	* @brief Create the session to Session Browser.
	*
	* @param GameMode game mode of the game.
	* @param GameMapName map of the game.
	* @param GameVersion version of the played game.
	* @param BotCount number of bot.
	* @param MaxPlayer maximum number of player can join the session.
	* @param OtherSettings other setting in json format
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateGameSession(FString const&GameMode, FString const&GameMapName, FString const&GameVersion, uint32 BotCount, uint32 MaxPlayer, TSharedPtr<FJsonObject> OtherSettings, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Update the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param CurrentPlayerCount current number of player in the session.
	* @param MaxPlayer maximum number of player can join the session.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateGameSession(FString const& SessionId, uint32 MaxPlayer, uint32 CurrentPlayerCount, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Remove the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void RemoveGameSession(FString const& SessionId, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Query game session.
	*
	* @param SessionType the session type, either "dedicated", or "p2p"
	* @param GameMode The game mode of the session to query.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserGetResult.
	* @param OnError This will be called when the operation failed.
	* @param Offset offset of the query
	* @param Limit number of data to return
	*/
	void GetGameSessions(FString const&SessionType, FString const&GameMode, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess, FErrorHandler const& OnError, uint32 Offset = 0, uint32 Limit = 50);

	/**
	* @brief Get game session data by session ID.
	*
	* @param SessionId id of the session
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void GetGameSession(FString const& SessionId, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess, FErrorHandler const& OnError);

private:
	SessionBrowser() = delete;
	SessionBrowser(SessionBrowser const&) = delete;
	SessionBrowser(SessionBrowser&&) = delete;

	FString GetSessionBrowserUrl();
};

} // Namespace Api
} // Namespace AccelByte