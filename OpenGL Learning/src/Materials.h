#pragma once
#include "Material.h"

static class Materials
{
public:
	enum class Type
	{
		Emerald, Jade, Obisidian, Pearl, Ruby, Turquoise, //gems
		Brass, Bronze, Chrome, Copper, Gold, Silver, //Metals
		BlackPlastic, CyanPlastic, GreenPlastic, RedPlastic, WhitePlastic, YellowPlastic, //Plastic
		BlackRubber, CyanRubber, GreenRubber, RedRubber, WhiteRubber, YellowRubber //Rubber
	};

	static Material GetMaterial(Materials::Type type)
	{
		switch (type)
		{
		case Type::Emerald: return GetEmerald();
		case Type::Jade: return GetJade();
		case Type::Obisidian:return GetObsidian();
		case Type::Pearl: return GetPearl();
		case Type::Ruby: return GetRuby();
		case Type::Turquoise: return GetTurquoise();
		case Type::Brass: return GetBrass();
		case Type::Bronze: return GetBronze();
		case Type::Chrome: return GetChrome();
		case Type::Copper: return GetCopper();
		case Type::Gold: return GetGold();
		case Type::Silver: return GetSilver();
		case Type::BlackPlastic: return GetBlackPlastic();
		case Type::CyanPlastic: return GetCyanPlastic();
		case Type::GreenPlastic: return GetGreenPlastic();
		case Type::RedPlastic: return GetRedPlastic();
		case Type::WhitePlastic: return GetWhitePlastic();
		case Type::YellowPlastic: return GetYellowPlastic();
		case Type::BlackRubber: return GetBlackRubber();
		case Type::CyanRubber: return GetCyanRubber();
		case Type::GreenRubber: return GetGreenRubber();
		case Type::RedRubber: return GetRedRubber();
		case Type::WhiteRubber: return GetWhiteRubber();
		case Type::YellowRubber: return GetYellowRubber();
		}
	}

	inline static Material GetEmerald()
	{
		glm::vec3 ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
		glm::vec3 diffuse = glm::vec3(0.07568f, 0.89f, 0.63f);
		glm::vec3 specular = glm::vec3(0.633f, 0.727811f, 0.633f);

		float shininess = 128.0f * 0.6f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetJade()
	{
		glm::vec3 ambient = glm::vec3(0.135f, 0.2225f, 0.1575f);
		glm::vec3 diffuse = glm::vec3(0.54f, 0.89f, 0.63f);
		glm::vec3 specular = glm::vec3(0.316228f, 0.316228f, 0.316228f);

		float shininess = 128.0f * 0.1f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetObsidian()
	{
		glm::vec3 ambient = glm::vec3(0.05375f, 0.05f, 0.06625f);
		glm::vec3 diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
		glm::vec3 specular = glm::vec3(0.332741f, 0.328634f, 0.346435f);

		float shininess = 128.0f * 0.3f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetPearl()
	{
		glm::vec3 ambient = glm::vec3(0.25f, 0.20725f, 0.20725f);
		glm::vec3 diffuse = glm::vec3(1.0f, 0.829f, 0.829f);
		glm::vec3 specular = glm::vec3(0.296648f, 0.296648f, 0.296648f);

		float shininess = 128.0f * 0.088f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetRuby()
	{
		glm::vec3 ambient = glm::vec3(0.1745f, 0.01175f, 0.01175f);
		glm::vec3 diffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f);
		glm::vec3 specular = glm::vec3(0.727811f, 0.626959f, 0.626959f);

		float shininess = 128.0f * 0.6f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetTurquoise()
	{
		glm::vec3 ambient = glm::vec3(0.1f, 0.18725f, 0.1745f);
		glm::vec3 diffuse = glm::vec3(0.396f, 0.74151f, 0.69102f);
		glm::vec3 specular = glm::vec3(0.297254f, 0.30829f, 0.306678f);

		float shininess = 128.0f * 0.1f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetBrass()
	{
		glm::vec3 ambient = glm::vec3(0.329412f, 0.223529f, 0.027451f);
		glm::vec3 diffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f);
		glm::vec3 specular = glm::vec3(0.992157f, 0.941176f, 0.807843f);

		float shininess = 128.0f * 0.21794872f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetBronze()
	{
		glm::vec3 ambient = glm::vec3(0.2125f, 0.1275f, 0.054f);
		glm::vec3 diffuse = glm::vec3(0.714f, 0.4284f, 0.18144f);
		glm::vec3 specular = glm::vec3(0.393548f, 0.271906f, 0.166721f);

		float shininess = 128.0f * 0.2f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetChrome()
	{
		glm::vec3 ambient = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
		glm::vec3 specular = glm::vec3(0.774597f, 0.774597f, 0.774597f);

		float shininess = 128.0f * 0.6f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetCopper()
	{
		glm::vec3 ambient = glm::vec3(0.19125f, 0.0735f, 0.0225f);
		glm::vec3 diffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f);
		glm::vec3 specular = glm::vec3(0.256777f, 0.137622f, 0.086014f);

		float shininess = 128.0f * 0.1f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetGold()
	{
		glm::vec3 ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
		glm::vec3 diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
		glm::vec3 specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);

		float shininess = 128.0f * 0.4f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetSilver()
	{
		glm::vec3 ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
		glm::vec3 diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
		glm::vec3 specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);

		float shininess = 128.0f * 0.4f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetBlackPlastic()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.01f, 0.01f, 0.01f);
		glm::vec3 specular = glm::vec3(0.50f, 0.50f, 0.50f);

		float shininess = 128.0f * 0.25f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetCyanPlastic()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.1f, 0.20725f);
		glm::vec3 diffuse = glm::vec3(1.0f, 0.829f, 0.829f);
		glm::vec3 specular = glm::vec3(0.296648f, 0.296648f, 0.296648f);

		float shininess = 128.0f * 0.25f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetGreenPlastic()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.1f, 0.35f, 0.1f);
		glm::vec3 specular = glm::vec3(0.45f, 0.55f, 0.45f);

		float shininess = 128.0f * 0.25f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetRedPlastic()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
		glm::vec3 specular = glm::vec3(0.7f, 0.6f, 0.6f);

		float shininess = 128.0f * 0.25f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetWhitePlastic()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.55f, 0.55f, 0.55f);
		glm::vec3 specular = glm::vec3(0.70f, 0.70f, 0.70f);

		float shininess = 128.0f * 0.25f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetYellowPlastic()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.0f);
		glm::vec3 specular = glm::vec3(0.60f, 0.60f, 0.50f);

		float shininess = 128.0f * 0.25f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetBlackRubber()
	{
		glm::vec3 ambient = glm::vec3(0.02f, 0.02f, 0.02f);
		glm::vec3 diffuse = glm::vec3(0.01f, 0.01f, 0.01f);
		glm::vec3 specular = glm::vec3(0.4f, 0.4f, 0.4f);

		float shininess = 128.0f * 0.078125f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetCyanRubber()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.05f, 0.05f);
		glm::vec3 diffuse = glm::vec3(0.4f, 0.5f, 0.5f);
		glm::vec3 specular = glm::vec3(0.04f, 0.7f, 0.7f);

		float shininess = 128.0f * 0.078125f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetGreenRubber()
	{
		glm::vec3 ambient = glm::vec3(0.0f, 0.05f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.4f, 0.5f, 0.4f);
		glm::vec3 specular = glm::vec3(0.04f, 0.7f, 0.04f);

		float shininess = 128.0f * 0.078125f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetRedRubber()
	{
		glm::vec3 ambient = glm::vec3(0.05f, 0.0f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.4f, 0.4f);
		glm::vec3 specular = glm::vec3(0.7f, 0.04f, 0.04f);

		float shininess = 128.0f * 0.078125f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetWhiteRubber()
	{
		glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 specular = glm::vec3(0.7f, 0.7f, 0.7f);

		float shininess = 128.0f * 0.078125f;

		return { ambient, diffuse, specular, shininess };
	}
	inline static Material GetYellowRubber()
	{
		glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.4f);
		glm::vec3 specular = glm::vec3(0.7f, 0.7f, 0.04f);

		float shininess = 128.0f * 0.078125f;

		return { ambient, diffuse, specular, shininess };
	}
};