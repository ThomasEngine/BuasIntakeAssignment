#pragma once

#include <vector>
#include "object.h"

namespace Tmpl8
{
	class World
	{
	public:
		void LoadWorld(std::string filename);
		void DrawWorld();
	
	private:
		std::vector<OBject> m_map;



	};
}