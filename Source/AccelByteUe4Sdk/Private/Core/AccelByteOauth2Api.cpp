// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{

FHttpRequestPtr Oauth2::ConstructTokenRequest(const FString& Url
	, const FString& ClientId
	, const FString& ClientSecret)
{
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	return Request;
}

void Oauth2::GetTokenWithAuthorizationCode(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	GetTokenWithAuthorizationCode(ClientId
		, ClientSecret
		, AuthorizationCode
		, RedirectUri
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl);
}
	
void Oauth2::GetTokenWithAuthorizationCode(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__)
		, TEXT("The API might be removed wihtout notice, so please use GetTokenWithAuthorizationCodeV3 instead!!"));

	const FString Url = FString::Printf(TEXT("%s/oauth/token")
		, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("authorization_code")},
		{TEXT("code"), AuthorizationCode },
		{TEXT("redirect_uri"), RedirectUri },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	GetTokenWithPasswordCredentials(ClientId
		, ClientSecret
		, Username
		, Password
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl);
}
	
void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__)
		, TEXT("The API might be removed without notice, please use GetTokenWithPasswordCredentialsV3 instead!!"));
	
	const FString Url = FString::Printf(TEXT("%s/oauth/token")
		, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetHeader(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password")},
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username) },
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password) },
		{TEXT("device_id"), FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId()) },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithClientCredentials(const FString& ClientId
	, const FString& ClientSecret
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
		, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=client_credentials")));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithDeviceId(const FString& ClientId
	, const FString& ClientSecret
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	GetTokenWithDeviceId(ClientId
		, ClientSecret
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl);
}

void Oauth2::GetTokenWithDeviceId(const FString& ClientId
	, const FString& ClientSecret 
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/platforms/device/token")
		, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetContentAsString(FString::Printf(TEXT("device_id=%s"), *FGenericPlatformHttp::UrlEncode(*FAccelByteUtilities::GetDeviceId())));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}
	
void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& PlatformId
	, const FString& PlatformToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    	
	GetTokenWithOtherPlatformToken(ClientId
		, ClientSecret
		, PlatformId
		, PlatformToken
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, true
		, IamUrl);
}
	
void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& PlatformId
	, const FString& PlatformToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bCreateHeadless
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token")
		, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl
		, *PlatformId);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("platform_token"), FGenericPlatformHttp::UrlEncode(PlatformToken)},
		{TEXT("createHeadless"), bCreateHeadless ? TEXT("true") : TEXT("false")},
		{TEXT("macAddress"), FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetMacAddress(true)) }
	}, TEXT(""));
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithRefreshToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& RefreshToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	GetTokenWithRefreshToken(ClientId
		, ClientSecret
		, RefreshToken
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl);
}
	
void Oauth2::GetTokenWithRefreshToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& RefreshToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
			, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("refresh_token")},
		{TEXT("refresh_token"), RefreshToken},
	}, TEXT(""));
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeToken(const FString& AccessToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("This method uses wrong auth type, so please use RevokeToken with ClientId and ClientSecret instead!"));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/revoke")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, TEXT(""), TEXT(""));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer " + AccessToken));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AccessToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	RevokeToken(ClientId
		, ClientSecret
		, AccessToken
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl);
}
	
void Oauth2::RevokeToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AccessToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/revoke")
				, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithAuthorizationCodeV3(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	GetTokenWithAuthorizationCodeV3(ClientId
		, ClientSecret
		, AuthorizationCode
		, RedirectUri
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl);
}
	
void Oauth2::GetTokenWithAuthorizationCodeV3(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("authorization_code") },
		{TEXT("code"), AuthorizationCode },
		{TEXT("redirect_uri"), RedirectUri },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, bool bRememberMe
	, const FString& IamUrl)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	GetTokenWithPasswordCredentialsV3(ClientId
		, ClientSecret
		, Username
		, Password
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, bRememberMe
		, IamUrl);
}
	
void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bRememberMe
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	Request->SetHeader(TEXT("Auth-Trust-Id"), FAccelByteUtilities::GetAuthTrustId());
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password")},
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username)},
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password)},
		{TEXT("extend_exp"), bRememberMe ? TEXT("true") : TEXT("false") },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::VerifyAndRememberNewDevice(const FString& ClientId
	, const FString& ClientSecret
	, const FString& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, const FString& Code 
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bRememberDevice
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Factor = FAccelByteUtilities::GetAuthenticatorString(AuthFactorType);

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/mfa/verify")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("mfaToken"), MfaToken},
		{TEXT("factor"), Factor},
		{TEXT("code"), Code},
		{TEXT("rememberDevice"), bRememberDevice ? TEXT("true") : TEXT("false") },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::CreateHeadlessAccountAndResponseToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& LinkingToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/headless/token")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetContentAsString(FString::Printf(TEXT("linkingToken=%s&client_id=%s"), *LinkingToken, *ClientId));
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}
	
void Oauth2::AuthenticationWithPlatformLink(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const FString& LinkingToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/authenticateWithLink")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username)},
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password)},
		{TEXT("linkingToken"), LinkingToken},
		{TEXT("client_id"), ClientId},
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::VerifyToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Token
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/verify")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *Token));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda([OnSuccess, Token](FJsonObject const& JsonObject)
			{
				FString CheckedToken;
				JsonObject.TryGetStringField("access_token", CheckedToken);
				if (Token.Equals(CheckedToken))
				{
					OnSuccess.ExecuteIfBound();
				}
			}),
			OnError),
		FPlatformTime::Seconds());
}
	
void Oauth2::GenerateOneTimeCode(const FString& AccessToken
	, const FString& PlatformId
	, const THandler<FGeneratedOneTimeCode>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/link/code/request")
					, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, TEXT(""), TEXT(""));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer " + AccessToken));
	Request->SetContentAsString(FString::Printf(TEXT("platformId=%s"), *PlatformId));
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}
	
void Oauth2::GenerateGameToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Code
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/token/exchange")
							, IamUrl.IsEmpty() ? *FRegistry::Settings.IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret);
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	Request->SetContentAsString(FString::Printf(TEXT("code=%s"), *Code));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}

} // Namespace Api
} // Namespace AccelByte
