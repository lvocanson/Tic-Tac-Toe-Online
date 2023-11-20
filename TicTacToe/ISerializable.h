#pragma once
#include "json.hpp"

using Json = nlohmann::json;

struct ISerializable
{
	/// <summary>
	/// Serialize the data into Json format
	/// </summary>
	/// <returns>Json type</returns>
	virtual Json Serialize() = 0;
	/// <summary>
	/// Retrieve the data stored in the json
	/// </summary>
	/// <param name="j"></param>
	virtual void Deserialize(Json j) = 0;
};
