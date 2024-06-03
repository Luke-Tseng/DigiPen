//---------------------------------------------------------
// file:	UserLoginState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The menu shown when the user is connecting to the user service, retrieving the necessary configuration
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "UserLoginState.h"
#include "ConnectingMenuState.h"
#include "GameStateManager.h"


UserLoginState::UserLoginState(NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator, std::string game_type, ClientConfiguration configuration)
	: scenario_state_creator_(scenario_state_creator),
  	  game_type_(game_type),
	  configuration_(configuration),
	  web_client_(utility::conversions::to_string_t(configuration.user_service))
{
	operation_description_ = "Logging in and connecting to the user service...";
	InitiateUserRequest();
}


UserLoginState::~UserLoginState() = default;


void UserLoginState::Update()
{
	// if the user presses ESC from the main menu, the process will exit.
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		GameStateManager::ReturnToBaseState();
		return;
	}

	// check to see if the web request is done
	// -- if it is, .get() will not block
	// -- this way, we can draw the screen while we wait...
	if (active_task_.is_done())
	{
		try
		{
			auto connect_response_data = active_task_.get();
			//TODO: retrieve the "avatar" field from the connect response data, and store it in configuration_.avatar
			configuration_.avatar = utility::conversions::to_utf8string(connect_response_data[U("avatar")].as_string());
			//TODO: retrieve the "token" field from the connect response data, and store it in configuration_.token
			configuration_.token = utility::conversions::to_utf8string(connect_response_data[U("token")].as_string());
			//TODO: retrieve the "game_port" field from the connect response data, and store it in configuration_.game_port
			// -- NOTE: game_port is an integer, not a string, in both the connect response data and the game_port...
			configuration_.game_port = connect_response_data[U("game_port")].as_integer();

			std::cout << "Connect token session: " << configuration_.token.c_str() << std::endl;
			auto* connecting_state = new ConnectingMenuState(scenario_state_creator_, game_type_, configuration_);
			GameStateManager::ApplyState(connecting_state);
		}
		catch (const std::exception& e)
		{
			std::cout << "Exception from web request: " << e.what() << std::endl << std::flush;
			GameStateManager::ReturnToBaseState();
		}
	}
}


void UserLoginState::Draw()
{
	// draw the description
	CP_Settings_TextSize(30);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText(operation_description_.c_str(), 0.0f, 0.0f);
}


void UserLoginState::InitiateUserRequest()
{
	//TODO: build a json object with the login request data
	// -- the username and password are stored in configuration_, but they need to be converted to string_t...
	auto login_data = web::json::value::parse("{}");
	login_data[U("username")] = web::json::value(utility::conversions::to_string_t(configuration_.username));
	login_data[U("password")] = web::json::value(utility::conversions::to_string_t(configuration_.password));
	//TODO: use web_client_ to perform a series of operations:
	// 1) login to the user service
	// 2) extract the json from the login response
	// 3) call "connect" on the user service,
	// --- note: the request data for connect should be the login response data, but you should add "game_type" to the login response data before sending
	// --- note: the value of the game_type field in the connect request data is game_type_
	// 4) extract the json from the connect response
	// NOTE: you should detect HTTP response codes that are not web::http::status_codes::OK, and throw an exception with meaningful text.  It will be logged in the catch block above.
	// NOTE:  This will be web_client.request(...).then(...).then(...).then(...)
	// *** The return value MUST be stored in active_task_! *** 
	// Example of storing in active_task_:
	// active_task_ = web_client_.request(web::http::methods::POST, U("some/path"), some_data)
	//    .then...

	active_task_ = web_client_.request(web::http::methods::POST, U("/api/v1/login"), login_data)
		.then([](web::http::http_response login_response) {
			std::cout << login_response.status_code() << std::endl << std::flush;
			if (login_response.status_code() != web::http::status_codes::OK)
			{
				throw std::exception("Login failed.");
			}
			return login_response.extract_json();
			})
		.then([=](web::json::value login_response_data) mutable
			{
				login_response_data[U("game_type")] = web::json::value(utility::conversions::to_string_t(game_type_));
				return web_client_.request(web::http::methods::POST, U("/api/v1/connect"), login_response_data);
			})
		.then([](web::http::http_response connect_response)
			{
				if (connect_response.status_code() != web::http::status_codes::OK)
				{
					throw std::exception("Connect failed.");
				}
				return connect_response.extract_json();
			});
}
