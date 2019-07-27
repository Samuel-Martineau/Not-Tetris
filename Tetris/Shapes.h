#pragma once
#include <vector>
#include "BlockColor.h"
using namespace std;

namespace Shapes {

	const std::vector<std::vector<std::vector<BlockColor>>> shapeI = {
		{
			{ BlockColor::NONE , BlockColor::NONE, BlockColor::NONE , BlockColor::NONE },
			{ BlockColor::I, BlockColor::I , BlockColor::I , BlockColor::I },
			{ BlockColor::NONE , BlockColor::NONE, BlockColor::NONE , BlockColor::NONE },
			{ BlockColor::NONE , BlockColor::NONE, BlockColor::NONE , BlockColor::NONE }
		},

		{ 
			{ BlockColor::NONE, BlockColor::I , BlockColor::NONE, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::I , BlockColor::NONE, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::I , BlockColor::NONE, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::I , BlockColor::NONE, BlockColor::NONE }
		}
	};

	const std::vector<std::vector<std::vector<BlockColor>>> shapeJ = {
		{ 
			{ BlockColor::J, BlockColor::NONE, BlockColor::NONE},
			{ BlockColor::J, BlockColor::J, BlockColor::J },
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::J, BlockColor::J},
			{ BlockColor::NONE, BlockColor::J, BlockColor::NONE},
			{ BlockColor::NONE, BlockColor::J, BlockColor::NONE} 
		},

		{ 
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE },
			{ BlockColor::J, BlockColor::J, BlockColor::J },
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::J } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::J, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::J, BlockColor::NONE },
			{ BlockColor::J, BlockColor::J, BlockColor::NONE } 
		}
	};

	const std::vector<std::vector<std::vector<BlockColor>>> shapeL = {
		{ 
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::L },
			{ BlockColor::L, BlockColor::L, BlockColor::L },
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::L, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::L, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::L,  BlockColor::L } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE },
			{ BlockColor::L, BlockColor::L, BlockColor::L },
			{ BlockColor::L, BlockColor::NONE, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::L, BlockColor::L, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::L, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::L, BlockColor::NONE } 
		}
	};

	const std::vector<std::vector<std::vector<BlockColor>>> shapeO = {
		{
			{ BlockColor::O, BlockColor::O },
			{ BlockColor::O, BlockColor::O }
		}
	};

	const std::vector<std::vector<std::vector<BlockColor>>> shapeS = {
		{ 
			{ BlockColor::NONE, BlockColor::S, BlockColor::S },
			{ BlockColor::S, BlockColor::S, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::S, BlockColor::NONE, BlockColor::NONE },
			{ BlockColor::S, BlockColor::S, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::S,  BlockColor::NONE }
		}
	};

	const std::vector<std::vector<std::vector<BlockColor>>> shapeT = {
		{ 
			{ BlockColor::NONE, BlockColor::T, BlockColor::NONE },
			{ BlockColor::T, BlockColor::T, BlockColor::T },
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::T, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::T, BlockColor::T },
			{ BlockColor::NONE, BlockColor::T, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE },
			{ BlockColor::T, BlockColor::T, BlockColor::T },
			{ BlockColor::NONE, BlockColor::T, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::T, BlockColor::NONE },
			{ BlockColor::T, BlockColor::T, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::T, BlockColor::NONE } 
		}
	};

	const std::vector<std::vector<std::vector<BlockColor>>> shapeZ = {
		{ 
			{ BlockColor::Z, BlockColor::Z, BlockColor::NONE },
			{ BlockColor::NONE, BlockColor::Z, BlockColor::Z },
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::NONE } 
		},

		{ 
			{ BlockColor::NONE, BlockColor::NONE, BlockColor::Z },
			{ BlockColor::NONE, BlockColor::Z, BlockColor::Z },
			{ BlockColor::NONE, BlockColor::Z, BlockColor::NONE } 
		}
	};

};